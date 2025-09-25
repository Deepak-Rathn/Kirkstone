FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"
SRC_URI_append = " \
         file://profiles/ \
         file://apparmor_platform_profile \
         "
do_install_append () {
    # profile folder creation
    if [ ! -d ${D}/etc/apparmor.d/ ]; then
           mkdir ${D}/etc/apparmor.d/
    fi
    #installing apparmor platform specific profiles for processes
    if [ ! -z "$(ls -A ${WORKDIR}/profiles)" ]; then
	   install -m 0555 ${WORKDIR}/profiles/* ${D}${sysconfdir}/apparmor.d/
    fi
    cat ${WORKDIR}/apparmor_platform_profile >> ${D}${sysconfdir}/apparmor/apparmor_defaults
}
