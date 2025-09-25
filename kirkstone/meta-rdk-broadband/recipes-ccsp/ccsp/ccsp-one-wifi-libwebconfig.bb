
SUMMARY = "CCSP OneWifi - webconfig encode/decode library"

LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=042d68aa6c083a648f58bb8d224a4d31"

SRC_URI = "${CMF_GIT_ROOT}/rdkb/components/opensource/ccsp/OneWifi;protocol=${CMF_GIT_PROTOCOL};branch=${CMF_GIT_BRANCH};name=libwebconfig"

SRCREV_libwebconfig = "${AUTOREV}"
SRCREV_FORMAT = "libwebconfig"
PV = "${RDK_RELEASE}+git${SRCPV}"

S = "${WORKDIR}/git"

inherit autotools pkgconfig systemd ${@bb.utils.contains("DISTRO_FEATURES", "kirkstone", "python3native", "pythonnative", d)} breakpad-logmapper

DEPENDS = "halinterface opensync-headers rbus libsyswrapper jansson webconfig-framework libev"
DEPENDS_append = " ${@bb.utils.contains('DISTRO_FEATURES', 'systemd', 'systemd', '', d)}"
DEPENDS_append = " ${@bb.utils.contains('DISTRO_FEATURES', 'safec', ' safec', " ", d)}"

CFLAGS += " -Wall -Werror -Wextra -Wno-implicit-function-declaration -Wno-type-limits -Wno-unused-parameter \
            -I${STAGING_INCDIR}/ccsp -I=${includedir}/rbus -DWIFI_HAL_VERSION_3"
CFLAGS_append = " -Wno-format-overflow -Wno-format-truncation -Wno-address-of-packed-member -Wno-tautological-compare -Wno-stringop-truncation -fcommon"
CFLAGS_append = " ${@bb.utils.contains('DISTRO_FEATURES', 'safec',  ' `pkg-config --cflags libsafec`', '-fPIC', d)}"
CFLAGS_append = " ${@bb.utils.contains('DISTRO_FEATURES', 'safec', '', ' -DSAFEC_DUMMY_API', d)}"
CFLAGS_append = " ${@bb.utils.contains('DISTRO_FEATURES', 'meshwifi', '-DENABLE_FEATURE_MESHWIFI', '', d)}"
# DISTRO_FEATURES_append = "FEATURE_IEEE80211BE" should be declared in local.conf
CFLAGS_append = " ${@bb.utils.contains("DISTRO_FEATURES", 'FEATURE_IEEE80211BE', ' -DFEATURE_IEEE80211BE', '', d)}"
CFLAGS_append_kirkstone = " -Wno-deprecated-declarations"

LDFLAGS_append = " -lrbus "
LDFLAGS_append = " ${@bb.utils.contains('DISTRO_FEATURES', 'safec', ' `pkg-config --libs libsafec`', '', d)}"
LDFLAGS_remove = " ${@bb.utils.contains('DISTRO_FEATURES', 'safec', ' -lsafec-3.5 ', '', d)}"
LDFLAGS_append_dunfell = " ${@bb.utils.contains('DISTRO_FEATURES', 'safec', ' -lsafec-3.5.1 ', '', d)}"
LDFLAGS_append_kirkstone = " ${@bb.utils.contains('DISTRO_FEATURES', 'safec', ' -lsafec ', '', d)}"

EXTRA_OECONF_append = " --enable-libwebconfig"
EXTRA_OECONF_append = " ${@bb.utils.contains('DISTRO_FEATURES', 'systemd', '--enable-notify', '', d)}"
ISSYSTEMD = "${@bb.utils.contains('DISTRO_FEATURES', 'systemd', 'true', 'false', d)}"

do_compile() {
    oe_runmake -C source/webconfig
}

do_install() {
    oe_runmake -C source/webconfig DESTDIR=${D} install

    install -d ${D}/usr/include/ccsp
    install -m 644 ${S}/include/webconfig_external_proto_ovsdb.h  ${D}/usr/include/ccsp
    install -m 644 ${S}/include/webconfig_external_proto.h  ${D}/usr/include/ccsp
    install -m 644 ${S}/include/webconfig_external_proto_tr181.h  ${D}/usr/include/ccsp
    install -m 644 ${S}/include/wifi_webconfig.h       ${D}/usr/include/ccsp
    install -m 644 ${S}/include/wifi_base.h       ${D}/usr/include/ccsp
    install -m 644 ${S}/source/utils/collection.h ${D}/usr/include/ccsp
}

FILES_${PN} += "${libdir}/*.so*"
FILES_SOLIBSDEV = ""
INSANE_SKIP_${PN} += "dev-so"

ERROR_QA_remove_morty = "la"

inherit comcast-package-deploy
CUSTOM_PKG_EXTNS="gtest"
SKIP_MAIN_PKG="yes"
DOWNLOAD_ON_DEMAND="yes"
# Breakpad processname and logfile mapping
