require ccsp_common_rpi.inc
CFLAGS_append_kirkstone = " -fcommon "
LDFLAGS_append_aarch64 = " -lnanomsg "
FILES_${PN} += " /lib/systemd/system "
