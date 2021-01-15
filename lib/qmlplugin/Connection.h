/*
 * Systemd KDE - A plasma widget to control systemd services
 *
 * SPDX-FileCopyrightText: 2021 Michael Jansen <info@michael-jansen.biz>
 * SPDX-License-Identifier:  LGPL-2.1-or-later
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
                       unitsChanged)

    Q_PROPERTY(QAbstractItemModel *unitFiles
                       READ
                               unitFiles
                       NOTIFY
                       unitFilesChanged)

    Q_PROPERTY(Systemd::Manager *manager
                       READ
                              manager)

public:

    enum Type {
        SessionBus,
        SystemBus
    };
    Q_ENUM(Type)

    explicit Connection(QObject *parent = nullptr);

    ~Connection() override;

    Type connection() const;

    Q_INVOKABLE bool setConnection(Type connection);

    QAbstractItemModel *units();

    Systemd::Manager *manager();

    QAbstractItemModel *unitFiles();



Q_SIGNALS:

    void connectionChanged() const;
    void unitsChanged() const;
    void unitFilesChanged() const;

private:
    ConnectionPrivate *d_ptr;

    Q_DECLARE_PRIVATE(Connection)
};


#endif //SYSTEMD_KDE_CONNECTION_H
