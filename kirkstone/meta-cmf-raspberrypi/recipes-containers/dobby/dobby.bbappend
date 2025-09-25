FILESEXTRAPATHS_prepend := "${THISDIR}/files:"
SRC_URI += "file://dobby.json"

EXTRA_OECMAKE_append = " -DSETTINGS_FILE=${WORKDIR}/dobby.json "
PACKAGECONFIG_append = " devicemapper"


CFLAGS_append_kirkstone = " -mfloat-abi=hard -mcpu=cortex-a7 "
CFLAGS_remove_aarch64_kirkstone = " -mfloat-abi=hard -mcpu=cortex-a7 "
