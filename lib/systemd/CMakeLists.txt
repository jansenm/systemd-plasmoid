# SPDX-FileCopyrightText: 2021 Michael Jansen <info@michael-jansen.biz>
# SPDX-License-Identifier: CC0-1.0

add_library(systemd-qt SHARED
        debug.cpp debug.h
        Changes.cpp Changes.h Changes_p.h
        UnitInfo.cpp UnitInfo.h UnitInfo_p.h
        UnitFile.cpp UnitFile.h UnitFile_p.h
        Unit.cpp Unit.h
        Manager.cpp Manager.h
        )

target_link_libraries(systemd-qt
        Qt5::Core
        Qt5::DBus)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)
generate_export_header(systemd-qt)

install(TARGETS systemd-qt
        # IMPORTANT: Add the QtSystemd library to the "export-set"
        RUNTIME DESTINATION "${KDE_INSTALL_BIN_DIR}"
        LIBRARY DESTINATION "${KDE_INSTALL_LIBDIR}"
        PUBLIC_HEADER DESTINATION "${KDE_INSTALL_INCLUDEDIR}")

