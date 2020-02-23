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
