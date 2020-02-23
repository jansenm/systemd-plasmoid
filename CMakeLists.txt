cmake_minimum_required(VERSION 3.10)
project(systemd_kde)

FIND_PACKAGE(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})

# CMake
include(FeatureSummary)
include(GenerateExportHeader)

# ECM
include(ECMAddTests)

# KDE
include(KDEInstallDirs)
include(KDECMakeSettings)
# This results in seriously inconvenient errors
# include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)
include(KDECompilerSettings NO_POLICY_SCOPE)

FIND_PACKAGE(Qt5
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

# find_package(KF5 ${KF5_REQUIRED_VERSION}
#         REQUIRED COMPONENTS
#         Plasma
#         I18n)

add_subdirectory(lib/systemd)
add_subdirectory(lib/qmlplugin)
add_subdirectory(lib/plasmoid)
add_subdirectory(tst/manual_execution)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
