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

#include "container_window.hpp"



ContainerWindow::ContainerWindow(status_message* i_pStatusMessage, char* media_path,QWidget *parent):
  QWidget(parent){

  m_pStatusMessage = i_pStatusMessage;
  
  QScreen* l_Screen = QGuiApplication::primaryScreen();
  m_screenSize = l_Screen->geometry();

  m_mainLabel = new QLabel(this);
  char l_cPathToBackGround[MAX_PATH_LENGTH];
  strcpy(l_cPathToBackGround, media_path);
  strcat(l_cPathToBackGround, "leather_metal.jpg");
  QPixmap backgroundImage(l_cPathToBackGround);
  backgroundImage = backgroundImage.scaled(m_screenSize.width(),m_screenSize.height());
  m_mainLabel->setPixmap(backgroundImage);
  m_mainLabel->setGeometry(transformResolution(0,0, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT));
  
  // QuitButton
  char l_cPathToQuitIcon[MAX_PATH_LENGTH];
  strcpy(l_cPathToQuitIcon, media_path);
  strcat(l_cPathToQuitIcon, "icons/quit.png");
  QIcon l_quitIcon(l_cPathToQuitIcon);
  m_Quit = new QPushButton(this);
  m_Quit->setIcon(l_quitIcon);
  m_Quit->setIconSize(QSize(30,30));
  m_Quit->setFlat(true);
  m_Quit->setGeometry(transformResolution(DEFAULT_SCREEN_WIDTH-30,1,30,30));

  // Welocme window
  welcome = new WelcomeWindow(m_screenSize, media_path, this);
  welcome->setGeometry(transformResolution(30,30,DEFAULT_SCREEN_WIDTH-60, DEFAULT_SCREEN_HEIGHT-60));

  // Radio Window
  radio = new RadioWindow(m_screenSize, media_path, this);
  radio->setGeometry(transformResolution(30,1000,DEFAULT_SCREEN_WIDTH-60, DEFAULT_SCREEN_HEIGHT-60));

  // Mp3 Window
  mp3 = new Mp3Window(m_screenSize, m_pStatusMessage,  media_path, this);
  mp3->setGeometry(transformResolution(30,1000,DEFAULT_SCREEN_WIDTH-60, DEFAULT_SCREEN_HEIGHT-60));

  setUpAnimations();
  
  // Do the connection
  connect(m_Quit, SIGNAL(clicked()), QApplication::instance(), SLOT(quit()) );
  connect(m_Quit, SIGNAL(clicked()), this, SLOT(sendQuitMessage()) );

  // radio transition
  connect(welcome->radioButton(), SIGNAL(clicked()), this , SLOT(showRadio()) );
  connect(radio->returnButton(), SIGNAL(clicked()), this, SLOT(returnFromRadio()) );
  connect(welcome->mp3Button(), SIGNAL(clicked()), this , SLOT(showMp3()) );
  connect(mp3->returnButton(), SIGNAL(clicked()), this, SLOT(returnFromMp3()) );
  
}

// To simplify the fact that this should work with many resolutions
// Lets assume that we always use a 1366x768, and use thisfunction
// to get the real values.
QRect
ContainerWindow::transformResolution(int x, int y, int width, int height){
  // THis function should only be called once we have the resolution
  float new_x = ((float) x)/DEFAULT_SCREEN_WIDTH*((float)m_screenSize.width());
  float new_y = ((float) y)/DEFAULT_SCREEN_HEIGHT*((float)m_screenSize.height());

  float new_width = ((float) width)/DEFAULT_SCREEN_WIDTH*((float)m_screenSize.width());
  float new_height = ((float) height)/DEFAULT_SCREEN_HEIGHT*((float)m_screenSize.height());

  return QRect((int)new_x, (int)new_y, (int)new_width, (int)new_height);
}


void
ContainerWindow::showRadio(){
  m_WelcomeToRadio->start();
}

void
ContainerWindow::returnFromRadio(){
  m_RadioToWelcome->start();
}

void
ContainerWindow::showMp3(){
  m_WelcomeToMp3->start();
}

void
ContainerWindow::returnFromMp3(){
  mp3->stopSong();
  m_Mp3ToWelcome->start();
}


void
ContainerWindow::sendQuitMessage(){
  m_pStatusMessage->locker.lock();
  m_pStatusMessage->RequiredAction = QUIT;
  m_pStatusMessage->Handled = false;
  m_pStatusMessage->locker.unlock();
  
}

void
ContainerWindow::setUpAnimations(){
  // Radio In
  m_RadioIn = new QPropertyAnimation(radio, "geometry");
  m_RadioIn->setDuration(DEFAULT_ANIMATION_DURATION);
  m_RadioIn->setStartValue(transformResolution(30, DEFAULT_SCREEN_HEIGHT,
					      DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT));
  m_RadioIn->setEndValue(transformResolution(30,30, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT));

  // Radio Out
  m_RadioOut = new QPropertyAnimation(radio, "geometry");
  m_RadioOut->setDuration(DEFAULT_ANIMATION_DURATION);
  m_RadioOut->setEndValue(transformResolution(30, DEFAULT_SCREEN_HEIGHT,
					      DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT));
  m_RadioOut->setStartValue(transformResolution(30,30, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT));

  //Mp3 in
  m_Mp3In = new QPropertyAnimation(mp3, "geometry");
  m_Mp3In->setDuration(DEFAULT_ANIMATION_DURATION);
  m_Mp3In->setStartValue(transformResolution(30, DEFAULT_SCREEN_HEIGHT,
					      DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT));
  m_Mp3In->setEndValue(transformResolution(30,30, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT));

  //Mp3 Out
  m_Mp3Out = new QPropertyAnimation(mp3, "geometry");
  m_Mp3Out->setDuration(DEFAULT_ANIMATION_DURATION);
  m_Mp3Out->setEndValue(transformResolution(30, DEFAULT_SCREEN_HEIGHT,
					      DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT));
  m_Mp3Out->setStartValue(transformResolution(30,30, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT));

  // Welcome In for radio
  m_WelcomeIn0 = new QPropertyAnimation(welcome, "geometry");
  m_WelcomeIn0->setDuration(DEFAULT_ANIMATION_DURATION);
  m_WelcomeIn0->setStartValue(transformResolution(30, -DEFAULT_SCREEN_HEIGHT,
					      DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT));
  m_WelcomeIn0->setEndValue(transformResolution(30,30, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT));

  // Welcome Out for radio
  m_WelcomeOut0 = new QPropertyAnimation(welcome, "geometry");
  m_WelcomeOut0->setDuration(DEFAULT_ANIMATION_DURATION);
  m_WelcomeOut0->setEndValue(transformResolution(30, -DEFAULT_SCREEN_HEIGHT,
					      DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT));
  m_WelcomeOut0->setStartValue(transformResolution(30,30, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT));

  // Welcome In for mp3
  m_WelcomeIn1 = new QPropertyAnimation(welcome, "geometry");
  m_WelcomeIn1->setDuration(DEFAULT_ANIMATION_DURATION);
  m_WelcomeIn1->setStartValue(transformResolution(30, -DEFAULT_SCREEN_HEIGHT,
					      DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT));
  m_WelcomeIn1->setEndValue(transformResolution(30,30, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT));

  // Welcome Out for mp3
  m_WelcomeOut1 = new QPropertyAnimation(welcome, "geometry");
  m_WelcomeOut1->setDuration(DEFAULT_ANIMATION_DURATION);
  m_WelcomeOut1->setEndValue(transformResolution(30, -DEFAULT_SCREEN_HEIGHT,
					      DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT));
  m_WelcomeOut1->setStartValue(transformResolution(30,30, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT));

  //Transitions
  
  m_WelcomeToRadio = new QParallelAnimationGroup;
  m_WelcomeToRadio->addAnimation(m_WelcomeOut0);
  m_WelcomeToRadio->addAnimation(m_RadioIn);

  m_RadioToWelcome = new QParallelAnimationGroup;
  m_RadioToWelcome->addAnimation(m_RadioOut);
  m_RadioToWelcome->addAnimation(m_WelcomeIn0);

  m_WelcomeToMp3 = new QParallelAnimationGroup;
  m_WelcomeToMp3->addAnimation(m_Mp3In);
  m_WelcomeToMp3->addAnimation(m_WelcomeOut1);

  m_Mp3ToWelcome = new QParallelAnimationGroup;
  m_Mp3ToWelcome->addAnimation(m_WelcomeIn1);
  m_Mp3ToWelcome->addAnimation(m_Mp3Out);
  
}
