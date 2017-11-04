#include "radio_window.hpp"



RadioWindow::RadioWindow(QRect i_screenSize,QWidget *parent):
  QWidget(parent){

  m_bIsFM = true;
  m_fCurrentFM = 107.5;
  m_fCurrentAM = 97.5;

  m_screenSize = i_screenSize;

  // Return Button
  QIcon l_returnIcon("../../media/icons/goUp.png");
  m_returnButton = new QPushButton(this);
  m_returnButton->setIcon(l_returnIcon);
  m_returnButton->setIconSize(QSize(BIG_ICON_SIZE, BIG_ICON_SIZE));
  m_returnButton->setFlat(true);
  m_returnButton->setGeometry(transformResolution(2*DEFAULT_SCREEN_WIDTH/5,
						  0,
						  BIG_BUTTON_WIDTH,
						  BIG_BUTTON_HEIGHT));

  // ForwardButton
  QIcon l_forwardIcon("../../media/icons/forward.png");
  m_increaseButton = new QPushButton(this);
  m_increaseButton->setIcon(l_forwardIcon);
  m_increaseButton->setIconSize(QSize(BIG_ICON_SIZE, BIG_ICON_SIZE));
  m_increaseButton->setFlat(true);
  m_increaseButton->setGeometry(transformResolution(3*DEFAULT_SCREEN_WIDTH/5,
						   2*DEFAULT_SCREEN_HEIGHT/5,
						   BIG_BUTTON_WIDTH,
						   BIG_BUTTON_HEIGHT));

  // Backward Button
  QIcon l_backwardIcon("../../media/icons/back.png");
  m_decreaseButton = new QPushButton(this);
  m_decreaseButton->setIcon(l_backwardIcon);
  m_decreaseButton->setIconSize(QSize(BIG_ICON_SIZE, BIG_ICON_SIZE));
  m_decreaseButton->setFlat(true);
  m_decreaseButton->setGeometry(transformResolution(DEFAULT_SCREEN_WIDTH/5,
						    2*DEFAULT_SCREEN_HEIGHT/5,
						    BIG_BUTTON_WIDTH,
						    BIG_BUTTON_HEIGHT));
  // Toggle Button
  m_fmIcon = QIcon("../../media/icons/fm.png");
  m_amIcon = QIcon("../../media/icons/am.png");
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
