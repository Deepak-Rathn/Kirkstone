FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

SRC_URI_append = " ${@bb.utils.contains('DISTRO_FEATURES', 'kirkstone', '', 'file://CVE-2022-23303-4.patch', d)} \
                 "
