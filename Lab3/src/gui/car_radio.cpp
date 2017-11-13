

#include <QApplication>
#include <QLabel>
#include <QPushButton>

#include "container_window.hpp"

int main(int argc, char **argv){
    QApplication app(argc, argv);

    ContainerWindow container;
    container.showFullScreen();

    return app.exec();
}
