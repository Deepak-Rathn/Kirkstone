SUMMARY = "Scripts for rdkb devices."
SECTION = "console/utils"

LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=175792518e4ac015ab6696d16c4f607e"

PV = "${RDK_RELEASE}+git${SRCPV}"

SYSINTB_DEVICE ??= "intel-x86-pc/rdk-broadband"
HASBCI = "${@bb.utils.contains('DISTRO_FEATURES', 'bci', 'true', 'false', d)}"
HASEPON = "${@bb.utils.contains('DISTRO_FEATURES', 'epon', 'true', 'false', d)}"
HASDSL = "${@bb.utils.contains('DISTRO_FEATURES', 'dsl', 'true', 'false', d)}"

SRC_URI = "${CMF_GIT_ROOT}/rdkb/components/opensource/ccsp/sysint;protocol=${CMF_GIT_PROTOCOL};branch=${CMF_GIT_BRANCH};name=sysintbroadband"
SRC_URI += "${CMF_GIT_ROOT}/rdkb/devices/intel-x86-pc/emulator/sysint;module=.;protocol=${CMF_GIT_PROTOCOL};branch=${CMF_GIT_BRANCH};destsuffix=git/device;name=sysintdevice"

SRCREV_sysintbroadband = "${AUTOREV}"
SRCREV_sysintdevice = "${AUTOREV}"
SRCREV_FORMAT = "sysintbroadband_sysintdevice"

#SRCREV = "${AUTOREV}"

S = "${WORKDIR}/git"

inherit systemd breakpad-logmapper

do_install() {
	install -d ${D}${sysconfdir}
        install -d ${D}/rdklogger
        install -d ${D}${systemd_unitdir}/system
        # Creating generic location for sysint utils 
        install -d ${D}${base_libdir}/rdk
        if [ -f ${S}/device/imagelist ]; then
	    install -m 0644 ${S}/device/imagelist ${D}${sysconfdir}
        fi
	install -m 0755 ${S}/*.sh ${D}/rdklogger/
        
        if ${@bb.utils.contains('DISTRO_FEATURES', 'snmppa', 'false', 'true', d)}; then
             rm -f ${D}/rdklogger/handlesnmpv3.sh
        fi

	if [ -f ${S}/device/etc/device.properties ]; then
            install -m 0644 ${S}/device/etc/device.properties ${D}${sysconfdir}
        else
            install -m 0644 ${S}/etc/device.properties ${D}${sysconfdir}
        fi
	if [ ${HASBCI} = "true" ]; then
	    echo "IS_BCI=yes" >> ${D}${sysconfdir}/device.properties
	    sed -i -e 's/PX5001/PX5001B/g' ${D}${sysconfdir}/device.properties
	fi

	if ${@bb.utils.contains('DISTRO_FEATURES', 'meshwifi', 'true', 'false', d)}; then
            echo "MESH_SUPPORTED=true" >> ${D}${sysconfdir}/device.properties
	else
	    echo "MESH_SUPPORTED=false" >> ${D}${sysconfdir}/device.properties
	fi

	if ${@bb.utils.contains('DISTRO_FEATURES', 'nohomesecurity', 'true', 'false', d)}; then
           echo "HOMESECURITY_SUPPORTED=no" >> ${D}${sysconfdir}/device.properties
	fi

	if [ ${HASEPON} = "true" ]; then
	    echo "WAN_TYPE=EPON" >> ${D}${sysconfdir}/device.properties
	elif [ ${HASDSL} = "true" ]; then
	    echo "WAN_TYPE=DSL" >> ${D}${sysconfdir}/device.properties
	else
	    echo "WAN_TYPE=DOCSIS" >> ${D}${sysconfdir}/device.properties
	fi

	if [ -f ${S}/device/etc/dcm.properties ]; then
	    install -m 0644 ${S}/device/etc/dcm.properties ${D}${sysconfdir}
	fi
	if [ -f ${S}/device/etc/include.properties ]; then
	    install -m 0644 ${S}/device/etc/include.properties ${D}${sysconfdir}
	fi
        install -m 0644 ${S}/etc/telemetry2_0.properties ${D}${sysconfdir}
	install -m 755 ${S}/log_timestamp.sh ${D}${sysconfdir}
   	install -m 755 ${S}/postwanstatusevent.sh ${D}${base_libdir}/rdk
        if ${@bb.utils.contains('DISTRO_FEATURES', 'snmppa', 'true', 'false', d)}; then
             install -m 755 ${S}/handlesnmpv3.sh ${D}${base_libdir}/rdk
        fi
        rm -f ${D}/rdklogger/log_timestamp.sh

        if ${@bb.utils.contains('DISTRO_FEATURES', 'systemd', 'true', 'false', d)}; then
             install -m 0644 ${S}/ocsp-support.service ${D}${systemd_unitdir}/system
        fi
        install -m 0755 ${S}/ocsp-support.sh ${D}${base_libdir}/rdk/

    	if ${@bb.utils.contains('DISTRO_FEATURES', 'WanFailOverSupportEnable', 'true', 'false', d)}; then
        	echo "WanFailOverSupportEnable=true" >> ${D}${sysconfdir}/device.properties
    	fi
    	if ${@bb.utils.contains('DISTRO_FEATURES', 'rdkb_extender', 'true', 'false', d)}; then
       		echo "rdkb_extender=true" >> ${D}${sysconfdir}/device.properties
    	fi
}

do_install_append_qemux86broadband() {
	install -d ${D}${systemd_unitdir}/system
        install -m 0755 ${S}/device/lib/rdk/* ${D}${base_libdir}/rdk
	install -m 0755 ${S}/device/systemd_units/* ${D}${systemd_unitdir}/system/
}

do_install_append_arrisxb3atom () {
# Config files and scripts
  install -m 0755 ${S}/flush_logs_atom.sh ${D}/rdklogger/flush_logs.sh
 install -m 0755 ${S}/atom_log_monitor.sh ${D}/rdklogger/atom_log_monitor.sh
}
do_install_append_container() {
       echo "CONTAINER_SUPPORT=1" >> ${D}${sysconfdir}/device.properties
       echo "LXC_BRIDGE_NAME=lxclink0" >> ${D}${sysconfdir}/device.properties
       install -d ${D}${systemd_unitdir}/system/
       install -d ${D}${base_libdir}/rdk
       install -m 0644 ${S}/lxc.path ${D}${systemd_unitdir}/system/
       install -m 0644 ${S}/iptables_lxc.service ${D}${systemd_unitdir}/system/
       install -m 0644 ${S}/iptables_lxc.path ${D}${systemd_unitdir}/system/
       install -m 0755 ${S}/getip_file.sh ${D}${base_libdir}/rdk
       install -m 0755 ${S}/getipv6_container.sh ${D}${base_libdir}/rdk
}

do_install_append_rdkzram() {
       echo "ZRAM_MEM_MAX_PERCENTAGE=50" >> ${D}${sysconfdir}/device.properties
       install -m 0755 ${S}/init-zram.sh ${D}${base_libdir}/rdk/
       #service for host side
       install -m 0755 ${S}/rdkzram_host.service ${D}${base_libdir}/rdk/rdkzram.service
       #service for peer/atom side
       install -d ${D}${systemd_unitdir}/system
       install -m 0755 ${S}/rdkzram.service ${D}${systemd_unitdir}/system/
	rm -f ${D}/rdklogger/init-zram.sh
}


SYSTEMD_SERVICE_${PN}_append_qemux86broadband = "  dropbear.service"
SYSTEMD_SERVICE_${PN}_append_rdkzram = " rdkzram.service"
SYSTEMD_SERVICE_${PN} += " ${@bb.utils.contains('DISTRO_FEATURES', 'systemd', 'ocsp-support.service', '', d)}"

FILES_${PN} += "${sysconfdir}/*"
FILES_${PN} += "rdklogger/*"
FILES_${PN} += "${base_libdir}/rdk/*"
FILES_${PN}_append_qemux86broadband += "${systemd_unitdir}/system/*"

FILES_${PN}_append_arrisxb3atom += " \
          /rdklogger/atom_log_monitor.sh \
	   /rdklogger/flush_logs_atom.sh \
         "

FILES_${PN}_append_container = " \
           ${systemd_unitdir}/system/iptables_lxc.service \
           ${systemd_unitdir}/system/iptables_lxc.path \
           ${base_libdir}/rdk/getip_file.sh \
           ${base_libdir}/rdk/getipv6_container.sh \
           ${base_libdir}/rdk/iptables_container.sh \
         "

SYSTEMD_SERVICE_${PN}_append_container = " iptables_lxc.path \
                                           lxc.path \
                                         "
# Breakpad processname and logfile mapping
BREAKPAD_LOGMAPPER_PROCLIST = "bleagent"
BREAKPAD_LOGMAPPER_LOGLIST = "Blelog.txt.0"
