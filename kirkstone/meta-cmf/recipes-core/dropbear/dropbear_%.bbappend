
# Adding telemetry to video, broadband, and camera.
# excluding extender
DEPENDS += "telemetry"
DEPENDS_remove_extender = "telemetry"

LDFLAGS += "-ltelemetry_msgsender"
LDFLAGS_remove_extender = "-ltelemetry_msgsender"

SRC_URI_remove_extender  = " file://ssh_telemetry_2017_uninit_init_add.patch"
SRC_URI_remove_extender = " file://ssh_telemetry_2019_uninit_init_add.patch"
