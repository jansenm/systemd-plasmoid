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
#ifndef SYSTEMD_KDE_UNITMODEL_H
#define SYSTEMD_KDE_UNITMODEL_H

#include "systemd-qt_export.h"
#include <QtCore/QAbstractListModel>
#include <QtQml/QQmlListProperty>
#include <Unit.h>
#include <UnitInfo.h>
#include <Manager.h>

class SYSTEMD_QT_EXPORT UnitModel : public QAbstractListModel {
Q_OBJECT
    Q_PROPERTY(QStringList units
                       READ
                               units
                       WRITE
                               setUnits
                       NOTIFY
                       unitsChanged);

public:
    explicit UnitModel(Systemd::Manager *manager, QObject *parent = nullptr);

    ~UnitModel() override;

    struct Roles {
        enum {
            Name = Qt::UserRole + 1,
            ActiveState,
            CanIsolate,
            CanReload,
            CanStart,
            CanStop,
            Description,
            LoadState,
            SubState,
            UnitFileState
        };
    };

    int rowCount(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;

    void setUnits(const QStringList &units);

    QStringList units() const;

signals:

    void unitsChanged() const;


public slots:

    Q_INVOKABLE void addUnit(const QString &unitName);

    Q_INVOKABLE void removeUnit(const QString &unitName);

private slots:

    void unitNew(const QString &unit, const QDBusObjectPath &path);

    void unitRemoved(const QString &unit, const QDBusObjectPath &path);

    void unitChanged(const Systemd::Unit &unit);

public slots:
    Q_INVOKABLE void startUnit(const QString &name, const QString &mode);

    Q_INVOKABLE void stopUnit(const QString &name, const QString &mode);

    Q_INVOKABLE void reloadUnit(const QString &name, const QString &mode);

    Q_INVOKABLE void restartUnit(const QString &name, const QString &mode);

private:

    void unloadUnits();

    void addUnit(const QDBusObjectPath &unitPath);

    class UnitModelPrivate *d_ptr;

    Q_DECLARE_PRIVATE(UnitModel);
};

#endif // SYSTEMD_KDE_UNITMODEL_H
