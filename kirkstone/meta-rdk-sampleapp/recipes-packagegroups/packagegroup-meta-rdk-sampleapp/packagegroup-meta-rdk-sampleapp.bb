SUMMARY = "Package group for meta-rdk-sampleapp"
DESCRIPTION = "Pulls in the sample app and any future dependencies into RDK-B images"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/files/common-licenses/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

PR = "r0"

inherit packagegroup

RDEPENDS:${PN} = "\
    sampleapp \
"
