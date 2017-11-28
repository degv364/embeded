SUMMARY = "Car radio image with Qt5 support"

inherit core-image
inherit populate_sdk_qt5

REQUIRED_DISTRO_FEATURES = "splash x11 pulseaudio"

IMAGE_FEATURES += "splash hwcodecs x11"

IMAGE_INSTALL += "glib-2.0 \
    mini-x-session \
    gstreamer1.0 \
    gst-player \
    gstreamer1.0-plugins-base \
    gstreamer1.0-plugins-good \
    gstreamer1.0-plugins-bad \
    gstreamer1.0-plugins-ugly \
    qtbase-plugins \
    qtbase-tools \
    cairo pango fontconfig freetype dbus \
    pulseaudio pulseaudio-server pulseaudio-misc \
    alsa-utils alsa-plugins alsa-tools \
    i2c-tools \ 
    qt-radio \
    liberation-fonts \
    "