#include "window.hpp"

Window::Window(QWidget *parent):
  QWidget(parent){
  
  // QuitButton
  m_Quit = new QPushButton("Quit \n(Should remove this button)", this);
  // FIXME: fixed position and size, got to be relavtive to screen resolution
  m_Quit->setGeometry(600,700,100,30);

  // radioButton
  m_radioButton = new QPushButton("Radio", this);
  m_radioButton->setGeometry(500,300,400,100);

  // mp3 Button
  m_mp3Button = new QPushButton("MP3", this);
  m_mp3Button->setGeometry(20,300,400,100);

  // Info
  m_TextMessage = new QTextEdit("Hello", this);
  m_TextMessage->setGeometry(500,500,400,30);
  m_TextMessage->setReadOnly(true);

  

  // Do the connection
  
  connect(m_radioButton, SIGNAL(clicked()), this, SLOT(radioSelected()) );

  connect(m_mp3Button, SIGNAL(clicked()), this, SLOT(mp3Selected()) );
  
  connect(m_Quit, SIGNAL(clicked()), QApplication::instance(), SLOT(quit()) );
  
}


void Window::radioSelected(){
  m_TextMessage->setText("Radio Window should open");
  return;
}
void Window::mp3Selected(){
  m_TextMessage->setText("MP3 Window shopuld open");
  return;
}
