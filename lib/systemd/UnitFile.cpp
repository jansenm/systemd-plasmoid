/*
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
#include "UnitFile_p.h"
#include "debug.h"

#include <QtDBus/QtDBus>

using namespace Systemd;

QDBusArgument&
Systemd::operator<<(QDBusArgument& argument, const UnitFilePrivate& unit)
{
    argument.beginStructure();
    // clang-format off
    argument << unit.id
             << unit.state;
    // clang-format on
    argument.endStructure();
    return argument;
}

const QDBusArgument&
Systemd::operator>>(const QDBusArgument& argument, UnitFilePrivate& unit)
{
    argument.beginStructure();
    // clang-format off
    argument >> unit.id
             >> unit.state;
    // clang-format on
    argument.endStructure();
    return argument;
}

UnitFile::UnitFile(const UnitFilePrivate& d_ptr, QObject* parent)
  : QObject(parent)
  , d_ptr(new UnitFilePrivate(d_ptr))
{}

UnitFile::~UnitFile() {
    delete d_ptr;
}

QString
UnitFile::id() const
{
    Q_D(const UnitFile);
    return d->id;
}

QString
UnitFile::name() const
{
    Q_D(const UnitFile);
    return d->id.section('/', -1);
}

QString
UnitFile::state() const
{
    Q_D(const UnitFile);
    return d->state;
}

