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
#ifndef SYSTEMD_KDE_UNITFILEMODEL_H
#define SYSTEMD_KDE_UNITFILEMODEL_H

#include "systemd-qt_export.h"
#include <QtCore/QAbstractListModel>
#include <Manager.h>

class SYSTEMD_QT_EXPORT UnitFileModel : public QAbstractListModel {
Q_OBJECT

public:

    enum Roles {
        Path = Qt::UserRole + 1,
        Name,
        State,
    };
    Q_ENUM(Roles);

    explicit UnitFileModel(Systemd::Manager *manager, QObject *parent = nullptr);

    ~UnitFileModel() override;

    QVariant data(const QModelIndex &index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;

    int rowCount(const QModelIndex &parent) const override;

    // Check if the given service exists
    Q_INVOKABLE bool has(const QString & service) const;


public slots:

    void unitFilesChanged();

private:

    class UnitFileModelPrivate *d_ptr;
    Q_DECLARE_PRIVATE(UnitFileModel);
};


#endif //SYSTEMD_KDE_UNITFILEMODEL_H
