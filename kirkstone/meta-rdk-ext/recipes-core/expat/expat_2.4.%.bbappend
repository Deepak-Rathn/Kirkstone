FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

SRC_URI_append = " file://CVE-2022-43680_fix.patch \
                 "

SRC_URI_remove_client = "file://CVE-2022-43680_fix.patch \
                        "
