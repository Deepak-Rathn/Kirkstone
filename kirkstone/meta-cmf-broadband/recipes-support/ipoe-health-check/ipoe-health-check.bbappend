SRC_URI ="${RDKB_COMPONENTS_OS_GIT}/ipoe_health_check/generic;protocol=${RDK_GIT_PROTOCOL};branch=stable2;name=IPoEHealthCheck"
SRC_URI ="${CMF_GIT_ROOT}/rdk/components/opensource/ipoe_health_check;protocol=${CMF_GIT_PROTOCOL};branch=${CMF_GIT_BRANCH};name=IPoEHealthCheck"

inherit coverity
