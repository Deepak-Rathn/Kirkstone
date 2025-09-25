require ccsp_common.inc

DEPENDS_append = " ccsp-common-library utopia libparodus"
DEPENDS_append = " ${@bb.utils.contains("DISTRO_FEATURES", "Opensync_4.4", "opensync-4.4.0", "opensync-2.4.1", d)}"
DEPENDS_append = " hal-wifi hal-cm  hal-dhcpv4c hal-ethsw hal-moca hal-mso_mgmt hal-mta hal-platform hal-vlan hal-wifi avro-c "
RDEPENDS_${PN}_append = " libparodus"

EXTRA_OECONF_append = " --enable-ccsp-common"
EXTRA_OECONF_append = " --enable-dml"
EXTRA_OECONF_append = " --enable-journalctl"

CFLAGS_append = " -I${STAGING_INCDIR}/dbus-1.0"
CFLAGS_append = " -I${STAGING_LIBDIR}/dbus-1.0/include"
CFLAGS_append = " -I${STAGING_INCDIR}/libparodus"

LDFLAGS_append = " -ldbus-1"
LDFLAGS_append = " -llibparodus"
LDFLAGS_append = " -ltrower-base64"
LDFLAGS_append = " -lutctx"
LDFLAGS_append = " -ldpp"

do_compile_prepend () {
    (${PYTHON} ${STAGING_BINDIR_NATIVE}/dm_pack_code_gen.py ${S}/config/TR181-WiFi-USGv2.XML ${S}/source/dml/wifi_ssp/dm_pack_datamodel.c)
}
