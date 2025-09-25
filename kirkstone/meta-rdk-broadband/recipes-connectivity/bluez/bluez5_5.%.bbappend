FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

# for bluez support
SRC_URI_append_broadband += "file://Bluetooth_service_dependency_broadband.patch"
