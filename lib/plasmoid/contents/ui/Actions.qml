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
            console.log("adding unit")
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
