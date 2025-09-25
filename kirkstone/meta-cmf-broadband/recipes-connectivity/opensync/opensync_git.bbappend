FILESEXTRAPATHS_prepend := "${THISDIR}/files:"
OPENSYNC_SERVICE_PROVIDER_URI += " ${@bb.utils.contains('DISTRO_FEATURES', 'extender', 'file://0001-Update-bhaul-credential.patch;patchdir=${WORKDIR}/git/service-provider/local', '', d)} "
