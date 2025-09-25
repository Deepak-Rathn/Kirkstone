SRC_URI_remove = "${RDKB_CCSP_ROOT_GIT}/RdkWanManager/generic;protocol=${RDK_GIT_PROTOCOL};branch=${CCSP_GIT_BRANCH};name=WanManager"
SRC_URI += "${CMF_GIT_ROOT}/rdkb/components/opensource/ccsp/RdkWanManager;protocol=${CMF_GIT_PROTOCOL};branch=${CMF_GIT_BRANCH};name=WanManager"

DEPENDS += "breakpad breakpad-wrapper nanomsg"

CFLAGS += "-I${STAGING_INCDIR}/breakpad "
CXXFLAGS += "-I${STAGING_INCDIR}/breakpad "

LDFLAGS += "-lbreakpadwrapper -lpthread"

CFLAGS_append = " -DFEATURE_802_1P_COS_MARKING "
inherit coverity
