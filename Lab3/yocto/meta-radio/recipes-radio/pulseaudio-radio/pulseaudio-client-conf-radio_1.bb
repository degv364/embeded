SUMMARY = "/etc/pulse/client.conf tailored for Qt5 Radio Car (based on Sato Pulse configuration)"
SECTION = "multimedia"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

SRC_URI = "file://50-radio.conf"
S = "${WORKDIR}"

inherit allarch

do_install() {
	install -d ${D}${sysconfdir}/pulse/client.conf.d
	install -m 0644 ${S}/50-radio.conf ${D}${sysconfdir}/pulse/client.conf.d/50-radio.conf
}

FILES_${PN} = "${sysconfdir}/pulse/client.conf.d/50-radio.conf"
CONFFILES_${PN} = "${sysconfdir}/pulse/client.conf.d/50-radio.conf"
