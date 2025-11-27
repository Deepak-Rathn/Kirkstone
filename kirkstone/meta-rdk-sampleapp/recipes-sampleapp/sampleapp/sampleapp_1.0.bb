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
# Important:
# - This recipe installs a systemd unit only when PACKAGECONFIG includes 'systemd'.
# - No systemctl invocations are performed during do_install; enabling is handled by
#   packaging metadata and deferred to target first-boot by the distro include, to
#   avoid build-time errors on hosts not booted with systemd.
#
# Additionally, ensure postinst never tries to invoke systemctl in the build/host
# environment. We explicitly defer service enablement to target first boot.


# Provide systemd support via PACKAGECONFIG toggle
PACKAGECONFIG ??= "systemd"
PACKAGECONFIG[systemd] = ",,systemd"

SYSTEMD_SERVICE:${PN} = "sampleapp.service"
# Defer enablement to the target; systemd.bbclass will create postinst to
# enable on first boot when systemd is present. Do not attempt to start.
SYSTEMD_AUTO_ENABLE:${PN} = "enable"

# Ensure any postinstall runs only on target (IMAGE_PREPROCESS and do_rootfs must not invoke systemctl)
pkg_postinst:${PN} () {
#!/bin/sh
if [ -n "$D" ]; then
    # In image creation (populate rootfs) context; do nothing and defer to target
    exit 0
fi
# On target, systemd.bbclass will handle enabling; do not try to start here.
# Guard against missing systemd (e.g., non-systemd images)
if [ -d /run/systemd/system ]; then
    :
fi
}

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
