//
// Created by mjansen on 1/29/20.
//

#include "QmlPlugin.h"
#include "Connection.h"
#include "QMLDebugger.h"
#include "UnitFileModel.h"
#include <QtQml/qqml.h>
#include <QtCore/QSortFilterProxyModel>

void
QmlPlugin::registerTypes(const char* uri)
{
    qmlRegisterType<Connection>(uri, 0, 1, "Connection");
    qmlRegisterType<QMLDebugger>(uri, 0, 1, "QMLDebugger");
    qmlRegisterUncreatableType<UnitFileModel>(uri, 0, 1, "UnitFileModel", "enum");
}
