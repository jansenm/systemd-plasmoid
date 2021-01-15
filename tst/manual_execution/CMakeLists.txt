# SPDX-FileCopyrightText: 2021 Michael Jansen <info@michael-jansen.biz>
# SPDX-License-Identifier: CC0-1.0

ecm_add_test(manual_execution.cpp
        TEST_NAME manual_execution
        LINK_LIBRARIES org_kde_systemd_qmlplugin Qt5::Test)
