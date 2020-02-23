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
