//
// Created by mjansen on 2/13/20.
//

#ifndef SYSTEMD_KDE_CONNECTION_H
#define SYSTEMD_KDE_CONNECTION_H


#include "Manager.h"
#include <QtCore/QtPlugin>
#include <QtCore/QAbstractItemModel>

#include "systemd-qt_export.h"

class ConnectionPrivate;

class SYSTEMD_QT_EXPORT Connection : public QObject {
Q_OBJECT

    Q_PROPERTY(Type connection
                       READ
                               connection
                       WRITE
                               setConnection
                       NOTIFY
                       connectionChanged)

    Q_PROPERTY(QAbstractItemModel *units
                       READ
                               units
                       NOTIFY
                       unitsChanged);

    Q_PROPERTY(QAbstractItemModel *unitFiles
                       READ
                               unitFiles
                       NOTIFY
                       unitFilesChanged);

    Q_PROPERTY(Systemd::Manager *manager
                       READ
                              manager);

public:

    enum Type {
        SessionBus,
        SystemBus
    };

    Q_ENUM(Type);

    explicit Connection(QObject *parent = nullptr);

    ~Connection() override;

    Type connection() const;

    void setConnection(Type connection);

    QAbstractItemModel *units();

    Systemd::Manager *manager();

    QAbstractItemModel *unitFiles();



signals:

    void connectionChanged(const Connection::Type &connection) const;
    void unitsChanged() const;
    void unitFilesChanged() const;

private:
    ConnectionPrivate *d_ptr;

    Q_DECLARE_PRIVATE(Connection)
};


#endif //SYSTEMD_KDE_CONNECTION_H
