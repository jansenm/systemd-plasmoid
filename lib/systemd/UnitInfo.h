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
#ifndef SYSTEMD_KDE_UNITINFO_H
#define SYSTEMD_KDE_UNITINFO_H

#include "systemd-qt_export.h"
#include <QtCore/QObject>
#include <QtDBus/QDBusReply>

namespace Systemd {


    class SYSTEMD_QT_EXPORT UnitInfo : public QObject {
    Q_OBJECT
        Q_PROPERTY(QString id
                           READ
                           id)

        class UnitInfoPrivate *d_ptr;

    public:
        /**
         * The unit state.
         *
         * > Units may be "active" (meaning started, bound, plugged in, ...,
         *depending on the unit type, see below), or > "inactive" (meaning stopped,
         *unbound, unplugged, ...), as well as in the process of being activated or
         * > deactivated, i.e. between the two states (these states are called
         *"activating", "deactivating"). A special > "failed" state is available as
         *well, which is very similar to "inactive" and is entered when the service
         *failed > in some way (process returned error code on exit, or crashed, or
         *an operation timed out). If this state is > entered, the cause will be
         *logged, for later reference. Note that the various unit types may have a
         *number of > additional substates, which are mapped to the five generalized
         *unit states described here.
         **/
        enum State {
            active,       //!< The unit is active
            inactive,     //!< The unit is inactive
            activating,   //!< The unit is activating
            deactivating, //!< The unit is deactivating
            failed        //!< The unit failed
        };

        Q_ENUM(State)

    public:
        /**
         * Create a new unit object.
         *
         * @param reply         The dbus reply object to initialize from
         * @param parent        The parent object or nullptr
         */
        UnitInfo(const UnitInfoPrivate &p, QObject *parent = nullptr);

        /**
         * Destroy the unit object.
         */
        ~UnitInfo() override;

        QString id() const;

    private:
        Q_DECLARE_PRIVATE(UnitInfo)
    };

    typedef QList<UnitInfo *> UnitInfoList;

}

Q_DECLARE_METATYPE(Systemd::UnitInfoList)

#endif // SYSTEMD_KDE_UNITINFO_H
