FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"
SRC_URI_append_kirkstone = " file://Define_macros_before_use_Kirkstone.patch"
