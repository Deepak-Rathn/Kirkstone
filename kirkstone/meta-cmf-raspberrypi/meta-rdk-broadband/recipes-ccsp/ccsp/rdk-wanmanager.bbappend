CFLAGS_append = " -D_PLATFORM_RASPBERRYPI_"
#REFPLTB-2136 - Need to remove the below line once RDKInterDeviceManager functionality support is added in rpi targets.
CFLAGS_remove = " -DFEATURE_RDKB_INTER_DEVICE_MANAGER"
LDFLAGS_append = " ${@bb.utils.contains('DISTRO_FEATURES', 'RbusBuildFlagEnable', '-lrbus', '', d)}"

do_configure_prepend() {
	if ${@bb.utils.contains('DISTRO_FEATURES', 'rdkb_wan_manager', 'true', 'false', d)}; then
        (python ${STAGING_BINDIR_NATIVE}/dm_pack_code_gen.py ${S}/config/RdkWanManager.xml ${S}/source/WanManager/dm_pack_datamodel.c)
	fi
}
