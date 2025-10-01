# Integrating a Third-Party Application into RDK-B using meta-rdk-sampleapp

## Introduction

This document explains how to integrate a real third-party application into the RDK-B stack using the meta-rdk-sampleapp Yocto/OpenEmbedded layer. It consolidates the actions already implemented in this layer and provides a step-by-step workflow to adapt them for a real application. The guidance aligns with the RDK Central “Integration Guide for third-party applications into RDK-B stack,” while focusing on concrete files and patterns present in this layer.

## Prerequisites

- Familiarity with Yocto/OpenEmbedded build system and RDK-B build environment.
- A functioning RDK-B build setup (e.g., Kirkstone) with other RDK layers included (meta-rdk, meta-rdk-broadband, meta-openembedded, etc.) per your platform requirements.
- meta-rdk-sampleapp added to bblayers.conf.
- Access to your third-party application source (e.g., a public Git repository or a tarball), including license details and any build instructions.
- Systemd-based target image assumed (default for RDK-B). If using an alternative init, adjust the service integration accordingly.

## Summary Description

The meta-rdk-sampleapp layer provides a minimal, working scaffold that demonstrates how to:
- Add an application recipe that builds from local files, with an easy path to switch to a real upstream source.
- Package the application and optionally install a systemd service to start it on boot.
- Pull the application into RDK-B images through a packagegroup and an image .bbappend.
- Provide placeholders for RDK-B integration hooks (e.g., Utopia/CCSP interactions, logging, and configuration defaults).
- Offer an optional distro include that appends defaults via local.conf or distro configuration.

You will adapt this scaffold by pointing the recipe to your real application source, declaring accurate dependencies, and adding any integration hooks that your application requires.

## Layer Structure

Below is the relevant structure of meta-rdk-sampleapp and what each component does.

- classes/
  - sampleapp-integrate.bbclass
- conf/
  - layer.conf
  - distro/include/
    - meta-rdk-sampleapp.inc
- recipes-core/images/
  - rdk-broadband-image.bbappend
- recipes-packagegroups/packagegroup-meta-rdk-sampleapp/
  - packagegroup-meta-rdk-sampleapp.bb
- recipes-sampleapp/sampleapp/
  - sampleapp_1.0.bb
  - sampleapp-native_1.0.bb
  - files/
    - LICENSE
    - Makefile
    - sampleapp.c
    - sampleapp.service
- recipes-support/rdk-logger/
  - rdk-logger.bbappend
- recipes-support/utopia/
  - utopia.bbappend
- INTEGRATION_STEPS.md
- README.md

## Main Files and Their Purpose

### classes/sampleapp-integrate.bbclass
- Purpose: A reusable class where you can centralize integration logic for third-party apps (e.g., defaults for systemd behavior, QA checks).
- Current content: Inherits systemd; contains an anonymous Python function placeholder for future policy or QA checks.

### conf/layer.conf
- Purpose: Registers this layer with BitBake, sets search paths for recipes and classes, and declares compatibility.
- Notes: LAYERSERIES_COMPAT is set to “kirkstone”. Priority and layer collection are defined. Optional LAYERDEPENDS can be enabled if your integration needs meta-rdk-specific classes/macros.

### conf/distro/include/meta-rdk-sampleapp.inc
- Purpose: Optional include that appends the sample packagegroup to IMAGE_INSTALL and enforces systemd defaults.
- Usage: Can be included from a distro config or local.conf to automatically pull in the application.

### recipes-core/images/rdk-broadband-image.bbappend
- Purpose: Appends the sample packagegroup to the RDK-B image so the app is included by default.
- Actionable: Copy/rename for your actual image recipe if it differs (e.g., if you use a custom final image name).

### recipes-packagegroups/packagegroup-meta-rdk-sampleapp/packagegroup-meta-rdk-sampleapp.bb
- Purpose: Provides a packagegroup that depends on the application package (sampleapp), making it easy to include via IMAGE_INSTALL.

### recipes-sampleapp/sampleapp/sampleapp_1.0.bb
- Purpose: Primary application recipe.
- Current behavior:
  - Builds from local files (Makefile + sampleapp.c).
  - Installs a binary and an optional systemd service (via PACKAGECONFIG systemd).
  - Enables service on boot by default.
- Customization: Replace local file SRC_URI with your app’s real source (git/tarball), define DEPENDS/RDEPENDS, and refine install paths.

### recipes-sampleapp/sampleapp/sampleapp-native_1.0.bb
- Purpose: Placeholder for a -native helper tool if your build requires host-side utilities (code generation, schema compilation, etc.).
- Current behavior: No-op.

### recipes-sampleapp/sampleapp/files/Makefile, sampleapp.c, sampleapp.service, LICENSE
- Purpose: Minimal sample application and service to prove out build and packaging.
- Customization: Replace these with your real application build instructions and service units if you continue to build locally, or remove when switching to upstream fetching.

### recipes-support/rdk-logger/rdk-logger.bbappend
- Purpose: Placeholder to integrate with rdk-logger if your app uses RDK logging conventions (e.g., setting defaults, adding configs).
- Customization: Add RDEPENDS and configuration as needed.

### recipes-support/utopia/utopia.bbappend
- Purpose: Placeholder for Utopia/CCSP integration hooks (e.g., syscfg defaults, startup ordering).
- Customization: Add CONFFILES, provide config templates, or patch startup scripts to integrate with the RDK-B management stack.

### INTEGRATION_STEPS.md and README.md
- Purpose: High-level guidance already in the layer explaining the scaffold and basic steps to include the app in images.

## Workflow: Integrating a Real Third-Party Application

Follow this sequence to replace the sample app with your real application.

### 1) Point the recipe to the real source
- Update recipes-sampleapp/sampleapp/sampleapp_1.0.bb:
  - Replace local file SRC_URI with your upstream source (e.g., a Git repository or a tarball).
  - For Git:
    - Set SRC_URI = "git://your.git.repo.url;protocol=https;branch=<branch>"
    - Set SRCREV to a commit hash.
    - Set S to "${WORKDIR}/git" (or according to your fetch).
  - For tarball:
    - Set SRC_URI = "https://example.com/yourapp-<ver>.tar.gz"
    - Add checksums (e.g., SRC_URI[sha256] = "<checksum>").
    - Set S to "${WORKDIR}/yourapp-<ver>".
- Remove local file entries (Makefile, sampleapp.c, sampleapp.service) if no longer used.
- Update HOMEPAGE and LIC_FILES_CHKSUM appropriately for your app’s license files.

### 2) Select the proper build system and compile steps
- Choose inherit classes that match your app’s build system (e.g., autotools, cmake, meson, go, cargo).
- If using a custom Makefile, ensure do_compile and do_install are adapted:
  - Use oe_runmake with required variables.
  - Install binaries, libraries, headers, and configs into the correct destinations (e.g., ${bindir}, ${sbindir}, ${libdir}, ${sysconfdir}).
- Define DEPENDS for build-time dependencies and RDEPENDS:${PN} for runtime requirements (e.g., systemd, libraries).
- If you need host tools, implement or expand a -native recipe.

### 3) Provide a systemd service (if applicable)
- If your application should run as a service:
  - Supply a proper service unit file and install it in do_install to ${systemd_system_unitdir}.
  - Ensure SYSTEMD_SERVICE:${PN} includes the correct service filename.
  - Consider whether to auto-enable on boot (SYSTEMD_AUTO_ENABLE:${PN} = "enable") or leave disabled for manual control.

### 4) RDK-B integration hooks
- Utopia/CCSP integration:
  - Use recipes-support/utopia/utopia.bbappend to add syscfg defaults, service dependencies, or configuration patches that fit the RDK-B management stack.
- Logging:
  - If using rdk-logger or another logging mechanism, add bbappends and configuration under recipes-support/rdk-logger/ to ship defaults or set up runtime dependencies.
- Configuration and data:
  - Ship configuration files to ${sysconfdir} and mark them in CONFFILES to preserve across updates.
  - Provide sensible defaults to work with RDK-B components out of the box.

### 5) Include the app in images
- Use one of the following:
  - Add packagegroup-meta-rdk-sampleapp to IMAGE_INSTALL in your local.conf or distro:
    - IMAGE_INSTALL:append = " packagegroup-meta-rdk-sampleapp"
  - Keep or adapt recipes-core/images/rdk-broadband-image.bbappend to your actual image recipe name.
  - Alternatively, include conf/distro/include/meta-rdk-sampleapp.inc in your distro to automatically pull in the packagegroup and systemd defaults.

### 6) Validate the integration
- Build the image and verify:
  - The package is produced and present in the image rootfs.
  - The service is installed, enabled, and starts successfully (if applicable).
  - Logs are produced and routed as expected (rdk-logger or syslog).
  - The application interacts correctly with RDK-B components (e.g., network brings-up before service starts).
- Add ptests or simple runtime checks if your app supports them.

## Customization Next Steps

- Replace sample placeholders:
  - Update SRC_URI, SRCREV/checksums, HOMEPAGE, and LIC_FILES_CHKSUM.
  - Remove sample local files once you switch to the upstream source.
- Build system alignment:
  - Inherit and configure the build tooling that your application uses (autotools, cmake, meson, or a custom make).
- Integration specifics:
  - Add RDEPENDS for runtime libraries.
  - Add config files under ${sysconfdir}; mark them in CONFFILES.
  - Add any D-Bus, netfilter, or other dependencies your app requires.
- Service tuning:
  - Adjust service unit ordering with After= and Wants= to match RDK-B bring-up (e.g., network-online.target).
  - Decide on auto-enable policies for production.
- RDK-B hooks:
  - Implement utopia.bbappend changes for syscfg defaults and service orchestration.
  - Integrate rdk-logger configs and log levels if needed.
- Security and compliance:
  - Verify licenses and compliance for your third-party code and dependencies.
  - Consider sandboxing, capabilities, or SELinux/AppArmor settings if used in your platform.

## Example: Minimal Recipe Changes for a Git-based App

Below is a sketch of how sampleapp_1.0.bb could evolve when switching to a Git source. Replace placeholders with your values.

```bitbake
SUMMARY = "YourApp: RDK-B integration"
DESCRIPTION = "Your third-party application integrated into RDK-B"
HOMEPAGE = "https://yourapp.example.com"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=<md5sum_of_license_file>"

PV = "1.0"
PR = "r0"

SRC_URI = "git://github.com/yourorg/yourapp.git;protocol=https;branch=main"
SRCREV = "<commit_hash>"
S = "${WORKDIR}/git"

inherit cmake systemd

DEPENDS = "openssl"
RDEPENDS:${PN} = "systemd"

SYSTEMD_SERVICE:${PN} = "yourapp.service"
SYSTEMD_AUTO_ENABLE:${PN} = "enable"

EXTRA_OECMAKE += "-DENABLE_FEATURE_X=ON"

do_install:append() {
    install -d ${D}${systemd_system_unitdir}
    install -m 0644 ${S}/packaging/systemd/yourapp.service ${D}${systemd_system_unitdir}/
}

FILES:${PN} += "${systemd_system_unitdir}/yourapp.service"
```

## References

- RDK Central: Integration Guide for third-party applications into RDK-B stack
  - https://wiki.rdkcentral.com/display/RDK/Integration+Guide+for+third-party+applications+into+RDK-B+stack
- meta-rdk-sampleapp existing documentation:
  - README.md
  - INTEGRATION_STEPS.md

## Conclusion

The meta-rdk-sampleapp layer provides a practical scaffold aligned with RDK-B integration practices. By switching the recipe to your real source, defining dependencies, and adding integration hooks (systemd, logging, Utopia/CCSP), you can bring a third-party application into the RDK-B image in a controlled and repeatable manner. Use the packagegroup and image .bbappend to include it in your target image, and validate its runtime behavior as part of your CI pipeline and on-device testing.
