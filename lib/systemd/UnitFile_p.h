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
#ifndef SYSTEMD_KDE_UNITFILE_P_H
#define SYSTEMD_KDE_UNITFILE_P_H

#include "UnitFile.h"
#include <QtCore/QMetaType>
#include <QtCore/QString>
#include <QtDBus/QDBusArgument>

namespace Systemd {

struct UnitFilePrivate
{
    QString id;
    QString state;
};
typedef QList<UnitFilePrivate> UnitFilePrivateList;

QDBusArgument&
operator<<(QDBusArgument& argument, const UnitFilePrivate& unit);

const QDBusArgument&
operator>>(const QDBusArgument& argument, UnitFilePrivate& unit);

}

// NOLINTNEXTLINE
Q_DECLARE_METATYPE(Systemd::UnitFilePrivate)
// NOLINTNEXTLINE
Q_DECLARE_METATYPE(Systemd::UnitFilePrivateList)

#endif // SYSTEMD_KDE_UNITFILE_P_H
