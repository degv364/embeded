#include "mp3_window.hpp"



Mp3Window::Mp3Window(QRect i_screenSize,QWidget *parent):
  QWidget(parent){
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
  m_nextButton = new QPushButton(this);
  m_nextButton->setIcon(l_forwardIcon);
  m_nextButton->setIconSize(QSize(LITTLE_BUTTON_WIDTH, LITTLE_BUTTON_HEIGHT));
  //m_nextButton->setFlat(true);
  m_nextButton->setGeometry(transformResolution(1040,
						480,
						LITTLE_BUTTON_WIDTH,
						LITTLE_BUTTON_HEIGHT));

  // Backward Button
  QIcon l_backwardIcon("../../media/icons/back.png");
  m_prevButton = new QPushButton(this);
  m_prevButton->setIcon(l_backwardIcon);
  m_prevButton->setIconSize(QSize(LITTLE_BUTTON_WIDTH, LITTLE_BUTTON_HEIGHT));
  //m_prevButton->setFlat(true);
  m_prevButton->setGeometry(transformResolution(280,
						480,
						LITTLE_BUTTON_WIDTH,
						LITTLE_BUTTON_HEIGHT));
  // Play Button
  playing = false;
  m_playIcon = QIcon("../../media/icons/play.png");
  m_pauseIcon = QIcon("../../media/icons/pause.png");
  m_playButton = new QPushButton(this);
  m_playButton->setIcon(m_playIcon);
  m_playButton->setIconSize(QSize(LITTLE_BUTTON_WIDTH, LITTLE_BUTTON_HEIGHT));
  m_playButton->setCheckable(true);
  m_playButton->setGeometry(transformResolution(640,
						480,
						LITTLE_BUTTON_WIDTH,
						LITTLE_BUTTON_HEIGHT));

  // Sop Button
  QIcon l_stopIcon("../../media/icons/stop.png");
  m_stopButton = new QPushButton(this);
  m_stopButton->setIcon(l_stopIcon);
  m_stopButton->setIconSize(QSize(LITTLE_BUTTON_WIDTH, LITTLE_BUTTON_HEIGHT));
  //m_stopButton->setFlat(true);
  m_stopButton->setGeometry(transformResolution(680,
						480,
						LITTLE_BUTTON_WIDTH,
						LITTLE_BUTTON_HEIGHT));

  // like Button
  /*
  m_likeIcon = QIcon("../../media/icons/like.png");
  m_likedIcon = QIcon("../../media/icons/liked.png");
  m_likeButton = new QPushButton(this);
  m_likeButton->setIcon(m_likeIcon);
  m_likeButton->setIconSize(QSize(DEFAULT_SCREEN_WIDTH/5, DEFAULT_SCREEN_HEIGHT/5));
  m_likeButton->setCheckable(true);
  m_likeButton->setGeometry(transformResolution(2*DEFAULT_SCREEN_WIDTH/5,
						3*DEFAULT_SCREEN_HEIGHT/5,
						DEFAULT_SCREEN_WIDTH/5,
						DEFAULT_SCREEN_HEIGHT/5));
  */

  // Info
  currentSong = 0;
  m_songIndicator = new QTextEdit("default", this);
  m_songIndicator->setGeometry(transformResolution(280,
						   160,
						   800,
						   280));
  
  m_songIndicator->setReadOnly(true);
  m_songIndicator->setFontPointSize(80);
  m_songIndicator->setAlignment(Qt::AlignCenter);
  // FIXME: show name of last played song
  m_songIndicator->setText("default 0");

  

  // Do the connection
  connect(m_nextButton, SIGNAL(clicked()), this, SLOT(goNext()) );
  connect(m_prevButton, SIGNAL(clicked()), this, SLOT(goPrev()) );
  connect(m_playButton, SIGNAL(clicked()), this, SLOT(play_pause()) );
  connect(m_stopButton, SIGNAL(clicked()), this, SLOT(stop()) );
  
}

// To simplify the fact that this should work with many resolutions
// Lets assume that we always use a 1366x768, and use thisfunction
// to get the real values.
QRect
Mp3Window::transformResolution(int x, int y, int width, int height){
  // THis function should only be called once we have the resolution
  float new_x = ((float) x)/DEFAULT_SCREEN_WIDTH*((float)m_screenSize.width());
  float new_y = ((float) y)/DEFAULT_SCREEN_HEIGHT*((float)m_screenSize.height());

  float new_width = ((float) width)/DEFAULT_SCREEN_WIDTH*((float)m_screenSize.width());
  float new_height = ((float) height)/DEFAULT_SCREEN_HEIGHT*((float)m_screenSize.height());

  return QRect((int)new_x, (int)new_y, (int)new_width, (int)new_height);
}

// Slots

void Mp3Window::goNext(){
  currentSong++;
  playing = false;
  updateSongIndicator();
}

void Mp3Window::goPrev(){
  currentSong--;
  playing = false;
  if (currentSong < 0){
    currentSong = 0;
  }
  updateSongIndicator();
}

void Mp3Window::play_pause(){
  playing = !playing;
  updateSongIndicator();
}

void Mp3Window::stop(){
  playing = false;
  //FIXME: RESET SONG PROGRESS
  updateSongIndicator();
}


void Mp3Window::updateSongIndicator(){
  QString newIndicator;
  if (playing){
    m_playButton->setIcon(m_pauseIcon);
    m_playButton->setIconSize(QSize(DEFAULT_SCREEN_WIDTH/5, DEFAULT_SCREEN_HEIGHT/5));
  }
  else {
    m_playButton->setIcon(m_playIcon);
    m_playButton->setIconSize(QSize(DEFAULT_SCREEN_WIDTH/5, DEFAULT_SCREEN_HEIGHT/5));

  }
  newIndicator = QString("%1").arg(currentSong);
  m_songIndicator->setText(newIndicator);
}


