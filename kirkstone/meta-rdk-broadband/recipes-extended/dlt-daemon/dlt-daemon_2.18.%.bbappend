FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

SRC_URI_append = " file://CVE-2023-26257_fix.patch \
                 "
