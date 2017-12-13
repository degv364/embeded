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



ContainerWindow::ContainerWindow(status_message* i_pStatusMessage, char* l_cMediaPath,QWidget *parent):
  QWidget(parent){

  m_pStatusMessage = i_pStatusMessage;
  
  QScreen* l_Screen = QGuiApplication::primaryScreen();
  m_ScreenSize = l_Screen->geometry();

  m_MainLabel = new QLabel(this);
  //Back gorund path
  char l_cPathToBackground[MAX_PATH_LENGTH];
  strcpy(l_cPathToBackground, l_cMediaPath);
  strcat(l_cPathToBackground, "leather_metal.jpg");
  // Initialize backgorund
  QPixmap BackgroundImage(l_cPathToBackground);
  BackgroundImage = BackgroundImage.scaled(m_ScreenSize.width(),m_ScreenSize.height());
  //Darken back ground to improve contrast with GUI
  QPainter l_BackgroundPainter(&BackgroundImage);
  QBrush l_BackgroundBrush(QColor(0,0,0,128)); //R,G,B,alpha 
  l_BackgroundPainter.setBrush(l_BackgroundBrush);
  l_BackgroundPainter.drawRect(0, 0, m_ScreenSize.width(),m_ScreenSize.height());
  m_MainLabel->setPixmap(BackgroundImage);
  m_MainLabel->setGeometry(TransformResolution(0,0, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT));

  // Welocme window
  Welcome = new WelcomeWindow(m_ScreenSize, l_cMediaPath, this);
  Welcome->setGeometry(TransformResolution(0,0,DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT));

  // Radio Window
  Radio = new RadioWindow(m_ScreenSize, l_cMediaPath, this);
  Radio->setGeometry(TransformResolution(0,1000,DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT));

  // Mp3 Window
  Mp3 = new Mp3Window(m_ScreenSize, m_pStatusMessage,  l_cMediaPath, this);
  Mp3->setGeometry(TransformResolution(0,1000,DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT));


    // QuitButton
  m_Quit = new QPushButton(this);
  m_Quit->setFlat(true);
  m_Quit->setGeometry(TransformResolution(DEFAULT_SCREEN_WIDTH-30,1,30,30));
  

  SetUpAnimations();
  
  // Do the connection
  connect(m_Quit, SIGNAL(clicked()), QApplication::instance(), SLOT(quit()) );
  connect(m_Quit, SIGNAL(clicked()), this, SLOT(SendQuitMessage()) );

  // Radio transition
  connect(Welcome->RadioButton(), SIGNAL(clicked()), this , SLOT(ShowRadio()) );
  connect(Radio->ReturnButton(), SIGNAL(clicked()), this, SLOT(ReturnFromRadio()) );
  connect(Welcome->Mp3Button(), SIGNAL(clicked()), this , SLOT(ShowMp3()) );
  connect(Mp3->ReturnButton(), SIGNAL(clicked()), this, SLOT(ReturnFromMp3()) );
  
}

// To simplify the fact that this should work with many resolutions
// Lets assume that we always use a 1366x768, and use thisfunction
// to get the real values.
QRect
ContainerWindow::TransformResolution(int x, int y, int width, int height){
  // THis function should only be called once we have the resolution
  float new_x = ((float) x)/DEFAULT_SCREEN_WIDTH*((float)m_ScreenSize.width());
  float new_y = ((float) y)/DEFAULT_SCREEN_HEIGHT*((float)m_ScreenSize.height());

  float new_width = ((float) width)/DEFAULT_SCREEN_WIDTH*((float)m_ScreenSize.width());
  float new_height = ((float) height)/DEFAULT_SCREEN_HEIGHT*((float)m_ScreenSize.height());

  return QRect((int)new_x, (int)new_y, (int)new_width, (int)new_height);
}


void
ContainerWindow::ShowRadio(){
  m_WelcomeToRadio->start();
}

void
ContainerWindow::ReturnFromRadio(){
  m_RadioToWelcome->start();
}

void
ContainerWindow::ShowMp3(){
  m_WelcomeToMp3->start();
}

void
ContainerWindow::ReturnFromMp3(){
  Mp3->StopSong();
  m_Mp3ToWelcome->start();
}


void
ContainerWindow::SendQuitMessage(){
  m_pStatusMessage->locker.lock();
  m_pStatusMessage->RequiredAction = QUIT;
  m_pStatusMessage->Handled = false;
  m_pStatusMessage->locker.unlock();
  
}

void
ContainerWindow::SetUpAnimations(){
  // Radio In
  m_RadioIn = new QPropertyAnimation(Radio, "geometry");
  m_RadioIn->setDuration(DEFAULT_ANIMATION_DURATION);
  m_RadioIn->setStartValue(TransformResolution(0, DEFAULT_SCREEN_HEIGHT,
					      DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT));
  m_RadioIn->setEndValue(TransformResolution(0,0, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT));

  // Radio Out
  m_RadioOut = new QPropertyAnimation(Radio, "geometry");
  m_RadioOut->setDuration(DEFAULT_ANIMATION_DURATION);
  m_RadioOut->setEndValue(TransformResolution(0, DEFAULT_SCREEN_HEIGHT,
					      DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT));
  m_RadioOut->setStartValue(TransformResolution(0,0, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT));

  //Mp3 in
  m_Mp3In = new QPropertyAnimation(Mp3, "geometry");
  m_Mp3In->setDuration(DEFAULT_ANIMATION_DURATION);
  m_Mp3In->setStartValue(TransformResolution(0, DEFAULT_SCREEN_HEIGHT,
					      DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT));
  m_Mp3In->setEndValue(TransformResolution(0,0, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT));

  //Mp3 Out
  m_Mp3Out = new QPropertyAnimation(Mp3, "geometry");
  m_Mp3Out->setDuration(DEFAULT_ANIMATION_DURATION);
  m_Mp3Out->setEndValue(TransformResolution(0, DEFAULT_SCREEN_HEIGHT,
					      DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT));
  m_Mp3Out->setStartValue(TransformResolution(0,0, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT));

  // Welcome In for Radio
  m_WelcomeIn0 = new QPropertyAnimation(Welcome, "geometry");
  m_WelcomeIn0->setDuration(DEFAULT_ANIMATION_DURATION);
  m_WelcomeIn0->setStartValue(TransformResolution(0, -DEFAULT_SCREEN_HEIGHT,
					      DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT));
  m_WelcomeIn0->setEndValue(TransformResolution(0,0, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT));

  // Welcome Out for Radio
  m_WelcomeOut0 = new QPropertyAnimation(Welcome, "geometry");
  m_WelcomeOut0->setDuration(DEFAULT_ANIMATION_DURATION);
  m_WelcomeOut0->setEndValue(TransformResolution(0, -DEFAULT_SCREEN_HEIGHT,
					      DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT));
  m_WelcomeOut0->setStartValue(TransformResolution(0,0, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT));

  // Welcome In for Mp3
  m_WelcomeIn1 = new QPropertyAnimation(Welcome, "geometry");
  m_WelcomeIn1->setDuration(DEFAULT_ANIMATION_DURATION);
  m_WelcomeIn1->setStartValue(TransformResolution(0, -DEFAULT_SCREEN_HEIGHT,
					      DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT));
  m_WelcomeIn1->setEndValue(TransformResolution(0,0, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT));

  // Welcome Out for Mp3
  m_WelcomeOut1 = new QPropertyAnimation(Welcome, "geometry");
  m_WelcomeOut1->setDuration(DEFAULT_ANIMATION_DURATION);
  m_WelcomeOut1->setEndValue(TransformResolution(0, -DEFAULT_SCREEN_HEIGHT,
					      DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT));
  m_WelcomeOut1->setStartValue(TransformResolution(0,0, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT));

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
