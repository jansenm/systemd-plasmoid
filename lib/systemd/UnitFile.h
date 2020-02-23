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
#ifndef SYSTEMD_KDE_UNITFILE_H
#define SYSTEMD_KDE_UNITFILE_H

#include "systemd-qt_export.h"
#include <QtCore/QObject>
#include <QtDBus/QDBusReply>

namespace Systemd {

class SYSTEMD_QT_EXPORT UnitFile : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id);
    Q_PROPERTY(QString name READ name);
    Q_PROPERTY(QString state READ state);

    class UnitFilePrivate* d_ptr;

  public:
    /**
     * @internal
     *
     * Create a new unit file object.
     *
     * @param p             The UnitFilePrivate object to use.
     * @param parent        The parent object or nullptr
     *
     * @endinternal
     */
    UnitFile(const UnitFilePrivate& p, QObject* parent = nullptr);

    /**
     * Destroy the unit object.
     */
    ~UnitFile() override;

    QString id() const;
    QString name() const;
    QString state() const;

  private:
    Q_DECLARE_PRIVATE(UnitFile);
};

typedef QList<UnitFile*> UnitFileList;

}

Q_DECLARE_METATYPE(Systemd::UnitFileList);

#endif // SYSTEMD_KDE_UNITFILE_H
