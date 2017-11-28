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
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
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
  explicit ContainerWindow(status_message* i_pStatusMessage, char* media_path, QWidget *parent = 0);

private slots:
  void ShowRadio();
  void ReturnFromRadio();
  void ShowMp3();
  void ReturnFromMp3();
  void SendQuitMessage();
				      
private:

  QRect TransformResolution(int x, int y, int width, int height);
  void SetUpAnimations();
  WelcomeWindow  *Welcome;
  RadioWindow *Radio;
  Mp3Window *Mp3;
  
  //background
  QLabel *m_MainLabel;
  QImage *m_Background;
  // This is QT5 specific. Can we use that with yocto?
  QRect m_ScreenSize;
  // FIXME: This button is for development
  QPushButton *m_Quit;

  status_message* m_pStatusMessage;


  // Animations
  QPropertyAnimation* m_RadioIn;
  QPropertyAnimation* m_RadioOut;
  QPropertyAnimation* m_Mp3In;
  QPropertyAnimation* m_Mp3Out;
  QPropertyAnimation* m_WelcomeIn0;
  QPropertyAnimation* m_WelcomeOut0;
  QPropertyAnimation* m_WelcomeIn1;
  QPropertyAnimation* m_WelcomeOut1;
  QAnimationGroup* m_WelcomeToRadio;
  QAnimationGroup* m_RadioToWelcome;
  QAnimationGroup* m_Mp3ToWelcome;
  QAnimationGroup* m_WelcomeToMp3;
  
};

#endif
