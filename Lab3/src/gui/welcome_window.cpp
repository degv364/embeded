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

#include "welcome_window.hpp"


WelcomeWindow::WelcomeWindow(QRect i_screenSize, char* media_path, QWidget *parent):
  QWidget(parent){

  m_screenSize = i_screenSize;
  char l_cPathToFile[MAX_PATH_LENGTH];

  // radioButton
  strcpy(l_cPathToFile, media_path);
  strcat(l_cPathToFile, "svg/radio.svg");
  QIcon l_radioIcon(l_cPathToFile);
  m_radioButton = new QPushButton(this);
  m_radioButton->setIcon(l_radioIcon);
  m_radioButton->setIconSize(QSize(BIG_ICON_SIZE, BIG_ICON_SIZE));
  m_radioButton->setFlat(true);
  m_radioButton->setGeometry(transformResolution(30,
						 30,
						 DEFAULT_SCREEN_WIDTH/2-45,
						 DEFAULT_SCREEN_HEIGHT-60));

  // mp3 Button
  strcpy(l_cPathToFile, media_path);
  strcat(l_cPathToFile, "svg/mp3.svg");
  QIcon l_mp3Icon(l_cPathToFile);
  m_mp3Button = new QPushButton(this);
  m_mp3Button->setIcon(l_mp3Icon);
  m_mp3Button->setIconSize(QSize(BIG_ICON_SIZE, BIG_ICON_SIZE));
  m_mp3Button->setFlat(true);
  m_mp3Button->setGeometry(transformResolution(DEFAULT_SCREEN_WIDTH/2+15,
					       30,
					       DEFAULT_SCREEN_WIDTH/2-45,
					       DEFAULT_SCREEN_HEIGHT-60));

 
}

// To simplify the fact that this should work with many resolutions
// Lets assume that we always use a 1366x768, and use thisfunction
// to get the real values.
QRect
WelcomeWindow::transformResolution(int x, int y, int width, int height){
  // THis function should only be called once we have the resolution
  float new_x = ((float) x)/DEFAULT_SCREEN_WIDTH*((float)m_screenSize.width());
  float new_y = ((float) y)/DEFAULT_SCREEN_HEIGHT*((float)m_screenSize.height());

  float new_width = ((float) width)/DEFAULT_SCREEN_WIDTH*((float)m_screenSize.width());
  float new_height = ((float) height)/DEFAULT_SCREEN_HEIGHT*((float)m_screenSize.height());

  return QRect((int)new_x, (int)new_y, (int)new_width, (int)new_height);
}

