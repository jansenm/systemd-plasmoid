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
#include "QMLDebugger.h"


QString QMLDebugger::properties(QObject *item, bool linebreak)
{
    const QMetaObject *meta = item->metaObject();

    QHash<QString, QVariant> list;
    for (int i = 0; i < meta->propertyCount(); i++)
    {
        QMetaProperty property = meta->property(i);
        const char* name = property.name();
        QVariant value = item->property(name).toString();
        list[QLatin1String(name)] = value;
    }

    QString out;
    QHashIterator<QString, QVariant> i(list);
    while (i.hasNext()) {
        i.next();
        if (!out.isEmpty())
        {
            out += QLatin1String(", ");
            if (linebreak) out += QLatin1String("\n");
        }
        out.append(i.key());
        out.append(QLatin1String(": "));
        out.append(i.value().toString());
    }
    return out;
}
