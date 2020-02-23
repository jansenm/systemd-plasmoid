//
// Created by mjansen on 2/13/20.
//

#include "debug.h"

#include <Manager.h>
#include <QtCore/QSortFilterProxyModel>
#include "Connection.h"
#include "UnitModel.h"
#include "UnitFileModel.h"

using Systemd::Manager;

struct ConnectionPrivate {

    ConnectionPrivate() : connection(Connection::SystemBus),
                          units(nullptr),
                          unitFiles(nullptr),
                          manager(new Manager(QDBusConnection::systemBus(), nullptr)) {}

    Connection::Type connection;
    UnitModel *units;
    UnitFileModel *unitFiles;
    Manager *manager;
};

Connection::Connection(QObject *parent) : QObject(parent), d_ptr(new ConnectionPrivate()) {
    Q_D(Connection);
}

Connection::~Connection() {
    delete d_ptr;
}

Connection::Type Connection::connection() const {
    Q_D(const Connection);
    return d->connection;
}

Manager *
Connection::manager() {
    Q_D(const Connection);
    return d->manager;
}

void Connection::setConnection(Connection::Type connection) {
    Q_D(Connection);
    if (d->connection == connection) {
        return;
    }

    delete d->manager;
    delete d->units;
    d->units=0;
    delete d->unitFiles;
    d->unitFiles=0;

    switch (connection) {
        case Connection::SystemBus:
            d->manager = new Manager(QDBusConnection::systemBus(), this);
            break;
        case Connection::SessionBus:
            delete d->manager;
            d->manager = new Manager(QDBusConnection::sessionBus(), this);
            break;
        default:
            qWarning() << "Connection::setConnection() called with unknown type" << connection;
            Q_ASSERT(false);
    }

    d->connection = connection;
    emit connectionChanged(connection);
}

QAbstractItemModel *Connection::units() {
    Q_D(Connection);

    if (!d->units) {
        d->units = new UnitModel(d->manager, this);
    }

    return d->units;
}

QAbstractItemModel *Connection::unitFiles() {
    Q_D(Connection);

    if (!d->unitFiles) {
        d->unitFiles = new UnitFileModel(d->manager, this);
    }

    return d->unitFiles;
}
