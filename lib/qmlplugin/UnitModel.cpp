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
#include "debug.h"
#include "UnitModel.h"

#include <Manager.h>
#include <QDBusConnection>
#include <QDBusPendingCallWatcher>
#include <QtCore/QThread>

using Systemd::Manager;

class UnitModelPrivate {
public:

    explicit UnitModelPrivate(Manager *manager)
            : m(manager) {}

    Manager *m;
    QList<QObject *> m_units;
    QStringList m_unitNames;
};

UnitModel::UnitModel(Manager *manager, QObject *parent)
        : QAbstractListModel(parent), d_ptr(new UnitModelPrivate(manager)) {
    Q_D(UnitModel);
    connect(d->m, &Systemd::Manager::unitNew, this, &UnitModel::unitNew);
    connect(d->m, &Systemd::Manager::unitRemoved, this, &UnitModel::unitRemoved);
}

UnitModel::~UnitModel() {
    Q_D(UnitModel);
    qDeleteAll(d->m_units);
    d->m_units.clear(); // Don't think its really needed.
    delete d_ptr;
}

QVariant
UnitModel::data(const QModelIndex &index, int role) const {
    Q_D(const UnitModel);

    if (!index.isValid()) {
        // Just wanna know if this can happen
        Q_ASSERT(index.isValid());
        return QVariant();
    }

    const int row = index.row();

    switch (role) {
        case Roles::Name:
            return d->m_units.at(row)->property("id");
        case Roles::ActiveState:
            return d->m_units.at(row)->property("activeState");
        case Roles::CanIsolate:
            return d->m_units.at(row)->property("canIsolate");
        case Roles::CanReload:
            return d->m_units.at(row)->property("canReload");
        case Roles::CanStart:
            return d->m_units.at(row)->property("canStart");
        case Roles::CanStop:
            return d->m_units.at(row)->property("canStop");
        case Roles::Description:
            return d->m_units.at(row)->property("description");
        case Roles::LoadState:
            return d->m_units.at(row)->property("loadState");
        case Roles::SubState:
            return d->m_units.at(row)->property("subState");
        case Roles::UnitFileState:
            return d->m_units.at(row)->property("unitFileState");
        default:
            qWarning() << "Got unhandled role " << role;
            Q_ASSERT(false);
    }

    return QVariant();
}

void
UnitModel::loadUnit(const QDBusObjectPath &unitPath) {
    Q_D(UnitModel);
    QDBusConnection bus = d->m->connection();
    auto *unit = new Systemd::Unit(bus, QDBusObjectPath(unitPath), this);
    beginInsertRows(QModelIndex(), 0, 0);
    d->m_units.prepend(unit);
    connect(unit, &Systemd::Unit::changed, this, &UnitModel::unitChanged);
    endInsertRows();
    d->m_unitNames.prepend(unit->id());
    emit unitsChanged();
}

void
UnitModel::loadUnit(const QString &unitName) {
    Q_D(UnitModel);

    auto call = d->m->loadUnitPath(
            unitName,
            [this, unitName](const QDBusPendingReply<QDBusObjectPath> &reply, const QDBusObjectPath &path) -> void {

                if (!reply.isValid()) {
                    // TODO: remove unit from unitlist. This means the name is invalid, not that the unit is not known.
                    qWarning() << "UnitModel::loadUnit(" << unitName << "): reply is not valid";
                }
                this->loadUnit(path);
            });
}

void UnitModel::reloadUnit(const QString &name, const QString &mode) {
    Q_D(UnitModel);
    QDBusPendingCall call = d->m->reloadUnit(name, mode);
}

void UnitModel::restartUnit(const QString &name, const QString &mode) {
    Q_D(UnitModel);
    QDBusPendingCall call = d->m->restartUnit(name, mode);
}


QHash<int, QByteArray>
UnitModel::roleNames() const {
    // Q_D(const UnitModel);
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
    roles[Roles::Name] = "Unit";
    roles[Roles::ActiveState] = "ActiveState";
    roles[Roles::CanIsolate] = "CanIsolate";
    roles[Roles::CanReload] = "CanReload";
    roles[Roles::CanStart] = "CanStart";
    roles[Roles::CanStop] = "CanStop";
    roles[Roles::Description] = "Description";
    roles[Roles::LoadState] = "LoadState";
    roles[Roles::SubState] = "SubState";
    roles[Roles::UnitFileState] = "UnitFileState";
    return roles;
}

int
UnitModel::rowCount(const QModelIndex &parent) const {
    Q_D(const UnitModel);
    Q_UNUSED(parent)
    return d->m_units.count();
}

void
UnitModel::setUnits(const QStringList &unitNames) {
    Q_D(UnitModel);
    if (d->m_unitNames == unitNames) {
        return;
    }

    d->m_unitNames = unitNames;

    for (const QString &unitName: unitNames) {
        loadUnit(unitName);
    }

    emit unitsChanged();
}

void
UnitModel::startUnit(const QString &name, const QString &mode) {
    Q_D(UnitModel);
    QDBusPendingCall call = d->m->startUnit(name, mode);
}

void UnitModel::stopUnit(const QString &name, const QString &mode) {
    Q_D(UnitModel);
    QDBusPendingCall call = d->m->stopUnit(name, mode);
}

void
UnitModel::unitChanged(const Systemd::Unit &unit) {
    Q_D(UnitModel);
    for (int i = 0; i < d->m_units.size(); ++i) {
        if (d->m_units.at(i)->property("id").toString() == unit.id()) {
            emit dataChanged(index(i, 0), index(i, 0));
            break;
        }
    }
}


void
UnitModel::unitNew(const QString &unit, const QDBusObjectPath &path) {
    Q_UNUSED(path);
    qDebug() << "unitNew(" << unit << ")";
}

void
UnitModel::unitRemoved(const QString &unit, const QDBusObjectPath &path) {
    Q_UNUSED(path);
    qDebug() << "unitRemoved(" << unit << ")";
}


QStringList
UnitModel::units() const {
    Q_D(const UnitModel);
    return d->m_unitNames;
}

