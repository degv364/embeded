SUMMARY = "Qt5 Car Radio Embedded Application"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

inherit qmake5

DEPENDS = "qtbase qtdeclarative gstreamer1.0"
RDEPENDS_${PN} = "qtbase qtdeclarative gstreamer1.0"

SRC_URI = "git://github.com/degv364/embeded.git;branch=qt-dev"
SRCREV = "${AUTOREV}"

S = "${WORKDIR}/git/Lab3/src"

do_install_append() {
     install -d ${D}${bindir}
     install -m 0755 ${WORKDIR}/build/qt_radio ${D}${bindir}

     install -d ${D}/opt/qt_radio/
     cp -r ${WORKDIR}/git/Lab3/media/ ${D}/opt/qt_radio/
}

FILES_${PN} += "/opt/qt_radio/"
