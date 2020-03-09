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
import QtQuick 2.14
import QtQuick.Layouts 1.14
import QtQuick.Controls 2.14
import QtQml 2.14
import org.kde.plasma.plasmoid 2.0
import org.kde.plasma.extras 2.0 as PlasmaExtra

import org.kde.systemd 0.1

Item {

    function bustypeValue(name) {
        switch(name) {
            case "Session": return 0;
            case "System": default: return 1;
        }
    }

    function bustypeName(value) {
        switch(value) {
            case 0: return "Session";
            case 1: default: return "System";
        }
    }

    function configNeedsSaving() {
        plasmoid.configNeedsSaving();
    }

    function saveConfiguration() {
        plasmoid.configuration.units = conn.units.units;
        // First empty the event loop in case there are more changes to the configuration. Then trigger a save
        Qt.callLater(configNeedsSaving);
    }

    function changeConnection(connection) {
        plasmoid.configuration.bus = bustypeValue(connection);
        switch(connection) {
            case "Session":
                conn.setConnection(Connection.SessionBus);
                break;
            default:
                console.warn("Invalid value ", connection, " for bus encountered. Using system bus.");
            case "System":
                conn.setConnection(Connection.SystemBus);
        }
    }

    function loadConfiguration() {
        // This will trigger connectionChanged which does the rest.
        changeConnection(bustypeName(plasmoid.configuration.bus));
    }

    function connectionChanged() {
        listview.model = conn.units;
        toolbar.selectBus(bustypeName(plasmoid.configuration.bus));
        conn.units.units = plasmoid.configuration.units;
        conn.units.onUnitsChanged.connect(saveConfiguration);
        conn.unitFiles.unitFilesChanged();
    }

    // TODO: Add something useful here.
    Layout.minimumWidth: 400
    Layout.minimumHeight: 200

    Plasmoid.toolTipMainText: i18n("Systemd Units")
    // Plasmoid.toolTipSubText: networkStatus.activeConnections
    // Plasmoid.icon: connectionIconProvider.connectionTooltipIcon
    // Plasmoid.switchWidth: units.gridUnit * 10
    // Plasmoid.switchHeight: units.gridUnit * 10
    // Plasmoid.compactRepresentation: CompactRepresentation { }
    // Plasmoid.fullRepresentation: PopupDialog {
    //     id: dialogItem
    //     Layout.minimumWidth: units.iconSizes.medium * 10
    //     Layout.minimumHeight: units.gridUnit * 20
    //     anchors.fill: parent
    //     focus: true
    // }

    Actions     { id: actions }
    Connection  { id: conn }
    QMLDebugger { id: qmlDebugger }

    ColumnLayout {
        anchors.fill: parent

        Toolbar {
            id: toolbar
            Layout.fillWidth: true
        }

        // Or should we use PlasmaExtra.ScrollArea here? So Confusing.
        PlasmaExtra.ScrollArea {
            id: scroller
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            ListView {
                id: listview
                model: conn.units
                delegate: UnitItem {
                    width: scroller.width
                }
                Component.onCompleted: {
                    conn.onConnectionChanged.connect(connectionChanged);
                    loadConfiguration();
                    toolbar.busRequested.connect( changeConnection );
                }
            }
        }
    }
}
