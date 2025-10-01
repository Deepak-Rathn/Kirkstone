# Include sample packagegroup in RDK-B image by default (adjust to your actual image name)
# If your project uses a different final image recipe, copy/rename this bbappend accordingly.
IMAGE_INSTALL:append = " packagegroup-meta-rdk-sampleapp"
