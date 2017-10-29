

#include <QApplication>
#include <QLabel>
#include <QPushButton>

#include "window.hpp"

int main(int argc, char **argv){
    QApplication app(argc, argv);

    WelcomeWindow welcome;
    welcome.showFullScreen();

    return app.exec();
}
