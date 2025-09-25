FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

SRC_URI_append_hybrid = " ${@bb.utils.contains('DISTRO_FEATURES', 'yocto-3.1.15', '', 'file://CVE-2021-42374_fix.patch', d)} "
SRC_URI_append_client = " file://CVE-2021-42374_fix.patch "
