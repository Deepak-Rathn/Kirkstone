#############################################################################
# If not stated otherwise in this file or this component's LICENSE file the
# following copyright and licenses apply:
#
# Copyright 2022 Sky
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#############################################################################
AM_CFLAGS = -D_ANSC_LINUX
AM_CFLAGS += -D_ANSC_USER
AM_LDFLAGS = -lccsp_common
AM_CPPFLAGS = -Wall -g -Werror
AM_CXXFLAGS = -std=c++11

ACLOCAL_AMFLAGS = -I m4
bin_PROGRAMS = RdkCellularManager_gtest.bin
RdkCellularManager_gtest_bin_CPPFLAGS = -I$(PKG_CONFIG_SYSROOT_DIR)$(includedir)/gtest \
					-I$(PKG_CONFIG_SYSROOT_DIR)$(includedir)/ccsp \
					-I$(PKG_CONFIG_SYSROOT_DIR)$(includedir)/glib-2.0 \
					-I$(PKG_CONFIG_SYSROOT_DIR)$(libdir)/glib-2.0/include \
					-I$(PKG_CONFIG_SYSROOT_DIR)$(includedir)/libqmi-glib \
					-I$(PKG_CONFIG_SYSROOT_DIR)$(includedir)/dbus-1.0 \
					-I$(PKG_CONFIG_SYSROOT_DIR)$(libdir)/dbus-1.0/include \
					-I$(PKG_CONFIG_SYSROOT_DIR)$(includedir)/libsafec \
					-I${PKG_CONFIG_SYSROOT_DIR}$(includedir)/rbus \
					-I${PKG_CONFIG_SYSROOT_DIR}$(includedir)/rtmessage \
					-I${top_srcdir}/gtest/include \
					-I${top_srcdir}/source \
					-I${top_srcdir}/source/include \
					-I${top_srcdir}/source/CellularManager \
					-I${top_srcdir}/source/TR-181/include \
					-I$(top_srcdir)/../hal/include \
					-I$(top_srcdir)/../CcspCommonLibrary/source/cosa/include/ \
					-I$(top_srcdir)/../CcspCommonLibrary/source/ccsp/components/include/ \
					-I${top_srcdir}/source/TR-181/middle_layer_src
RdkCellularManager_gtest_bin_SOURCES =  ../CellularManager/cellularmgr_sm.c \
					../CellularManager/cellularmgr_bus_utils.c \
					../CellularManager/cellularmgr_cellular_apis.c \
					../CellularManager/cellularmgr_cellular_internal.c \
					../CellularManager/cellularmgr_messagebus_interface.c \
					../CellularManager/cellularmgr_ssp_action.c \
					../CellularManager/cellular_hal_utils.c \
					../CellularManager/cellular_hal_qmi_apis.c \
					../CellularManager/cellular_hal.c \
					../CellularManager/cellularmgr_ssp_internal.h \
					../TR-181/middle_layer_src/cellularmgr_cellular_webconfig_api.c \
					../TR-181/middle_layer_src/cellularmgr_cellular_param.c \
					../TR-181/middle_layer_src/cellularmgr_plugin_main.c \
					../TR-181/middle_layer_src/cellularmgr_plugin_main_apis.c \
					../TR-181/middle_layer_src/cellularmgr_utils.c \
					../TR-181/middle_layer_src/cellularmgr_cellular_helpers.c \
					../TR-181/middle_layer_src/cellularmgr_rbus_helpers.c \
					../TR-181/middle_layer_src/cellularmgr_rbus_events.c \
					../TR-181/middle_layer_src/cellularmgr_rbus_dml.c \
					../TR-181/middle_layer_src/cellularmgr_cellular_dml.c \
					cellmgr_sm_test.cpp \
					cellmgr_bus_utils_test.cpp \
					cellmgr_rbus_events_test.cpp \
					cellmgr_cellular_apis_test.cpp \
					mocks/mock_SMPThread.cpp \
					mocks/psmHandler_mock.cpp \
					mocks/rbusevent_mock.cpp \
					mocks/cellular_api_mock.cpp \
					cellmgr_mock.cpp \
					gtest_main.cpp
RdkCellularManager_gtest_bin_LDFLAGS = -lgtest -lgmock -lgcov -lccsp_common -ldl -lrbus $(SYSTEMD_LDFLAGS) -lrdkloggers -lsyscfg -lnanomsg -lsysevent -lwebconfig_framework -lmsgpackc -lcurl -ltrower-base64 -lsecure_wrapper -lrtMessage
