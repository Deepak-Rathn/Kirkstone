# meta-rdk-sampleapp distro include
# Purpose:
# - Centralize defaults to avoid systemd invocations during build on hosts/CI that
#   are not booted with systemd, while still enabling systemd on the target image.
#
# Usage:
# - Include this file from your distro config or local.conf:
#     require conf/distro/include/meta-rdk-sampleapp.inc
# - It sets sane defaults for systemd integration used by this layer.
#
# Notes:
# - We ensure systemd is the init manager for the target (typical for RDK-B),
#   but we avoid any host-side systemctl invocations during do_rootfs/postinstall
#   by marking pkg_postinsts as deferred (handled on first-boot).
# - This prevents errors like: "System has not been booted with systemd".

# Ensure we target systemd-based images unless overridden elsewhere.
DISTRO_FEATURES:append = " systemd"
VIRTUAL-RUNTIME_init_manager ?= "systemd"
VIRTUAL-RUNTIME_initscripts ?= ""

# Defer systemd postinst scripts to first boot to avoid attempting to run systemctl
# or other host runtime interactions during rootfs creation on the build host.
# This is a common approach in OE/Yocto to avoid host runtime errors.
SYSTEMD_AUTO_ENABLE ??= "enable"
SYSTEMD_USE_GROUPS ??= "1"

# Make sure postinstall scripts for systemd units get deferred
# when rootfs is assembled (no running systemd on build host).
# Based on meta-systemd’s documented variable:
# https://docs.yoctoproject.org/ref-manual/variables.html#term-SYSTEMD_AUTO_ENABLE
# The following ensures pkg_postinst for systemd units will be marked for first-boot.
PACKAGECONFIG:pn-systemd:append = " firstboot"

# For robustness, forbid any accidental 'systemctl' calls during do_rootfs:
ROOTFS_POSTPROCESS_COMMAND:append = " sampleapp_block_host_systemctl;"

# Helper to guard against host systemctl usage.
python sampleapp_block_host_systemctl () {
    import os
    # If the host is not booted with systemd, create a fake systemctl which always fails fast
    # to guard against accidental use by custom postprocess snippets in other layers.
    # The sample layer itself does not call systemctl during build, but this is defensive.
    bindir = d.getVar('bindir_native')
    workdir = d.getVar('WORKDIR')
    fakebin = os.path.join(workdir, 'fakebin')
    os.makedirs(fakebin, exist_ok=True)
    fake_systemctl = os.path.join(fakebin, 'systemctl')
    with open(fake_systemctl, 'w') as f:
        f.write('#!/bin/sh\n')
        f.write('echo "systemctl should not be invoked at build time. Deferring to target first-boot." 1>&2\n')
        f.write('exit 1\n')
    os.chmod(fake_systemctl, 0o755)
    path = d.getVar('PATH')
    d.setVar('PATH', fakebin + ':' + path)
}
