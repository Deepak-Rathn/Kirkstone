#
# If not stated otherwise in this file or this component's Licenses.txt file the
# following copyright and licenses apply:
#
# Copyright 2017 RDK Management
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
SUMMARY = "This receipe provides platform manager support."
SECTION = "console/utils"
LICENSE = "CLOSED"

DEPENDS = "ccsp-common-library dbus rdk-logger hal-platform util-linux ruli utopia "
DEPENDS_append = " ${@bb.utils.contains('DISTRO_FEATURES', 'safec', ' safec', " ", d)}"

require recipes-ccsp/ccsp/ccsp_common.inc

SRC_URI ="${RDKB_CCSP_ROOT_GIT}/PlatformManager/generic;protocol=${RDK_GIT_PROTOCOL};branch=${CCSP_GIT_BRANCH};name=PlatformManager"

SRCREV_PlatformManager = "${AUTOREV}"
SRCREV_FORMAT = "PlatformManager"
PV = "${RDK_RELEASE}+git${SRCPV}"
S = "${WORKDIR}/git"

inherit autotools systemd

CFLAGS += " -Wall -Werror -Wextra -Wno-pointer-sign -Wno-sign-compare -Wno-deprecated-declarations -Wno-address -Wno-type-limits -Wno-unused-parameter "


CFLAGS_append = " ${@bb.utils.contains('DISTRO_FEATURES', 'bci', '-DCISCO_CONFIG_TRUE_STATIC_IP -DCISCO_CONFIG_DHCPV6_PREFIX_DELEGATION', '', d)}"
CFLAGS_append += " ${@bb.utils.contains('DISTRO_FEATURES', 'bci', '', '-DFEATURE_SUPPORT_ONBOARD_LOGGING',d)}"
CFLAGS_append = " ${@bb.utils.contains('DISTRO_FEATURES', 'dslite', '-DDSLITE_FEATURE_SUPPORT', '', d)}"

inherit autotools breakpad-logmapper
CFLAGS_append = " ${@bb.utils.contains('DISTRO_FEATURES', 'safec',  ' `pkg-config --cflags libsafec`', '-fPIC', d)}"

LDFLAGS_append = " ${@bb.utils.contains('DISTRO_FEATURES', 'safec', ' `pkg-config --libs libsafec`', '', d)}"
LDFLAGS_remove = "${@bb.utils.contains('DISTRO_FEATURES', 'safec', '-lsafec-3.5', '', d)}"
LDFLAGS_append_dunfell = "${@bb.utils.contains('DISTRO_FEATURES', 'safec', ' -lsafec-3.5.1 ', '', d)}"
LDFLAGS_append_kirkstone = " ${@bb.utils.contains('DISTRO_FEATURES', 'safec', ' -lsafec ', '', d)}"
CFLAGS_append = " ${@bb.utils.contains('DISTRO_FEATURES', 'safec', '', ' -DSAFEC_DUMMY_API', d)}"

EXTRA_OECONF_append = " ${@bb.utils.contains('DISTRO_FEATURES', 'rdkb_wan_manager', '--enable-wanmgr', '', d)}"
LDFLAGS += "-pthread -ltelemetry_msgsender"
LDFLAGS += "-lhal_platform"

do_install_append () {
    # Config files and scripts
    install -d ${D}${exec_prefix}/ccsp/platformmanager
    install -d ${D}${systemd_unitdir}/system
    install -D -m 0644 ${S}/config/PlatformManager.service ${D}${systemd_unitdir}/system/PlatformManager.service
}
SYSTEMD_SERVICE_${PN} = "PlatformManager.service"
FILES_${PN} += " \
    ${exec_prefix}/ccsp/platformmanager \
    /usr/bin/* \
        ${systemd_unitdir}/system/PlatformManager.service \
"
