#ifndef WELCOME_H
#define WELCOME_H

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
  explicit WelcomeWindow(QRect i_screenSize,QWidget *parent = 0);

  QPushButton* radioButton(){return m_radioButton;};								
				      
private slots:
  void radioSelected();
  void mp3Selected();
  
private:

  QRect transformResolution(int x, int y, int width, int height);
  
  QPushButton *m_radioButton;
  QPushButton *m_mp3Button;


  // This is QT5 specific. Can we use that with yocto?
  QRect m_screenSize;
  // FIXME: This message is for development
  QTextEdit *m_TextMessage;
};

#endif
