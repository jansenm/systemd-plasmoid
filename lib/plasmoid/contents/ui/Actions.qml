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
import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQml 2.14

Item {
    // TODO We use implicit knowledge about toolbar here. We could solve that by declaring a unitName here and using
    //      using a property binding between them defined in main.qml.

    property var addUnitAction:  Action {
        id: addUnitAction
        text: i18n("Add Unit")
        icon.name: ("list-add")
        enabled: false

        onTriggered: {
            // Play it twice as safe. Make sure the unit isn't already added.
            if (!conn.units.units.includes(toolbar.unitName)) {
                conn.units.addUnit(toolbar.unitName);
            }
        }

        Binding on enabled {
            // Enable it when something new is selected
            // when: unitFileName.currentIndex != -1 &&
            when: !conn.units.units.includes(toolbar.unitName) && conn.unitFiles.has(toolbar.unitName)
            value: true
            restoreMode: Binding.RestoreBindingOrValue
        }
    }
}
