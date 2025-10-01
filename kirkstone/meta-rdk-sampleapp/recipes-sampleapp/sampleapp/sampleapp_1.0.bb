SUMMARY = "Sample third-party style app for RDK-B integration scaffolding"
DESCRIPTION = "A simple hello-world style daemon used to demonstrate RDK-B integration of third-party apps."
HOMEPAGE = "https://example.com/sampleapp"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://LICENSE;md5=3f83ef8a3f7e0a98a1f3fb0a7d4f1d29"

PV = "1.0"
PR = "r0"

# Build from local files; in a real integration this would be git/url fetch
SRC_URI = "\
    file://LICENSE \
    file://Makefile \
    file://sampleapp.c \
    file://sampleapp.service \
"

S = "${WORKDIR}"

inherit pkgconfig systemd

# Provide systemd support via PACKAGECONFIG toggle
PACKAGECONFIG ??= "systemd"
PACKAGECONFIG[systemd] = ",,systemd"

SYSTEMD_SERVICE:${PN} = "sampleapp.service"
SYSTEMD_AUTO_ENABLE:${PN} = "enable"

do_compile() {
    oe_runmake
}

do_install() {
    # Install binary
    install -d ${D}${bindir}
    install -m 0755 sampleapp ${D}${bindir}/sampleapp

    # Install systemd service if enabled
    if ${@bb.utils.contains('PACKAGECONFIG', 'systemd', 'true', 'false', d)}; then
        install -d ${D}${systemd_system_unitdir}
        install -m 0644 ${WORKDIR}/sampleapp.service ${D}${systemd_system_unitdir}/
    fi
}

FILES:${PN} += "\
    ${bindir}/sampleapp \
    ${systemd_system_unitdir}/sampleapp.service \
"

RDEPENDS:${PN} = "${@bb.utils.contains('PACKAGECONFIG', 'systemd', 'systemd', '', d)}"

# Simple runtime test via 'sampleapp --version' could be extended in ptests
