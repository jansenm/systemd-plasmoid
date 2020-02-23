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
#include "Unit.h"
#include "debug.h"

#include <QtDBus/QtDBus>

using namespace Systemd;

struct Systemd::UnitPrivate {
    UnitPrivate(const QDBusConnection &bus,
                QDBusObjectPath path,
                QObject *parent)
            : unit_iface(QLatin1String("org.freedesktop.systemd1"),
                         path.path(),
                         "org.freedesktop.systemd1.Unit",
                         bus,
                         parent) {}

    QDBusInterface unit_iface;
};

Unit::Unit(const QDBusConnection &bus,
           const QDBusObjectPath &path,
           QObject *parent)
        : QObject(parent), d_ptr(new UnitPrivate(bus, path, this)) {
    Q_D(Unit);

    d->unit_iface.connection().connect("org.freedesktop.systemd1", path.path(), "org.freedesktop.DBus.Properties",
                                       "PropertiesChanged", this,
                                       SLOT(propertiesChanged(QString, QVariantMap, QStringList)));
}

Unit::~Unit() {
    delete d_ptr;
}

QString
Unit::activeState() const {
    Q_D(const Unit);
    return d->unit_iface.property("ActiveState").toString();
}

bool Unit::canIsolate() const {
    Q_D(const Unit);
    return d->unit_iface.property("CanIsolate").toBool();
}

bool Unit::canReload() const {
    Q_D(const Unit);
    return d->unit_iface.property("CanReload").toBool();
}

bool Unit::canStart() const {
    Q_D(const Unit);
    return d->unit_iface.property("CanStart").toBool();
}

bool Unit::canStop() const {
    Q_D(const Unit);
    return d->unit_iface.property("CanStop").toBool();
}

QString
Unit::description() const {
    Q_D(const Unit);
    return d->unit_iface.property("Description").toString();
}

QString
Unit::fragmentPath() const {
    Q_D(const Unit);
    return d->unit_iface.property("FragmentPath").toString();
}

QString
Unit::id() const {
    Q_D(const Unit);
    return d->unit_iface.property("Id").toString();
}

bool
Unit::isValid() const {
    Q_D(const Unit);
    return d->unit_iface.isValid();
}

QString
Unit::loadState() const {
    Q_D(const Unit);
    return d->unit_iface.property("LoadState").toString();
}

QStringList
Unit::names() const {
    Q_D(const Unit);
    return d->unit_iface.property("Names").toStringList();
}


void
Unit::propertiesChanged(const QString &interface, const QVariantMap changed, QStringList invalid) {
    // TODO : handle invalid
    Q_UNUSED(interface);
    Q_UNUSED(invalid);

    QVariantMap::const_iterator it = changed.begin();
    while(it != changed.end()) {
        if (it.key() == "loadState") {
            emit loadStateChanged(it.value().toString());
        } else if (it.key() == "activeState") {
            emit activeStateChanged(it.value().toString());
        }
        // TODO handle the rest
        ++it;
    }

    emit this->changed(*this);
}

QString
Unit::subState() const {
    Q_D(const Unit);
    return d->unit_iface.property("SubState").toString();
}

QString
Unit::unitFileState() const {
    Q_D(const Unit);
    return d->unit_iface.property("UnitFileState").toString();
}
