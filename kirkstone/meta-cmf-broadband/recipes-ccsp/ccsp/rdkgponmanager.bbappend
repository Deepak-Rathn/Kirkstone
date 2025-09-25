SRC_URI_remove = "${RDKB_CCSP_ROOT_GIT}/RdkGponManager/generic;protocol=${RDK_GIT_PROTOCOL};branch=${CCSP_GIT_BRANCH};name=GponManager"
SRC_URI += "${CMF_GIT_ROOT}/rdkb/components/opensource/ccsp/RdkGponManager;protocol=${CMF_GIT_PROTOCOL};branch=${CMF_GIT_BRANCH};name=GponManager"

inherit coverity

DEPENDS += " utopia"

CFLAGS_append = " -Wno-incompatible-pointer-types -Wno-implicit-function-declaration "

CFLAGS_append = " \
    -I${S}/source/GponManager \
    -I${S}/source/TR-181/middle_layer_src \
    -I${S}/source/TR-181/include \
    "
