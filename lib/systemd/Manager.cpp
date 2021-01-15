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
#include "Changes.h"
#include "Changes_p.h"
#include "UnitFile_p.h"
#include "UnitInfo_p.h"

#include <QtDBus/QDBusInterface>
#include <QtDBus/QtDBus>

using namespace Systemd;

struct Systemd::ManagerPrivate
{
    ManagerPrivate(const QDBusConnection& bus, QObject* parent)
      : iface(QLatin1String("org.freedesktop.systemd1"),
              "/org/freedesktop/systemd1",
              "org.freedesktop.systemd1.Manager",
              bus,
              parent)
    {
        Q_ASSERT(iface.isValid());
    }

    QDBusInterface iface;
};

Manager::Manager(QDBusConnection bus, QObject* parent)
  : QObject(parent)
  , d_ptr(new ManagerPrivate(bus, this))
{
    Q_D(Manager);

    qDBusRegisterMetaType<UnitInfoPrivate>();
    qDBusRegisterMetaType<UnitInfoPrivateList>();
    qDBusRegisterMetaType<UnitFilePrivate>();
    qDBusRegisterMetaType<UnitFilePrivateList>();
    qDBusRegisterMetaType<ChangesPrivate>();
    qDBusRegisterMetaType<ChangesPrivateList>();

    subscribe();

    connect(&d->iface,
            SIGNAL(UnitNew(QString, QDBusObjectPath)),
            this,
            SIGNAL(unitNew(QString, QDBusObjectPath)));
    connect(&d->iface,
            SIGNAL(UnitRemoved(QString, QDBusObjectPath)),
            this,
            SIGNAL(unitRemoved(QString, QDBusObjectPath)));
    connect(
      &d->iface, SIGNAL(UnitFilesChanged()), this, SIGNAL(unitFilesChanged()));
}

Manager::~Manager()
{
    delete d_ptr;
}

QDBusConnection
Manager::connection() const
{
    Q_D(const Manager);
    return d->iface.connection();
}

QDBusPendingCall
Manager::enableUnitFiles(
  const QStringList& unitFiles,
  bool runtimeOnly,
  bool replace,
  std::function<void(const QDBusPendingReply<> reply,
                     // TODO is this really success here?
                     bool success,
                     const ChangesList& changes)> callback,
  QObject* parent)
{
    Q_D(Manager);

    if (!d->iface.isValid()) {
        Q_ASSERT(d->iface.isValid());
        return QDBusPendingCall::fromError(d->iface.lastError());
    }

    QDBusMessage message = QDBusMessage::createMethodCall(d->iface.service(),
                                                          d->iface.path(),
                                                          d->iface.interface(),
                                                          "EnableUnitFiles");
    message.setArguments(QVariantList({ unitFiles, runtimeOnly, replace }));
    // TODO: Why is it needed here but not used for start/stop/reload?
    message.setInteractiveAuthorizationAllowed(true);
    QDBusPendingCall call = QDBusConnection::systemBus().asyncCall(message);

    if (!callback) {
        return call;
    }

    auto* watcher = new QDBusPendingCallWatcher(call, this);
    connect(watcher,
            &QDBusPendingCallWatcher::finished,
            [callback, parent](QDBusPendingCallWatcher* watcher) {
                QDBusPendingReply<bool, ChangesPrivateList> reply = *watcher;
                ChangesList list;
                Q_FOREACH (auto change,
                           reply.argumentAt(1).value<ChangesPrivateList>()) {
                    list.append(new Changes(change, parent));
                }
                callback(reply,
                         reply.argumentAt(0).toBool(),
                         reply.argumentAt(1).value<ChangesList>());
                watcher->deleteLater();
            });
    return call;
}

QDBusPendingCall
Manager::disableUnitFiles(
  const QStringList& unitFiles,
  bool runtimeOnly,
  std::function<void(const QDBusPendingReply<> reply,
                     const ChangesList& changes)> callback,
  QObject* parent)
{
    Q_D(Manager);

    if (!d->iface.isValid()) {
        Q_ASSERT(d->iface.isValid());
        return QDBusPendingCall::fromError(d->iface.lastError());
    }

    QDBusMessage message = QDBusMessage::createMethodCall(d->iface.service(),
                                                          d->iface.path(),
                                                          d->iface.interface(),
                                                          "DisableUnitFiles");
    message.setArguments(QVariantList({ unitFiles, runtimeOnly }));
    // TODO: Why is it needed here but not used for start/stop/reload?
    message.setInteractiveAuthorizationAllowed(true);
    QDBusPendingCall call = QDBusConnection::systemBus().asyncCall(message);
    if (!callback) {
        return call;
    }

    auto* watcher = new QDBusPendingCallWatcher(call, this);
    connect(watcher,
            &QDBusPendingCallWatcher::finished,
            [parent, callback](QDBusPendingCallWatcher* watcher) {
                QDBusPendingReply<ChangesPrivateList> reply = *watcher;
                ChangesList list;
                Q_FOREACH (auto change,
                           reply.argumentAt(0).value<ChangesPrivateList>()) {
                    list.append(new Changes(change, parent));
                }
                callback(reply, reply.argumentAt(0).value<ChangesList>());
                watcher->deleteLater();
            });
    return call;
}

QDBusPendingCall
Manager::getUnitFileState(
  const QString& name,
  std::function<void(const QDBusPendingReply<> reply, const QString&)> callback)
{
    Q_D(Manager);

    if (!d->iface.isValid()) {
        Q_ASSERT(d->iface.isValid());
        return QDBusPendingCall::fromError(d->iface.lastError());
    }

    QDBusPendingCall call = d->iface.asyncCall("GetUnitFileState", name);
    if (!callback) {
        return call;
    }

    auto* watcher = new QDBusPendingCallWatcher(call, this);
    connect(watcher,
            &QDBusPendingCallWatcher::finished,
            [callback](QDBusPendingCallWatcher* watcher) {
                QDBusPendingReply<QString> reply = *watcher;
                callback(reply, reply.value());
                watcher->deleteLater();
            });
    return call;
}

QDBusPendingCall
Manager::getUnitPath(const QString& name,
                     std::function<void(const QDBusPendingReply<>,
                                        const QDBusObjectPath&)> callback)
{
    Q_D(Manager);

    if (!d->iface.isValid()) {
        Q_ASSERT(d->iface.isValid());
        return QDBusPendingCall::fromError(d->iface.lastError());
    }

    QDBusPendingCall call = d->iface.asyncCall("GetUnit", name);
    if (!callback) {
        return call;
    }

    auto* watcher = new QDBusPendingCallWatcher(call, this);
    connect(watcher,
            &QDBusPendingCallWatcher::finished,
            [callback](QDBusPendingCallWatcher* watcher) {
                QDBusPendingReply<QDBusObjectPath> reply = *watcher;
                callback(reply, reply.value());
                watcher->deleteLater();
            });
    return call;
}

QDBusPendingCall
Manager::listUnits(
  std::function<void(const QDBusPendingReply<>, UnitInfoList)> callback,
  QObject* parent)
{
    Q_D(Manager);

    if (!d->iface.isValid()) {
        Q_ASSERT(d->iface.isValid());
        return QDBusPendingCall::fromError(d->iface.lastError());
    }

    QDBusPendingCall call = d->iface.asyncCall("ListUnits");
    auto* watcher = new QDBusPendingCallWatcher(call, this);
    connect(watcher,
            &QDBusPendingCallWatcher::finished,
            [parent, callback](QDBusPendingCallWatcher* watcher) {
                UnitInfoList list;
                QDBusPendingReply<UnitInfoPrivateList> reply = *watcher;
                Q_FOREACH (UnitInfoPrivate unit, reply.value()) {
                    list.append(new UnitInfo(unit, parent));
                }
                callback(reply, list);
                watcher->deleteLater();
            });
    return call;
}

QDBusPendingCall
Manager::listUnitFiles(
  std::function<void(const QDBusPendingReply<>, UnitFileList)> callback,
  QObject* parent)
{
    Q_D(Manager);

    if (!d->iface.isValid()) {
        Q_ASSERT(d->iface.isValid());
        return QDBusPendingCall::fromError(d->iface.lastError());
    }

    QDBusPendingCall call = d->iface.asyncCall("ListUnitFiles");
    if (!callback) {
        return call;
    }

    auto* watcher = new QDBusPendingCallWatcher(call, this);
    connect(watcher,
            &QDBusPendingCallWatcher::finished,
            [parent, callback](QDBusPendingCallWatcher* watcher) {
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
Manager::loadUnitPath(const QString& name,
                      std::function<void(const QDBusPendingReply<>,
                                         const QDBusObjectPath&)> callback)
{
    Q_D(Manager);

    if (!d->iface.isValid()) {
        Q_ASSERT(d->iface.isValid());
        return QDBusPendingCall::fromError(d->iface.lastError());
    }

    QDBusPendingCall call = d->iface.asyncCall("LoadUnit", name);
    if (!callback) {
        return call;
    }

    auto* watcher = new QDBusPendingCallWatcher(call, this);
    connect(watcher,
            &QDBusPendingCallWatcher::finished,
            [callback](QDBusPendingCallWatcher* watcher) {
                QDBusPendingReply<QDBusObjectPath> reply = *watcher;
                callback(reply, reply.value());
                watcher->deleteLater();
            });
    return call;
}

QDBusPendingCall
Manager::reload()
{
    Q_D(Manager);

    if (!d->iface.isValid()) {
        Q_ASSERT(d->iface.isValid());
        return QDBusPendingCall::fromError(d->iface.lastError());
    }

    QDBusPendingCall call = d->iface.asyncCall("Reload");
    return call;
}

QDBusPendingCall
Manager::reloadUnit(const QString& name,
                    const QString& mode,
                    std::function<void(const QDBusPendingReply<> reply,
                                       const QDBusObjectPath& path)> callback)
{
    Q_D(Manager);

    if (!d->iface.isValid()) {
        Q_ASSERT(d->iface.isValid());
        return QDBusPendingCall::fromError(d->iface.lastError());
    }

    QDBusPendingCall call = d->iface.asyncCall("ReloadUnit", name, mode);
    if (!callback) {
        return call;
    }

    auto* watcher = new QDBusPendingCallWatcher(call, this);
    connect(watcher,
            &QDBusPendingCallWatcher::finished,
            [callback](QDBusPendingCallWatcher* watcher) {
                QDBusPendingReply<QDBusObjectPath> reply = *watcher;
                callback(reply, reply.value());
                watcher->deleteLater();
            });
    return call;
}

QDBusPendingCall
Manager::restartUnit(const QString& name,
                     const QString& mode,
                     std::function<void(const QDBusPendingReply<> reply,
                                        const QDBusObjectPath& path)> callback)
{
    Q_D(Manager);

    if (!d->iface.isValid()) {
        Q_ASSERT(d->iface.isValid());
        return QDBusPendingCall::fromError(d->iface.lastError());
    }

    QDBusPendingCall call = d->iface.asyncCall("RestartUnit", name, mode);
    if (!callback) {
        return call;
    }
    auto* watcher = new QDBusPendingCallWatcher(call, this);
    connect(watcher,
            &QDBusPendingCallWatcher::finished,
            [callback](QDBusPendingCallWatcher* watcher) {
                QDBusPendingReply<QDBusObjectPath> reply = *watcher;
                callback(reply, reply.value());
                watcher->deleteLater();
            });
    return call;
}

QDBusPendingCall
Manager::startUnit(const QString& name,
                   const QString& mode,
                   std::function<void(const QDBusPendingReply<> reply,
                                      const QDBusObjectPath& path)> callback)
{
    Q_D(Manager);

    if (!d->iface.isValid()) {
        Q_ASSERT(d->iface.isValid());
        return QDBusPendingCall::fromError(d->iface.lastError());
    }

    QDBusPendingCall call = d->iface.asyncCall("StartUnit", name, mode);
    if (!callback) {
        return call;
    }

    auto* watcher = new QDBusPendingCallWatcher(call, this);
    connect(watcher,
            &QDBusPendingCallWatcher::finished,
            [callback](QDBusPendingCallWatcher* watcher) {
                QDBusPendingReply<QDBusObjectPath> reply = *watcher;
                callback(reply, reply.value());
                watcher->deleteLater();
            });
    return call;
}

QDBusPendingCall
Manager::stopUnit(const QString& name,
                  const QString& mode,
                  std::function<void(const QDBusPendingReply<> reply,
                                     const QDBusObjectPath& path)> callback)
{
    Q_D(Manager);

    if (!d->iface.isValid()) {
        Q_ASSERT(d->iface.isValid());
        return QDBusPendingCall::fromError(d->iface.lastError());
    }

    QDBusPendingCall call = d->iface.asyncCall("StopUnit", name, mode);
    if (!callback) {
        return call;
    }

    auto* watcher = new QDBusPendingCallWatcher(call, this);
    connect(watcher,
            &QDBusPendingCallWatcher::finished,
            [callback](QDBusPendingCallWatcher* watcher) {
                QDBusPendingReply<QDBusObjectPath> reply = *watcher;
                callback(reply, reply.value());
                watcher->deleteLater();
            });
    return call;
}

QDBusPendingCall
Manager::subscribe()
{
    Q_D(Manager);

    if (!d->iface.isValid()) {
        Q_ASSERT(d->iface.isValid());
        return QDBusPendingCall::fromError(d->iface.lastError());
    }

    QDBusPendingCall call = d->iface.asyncCall("Subscribe");
    return call;
}

QString
Manager::version() const
{
    Q_D(const Manager);
    return d_ptr->iface.property("Version").toString();
}
