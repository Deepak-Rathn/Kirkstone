RDEPENDS_${PN} += " ${@bb.utils.contains('DISTRO_FEATURES', 'apparmor', 'apparmor', '', d)}"
RDEPENDS_${PN}-analyze += " ${@bb.utils.contains('DISTRO_FEATURES', 'apparmor', 'apparmor', '', d)}"
