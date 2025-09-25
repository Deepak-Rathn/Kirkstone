SUMMARY = "IPoE Health Check component"

LICENSE = "Apache-2.0"

LIC_FILES_CHKSUM = "file://LICENSE;md5=175792518e4ac015ab6696d16c4f607e"

DEPENDS = "rdk-logger rdk-wanmanager"

SRC_URI ="${RDKB_COMPONENTS_OS_GIT}/ipoe_health_check/generic;protocol=${RDK_GIT_PROTOCOL};branch=stable2;name=IPoEHealthCheck"

SRCREV_IPoEHealthCheck = "${AUTOREV}"
SRCREV_FORMAT = ""

PV = "${RDK_RELEASE}+git${SRCPV}"

S = "${WORKDIR}/git"

inherit autotools pkgconfig

CFLAGS_append = " \
    -I${STAGING_INCDIR} \
    -I${STAGING_INCDIR}/ccsp \
    -I ${STAGING_INCDIR}/syscfg \
    -I ${STAGING_INCDIR}/sysevent \
    "

FILES_${PN} = " \
   ${bindir}/ipoe_health_check \
"
