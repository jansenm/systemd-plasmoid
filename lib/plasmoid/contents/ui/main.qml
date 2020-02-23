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
//import QtQuick.Controls 2.14
import org.kde.plasma.plasmoid 2.0
// import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.plasma.extras 2.0 as PlasmaExtra
// import org.kde.plasma.core 2.0 as PlasmaCore

import org.kde.systemd 0.1

Item {

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


    QMLDebugger {
        id: qmlDebugger
    }

    Connection {
        id: conn
    }

    ColumnLayout {
        anchors.fill: parent

        Toolbar { }

        // Or should we use PlasmaExtra.ScrollArea here? So Confusing.
        PlasmaExtra.ScrollArea {
            id: scroller
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            ListView {
                id: listview
                model: conn.units
                delegate: UnitItem {}
                Component.onCompleted: {
                    // TODO read this from a configuration file. How to make sure i can add two of these plasmoids with different configurations?
                    conn.units.units = [ "-.mount", "cups.service", "ddd.mount", "apache2.service" ];
                    conn.unitFiles.unitFilesChanged();
                }
            }
        }
    }

}
