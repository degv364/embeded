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



RadioWindow::RadioWindow(QRect i_ScreenSize , char * i_pMediaPath, QWidget *parent):
  QWidget(parent){

  m_bIsFM = true;
  m_fCurrentFM = 107.5;
  m_fCurrentAM = 97.5;

  m_ScreenSize = i_ScreenSize;
  char l_cPathToFile[MAX_PATH_LENGTH];

  //QLabel *label = new QLabel(this);
  //label->setText("first line\nsecond line");

  // Return Button
  strcpy(l_cPathToFile, i_pMediaPath);
  strcat(l_cPathToFile, "icons/goUp.png");
  QIcon l_returnIcon(l_cPathToFile);
  m_ReturnButton = new QPushButton(this);
  m_ReturnButton->setIcon(l_returnIcon);
  m_ReturnButton->setIconSize(QSize(BIG_ICON_SIZE, BIG_ICON_SIZE));
  m_ReturnButton->setFlat(true);
  m_ReturnButton->setGeometry(TransformResolution(2*DEFAULT_SCREEN_WIDTH/5,
						  0,
						  BIG_BUTTON_WIDTH,
						  BIG_BUTTON_HEIGHT));

  // ForwardButton
  strcpy(l_cPathToFile, i_pMediaPath);
  strcat(l_cPathToFile, "icons/forward.png");
  QIcon l_ForwardIcon(l_cPathToFile);
  m_IncreaseButton = new QPushButton(this);
  m_IncreaseButton->setIcon(l_ForwardIcon);
  m_IncreaseButton->setIconSize(QSize(BIG_ICON_SIZE, BIG_ICON_SIZE));
  m_IncreaseButton->setFlat(true);
  m_IncreaseButton->setGeometry(TransformResolution(3*DEFAULT_SCREEN_WIDTH/5,
						   2*DEFAULT_SCREEN_HEIGHT/5,
						   BIG_BUTTON_WIDTH,
						   BIG_BUTTON_HEIGHT));

  // Backward Button
  strcpy(l_cPathToFile, i_pMediaPath);
  strcat(l_cPathToFile, "icons/back.png");
  QIcon l_BackwardIcon(l_cPathToFile);
  m_DecreaseButton = new QPushButton(this);
  m_DecreaseButton->setIcon(l_BackwardIcon);
  m_DecreaseButton->setIconSize(QSize(BIG_ICON_SIZE, BIG_ICON_SIZE));
  m_DecreaseButton->setFlat(true);
  m_DecreaseButton->setGeometry(TransformResolution(DEFAULT_SCREEN_WIDTH/5,
						    2*DEFAULT_SCREEN_HEIGHT/5,
						    BIG_BUTTON_WIDTH,
						    BIG_BUTTON_HEIGHT));
  // Toggle Button
  strcpy(l_cPathToFile, i_pMediaPath);
  strcat(l_cPathToFile, "icons/fm.png");
  m_fmIcon = QIcon(l_cPathToFile);
  strcpy(l_cPathToFile, i_pMediaPath);
  strcat(l_cPathToFile, "icons/am.png");
  m_amIcon = QIcon(l_cPathToFile);
  m_ToggleAMFMButton = new QPushButton(this);
  m_ToggleAMFMButton->setIcon(m_fmIcon);
  m_ToggleAMFMButton->setIconSize(QSize(DEFAULT_SCREEN_WIDTH/5-100, DEFAULT_SCREEN_HEIGHT/5-100));
  m_ToggleAMFMButton->setCheckable(true);
  m_ToggleAMFMButton->setGeometry(TransformResolution(2*DEFAULT_SCREEN_WIDTH/5,
						      3*DEFAULT_SCREEN_HEIGHT/5,
						      DEFAULT_SCREEN_WIDTH/5,
						      DEFAULT_SCREEN_HEIGHT/5));


  // Info
  m_StationIndicator = new QLabel( this);
  m_StationIndicator->setGeometry(TransformResolution(2*DEFAULT_SCREEN_WIDTH/5,
						      2*DEFAULT_SCREEN_HEIGHT/5,
						      DEFAULT_SCREEN_WIDTH/5,
						      DEFAULT_SCREEN_HEIGHT/5));
  QFont l_Font = m_StationIndicator->font();
  l_Font.setPointSize(TransformFontSize(60));
  l_Font.setBold(true);
  m_StationIndicator->setFont(l_Font);
  m_StationIndicator->setAlignment(Qt::AlignCenter);
  m_StationIndicator->setText("107.5");
  m_StationIndicator->setAlignment(Qt::AlignCenter);
  m_StationIndicator->setStyleSheet("color: white");
  

  // Do the connection
  
  connect(m_DecreaseButton, SIGNAL(clicked()), this, SLOT(GoDecrease()) );

  connect(m_IncreaseButton, SIGNAL(clicked()), this, SLOT(GoIncrease()) );

  connect(m_ToggleAMFMButton, SIGNAL(clicked()), this, SLOT(ToggleAMFM()) );
}

// To simplify the fact that this should work with many resolutions
// Lets assume that we always use a 1366x768, and use thisfunction
// to get the real values.
QRect
RadioWindow::TransformResolution(int x, int y, int width, int height){
  // THis function should only be called once we have the resolution
  float new_x = ((float) x)/DEFAULT_SCREEN_WIDTH*((float)m_ScreenSize.width());
  float new_y = ((float) y)/DEFAULT_SCREEN_HEIGHT*((float)m_ScreenSize.height());

  float new_width = ((float) width)/DEFAULT_SCREEN_WIDTH*((float)m_ScreenSize.width());
  float new_height = ((float) height)/DEFAULT_SCREEN_HEIGHT*((float)m_ScreenSize.height());

  return QRect((int)new_x, (int)new_y, (int)new_width, (int)new_height);
}

int
RadioWindow::TransformFontSize(int i_iSize){
  float l_fMean = m_ScreenSize.width()*0.5+m_ScreenSize.height()*0.5;
  return (int) i_iSize*2.0f/(DEFAULT_SCREEN_WIDTH+DEFAULT_SCREEN_HEIGHT)*l_fMean;
}

// Slots

void RadioWindow::GoIncrease(){
  if (m_bIsFM){
    m_fCurrentFM+=0.5;
  }
  else {
    m_fCurrentAM+=0.5;
  }
  UpdateStationIndicator();
}

void RadioWindow::GoDecrease(){
  if (m_bIsFM){
    m_fCurrentFM-=0.5;
  }
  else {
    m_fCurrentAM-=0.5;
  }
    UpdateStationIndicator();
}

void RadioWindow::ToggleAMFM(){
  m_bIsFM = !m_bIsFM;
  UpdateStationIndicator();
}


void RadioWindow::UpdateStationIndicator(){
  QString newIndicator;
  if (m_bIsFM){
    newIndicator = QString("%1").arg(m_fCurrentFM);
    m_ToggleAMFMButton->setIcon(m_fmIcon);
    m_ToggleAMFMButton->setIconSize(QSize(DEFAULT_SCREEN_WIDTH/5-100, DEFAULT_SCREEN_HEIGHT/5-100));
  }
  else {
    newIndicator = QString("%1").arg(m_fCurrentAM);
    m_ToggleAMFMButton->setIcon(m_amIcon);
    m_ToggleAMFMButton->setIconSize(QSize(DEFAULT_SCREEN_WIDTH/5-100, DEFAULT_SCREEN_HEIGHT/5-100));

  }
  m_ToggleAMFMButton->setGeometry(TransformResolution(2*DEFAULT_SCREEN_WIDTH/5,
						      3*DEFAULT_SCREEN_HEIGHT/5,
						      DEFAULT_SCREEN_WIDTH/5,
						      DEFAULT_SCREEN_HEIGHT/5));
  m_StationIndicator->setText(newIndicator);
}
