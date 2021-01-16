/*
 * Systemd KDE - A plasma widget to control systemd services
 *
 * SPDX-FileCopyrightText: 2021 Michael Jansen <info@michael-jansen.biz>
 * SPDX-License-Identifier:  LGPL-2.1-or-later
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
#ifndef SYSTEMD_KDE_MANAGER_H
#define SYSTEMD_KDE_MANAGER_H

#include <QtCore/QObject>
#include <QtDBus/QDBusConnection>

#include "UnitFile.h"
#include "UnitInfo.h"
#include "Changes.h"
#include "systemd-qt_export.h"

namespace Systemd {

    class SYSTEMD_QT_EXPORT Manager : public QObject {
    Q_OBJECT
        Q_PROPERTY(QString version
                           READ
                                   version
                           CONSTANT)

    public:
        Manager(QDBusConnection conn, QObject *parent);

        ~Manager() override;

        QDBusConnection connection() const;

        /**
         * @warning systemd changes the files but not its state. Call reload() when finished.
         *
         * Also mask and unmask (from systemd kcm)
         */
        QDBusPendingCall enableUnitFiles(
                const QStringList &unitFiles,
                bool runtimeOnly,
                bool replace,
                std::function<void(const QDBusPendingReply<> reply,
                                   bool success,
                                   const ChangesList &changes)> callback = nullptr,
                QObject *parent = nullptr);

        /**
         * @warning systemd changes the files but not its state. Call reload() when finished.
         */
        QDBusPendingCall disableUnitFiles(
                const QStringList &unitFiles,
                bool runtimeOnly,
                std::function<void(const QDBusPendingReply<> reply,
                                   const ChangesList &changes)> callback = nullptr,
                QObject *parent = nullptr);

        QDBusPendingCall getUnitFileState(
                const QString &name,
                std::function<void(const QDBusPendingReply<> reply,
                                   const QString &state)>);

        QDBusPendingCall getUnitPath(
                const QString &name,
                std::function<void(const QDBusPendingReply<> reply,
                                   const QDBusObjectPath &path)>);

        QDBusPendingCall listUnits(
                std::function<void(const QDBusPendingReply<> reply, UnitInfoList units)>
                callback,
                QObject *parent);

        QDBusPendingCall listUnitFiles(
                std::function<void(const QDBusPendingReply<> reply,
                                   UnitFileList unitFiles)> callback,
                QObject *parent);

        QDBusPendingCall loadUnitPath(
                const QString &name,
                std::function<void(const QDBusPendingReply<> reply,
                                   const QDBusObjectPath &path)>);

        QDBusPendingCall reload();

        QDBusPendingCall reloadUnit(
                const QString &name,
                const QString &mode,
                std::function<void(const QDBusPendingReply<> reply,
                                   const QDBusObjectPath &path)> callback = nullptr);

        QDBusPendingCall restartUnit(
                const QString &name,
                const QString &mode,
                std::function<void(const QDBusPendingReply<> reply,
                                   const QDBusObjectPath &path)> callback = nullptr);

        QDBusPendingCall startUnit(
                const QString &name,
                const QString &mode,
                std::function<void(const QDBusPendingReply<> reply,
                                   const QDBusObjectPath &path)> callback = nullptr);

        QDBusPendingCall stopUnit(
                const QString &name,
                const QString &mode,
                std::function<void(const QDBusPendingReply<> reply,
                                   const QDBusObjectPath &path)> callback = nullptr);

        QDBusPendingCall subscribe();

        QDBusPendingCall unsubscribe();

        QString version() const;

    Q_SIGNALS:

        void unitNew(const QString &unit, const QDBusObjectPath &path) const;

        void unitRemoved(const QString &unit, const QDBusObjectPath &path) const;

        void unitFilesChanged() const;

    private:

        struct ManagerPrivate *d_ptr;

        Q_DECLARE_PRIVATE(Manager)
    };

}

#endif // SYSTEMD_KDE_MANAGER_H
