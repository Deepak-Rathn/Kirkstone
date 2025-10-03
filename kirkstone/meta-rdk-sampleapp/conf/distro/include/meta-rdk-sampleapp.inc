# This optional include can be used by a distro or local.conf to pull in meta-rdk-sampleapp defaults.

# Include the sample packagegroup by default when this include is required
IMAGE_INSTALL:append = " packagegroup-meta-rdk-sampleapp"

# Systemd as default for the sample app integration (no-op if systemd is already in DISTRO_FEATURES)
DISTRO_FEATURES:append = " systemd"
VIRTUAL-RUNTIME_init_manager ?= "systemd"
VIRTUAL-RUNTIME_initscripts ?= ""
