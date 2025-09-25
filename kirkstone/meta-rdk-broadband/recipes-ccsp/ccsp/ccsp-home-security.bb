SUMMARY = "CCSP Home Security."
HOMEPAGE = "http://github.com/belvedere-yocto/CcspHomeSecurity"

LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=175792518e4ac015ab6696d16c4f607e"

DEPENDS = "libxml2 ccsp-common-library utopia curl"
require ccsp_common.inc
SRC_URI = "${CMF_GIT_ROOT}/rdkb/components/opensource/ccsp/CcspHomeSecurity;protocol=${CMF_GIT_PROTOCOL};branch=${CMF_GIT_BRANCH};name=CcspHomeSecurity"

SRCREV_CcspHomeSecurity = "${AUTOREV}"
SRCREV_FORMAT = "CcspHomeSecurity"
PV = "${RDK_RELEASE}+git${SRCPV}"

S = "${WORKDIR}/git"

inherit autotools breakpad-logmapper

CFLAGS += " -Wall -Werror -Wextra -Wno-format-truncation "

CFLAGS_append = " \
    -I=${includedir}/dbus-1.0 \
    -I=${libdir}/dbus-1.0/include \
    -I=${includedir}/ccsp \
    -I=${includedir}/libxml2 \
    "

do_install_append () {
}

PACKAGES += "${PN}-ccsp"
PACKAGES =+ "${@bb.utils.contains('DISTRO_FEATURES', 'gtestapp', '${PN}-gtest', '', d)}"

FILES_${PN}-gtest = "\
    ${@bb.utils.contains('DISTRO_FEATURES', 'gtestapp', '${bindir}/CcspHomeSecurity_gtest.bin', '', d)} \
"

FILES_${PN}-ccsp = " \
"

FILES_${PN}-dbg = " \
    ${prefix}/ccsp/.debug \
    ${prefix}/src/debug \
    ${bindir}/.debug \
    ${libdir}/.debug \
"

DOWNLOAD_APPS="${@bb.utils.contains('DISTRO_FEATURES', 'gtestapp', 'gtestapp-CcspHomeSecurity', '', d)}"
inherit comcast-package-deploy
CUSTOM_PKG_EXTNS="gtest"
SKIP_MAIN_PKG="yes"
DOWNLOAD_ON_DEMAND="yes"
# Breakpad processname and logfile mapping
BREAKPAD_LOGMAPPER_PROCLIST = "CcspHomeSecurit"
BREAKPAD_LOGMAPPER_LOGLIST = "ADVSEClog.txt.0,agent.txt"
