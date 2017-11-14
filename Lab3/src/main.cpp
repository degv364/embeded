
#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <thread>

//Internal includes
#include "audio/audio_server.hpp"
#include "gui/container_window.hpp"

int main(int argc, char **argv){
    QApplication app(argc, argv);
    gst_init (&argc, &argv);

    ContainerWindow container;
    container.showFullScreen();

    std::thread l_AudioServer(AudioServer);
    
    app.exec();
    l_AudioServer.join();
    return 0;
}

