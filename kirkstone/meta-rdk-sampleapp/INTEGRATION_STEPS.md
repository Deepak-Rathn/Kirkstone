Integration steps after layer.conf (per RDK-B guide, adapted for generic third-party app):

1) Application recipe: Provided at recipes-sampleapp/sampleapp/sampleapp_1.0.bb
   - Builds from local files; replace with real SRC_URI (git/tarball) and checksums.
   - Installs a systemd service (optional via PACKAGECONFIG).

2) Packagegroup: Provided at recipes-packagegroups/packagegroup-meta-rdk-sampleapp/packagegroup-meta-rdk-sampleapp.bb
   - Add to IMAGE_INSTALL to include the app.

3) Image inclusion:
   - Provided bbappend: recipes-core/images/rdk-broadband-image.bbappend (adjust image name as needed).
   - Alternatively, include conf/distro/include/meta-rdk-sampleapp.inc in your distro and/or local.conf.

4) Integration hooks:
   - Stubs at recipes-support/ for rdkspecific components (e.g., utopia, rdk-logger).

5) Licensing and documentation:
   - LICENSE, COPYING.MIT and README.md provided.

Assumptions:
- Systemd is available on target (toggle via PACKAGECONFIG/systemd).
- No special kernel/CCSP/HAL changes required for sample.
- Adjust image bbappend filename to match your actual image recipe.

Next steps for a real app:
- Update fetch (SRC_URI), dependencies, and install paths.
- Add real configuration files, logging config, syscfg defaults as needed.
- Add tests/ptests if applicable.
