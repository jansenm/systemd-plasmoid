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
#ifndef SYSTEMD_KDE_CHANGES_H
#define SYSTEMD_KDE_CHANGES_H

#include "systemd-qt_export.h"
#include <QtCore/QObject>
#include <QtDBus/QDBusReply>

namespace Systemd {


    class SYSTEMD_QT_EXPORT Changes : public QObject {
    Q_OBJECT
        Q_PROPERTY(QString type
                           READ
                                   type
                           CONSTANT);
        Q_PROPERTY(QString linkName
                           READ
                                   linkName
                           CONSTANT);
        Q_PROPERTY(QString linkDestination
                           READ
                                   linkDestination
                           CONSTANT);

        class ChangesPrivate *d_ptr;

    public:
        /**
         * Create a new unit object.
         *
         * @param reply         The dbus reply object to initialize from
         * @param parent        The parent object or nullptr
         */
        Changes(const ChangesPrivate &p, QObject *parent = nullptr);

        /**
         * Destroy the unit object.
         */
        ~Changes() override;

        QString type() const;

        QString linkName() const;

        QString linkDestination() const;

    private:
        Q_DECLARE_PRIVATE(Changes);
    };

    typedef QList<Changes *> ChangesList;

}

Q_DECLARE_METATYPE(Systemd::ChangesList);

#endif // SYSTEMD_KDE_CHANGES_H
