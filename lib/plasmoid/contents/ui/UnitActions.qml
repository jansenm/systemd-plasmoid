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

    property var enableUnitAction:  Action {
        text: i18n("Enable")
        enabled: false

        onTriggered: {
            conn.units.enableUnitFiles([model.Unit], false, true);
        }

        Binding on enabled {
            when: ["disabled", "enabled-runtime", "linked", "indirect"].indexOf(model.UnitFileState) >= 0
            value: true
            restoreMode: Binding.RestoreBindingOrValue
        }
    }

    property var disableUnitAction:  Action {
        text: i18n("Disable")
        enabled: false

        onTriggered: {
            conn.units.disableUnitFiles([model.Unit], false, true);
        }

        Binding on enabled {
            when: ["enabled", "enabled-runtime"].indexOf(model.UnitFileState) >= 0
            value: true
            restoreMode: Binding.RestoreBindingOrValue
        }
    }

    property var isolateUnitAction:  Action {
        text: i18n("Isolate")
        enabled: false

        onTriggered: {
            conn.units.isolateUnit("replace");
        }

        Binding on enabled {
            when: model.CanIsolate
            value: true
            restoreMode: Binding.RestoreBindingOrValue
        }
    }

    property var startUnitAction:  Action {
        text: i18n("Start")
        enabled: false

        onTriggered: {
            conn.units.startUnit(model.Unit, "replace");
        }

        Binding on enabled {
            when: model.CanStart && ( ["inactive", "failed"].indexOf(model.ActiveState) >= 0 );
            value: true
            restoreMode: Binding.RestoreBindingOrValue
        }
    }

    property var reloadUnitAction:  Action {
        text: i18n("Reload")
        enabled: false

        onTriggered: {
            conn.units.reloadUnit(model.Unit, "replace");
        }

        Binding on enabled {
            when: model.CanReload && ( ["running"].indexOf(model.SubState) >= 0 );
            value: true
            restoreMode: Binding.RestoreBindingOrValue
        }
    }

    property var removeUnitAction:  Action {
        text: i18n("Remove")

        onTriggered: {
            conn.units.removeUnit(model.Unit);
        }
    }

    property var restartUnitAction:  Action {
        text: i18n("Restart")
        enabled: false

        onTriggered: {
            conn.units.restartUnit(model.Unit, "replace");
        }

        Binding on enabled {
            when: model.CanStart && model.CanStop && ( ["active"].indexOf(model.ActiveState) >= 0 );
            value: true
            restoreMode: Binding.RestoreBindingOrValue
        }
    }

    property var stopUnitAction:  Action {
        text: i18n("Stop")
        enabled: false

        onTriggered: {
            conn.units.stopUnit(model.Unit, "replace");
        }

        Binding on enabled {
            when: model.CanStop && ( ["active"].indexOf(model.ActiveState) >= 0 );
            value: true
            restoreMode: Binding.RestoreBindingOrValue
        }
    }
}
