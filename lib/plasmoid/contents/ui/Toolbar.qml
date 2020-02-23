import QtQuick 2.14
import QtQuick.Layouts 1.14
import QtQuick.Controls 2.14
import QtQml 2.14

import org.kde.plasma.components 2.0 as PlasmaComponents

import org.kde.systemd 0.1

PlasmaComponents.ToolBar {

    id: toolbar
    Layout.fillWidth: true

    tools: PlasmaComponents.ToolBarLayout {

        ComboBox {
            id: unitFileName
            Layout.fillWidth: true
            editable: true
            textRole: "Name"
            model: conn.unitFiles

            delegate: ItemDelegate {

                contentItem: Column {
                    Text {
                        bottomPadding: 1
                        text: "<b>%1</b><br><small>%2</small>".arg(model.Name).arg(model.State)
                    }
                }
            }
        }

        PlasmaComponents.ToolButton {
            id: addButton
            iconSource: "list-add"
            flat: true
            enabled: false

            Binding on enabled {
                // Enable it when something new is selected
                when: unitFileName.currentIndex != -1 && !conn.units.units.includes(unitFileName.currentText)
                value: true
                restoreMode: Binding.RestoreBindingOrValue
            }

            onClicked: {
                // Play it twice as safe. Make sure the unit isn't already added.
                if (!conn.units.units.includes(unitFileName.currentText)) {
                    conn.units.loadUnit(unitFileName.currentText);
                }
            }
        }

        PlasmaComponents.ToolButton {
            id: configureButton
            iconSource: "configure"
            flat: true
        }

    }
}
