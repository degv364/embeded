#include "window.hpp"



WelcomeWindow::WelcomeWindow(QWidget *parent):
  QWidget(parent){

  QScreen* l_Screen = QGuiApplication::primaryScreen();
  m_screenSize = l_Screen->geometry();

  m_mainLabel = new QLabel(this);
  QPixmap backgroundImage("../../media/leather_metal.jpg");
  m_mainLabel->setPixmap(backgroundImage);
  
  // QuitButton
  
  QIcon l_quitIcon("../../media/icons/quit.png");
  m_Quit = new QPushButton(this);
  m_Quit->setIcon(l_quitIcon);
  m_Quit->setIconSize(QSize(30,30));
  m_Quit->setFlat(true);
  m_Quit->setGeometry(transformResolution(DEFAULT_SCREEN_WIDTH-30,1,30,30));

  // radioButton
  QIcon l_radioIcon("../../media/icons/radio.png");
  m_radioButton = new QPushButton(this);
  m_radioButton->setIcon(l_radioIcon);
  m_radioButton->setIconSize(QSize(BIG_ICON_SIZE, BIG_ICON_SIZE));
  m_radioButton->setFlat(true);
  m_radioButton->setGeometry(transformResolution(DEFAULT_SCREEN_WIDTH/5,
						 2*DEFAULT_SCREEN_HEIGHT/5,
						 BIG_BUTTON_WIDTH,
						 BIG_BUTTON_HEIGHT));

  // mp3 Button
  QIcon l_mp3Icon("../../media/icons/playList.png");
  m_mp3Button = new QPushButton(this);
  m_mp3Button->setIcon(l_mp3Icon);
  m_mp3Button->setIconSize(QSize(BIG_ICON_SIZE, BIG_ICON_SIZE));
  m_mp3Button->setFlat(true);
  m_mp3Button->setGeometry(transformResolution(3*DEFAULT_SCREEN_WIDTH/5,
					       2*DEFAULT_SCREEN_HEIGHT/5,
					       BIG_BUTTON_WIDTH,
					       BIG_BUTTON_HEIGHT));

  // Info
  m_TextMessage = new QTextEdit("Hello", this);
  m_TextMessage->setGeometry(transformResolution(500,500,400,30));
  m_TextMessage->setReadOnly(true);

  

  // Do the connection
  
  connect(m_radioButton, SIGNAL(clicked()), this, SLOT(radioSelected()) );

  connect(m_mp3Button, SIGNAL(clicked()), this, SLOT(mp3Selected()) );
  
  connect(m_Quit, SIGNAL(clicked()), QApplication::instance(), SLOT(quit()) );
  
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

// Slots

void WelcomeWindow::radioSelected(){
  m_TextMessage->setText("Radio Window should open");
  return;
}
void WelcomeWindow::mp3Selected(){
  m_TextMessage->setText("MP3 Window shopuld open");
  return;
}
