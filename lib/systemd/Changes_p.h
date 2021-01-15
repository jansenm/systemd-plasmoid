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
#ifndef SYSTEMD_KDE_CHANGES_P_H
#define SYSTEMD_KDE_CHANGES_P_H

#include "Changes.h"

#include <QtCore/QString>
#include <QtDBus/QDBusObjectPath>

namespace Systemd {

    struct ChangesPrivate {
        QString type;
        QString linkName;
        QString linkDestination;
    };

    typedef QList<ChangesPrivate> ChangesPrivateList;

    QDBusArgument &
    operator<<(QDBusArgument &argument, const ChangesPrivate &unit);

    const QDBusArgument &
    operator>>(const QDBusArgument &argument, ChangesPrivate &unit);

}

Q_DECLARE_METATYPE(Systemd::ChangesPrivate)

Q_DECLARE_METATYPE(Systemd::ChangesPrivateList)

#endif // SYSTEMD_KDE_CHANGES_P_H
