SRC_URI_remove = "${RDKB_CCSP_ROOT_GIT}/RdkXdslManager/generic;protocol=${RDK_GIT_PROTOCOL};branch=${CCSP_GIT_BRANCH};name=xDSLManager"
SRC_URI += "${CMF_GIT_ROOT}/rdkb/components/opensource/ccsp/RdkXdslManager;protocol=${CMF_GIT_PROTOCOL};branch=${CMF_GIT_BRANCH};name=xDSLManager"

DEPENDS += "breakpad-wrapper"

inherit coverity
