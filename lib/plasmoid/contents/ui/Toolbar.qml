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
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14
import QtQuick.Templates 2.7 as T

import org.kde.plasma.components 3.0 as PlasmaComponents3

PlasmaComponents3.ToolBar {
    id: toolbar

    property string unitName: unitNameField.text

    signal busRequested(string bus)

    function selectBus(busName) {
        bus.currentIndex = bus.find(busName)
    }

    RowLayout {
        width: toolbar.width

        PlasmaComponents3.ComboBox {
            id: bus
            model: ["Session", "System"]
            onActivated: {
                toolbar.busRequested(bus.currentText);
            }
        }

        PlasmaComponents3.TextField {
            id: unitNameField
            placeholderText: i18n("Add systemd unit")
            Layout.fillWidth: true
        }

        PlasmaComponents3.ToolButton {
            id: addButton
            text: ""        // fixme Neded because the plasma toolbutton prior to an currently (29. Feb. 2020) unreleased version
                            // Ignore display and only check if text is filled. Using action fills it.
            display: T.AbstractButton.IconOnly
            action: actions.addUnitAction
        }
    }
}
