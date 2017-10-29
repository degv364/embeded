#ifndef WINDOW_H
#define WINDOW_H

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

class WelcomeWindow : public QWidget{
  Q_OBJECT  
public:
  explicit WelcomeWindow(QWidget *parent = 0);
				      
private slots:
  void radioSelected();
  void mp3Selected();
  
private:

  QRect transformResolution(int x, int y, int width, int height);
  
  QPushButton *m_radioButton;
  QPushButton *m_mp3Button;

  //background
  QLabel *m_mainLabel;
  QImage *m_background;
  // This is QT5 specific. Can we use that with yocto?
  QRect m_screenSize;
  // FIXME: This button is for development
  QPushButton *m_Quit;
  QTextEdit *m_TextMessage;
};

#endif
