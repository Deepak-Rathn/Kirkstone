FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

SRC_URI_append = " ${@bb.utils.contains('DISTRO_FEATURES', 'kirkstone', '', 'file://CVE-2021-3905_fix.patch \
                                                                             file://CVE-2021-36980_fix.patch \
                                                                             file://CVE-2023-5366_fix.patch', d)} \ 
                 "

SRC_URI_append = " ${@bb.utils.contains('DISTRO_FEATURES', 'kirkstone', 'file://CVE-2023-5366_2.17_fix.patch', '', d)} \
                 "
DEPENDS += "bison-native"