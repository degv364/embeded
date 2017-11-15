
#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <thread>

//Internal includes
#include "file_names.hpp"
#include "messages.hpp"

#include "audio/audio_server.hpp"
#include "gui/container_window.hpp"

int main(int argc, char **argv){
    QApplication app(argc, argv);
    gst_init (&argc, &argv);

    // Initialize intercommunication message
    status_message l_stStatusMessage;
    l_stStatusMessage.Play = false;
    l_stStatusMessage.Reset = false;
    l_stStatusMessage.Handled = true;
    
    ContainerWindow container(&l_stStatusMessage);
    container.showFullScreen();

    std::thread l_AudioServer(AudioServer, &l_stStatusMessage);
    
    app.exec();
    l_AudioServer.join();
    return 0;
}

