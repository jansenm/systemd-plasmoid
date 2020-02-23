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
#ifndef SYSTEMD_KDE_UNITINFO_P_H
#define SYSTEMD_KDE_UNITINFO_P_H

#include "UnitInfo.h"

#include <QtCore/QString>
#include <QtDBus/QDBusObjectPath>

namespace Systemd {

struct UnitInfoPrivate
{
    QString unit_file_status;
    QString unit_file;
    QString job_type;
    QString following;
    QString sub_state;
    QString active_state;
    QString load_state;
    QString description;
    QString id;
    QDBusObjectPath unit_path;
    QDBusObjectPath job_path;
    unsigned int job_id;
};

typedef QList<UnitInfoPrivate> UnitInfoPrivateList;

QDBusArgument&
operator<<(QDBusArgument& argument, const UnitInfoPrivate& unit);

const QDBusArgument&
operator>>(const QDBusArgument& argument, UnitInfoPrivate& unit);

}

Q_DECLARE_METATYPE(Systemd::UnitInfoPrivate);
Q_DECLARE_METATYPE(Systemd::UnitInfoPrivateList);

#endif // SYSTEMD_KDE_UNITINFO_P_H
