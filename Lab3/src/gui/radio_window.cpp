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

#include "radio_window.hpp"



RadioWindow::RadioWindow(QRect i_screenSize , char * media_path, QWidget *parent):
  QWidget(parent){

  m_bIsFM = true;
  m_fCurrentFM = 107.5;
  m_fCurrentAM = 97.5;

  m_screenSize = i_screenSize;
  char l_cPathToFile[MAX_PATH_LENGTH];

  // Return Button
  strcpy(l_cPathToFile, media_path);
  strcat(l_cPathToFile, "icons/goUp.png");
  QIcon l_returnIcon(l_cPathToFile);
  m_returnButton = new QPushButton(this);
  m_returnButton->setIcon(l_returnIcon);
  m_returnButton->setIconSize(QSize(BIG_ICON_SIZE, BIG_ICON_SIZE));
  m_returnButton->setFlat(true);
  m_returnButton->setGeometry(transformResolution(2*DEFAULT_SCREEN_WIDTH/5,
						  0,
						  BIG_BUTTON_WIDTH,
						  BIG_BUTTON_HEIGHT));

  // ForwardButton
  strcpy(l_cPathToFile, media_path);
  strcat(l_cPathToFile, "icons/forward.png");
  QIcon l_forwardIcon(l_cPathToFile);
  m_increaseButton = new QPushButton(this);
  m_increaseButton->setIcon(l_forwardIcon);
  m_increaseButton->setIconSize(QSize(BIG_ICON_SIZE, BIG_ICON_SIZE));
  m_increaseButton->setFlat(true);
  m_increaseButton->setGeometry(transformResolution(3*DEFAULT_SCREEN_WIDTH/5,
						   2*DEFAULT_SCREEN_HEIGHT/5,
						   BIG_BUTTON_WIDTH,
						   BIG_BUTTON_HEIGHT));

  // Backward Button
  strcpy(l_cPathToFile, media_path);
  strcat(l_cPathToFile, "icons/back.png");
  QIcon l_backwardIcon(l_cPathToFile);
  m_decreaseButton = new QPushButton(this);
  m_decreaseButton->setIcon(l_backwardIcon);
  m_decreaseButton->setIconSize(QSize(BIG_ICON_SIZE, BIG_ICON_SIZE));
  m_decreaseButton->setFlat(true);
  m_decreaseButton->setGeometry(transformResolution(DEFAULT_SCREEN_WIDTH/5,
						    2*DEFAULT_SCREEN_HEIGHT/5,
						    BIG_BUTTON_WIDTH,
						    BIG_BUTTON_HEIGHT));
  // Toggle Button
  strcpy(l_cPathToFile, media_path);
  strcat(l_cPathToFile, "icons/fm.png");
  m_fmIcon = QIcon(l_cPathToFile);
  strcpy(l_cPathToFile, media_path);
  strcat(l_cPathToFile, "icons/am.png");
  m_amIcon = QIcon(l_cPathToFile);
  m_toggleAMFMButton = new QPushButton(this);
  m_toggleAMFMButton->setIcon(m_fmIcon);
  m_toggleAMFMButton->setIconSize(QSize(DEFAULT_SCREEN_WIDTH/5, DEFAULT_SCREEN_HEIGHT/5));
  m_toggleAMFMButton->setCheckable(true);
  m_toggleAMFMButton->setGeometry(transformResolution(2*DEFAULT_SCREEN_WIDTH/5,
						      3*DEFAULT_SCREEN_HEIGHT/5,
						      DEFAULT_SCREEN_WIDTH/5,
						      DEFAULT_SCREEN_HEIGHT/5));


  // Info
  m_stationIndicator = new QTextEdit("107.5 FM", this);
  m_stationIndicator->setGeometry(transformResolution(2*DEFAULT_SCREEN_WIDTH/5,
						      2*DEFAULT_SCREEN_HEIGHT/5,
						      DEFAULT_SCREEN_WIDTH/5,
						      DEFAULT_SCREEN_HEIGHT/5));
  
  m_stationIndicator->setReadOnly(true);
  m_stationIndicator->setFontPointSize(80);
  m_stationIndicator->setAlignment(Qt::AlignCenter);
  m_stationIndicator->setText("107.5");

  

  // Do the connection
  
  connect(m_decreaseButton, SIGNAL(clicked()), this, SLOT(goDecrease()) );

  connect(m_increaseButton, SIGNAL(clicked()), this, SLOT(goIncrease()) );

  connect(m_toggleAMFMButton, SIGNAL(clicked()), this, SLOT(toggleAMFM()) );
}

// To simplify the fact that this should work with many resolutions
// Lets assume that we always use a 1366x768, and use thisfunction
// to get the real values.
QRect
RadioWindow::transformResolution(int x, int y, int width, int height){
  // THis function should only be called once we have the resolution
  float new_x = ((float) x)/DEFAULT_SCREEN_WIDTH*((float)m_screenSize.width());
  float new_y = ((float) y)/DEFAULT_SCREEN_HEIGHT*((float)m_screenSize.height());

  float new_width = ((float) width)/DEFAULT_SCREEN_WIDTH*((float)m_screenSize.width());
  float new_height = ((float) height)/DEFAULT_SCREEN_HEIGHT*((float)m_screenSize.height());

  return QRect((int)new_x, (int)new_y, (int)new_width, (int)new_height);
}

// Slots

void RadioWindow::goIncrease(){
  if (m_bIsFM){
    m_fCurrentFM+=0.5;
  }
  else {
    m_fCurrentAM+=0.5;
  }
  updateStationIndicator();
}

void RadioWindow::goDecrease(){
  if (m_bIsFM){
    m_fCurrentFM-=0.5;
  }
  else {
    m_fCurrentAM-=0.5;
  }
    updateStationIndicator();
}

void RadioWindow::toggleAMFM(){
  m_bIsFM = !m_bIsFM;
  updateStationIndicator();
}


void RadioWindow::updateStationIndicator(){
  QString newIndicator;
  if (m_bIsFM){
    newIndicator = QString("%1").arg(m_fCurrentFM);
    m_toggleAMFMButton->setIcon(m_fmIcon);
    m_toggleAMFMButton->setIconSize(QSize(DEFAULT_SCREEN_WIDTH/5, DEFAULT_SCREEN_HEIGHT/5));
  }
  else {
    newIndicator = QString("%1").arg(m_fCurrentAM);
    m_toggleAMFMButton->setIcon(m_amIcon);
    m_toggleAMFMButton->setIconSize(QSize(DEFAULT_SCREEN_WIDTH/5, DEFAULT_SCREEN_HEIGHT/5));

  }
  m_stationIndicator->setText(newIndicator);
}
