# SPDX-FileCopyrightText: 2021 Michael Jansen <info@michael-jansen.biz>
# SPDX-License-Identifier: CC0-1.0

cmake_minimum_required(VERSION 3.10)
project(systemd_kde
        DESCRIPTION "A plasma widget to control systemd units"
        HOMEPAGE_URL "https://github.com/jansenm/systemd-plasmoid"
        VERSION 2.0.0)

find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})

# CMake
include(FeatureSummary)
include(GenerateExportHeader)

# ECM
include(ECMAddTests)

# KDE
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)
# TODO Wonder if i also need this. Need to find out
include(KDECompilerSettings NO_POLICY_SCOPE)

find_package(Qt5
        CONFIG REQUIRED
        Core
        DBus
        Quick
        Test
        )

find_package(KF5 ${KF5_REQUIRED_VERSION}
        REQUIRED COMPONENTS
        I18n
        Notifications
        Plasma
        )

add_subdirectory(lib/systemd)
add_subdirectory(lib/qmlplugin)
add_subdirectory(lib/plasmoid)
add_subdirectory(tst/manual_execution)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
