/**
 Copyright 2017 Daniel Garcia Vaglio <degv364@gmail.com> Esteban Zamora Alvarado <estebanzacr.20@gmail.com>

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute,
 sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
 OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 **/

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

#include "file_names.hpp"
#include "messages.hpp"

#include "def.hpp"
#include "welcome_window.hpp"
#include "radio_window.hpp"
#include "mp3_window.hpp"

class ContainerWindow : public QWidget{
  Q_OBJECT  
public:
  explicit ContainerWindow(status_message* i_pStatusMessage,QWidget *parent = 0);

private slots:
  void showRadio();
  void returnFromRadio();
  void showMp3();
  void returnFromMp3();
  void sendQuitMessage();
				      
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

  status_message* m_pStatusMessage;
};

#endif