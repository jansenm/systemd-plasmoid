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
#include "Manager.h"
#include "UnitInfo_p.h"
#include "UnitFile_p.h"

#include <QtDBus/QDBusInterface>
#include <QtDBus/QtDBus>

using namespace Systemd;

struct Systemd::ManagerPrivate {
    ManagerPrivate(const QDBusConnection &bus, QObject *parent)
            : iface(QLatin1String("org.freedesktop.systemd1"),
                    "/org/freedesktop/systemd1",
                    "org.freedesktop.systemd1.Manager",
                    bus,
                    parent) {
        Q_ASSERT(iface.isValid());
    }

    QDBusInterface iface;
};

Manager::Manager(QDBusConnection bus, QObject *parent)
        : QObject(parent), d_ptr(new ManagerPrivate(bus, this)) {
    Q_D(Manager);

    qDBusRegisterMetaType<UnitInfoPrivate>();
    qDBusRegisterMetaType<UnitInfoPrivateList>();
    qDBusRegisterMetaType<UnitFilePrivate>();
    qDBusRegisterMetaType<UnitFilePrivateList>();

    connect(&d->iface, SIGNAL(UnitNew(QString, QDBusObjectPath)),
            this, SIGNAL(unitNew(QString, QDBusObjectPath)));
    connect(&d->iface, SIGNAL(UnitRemoved(QString, QDBusObjectPath)),
            this, SIGNAL(unitRemoved(QString, QDBusObjectPath)));
}

Manager::~Manager() {
    delete d_ptr;
}

QDBusConnection
Manager::connection() const {
    Q_D(const Manager);
    return d->iface.connection();
}

QDBusPendingCall
Manager::getUnitFileState(const QString &name, std::function<void(const QDBusPendingReply<> reply,
                                                                  const QString &)> callback) {
    Q_D(Manager);

    if (!d->iface.isValid()) {
        Q_ASSERT(d->iface.isValid());
        return QDBusPendingCall::fromError(d->iface.lastError());
    }

    QDBusPendingCall call = d->iface.asyncCall("GetUnitFileState", name);
    if (!callback) {
        return call;
    }

    auto *watcher = new QDBusPendingCallWatcher(call, this);
    connect(watcher,
            &QDBusPendingCallWatcher::finished,
            [callback](QDBusPendingCallWatcher *watcher) {
                QDBusPendingReply<QString> reply = *watcher;
                callback(reply, reply.value());
                watcher->deleteLater();
            });
    return call;
}

QDBusPendingCall
Manager::getUnitPath(const QString &name,
                     std::function<void(const QDBusPendingReply<>, const QDBusObjectPath &)> callback) {
    Q_D(Manager);

    if (!d->iface.isValid()) {
        Q_ASSERT(d->iface.isValid());
        return QDBusPendingCall::fromError(d->iface.lastError());
    }

    QDBusPendingCall call = d->iface.asyncCall("GetUnit", name);
    if (!callback) {
        return call;
    }

    auto *watcher = new QDBusPendingCallWatcher(call, this);
    connect(watcher,
            &QDBusPendingCallWatcher::finished,
            [callback](QDBusPendingCallWatcher *watcher) {
                QDBusPendingReply<QDBusObjectPath> reply = *watcher;
                callback(reply, reply.value());
                watcher->deleteLater();
            });
    return call;
}

QDBusPendingCall
Manager::listUnits(std::function<void(const QDBusPendingReply<>, UnitInfoList)> callback, QObject *parent) {
    Q_D(Manager);

    if (!d->iface.isValid()) {
        Q_ASSERT(d->iface.isValid());
        return QDBusPendingCall::fromError(d->iface.lastError());
    }

    QDBusPendingCall call = d->iface.asyncCall("ListUnits");
    auto *watcher = new QDBusPendingCallWatcher(call, this);
    connect(watcher,
            &QDBusPendingCallWatcher::finished,
            [parent, callback](QDBusPendingCallWatcher *watcher) {
                UnitInfoList list;
                QDBusPendingReply<UnitInfoPrivateList> reply = *watcher;
                        foreach (UnitInfoPrivate unit, reply.value()) {
                        list.append(new UnitInfo(unit, parent));
                    }
                callback(reply, list);
                watcher->deleteLater();
            });
    return call;
}

QDBusPendingCall
Manager::listUnitFiles(std::function<void(const QDBusPendingReply<>, UnitFileList)> callback,
                       QObject *parent) {
    Q_D(Manager);

    if (!d->iface.isValid()) {
        Q_ASSERT(d->iface.isValid());
        return QDBusPendingCall::fromError(d->iface.lastError());
    }

    QDBusPendingCall call = d->iface.asyncCall("ListUnitFiles");
    if (!callback) {
        return call;
    }

    auto *watcher = new QDBusPendingCallWatcher(call, this);
    connect(watcher,
            &QDBusPendingCallWatcher::finished,
            [parent, callback](QDBusPendingCallWatcher *watcher) {
                UnitFileList list;
                QDBusPendingReply<UnitFilePrivateList> reply = *watcher;
                        foreach (UnitFilePrivate unit, reply.value()) {
                        list.append(new UnitFile(unit, parent));
                    }
                callback(reply, list);
                watcher->deleteLater();
            });
    return call;
}

QDBusPendingCall
Manager::loadUnitPath(const QString &name,
                      std::function<void(const QDBusPendingReply<>, const QDBusObjectPath &)> callback) {
    Q_D(Manager);

    if (!d->iface.isValid()) {
        Q_ASSERT(d->iface.isValid());
        return QDBusPendingCall::fromError(d->iface.lastError());
    }

    QDBusPendingCall call = d->iface.asyncCall("LoadUnit", name);
    if (!callback) {
        return call;
    }

    auto *watcher = new QDBusPendingCallWatcher(call, this);
    connect(watcher,
            &QDBusPendingCallWatcher::finished,
            [callback](QDBusPendingCallWatcher *watcher) {
                QDBusPendingReply<QDBusObjectPath> reply = *watcher;
                callback(reply, reply.value());
                watcher->deleteLater();
            });
    return call;
}

QDBusPendingCall
Manager::reloadUnit(
        const QString &name,
        const QString &mode,
        std::function<void(const QDBusPendingReply<> reply,
                           const QDBusObjectPath &path)> callback) {
    Q_D(Manager);

    if (!d->iface.isValid()) {
        Q_ASSERT(d->iface.isValid());
        return QDBusPendingCall::fromError(d->iface.lastError());
    }

    QDBusPendingCall call = d->iface.asyncCall("ReloadUnit", name, mode);
    if (!callback) {
        return call;
    }

    auto *watcher = new QDBusPendingCallWatcher(call, this);
    connect(watcher,
            &QDBusPendingCallWatcher::finished,
            [callback](QDBusPendingCallWatcher *watcher) {
                QDBusPendingReply<QDBusObjectPath> reply = *watcher;
                callback(reply, reply.value());
                watcher->deleteLater();
            });
    return call;
}

QDBusPendingCall
Manager::restartUnit(
        const QString &name,
        const QString &mode,
        std::function<void(const QDBusPendingReply<> reply,
                           const QDBusObjectPath &path)> callback) {
    Q_D(Manager);

    if (!d->iface.isValid()) {
        Q_ASSERT(d->iface.isValid());
        return QDBusPendingCall::fromError(d->iface.lastError());
    }

    QDBusPendingCall call = d->iface.asyncCall("RestartUnit", name, mode);
    if (!callback) {
        return call;
    }
    auto *watcher = new QDBusPendingCallWatcher(call, this);
    connect(watcher,
            &QDBusPendingCallWatcher::finished,
            [callback](QDBusPendingCallWatcher *watcher) {
                QDBusPendingReply<QDBusObjectPath> reply = *watcher;
                callback(reply, reply.value());
                watcher->deleteLater();
            });
    return call;

}

QDBusPendingCall
Manager::startUnit(
        const QString &name,
        const QString &mode,
        std::function<void(const QDBusPendingReply<> reply,
                           const QDBusObjectPath &path)> callback) {
    Q_D(Manager);

    if (!d->iface.isValid()) {
        Q_ASSERT(d->iface.isValid());
        return QDBusPendingCall::fromError(d->iface.lastError());
    }

    QDBusPendingCall call = d->iface.asyncCall("StartUnit", name, mode);
    if (!callback) {
        return call;
    }

    auto *watcher = new QDBusPendingCallWatcher(call, this);
    connect(watcher,
            &QDBusPendingCallWatcher::finished,
            [callback](QDBusPendingCallWatcher *watcher) {
                QDBusPendingReply<QDBusObjectPath> reply = *watcher;
                callback(reply, reply.value());
                watcher->deleteLater();
            });
    return call;
}

QDBusPendingCall
Manager::stopUnit(
        const QString &name,
        const QString &mode,
        std::function<void(const QDBusPendingReply<> reply,
                           const QDBusObjectPath &path)> callback) {
    Q_D(Manager);

    if (!d->iface.isValid()) {
        Q_ASSERT(d->iface.isValid());
        return QDBusPendingCall::fromError(d->iface.lastError());
    }

    QDBusPendingCall call = d->iface.asyncCall("StopUnit", name, mode);
    if (!callback) {
        return call;
    }

    auto *watcher = new QDBusPendingCallWatcher(call, this);
    connect(watcher,
            &QDBusPendingCallWatcher::finished,
            [callback](QDBusPendingCallWatcher *watcher) {
                QDBusPendingReply<QDBusObjectPath> reply = *watcher;
                callback(reply, reply.value());
                watcher->deleteLater();
            });
    return call;
}

QString
Manager::version() const {
    Q_D(const Manager);
    return d_ptr->iface.property("Version").toString();
}

