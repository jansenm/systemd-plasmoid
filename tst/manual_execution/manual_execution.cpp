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

#include <Manager.h>
#include <QtCore/QObject>
#include <QtDBus/QDBusInterface>
#include <QtTest/QtTest>
#include <Unit.h>
#include <UnitInfo_p.h>

using namespace Systemd;

class SystemDTest : public QObject
{
    Q_OBJECT

  private slots:

    void testGetUnits()
    {
        QDBusConnection bus = QDBusConnection::systemBus();
        Manager m(bus, nullptr);
        QCOMPARE(m.version(), QLatin1String("234"));

        bool gotSignal = false;
        auto call = m.listUnits(
          [this, &gotSignal](QDBusPendingReply<> reply,
                             UnitInfoList list) -> void {
              gotSignal = true;
              QVERIFY(reply.isValid());
              QVERIFY(!reply.isError());
              QVERIFY(list.length() > 0);
          },
          this);

        // Wait for the signal
        // Verify the result
        call.waitForFinished();
        QTest::qWait(100);
        QVERIFY(gotSignal);
        QVERIFY(call.isValid());
        QCOMPARE(call.error().message(), QLatin1String(""));
    }

    void testGetUnitFiles()
    {
        QDBusConnection bus = QDBusConnection::systemBus();
        Manager m(bus, nullptr);
        QCOMPARE(m.version(), QLatin1String("234"));

        bool gotSignal = false;
        auto call = m.listUnitFiles(
          [this, &gotSignal](QDBusPendingReply<> reply,
                             UnitFileList list) -> void {
              gotSignal = true;
              QVERIFY(reply.isValid());
              QVERIFY(!reply.isError());
              QVERIFY(list.length() > 0);
          },
          this);

        // Wait for the signal
        // Verify the result
        call.waitForFinished();
        QTest::qWait(100);
        QVERIFY(gotSignal);
        QVERIFY(call.isValid());
        QCOMPARE(call.error().message(), QLatin1String(""));
    }

    void testGetUnitFileState()
    {
        QDBusConnection bus = QDBusConnection::systemBus();
        Manager m(bus, nullptr);
        bool gotSignal = false;
        auto call = m.getUnitFileState(
                "-.mount",
                [this, &gotSignal](QDBusPendingReply<> reply,
                                   QString state) -> void {
                    gotSignal = true;
                    QVERIFY(reply.isValid());
                    QVERIFY(!reply.isError());
                    QCOMPARE(state, QLatin1String("generated"));
                });

        // Wait for the signal
        // Verify the result
        call.waitForFinished();
        QTest::qWait(100);
        QVERIFY(gotSignal);
        QVERIFY(call.isValid());
        QCOMPARE(call.error().message(), QLatin1String(""));
    }

    void testGetUnitFileStateInvalid()
    {
        QDBusConnection bus = QDBusConnection::systemBus();
        Manager m(bus, nullptr);
        bool gotSignal = false;
        auto call = m.getUnitFileState(
                "dddd.mount",
                [this, &gotSignal](QDBusPendingReply<> reply,
                                   QString state) -> void {
                    gotSignal = true;
                    QVERIFY(!reply.isValid());
                    QVERIFY(reply.isError());
                    QCOMPARE(reply.error().message(), QLatin1String("No such file or directory"));
                    QCOMPARE(state, QLatin1String("No such file or directory"));
                });

        // Wait for the signal
        // Verify the result
        call.waitForFinished();
        QTest::qWait(100);
        QVERIFY(gotSignal);
        QVERIFY(!call.isValid());
        QCOMPARE(call.error().message(), QLatin1String("No such file or directory"));
    }

    void testGetUnitPath()
    {
        QDBusConnection bus = QDBusConnection::systemBus();
        Manager m(bus, nullptr);

        bool gotSignal = false;
        auto call = m.getUnitPath(
          "-.mount",
          [&gotSignal](QDBusPendingReply<> reply,
                       const QDBusObjectPath& path) -> void {
              gotSignal = true;
              QVERIFY(reply.isValid());
              QVERIFY(!reply.isError());
              QCOMPARE(
                path.path(),
                QLatin1String("/org/freedesktop/systemd1/unit/_2d_2emount"));
          });

        // Wait for the signal
        // Verify the result
        call.waitForFinished();
        QTest::qWait(100);
        QVERIFY(gotSignal);
        QVERIFY(call.isValid());
        QCOMPARE(call.error().message(), QLatin1String(""));
    }

    void testGetUnitPathInvalid()
    {
        QDBusConnection bus = QDBusConnection::systemBus();
        Manager m(bus, nullptr);

        bool gotSignal = false;
        auto call = m.getUnitPath(
          "doesnotexist.mount",
          [&gotSignal](QDBusPendingReply<> reply, QDBusObjectPath) -> void {
              gotSignal = true;
              QVERIFY(!reply.isValid());
              QVERIFY(reply.isError());
          });

        // Wait for the signal
        // Verify the result
        call.waitForFinished();
        QTest::qWait(100);
        QVERIFY(gotSignal);
        QVERIFY(!call.isValid());
        QCOMPARE(call.error().message(),
                 QLatin1String("Unit doesnotexist.mount not loaded."));
    }

    void testGetUnit()
    {
        QDBusConnection bus = QDBusConnection::systemBus();
        Unit root(
          bus, QDBusObjectPath("/org/freedesktop/systemd1/unit/_2d_2emount"));
        QVERIFY(root.isValid());
        QCOMPARE(root.id(), QLatin1String("-.mount"));
        QCOMPARE(root.subState(), QLatin1String("mounted"));
        QCOMPARE(root.loadState(), QLatin1String("loaded"));
        QCOMPARE(root.activeState(), QLatin1String("active"));
        QCOMPARE(root.unitFileState(), QLatin1String("generated"));
    }

    void testGetInvalidUnit()
    {
        QDBusConnection bus = QDBusConnection::systemBus();
        Unit root(bus, QDBusObjectPath("/org/freedesktop/systemd1/unit/_2d_2emountdddddddd"));
        QEXPECT_FAIL("", "How do we find out this is invalid?", Continue);
        QVERIFY(!root.isValid());
        QCOMPARE(root.id(), QLatin1String(""));
        QCOMPARE(root.loadState(), QLatin1String(""));
        QCOMPARE(root.subState(), QLatin1String(""));
        QCOMPARE(root.activeState(), QLatin1String(""));
        QCOMPARE(root.unitFileState(), QLatin1String(""));
    }
};

QTEST_MAIN(SystemDTest)

#include "manual_execution.moc"
