TEMPLATE += app
  QT += widgets

  QMAKE_CXXFLAGS += -Wall -pedantic -pthread -I=/usr/include/gstreamer-1.0 -I=/usr/include/glib-2.0 -I=/usr/lib/glib-2.0/include

  QMAKE_LIBS += -pthread -lgobject-2.0 -lgstreamer-1.0 -lglib-2.0

  HEADERS += file_names.hpp
  HEADERS += messages.hpp

  HEADERS += gui/welcome_window.hpp
  HEADERS += gui/radio_window.hpp
  HEADERS += gui/mp3_window.hpp
  HEADERS += gui/container_window.hpp
  HEADERS += gui/def.hpp

  HEADERS += audio/audio_server.hpp
  HEADERS += audio/mp3_handler.hpp


  SOURCES += gui/welcome_window.cpp
  SOURCES += gui/radio_window.cpp
  SOURCES += gui/mp3_window.cpp
  SOURCES += gui/container_window.cpp

  SOURCES += audio/audio_server.cpp
  SOURCES += audio/mp3_handler.cpp


SOURCES += qt_radio.cpp
