FILESEXTRAPATHS_prepend := "${THISDIR}/dbus:"

RDEPENDS_${PN} += " ${@bb.utils.contains('DISTRO_FEATURES', 'apparmor', 'apparmor', '', d)}"
