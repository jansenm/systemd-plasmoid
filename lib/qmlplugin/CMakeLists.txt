# SPDX-FileCopyrightText: 2021 Michael Jansen <info@michael-jansen.biz>
# SPDX-License-Identifier: CC0-1.0

set(org_kde_systemd_qmlplugin_SRCS
        QmlPlugin.cpp
        UnitModel.cpp UnitModel.h
        Connection.cpp Connection.h
        QMLDebugger.cpp QMLDebugger.h
        UnitFileModel.cpp UnitFileModel.h)

add_library(org_kde_systemd_qmlplugin
        SHARED ${org_kde_systemd_qmlplugin_SRCS})

target_link_libraries(org_kde_systemd_qmlplugin
        systemd-qt
        Qt5::Quick
        KF5::I18n
        )

install(TARGETS org_kde_systemd_qmlplugin
        DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/systemd)
install(FILES qmldir DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/systemd)
