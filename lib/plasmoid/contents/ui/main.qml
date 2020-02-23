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
