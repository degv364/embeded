SUMMARY = "Car radio image with Qt5 support"

inherit core-image
# inherit populate_sdk_qt5

REQUIRED_DISTRO_FEATURES = "splash x11 pulseaudio"

IMAGE_FEATURES += "splash hwcodecs x11"

# pulseaudio-client-conf-radio
# qtdeclarative
# qtdeclarative-tools
# qtdeclarative-qmlplugins
# qtmultimedia
# qtmultimedia-plugins
# qtmultimedia-qmlplugins
# qtsvg
# qtsvg-plugins
# qtsensors
# qtimageformats-plugins
# qtsystems
# qtsystems-tools
# qtsystems-qmlplugins
# qtscript
# qt3d
# qt3d-qmlplugins
# qt3d-tools
# qtgraphicaleffects-qmlplugins
# qtconnectivity-qmlplugins
# gst-player
# alsa-plugins 

IMAGE_INSTALL += "glib-2.0 \
    mini-x-session \
    psplash \
    gstreamer1.0 \
    gstreamer1.0-plugins-base \
    gstreamer1.0-plugins-good \
    gstreamer1.0-plugins-bad \
    gstreamer1.0-plugins-ugly \
    qtbase-plugins \
    qtbase-tools \
    cairo pango fontconfig freetype dbus \
    pulseaudio pulseaudio-server pulseaudio-misc \
    alsa-utils alsa-tools \
    i2c-tools \ 
    qt-radio \
    liberation-fonts \
    "