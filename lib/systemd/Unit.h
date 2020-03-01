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
#ifndef SYSTEMD_KDE_UNIT_H
#define SYSTEMD_KDE_UNIT_H

#include "systemd-qt_export.h"

#include <QtCore/QObject>
#include <QtDBus/QDBusReply>
#include <QDebug>

namespace Systemd {

    /**
     * A systemd.unit
     *
     * The unit is the »base class« for all systemd types.
     *
     * @see the systemd.unit man page
     * @see »systemctl --type help«
     */
    class SYSTEMD_QT_EXPORT Unit : public QObject {
    Q_OBJECT

        Q_PROPERTY(QString id
                           READ
                                   id
                           CONSTANT);

        Q_PROPERTY(QString activeState
                           READ
                                   activeState
                           NOTIFY
                           activeStateChanged);

        Q_PROPERTY(bool canIsolate
                           READ
                                   canIsolate
                           CONSTANT);

        Q_PROPERTY(bool canReload
                           READ
                                   canReload
                           CONSTANT);

        Q_PROPERTY(bool canStart
                           READ
                                   canStart
                           CONSTANT);

        Q_PROPERTY(bool canStop
                           READ
                                   canStop
                           CONSTANT);

        Q_PROPERTY(QString description
                           READ
                                   description
                           CONSTANT);

        Q_PROPERTY(QString fragmentPath
                           READ
                                   fragmentPath
                           CONSTANT);

        Q_PROPERTY(QString loadState
                           READ
                                   loadState
                           CONSTANT);

        Q_PROPERTY(QStringList names
                           READ
                                   names
                           CONSTANT);

        Q_PROPERTY(QString subState
                           READ
                                   subState
                           NOTIFY
                           subStateChanged);

        Q_PROPERTY(QString unitFileState
                           READ
                                   unitFileState
                           CONSTANT);

        struct UnitPrivate *d_ptr;

    public:
        Unit(const QDBusConnection &bus,
             const QDBusObjectPath &path,
             QObject *parent = nullptr);

        /**
         * Destroy the unit object.
         */
        ~Unit() override;


        /**
         * Returns true if this is a valid reference to a remote object. It returns false if there was an error during
         * the creation of this interface (for instance, if the remote application does not exist).
         *
         * @note: when dealing with remote objects, it is not always possible to determine if it exists when creating a
         * QDBusInterface.
         *
         * @return
         *
         * @see QDBusAbstractInterface::isValid
         */
        bool isValid() const;

        /**
         * Returns the unis activate state.
         *
         * Possible values:
         *
         *   * active
         *   * activating
         *   * inactive
         *   * deactivating
         *   * reloading
         *   * failed
         *
         * @return the units active state
         *
         * @see »systemctl --state help«
         */
        QString activeState() const;

        /**
         * Does the unit support the isolate operation?
         */
        bool canIsolate() const;

        /**
         * Does the unit support the reload operation?
         */
        bool canReload() const;

        /**
         * Does the unit support the start operation?
         */
        bool canStart() const;

        /**
         * Does the unit support the stop operation?
         */
        bool canStop() const;

        /**
         * A free-form string describing the unit.
         *
         * @return the description
         */
        QString description() const;

        /**
         * The unit file path this unit was read from, if there is any
         *
         * @return the unit file path or an empty string.
         */
        QString fragmentPath() const;

        /**
         * The primary name of the unit.
         *
         * @return the unit id.
         */
        QString id() const;

        /**
         * The units load state.
         *
         * Possible values for loaded units (there are more)
         *
         * * stub
         * * loaded
         * * not-found
         * * error
         * * merged
         * * masked
         *
         * @return the units load state:
         *
         * @see »systemctl --state help«
         * @see »man systemctl« for »is-enabled« command
         */
        QString loadState() const;

        /**
         * All names of the unit, including the primary name that is also exposed in Id
         *
         * @return all known names for the unit.
         */
        QStringList names() const;

        /**
         * The units substate.
         *
         * This state is dependend on the type of the unit.
         *
         * @return the substate.
         *
         * @see »systemctl --state help«
         */
        QString subState() const;

        /**
         * The unitfiles state.
         *
         * @return the state.
         *
         * @see »systemctl --state help«
         */
        QString unitFileState() const;

    signals:

        void activeStateChanged(const QString &newActiveState) const;

        void resultChanged(const QString &result) const;

        void statusErrnoChanged(const int &errNo) const;

        void statusTextChanged(const QString &error) const;

        void subStateChanged(const QString &newSubState) const;

        void changed(const Unit &unit) const;

    private slots:

        void propertiesChanged(const QString &interface, const QMap<QString, QVariant> changed, QStringList invalid);


    private:
        Q_DECLARE_PRIVATE(Unit);
    };

    typedef QList<Unit *> UnitList;

}

Q_DECLARE_METATYPE(Systemd::UnitList);

#endif // SYSTEMD_KDE_UNIT_H
