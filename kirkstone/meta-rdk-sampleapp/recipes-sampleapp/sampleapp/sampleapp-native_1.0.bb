SUMMARY = "Native helper for sampleapp (placeholder)"
DESCRIPTION = "Placeholder native recipe demonstrating -native pattern if needed by build tooling."
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/files/common-licenses/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

inherit native sampleapp-integrate

SRC_URI = ""
S = "${WORKDIR}"

do_compile[noexec] = "1"
do_install[noexec] = "1"
