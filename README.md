# SystemD Plasmoid

A [kde plasma 5](https://en.wikipedia.org/wiki/KDE_Plasma_5) widget that allows to control and monitor
[systemd](https://en.wikipedia.org/wiki/Systemd) units.

**This is work in progress.**

## How it works

The widgets utilizes the [systemd dbus api](https://www.freedesktop.org/wiki/Software/systemd/dbus/).

## Compiling

To test the widget without installing it:

    mkdir cmake-build-debug
    cd cmake-build-debug
    cmake ../ -DCMAKE_INSTALL_PREFIX=$PWD/install
    source prefix.sh    
    make
    make install
    # the default style currently has a bug
    QT_QUICK_CONTROLS_STYLE="Material" plasmoidviewer --applet ../lib/plasmoid/

## Required Dependencies


The following KDE-Frameworks are required
 * I18n
 * Notifications
 * Plasma

The following Qt5 Modules are required
  * Core
  * DBus
  * Quick
  * Test

And
  * Extra-CMake-Modules 

## Authors

* Michael Jansen <kde@michael-jansen.biz>

## Licensing

    SPDX-FileCopyrightText: 2021 Michael Jansen <info@michael-jansen.biz>
    SPDX-License-Identifier:  LGPL-2.1-or-later

This work is licesed under the [»GNU Lesser General Public License v2.1 or later«](LICENSES/LGPL-2.1-or-later.txt).


