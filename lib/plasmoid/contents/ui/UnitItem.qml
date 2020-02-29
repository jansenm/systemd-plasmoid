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

import org.kde.plasma.components 3.0 as PlasmaComponents
import org.kde.plasma.extras 2.0 as PlasmaExtras


PlasmaComponents.ItemDelegate {
    id: unitItem

    height: unitRow.height + units.smallSpacing

    UnitActions { id: actions }

    MouseArea {
        id: mousearea
        anchors.fill: unitItem
        hoverEnabled: true
    }

    Rectangle{
        id: background
        anchors.fill: unitItem
        color: theme.backgroundColor
        opacity: 0.1
        z: -1
    }

    states: State {
        name: "Focused"
        when: mousearea.containsMouse
        PropertyChanges {
            target: background
            color: theme.viewHoverColor
        }
    }

    RowLayout {
        id: unitRow
        width: parent.width

        ColumnLayout {
            Layout.fillWidth: true

            RowLayout {
                Layout.fillWidth: true

                PlasmaComponents.Label {
                    text: model.Unit
                    font.bold: true
                    Layout.fillWidth: true
                }

                PlasmaComponents.Label {
                    text: model.ActiveState
                    font.pointSize: theme.smallestFont.pointSize
                }

                PlasmaComponents.Label {
                    text: model.LoadState
                    font.pointSize: theme.smallestFont.pointSize
                }

                PlasmaComponents.Label {
                    text: model.SubState
                    font.pointSize: theme.smallestFont.pointSize
                }
            }

            RowLayout {

                PlasmaComponents.Label {
                    Layout.fillWidth: true
                    text: model.Description
                    font.italic: true
                    font.pointSize: theme.smallestFont.pointSize
                }

            }
        }

        PlasmaComponents.ToolButton {
            id: unitMenu
            icon.name: "application-menu"

            onClicked: {
                    menu.open()
            }

            PlasmaComponents.Menu {
                id: menu

                PlasmaComponents.MenuItem {
                    action: actions.startUnitAction
                }

                PlasmaComponents.MenuItem {
                    action: actions.stopUnitAction
                }

                PlasmaComponents.MenuItem {
                    action: actions.reloadUnitAction
                }

                PlasmaComponents.MenuItem {
                    action: actions.isolateUnitAction
                }

                PlasmaComponents.MenuItem {
                    action: actions.reloadUnitAction
                }
            }
        }
    }


}
