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



ContainerWindow::ContainerWindow(status_message* i_pStatusMessage,QWidget *parent):
  QWidget(parent){

  m_pStatusMessage = i_pStatusMessage;
  
  QScreen* l_Screen = QGuiApplication::primaryScreen();
  m_screenSize = l_Screen->geometry();

  m_mainLabel = new QLabel(this);
  QPixmap backgroundImage("../media/leather_metal.jpg");
  backgroundImage = backgroundImage.scaled(m_screenSize.width(),m_screenSize.height());
  m_mainLabel->setPixmap(backgroundImage);
  m_mainLabel->setGeometry(transformResolution(0,0, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT));
  
  // QuitButton
  
  QIcon l_quitIcon("../media/icons/quit.png");
  m_Quit = new QPushButton(this);
  m_Quit->setIcon(l_quitIcon);
  m_Quit->setIconSize(QSize(30,30));
  m_Quit->setFlat(true);
  m_Quit->setGeometry(transformResolution(DEFAULT_SCREEN_WIDTH-30,1,30,30));

  // Welocme window
  welcome = new WelcomeWindow(m_screenSize, this);
  welcome->setGeometry(transformResolution(30,30,DEFAULT_SCREEN_WIDTH-60, DEFAULT_SCREEN_HEIGHT-60));

  // Radio Window
  radio = new RadioWindow(m_screenSize, this);
  radio->setGeometry(transformResolution(30,1000,DEFAULT_SCREEN_WIDTH-60, DEFAULT_SCREEN_HEIGHT-60));

  // Mp3 Window
  mp3 = new Mp3Window(m_screenSize, m_pStatusMessage, this);
  mp3->setGeometry(transformResolution(30,1000,DEFAULT_SCREEN_WIDTH-60, DEFAULT_SCREEN_HEIGHT-60));
  
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


// FIXME: add nice transition animation
void
ContainerWindow::showRadio(){
  welcome->setGeometry(transformResolution(30, -DEFAULT_SCREEN_HEIGHT,
					   DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT));
  
  radio->setGeometry(transformResolution(30,30, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT));
}

void
ContainerWindow::returnFromRadio(){
  radio->setGeometry(transformResolution(30, DEFAULT_SCREEN_HEIGHT,
					   DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT));
  
  welcome->setGeometry(transformResolution(30,30, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT));
}

void
ContainerWindow::showMp3(){
  welcome->setGeometry(transformResolution(30, -DEFAULT_SCREEN_HEIGHT,
					   DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT));
  
  mp3->setGeometry(transformResolution(30,30, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT));
}

void
ContainerWindow::returnFromMp3(){
  mp3->setGeometry(transformResolution(30, DEFAULT_SCREEN_HEIGHT,
					 DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT));
  
  welcome->setGeometry(transformResolution(30,30, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT));
}


void
ContainerWindow::sendQuitMessage(){
  m_pStatusMessage->locker.lock();
  m_pStatusMessage->RequiredAction = QUIT;
  m_pStatusMessage->Handled = false;
  m_pStatusMessage->locker.unlock();
  
}
