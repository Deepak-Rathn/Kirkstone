FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

SRC_URI_append = " file://ocsp_request_to_CA_Directly_curl_7.82.patch"

SRC_URI_append = " file://CVE-2022-32221_7.82.0_fix.patch \
                   file://CVE-2022-43552_7.82.0_fix.patch \
                 "

CURLGNUTLS = "--without-gnutls --with-ssl"
DEPENDS += " openssl"

SRC_URI_append_broadband = " file://CVE-2023-27534_7.82_fix.patch \
                             file://CVE-2023-27538_7.82_fix.patch \
                             file://CVE-2023-28320_7.82_fix.patch \
                           "

# see https://lists.yoctoproject.org/pipermail/poky/2013-December/009435.html
# We should ideally drop ac_cv_sizeof_off_t from site files but until then
EXTRA_OECONF += "${@bb.utils.contains('DISTRO_FEATURES', 'largefile', 'ac_cv_sizeof_off_t=8', '', d)}"

PACKAGECONFIG_append = " ipv6 "
PACKAGECONFIG[ipv6] = "--enable-ipv6,--disable-ipv6,"
