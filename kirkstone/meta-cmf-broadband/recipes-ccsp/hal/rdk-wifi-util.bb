SUMMARY = "DPP HAL for RDK CCSP components"
HOMEPAGE = "http://github.com/belvedere-yocto/hal"

LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://../LICENSE;md5=5d50b1d1fb741ca457897f9e370bc747"

PROVIDES = "rdk-wifi-util"
RPROVIDES_${PN} = "rdk-wifi-util"

DEPENDS += "openssl halinterface"
SRC_URI = "${CMF_GIT_ROOT}/rdkb/components/opensource/ccsp/hal/rdk-wifi-hal;protocol=${CMF_GIT_PROTOCOL};branch=${CMF_GIT_BRANCH};name=rdk-wifi-util"


SRCREV_rdk-wifi-util = "${AUTOREV}"
SRCREV_FORMAT = "rdk-wifi-util"

PV = "${RDK_RELEASE}+git${SRCPV}"
S = "${WORKDIR}/git/util"

# Add flags to support mesh wifi if the feature is available.
CFLAGS_append = " ${@bb.utils.contains('DISTRO_FEATURES', 'meshwifi', '-DENABLE_FEATURE_MESHWIFI', '', d)}"
CFLAGS_append = " -I=${includedir}/ccsp "

inherit autotools coverity
