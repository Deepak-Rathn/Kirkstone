# meta-rdk-sampleapp

This Yocto/OpenEmbedded meta layer provides integration scaffolding for a generic third-party application on RDK-B (RDK-Broadband) per the "Integration Guide for third-party applications into RDK-B stack".

Scope:
- This layer assumes you have added meta-rdk-sampleapp to your bblayers (via bblayers.conf) and created conf/layer.conf (already present).
- It provides a reference recipe for a simple C sample app, a packagegroup to include it in images, and bbappends to hook into common RDK-B build customizations.
- It uses only open-source best practices; replace sample placeholders with real application specifics when integrating a real third-party app.

Integration Guide steps (condensed) after layer.conf:
1) Add layer to bblayers.conf (done externally in your build's conf/bblayers.conf).
2) Provide application recipe (.bb) including fetch, build, install, packaging.
3) If needed, add a -native or -cross helper (not required in this generic sample).
4) Create a packagegroup to include the app in images and/or update image recipes/bbappends to pull it in.
5) Provide config files, service units, and RDK-B integration hooks (systemd service, logging, syscfg defaults as applicable).
6) Add bbappends to adjust existing RDK-B components if needed.
7) Licensing and documentation.

Assumptions:
- No specific third-party source is provided; a simple "hello world" C app is built locally from files/ using autotools-like or simple make.
- Systemd is the default init system on the target image.
- No special kernel modules or HAL hooks are required for the sample.
- The application is safe to auto-start on boot as a demonstration.

How to include in your image:
- Ensure this layer is in your bblayers.conf.
- Add `packagegroup-meta-rdk-sampleapp` to IMAGE_INSTALL, for example via local.conf:
  IMAGE_INSTALL:append = " packagegroup-meta-rdk-sampleapp"
- Alternatively, add a bbappend for your target image to include the packagegroup.

Directory summary:
- conf/layer.conf           : Layer configuration (already present)
- recipes-sampleapp/        : Application recipe and files
- recipes-packagegroups/    : Packagegroup to include the app
- recipes-core/images/      : Optional bbappend to extend RDK-B images
- recipes-support/          : Placeholders for any bbappends to existing components
- LICENSE                   : License for this layer content

Replace sample content with your app specifics during real integration.
