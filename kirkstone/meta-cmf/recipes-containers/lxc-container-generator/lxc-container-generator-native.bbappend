FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

SRC_URI += "${@bb.utils.contains('DISTRO_FEATURES', 'disable-lxcgen-warnings','file://0001-convert-error-to-warning.patch;apply=no','',d)} \
           ${@bb.utils.contains('DISTRO_FEATURES', 'disable-lxcgen-warnings','file://0001-switch-exceptions-to-warnings.diff;apply=no','',d)} \
           ${@bb.utils.contains('DISTRO_FEATURES', 'disable-lxcgen-warnings','file://0001-capkeep-support.diff;apply=no','',d)} \
"

addtask do_apply_patch after do_unpack before do_configure

do_apply_patch() {
    cd ${S}
    if [ ! -e patch_applied ]; then
        if [ "${@bb.utils.contains('DISTRO_FEATURES', 'disable-lxcgen-warnings', 'true', 'false', d)}" = "true" ]; then
                patch -p1 < ${WORKDIR}/0001-convert-error-to-warning.patch
                patch -p1 < ${WORKDIR}/0001-switch-exceptions-to-warnings.diff
                patch -p1 < ${WORKDIR}/0001-capkeep-support.diff
        fi
        touch patch_applied
    fi
}
