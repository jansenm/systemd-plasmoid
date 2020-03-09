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
