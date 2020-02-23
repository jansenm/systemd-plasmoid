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
import QtQuick.Layouts 1.14

import org.kde.plasma.components 2.0 as PlasmaComponents

PlasmaComponents.ListItem {

    id: listitem

    // TODO listitem.width is to wide. Why? Can i get rid of that magic number?
    // TODO doesn't work anymore after i moved it to its own file ... why?
    // width: listitem.ListView.view.width - 20

    // Qt-Default: true, Plasma-Default: false, Developer Confusion: priceless. Enables the onClicked Signal
    enabled: true

    onClicked: {
        if (details.show) {
            details.visible = !details.visible;
        }
    }

    ColumnLayout {
        // How to make this full width
        width: listitem.width

        RowLayout {

            ColumnLayout {

                PlasmaComponents.Label {
                    Layout.fillWidth: true
                    text: model.Unit
                }

                PlasmaComponents.Label {
                    Layout.fillWidth: true
                    text: model.Description
                    font.pointSize: theme.smallestFont.pointSize

                }
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
            id: details
            visible: false

            property bool show: isolateButton.show ||
                      startButton.show ||
                      restartButton.show ||
                      stopButton.show ||
                      reloadButton.show;

            PlasmaComponents.Button {
                id: isolateButton
                text: "isolate"
                visible: show
                property bool show: {
                    return model.CanIsolate
                }
                onClicked: {
                    conn.units.isolateUnit("replace");
                }
            }

            PlasmaComponents.Button {
                id: startButton
                text: "start"
                visible: show
                property bool show: {
                    return model.CanStart && ( ["inactive", "failed"].indexOf(model.ActiveState) >= 0 );
                }
                onClicked: {
                    conn.units.startUnit(model.Unit, "replace");
                }
            }

            PlasmaComponents.Button {
                id: restartButton
                text: "restart"
                visible: show
                property bool show: {
                    return model.CanStart && model.CanStop && ( ["active"].indexOf(model.ActiveState) >= 0 );
                }
                onClicked: {
                    conn.units.restartUnit(model.Unit, "replace");
                }
            }

            PlasmaComponents.Button {
                id: stopButton
                text: "stop"
                visible: show
                property bool show: {
                    return model.CanStop && ( ["active"].indexOf(model.ActiveState) >= 0 );
                }
                onClicked: {
                    conn.units.stopUnit(model.Unit, "replace");
                }
            }

            PlasmaComponents.Button {
                id: reloadButton
                text: "reload"
                visible: show
                property bool show: {
                    return model.CanReload && ( ["running"].indexOf(model.SubState) >= 0 );
                }
                onClicked: {
                    conn.units.reloadUnit(model.Unit, "replace");
                }
            }
        }
    }
}
