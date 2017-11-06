#ifndef CONTAINER_H
#define CONTAINER_H

#include <QWidget>
#include <QApplication>
#include <QPushButton>
#include <QProgressBar>
#include <QSlider>
#include <QTextEdit>
#include <QScreen>
#include <QLabel>
#include <QPixmap>
#include <QIcon>
// Included for debugging. Final product wont print to stdout
#include <iostream>

#include "def.hpp"
#include "welcome_window.hpp"
#include "radio_window.hpp"
#include "mp3_window.hpp"

class ContainerWindow : public QWidget{
  Q_OBJECT  
public:
  explicit ContainerWindow(QWidget *parent = 0);

private slots:
  void showRadio();
  void returnFromRadio();
				      
private:

  QRect transformResolution(int x, int y, int width, int height);
  WelcomeWindow  *welcome;
  RadioWindow *radio;
  Mp3Window *mp3;
  
  //background
  QLabel *m_mainLabel;
  QImage *m_background;
  // This is QT5 specific. Can we use that with yocto?
  QRect m_screenSize;
  // FIXME: This button is for development
  QPushButton *m_Quit;
};

#endif
