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

#include "UnitFileModel.h"

using Systemd::Manager;

class UnitFileModelPrivate {
public:

    explicit UnitFileModelPrivate(Manager *manager)
            : m(manager) {}

    Manager *m;
    Systemd::UnitFileList m_unitFiles;
};

UnitFileModel::UnitFileModel(Systemd::Manager *manager, QObject *parent) : QAbstractListModel(parent),
                                                                           d_ptr(new UnitFileModelPrivate(manager)) {
    Q_D(const UnitFileModel);
    connect(d->m, &Systemd::Manager::unitFilesChanged, this, &UnitFileModel::unitFilesChanged);
}

UnitFileModel::~UnitFileModel() {
    Q_D(UnitFileModel);
    qDeleteAll(d->m_unitFiles);
    d->m_unitFiles.clear(); // Don't think its really needed.
    delete d_ptr;
}

QVariant UnitFileModel::data(const QModelIndex &index, int role) const {
    Q_D(const UnitFileModel);

    if (!index.isValid()) {
        // Just wanna know if this can happen
        Q_ASSERT(index.isValid());
        return QVariant();
    }

    const int row = index.row();

    switch (role) {
        case Roles::Path:
            return d->m_unitFiles.at(row)->id();
        case Roles::Name:
            return d->m_unitFiles.at(row)->name();
        case Roles::State:
            return d->m_unitFiles.at(row)->state();
        default:
            qWarning() << "Got unhandled role " << role;
            Q_ASSERT(false);
    }
    return QVariant();
}

QHash<int, QByteArray> UnitFileModel::roleNames() const {
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
    roles[Roles::Path] = "Path";
    roles[Roles::Name] = "Name";
    roles[Roles::State] = "State";
    return roles;
}

int UnitFileModel::rowCount(const QModelIndex &parent) const {
    Q_D(const UnitFileModel);
    Q_UNUSED(parent)
    return d->m_unitFiles.count();
}

void UnitFileModel::unitFilesChanged() {
    Q_D(UnitFileModel);
    // First free all units currently loaded
    qDeleteAll(d->m_unitFiles);
    d->m_unitFiles.clear();

    // Then load them again
    auto call = d->m->listUnitFiles(
            [this](QDBusPendingReply<> reply,
                   Systemd::UnitFileList list) -> void {
                Q_D(UnitFileModel);
                if (reply.isValid()) {
                    std::sort(list.begin(), list.end(),
                              [](Systemd::UnitFile *a, Systemd::UnitFile *b) { return a->name()
                              < b->name(); });
                    beginInsertRows(QModelIndex(), 0, list.size());
                    d->m_unitFiles = list;
                    endInsertRows();
                } else {
                    qWarning() << "call to listUnitFiles failed:" << reply.error().message();
                }
            }, this);
}

