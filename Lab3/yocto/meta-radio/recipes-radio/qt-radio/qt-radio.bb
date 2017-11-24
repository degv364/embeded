SUMMARY = "Qt5 Car Radio Embedded Application"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

inherit qmake5
inherit systemd

DEPENDS = "qtbase qtdeclarative gstreamer1.0"
RDEPENDS_${PN} = "qtbase qtdeclarative gstreamer1.0"

SRC_URI = "git://github.com/degv364/embeded.git;branch=qt-dev"
SRC_URI += "file://qt_radio.service"
SRCREV = "${AUTOREV}"

S = "${WORKDIR}/git/Lab3/src"

SYSTEMD_SERVICE_${PN} = " qt_radio.service"

FILES_${PN} += "/opt/qt_radio/ \
                qt_radio.service \
                "

do_install_append() {
     # Install qt_radio executable
     install -d ${D}${bindir}
     install -m 0755 ${WORKDIR}/build/qt_radio ${D}${bindir}

     # Include qt_radio media files to image
     install -d ${D}/opt/qt_radio/
     cp -r ${WORKDIR}/git/Lab3/media/ ${D}/opt/qt_radio/

     # Install systemd service
     install -d ${D}${nonarch_base_libdir}/systemd/system
     install -m 0755 ${WORKDIR}/qt_radio.service ${D}${nonarch_base_libdir}/systemd/system
}