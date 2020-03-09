/*
 * Systemd KDE - A plasma widget to control systemd services
 * Copyright (C) 2020  Michael Jansen <info@michael-jansen.biz>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */
#include "debug.h"

#include <Manager.h>
#include <QtCore/QSortFilterProxyModel>
#include "Connection.h"
#include "UnitModel.h"
#include "UnitFileModel.h"

using Systemd::Manager;

class ConnectionPrivate {
public:

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

bool Connection::setConnection(Connection::Type connection) {
    Q_D(Connection);
    if (d->manager && d->connection == connection) {
        return false;
    }

    delete d->units;
    d->units = nullptr;
    delete d->unitFiles;
    d->unitFiles = nullptr;
    delete d->manager;

    switch (connection) {
        case Connection::SystemBus:
            d->manager = new Manager(QDBusConnection::systemBus(), this);
            break;
        case Connection::SessionBus:
            d->manager = new Manager(QDBusConnection::sessionBus(), this);
            break;
        default:
            qWarning() << "Connection::setConnection() called with unknown type" << connection;
            Q_ASSERT(false);
    }

    d->connection = connection;
    emit connectionChanged();
    return true;
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
