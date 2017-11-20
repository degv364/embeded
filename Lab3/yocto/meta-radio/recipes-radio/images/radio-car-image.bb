DESCRIPTION = "Car radio image with Qt5 support"

# require recipes-core/images/core-image-minimal.bb
require recipes-graphics/images/core-image-x11.bb

EXTRA_IMAGE_FEATURES = "debug-tweaks ssh-server-openssh"

IMAGE_INSTALL += " gcc g++ binutils libgcc libgcc-dev libstdc++ libstdc++-dev libstdc++-staticdev \
autoconf automake ccache chkconfig glib-networking  \
packagegroup-core-buildessential pkgconfig  \
boost cmake zlib glib-2.0  \
    gdb \
    gstreamer1.0 \
    gst-player \
    gstreamer1.0-plugins-base \
    gstreamer1.0-plugins-good \
    gstreamer1.0-plugins-bad \
    gstreamer1.0-plugins-ugly \
    openssh-sftp-server \
    qtbase-plugins \
    qtbase-tools \
    qtdeclarative \
    qtdeclarative-tools \
    qtdeclarative-qmlplugins \
    qtmultimedia \
    qtmultimedia-plugins \
    qtmultimedia-qmlplugins \
    qtsvg \
    qtsvg-plugins \
    qtsensors \
    qtimageformats-plugins \
    qtsystems \
    qtsystems-tools \
    qtsystems-qmlplugins \
    qtscript \
    qt3d \
    qt3d-qmlplugins \
    qt3d-tools \
    qtgraphicaleffects-qmlplugins \
    qtconnectivity-qmlplugins \
    cinematicexperience \
    cairo pango fontconfig freetype pulseaudio dbus \
    alsa-utils alsa-tools \
    i2c-tools \ 
    qt-radio \
    "
inherit populate_sdk_qt5