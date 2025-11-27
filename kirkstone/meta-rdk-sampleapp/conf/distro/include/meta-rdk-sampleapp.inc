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
#   by deferring enablement to first-boot (standard systemd.bbclass behavior).
# - This prevents errors like: "System has not been booted with systemd" on CI/build hosts.

# Ensure we target systemd-based images unless overridden elsewhere.
DISTRO_FEATURES:append = " systemd"
VIRTUAL-RUNTIME_init_manager ?= "systemd"
VIRTUAL-RUNTIME_initscripts ?= ""

# Defer systemd postinst scripts to first boot to avoid attempting to run systemctl
# or other host runtime interactions during rootfs creation on the build host.
# This is a common approach in OE/Yocto to avoid host runtime errors.
SYSTEMD_AUTO_ENABLE ??= "enable"
SYSTEMD_USE_GROUPS ??= "1"

# Keep builds deterministic: do not modify PATH or inject fake binaries globally.
# Rely on standard systemd.bbclass behavior to defer unit enabling to first boot.
# If any other layer attempts to run systemctl during rootfs on the host, it should
# be fixed at the source recipe by guarding with `[ -d /run/systemd/system ]` before use.
