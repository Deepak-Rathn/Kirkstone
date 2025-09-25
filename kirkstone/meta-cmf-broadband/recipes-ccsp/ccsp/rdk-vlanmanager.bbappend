SRC_URI_remove = "${RDKB_CCSP_ROOT_GIT}/RdkVlanManager/generic;protocol=${RDK_GIT_PROTOCOL};branch=${CCSP_GIT_BRANCH};name=VlanManager"
SRC_URI += "${CMF_GIT_ROOT}/rdkb/components/opensource/ccsp/RdkVlanBridgingManager;protocol=${CMF_GIT_PROTOCOL};branch=${CMF_GIT_BRANCH};name=VlanManager"

inherit coverity

DEPENDS += "json-c"

LDFLAGS += "-lbreakpadwrapper -lpthread"
