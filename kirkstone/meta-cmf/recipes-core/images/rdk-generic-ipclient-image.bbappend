#REFPLTV-976 removing the Control Manager service, as feature not fully functional.
require add-non-root-user-group.inc
require lxc-image.inc
require recipes-core/images/rdk-generic.inc

ROOTFS_POSTPROCESS_COMMAND += "remove_Failure_case_dsmgr_services; "

IMAGE_INSTALL_append = " ${@bb.utils.contains('DISTRO_FEATURES', 'streamfs', 'streamfs streamfs-fcc', '', d)}"

# Remove QT5 dependencies
IMAGE_INSTALL_remove = "packagegroup-rdk-qt5"

remove_Failure_case_dsmgr_services () {
                       sed -i '/OnFailure=reboot-notifier@%i.service/ c\#OnFailure=reboot-notifier@%i.service' ${IMAGE_ROOTFS}${systemd_unitdir}/system/dsmgr.service
}
