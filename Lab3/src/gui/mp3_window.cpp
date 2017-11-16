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

#include "mp3_window.hpp"



Mp3Window::Mp3Window(QRect i_screenSize,  status_message* i_pStatusMessage, QWidget *parent):
  QWidget(parent){
  m_screenSize = i_screenSize;

  m_pStatusMessage = i_pStatusMessage;
  InitializeFileNames();
  InitializeFileTimes();

  // Return Button
  QIcon l_returnIcon("../media/icons/goUp.png");
  m_returnButton = new QPushButton(this);
  m_returnButton->setIcon(l_returnIcon);
  m_returnButton->setIconSize(QSize(BIG_ICON_SIZE, BIG_ICON_SIZE));
  m_returnButton->setFlat(true);
  m_returnButton->setGeometry(transformResolution(2*DEFAULT_SCREEN_WIDTH/5,
						  0,
						  BIG_BUTTON_WIDTH,
						  BIG_BUTTON_HEIGHT-50));

  // ForwardButton
  QIcon l_forwardIcon("../media/icons/forward.png");
  m_nextButton = new QPushButton(this);
  m_nextButton->setIcon(l_forwardIcon);
  m_nextButton->setIconSize(QSize(LITTLE_BUTTON_WIDTH, LITTLE_BUTTON_HEIGHT));
  //m_nextButton->setFlat(true);
  m_nextButton->setGeometry(transformResolution(1040,
						480,
						LITTLE_BUTTON_WIDTH,
						LITTLE_BUTTON_HEIGHT));

  // Backward Button
  QIcon l_backwardIcon("../media/icons/back.png");
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
  m_playIcon = QIcon("../media/icons/play.png");
  m_pauseIcon = QIcon("../media/icons/pause.png");
  m_playButton = new QPushButton(this);
  m_playButton->setIcon(m_playIcon);
  m_playButton->setIconSize(QSize(LITTLE_BUTTON_WIDTH, LITTLE_BUTTON_HEIGHT));
  //m_playButton->setCheckable(true);
  m_playButton->setGeometry(transformResolution(640,
						480,
						LITTLE_BUTTON_WIDTH,
						LITTLE_BUTTON_HEIGHT));

  // Sop Button
  QIcon l_stopIcon("../media/icons/stop.png");
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
  m_likeIcon = QIcon("../media/icons/like.png");
  m_likedIcon = QIcon("../media/icons/liked.png");
  m_likeButton = new QPushButton(this);
  m_likeButton->setIcon(m_likeIcon);
  m_likeButton->setIconSize(QSize(DEFAULT_SCREEN_WIDTH/5, DEFAULT_SCREEN_HEIGHT/5));
  m_likeButton->setCheckable(true);
  m_likeButton->setGeometry(transformResolution(2*DEFAULT_SCREEN_WIDTH/5,
						3*DEFAULT_SCREEN_HEIGHT/5,
						DEFAULT_SCREEN_WIDTH/5,
						DEFAULT_SCREEN_HEIGHT/5));
  */

  // Song progress
  m_songProgressBar = new QProgressBar(this);
  m_songProgressBar->setGeometry(transformResolution(280,440,800,40));
  m_songProgressBar->setRange(0,100);
  m_songProgressBar->setValue(0);

  // timer
  m_fProgress = 0;
  m_timer = new QTimer(this);
  // FIXME: revisit this interval in the future
  m_timer->start(DEFAULT_TIMER_INTERVAL);

  // Info
  currentSong = 0;
  m_songIndicator = new QTextEdit("default", this);
  m_songIndicator->setGeometry(transformResolution(280,
						   160,
						   800,
						   280));
  
  m_songIndicator->setReadOnly(true);
  m_songIndicator->setFontPointSize(20);
  m_songIndicator->setAlignment(Qt::AlignCenter);
  // FIXME: show name of last played song
  m_songIndicator->setText(m_MusicFileNames[0]);

  

  // Do the connection
  connect(m_nextButton, SIGNAL(clicked()), this, SLOT(goNext()) );
  connect(m_prevButton, SIGNAL(clicked()), this, SLOT(goPrev()) );
  connect(m_playButton, SIGNAL(clicked()), this, SLOT(play_pause()) );
  connect(m_stopButton, SIGNAL(clicked()), this, SLOT(stop()) );
  connect(m_timer, SIGNAL(timeout()), this, SLOT(updateProgress()) );
  
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
  m_fProgress = 0;
  playing = false;
  if (currentSong >= LAST_FILE){
    currentSong = LAST_FILE-1;
  }
  // send  message
  m_pStatusMessage->locker.lock();
  m_pStatusMessage->RequiredAction = FORWARD;
  m_pStatusMessage->Handled = false;
  m_pStatusMessage->locker.unlock();
  // Update display
  updateSongIndicator();
}

void Mp3Window::goPrev(){
  currentSong--;
  m_fProgress = 0;
  playing = false;
  if (currentSong < 0){
    currentSong = 0;
  }
    // send  message
  m_pStatusMessage->locker.lock();
  m_pStatusMessage->RequiredAction = BACKWARD;
  m_pStatusMessage->Handled = false;
  m_pStatusMessage->locker.unlock();
  // Update display
  updateSongIndicator();
}

void Mp3Window::play_pause(){
  playing = !playing;
  // send message
  m_pStatusMessage->locker.lock();
  m_pStatusMessage->RequiredAction = (playing)? PLAY:PAUSE;
  m_pStatusMessage->Handled = false;
  m_pStatusMessage->locker.unlock();
  //update gui
  updateSongIndicator();
}

void Mp3Window::stop(){
  playing = false;
  m_fProgress = 0;
  // send message
  m_pStatusMessage->locker.lock();
  m_pStatusMessage->RequiredAction = STOP;
  m_pStatusMessage->Handled = false;
  m_pStatusMessage->locker.unlock();
  //update gui
  updateSongIndicator();
}


void Mp3Window::updateSongIndicator(){
  QString newIndicator;
  if (playing){
    m_playButton->setIcon(m_pauseIcon);
    m_playButton->setIconSize(QSize(LITTLE_BUTTON_WIDTH, LITTLE_BUTTON_HEIGHT));
  }
  else {
    m_playButton->setIcon(m_playIcon);
    m_playButton->setIconSize(QSize(LITTLE_BUTTON_WIDTH, LITTLE_BUTTON_HEIGHT));
  }
  newIndicator = m_MusicFileNames[currentSong];
  m_songIndicator->setText(newIndicator);
}

void Mp3Window::updateProgress(){
  if (playing){
    m_fProgress+=DEFAULT_TIMER_INTERVAL;
  }
  m_iShownProgress = m_fProgress*(100.0/(m_MusicFileTimes[currentSong]));

  if (m_iShownProgress>=100){
    playing = false;
    m_fProgress = 0;
    updateSongIndicator();
  }
  m_songProgressBar->setValue(m_iShownProgress);
}

void Mp3Window::InitializeFileNames(void){
  strcpy(m_MusicFileNames[ALL_STAR_TRIO], ALL_STAR_TRIO_STR);
  
  strcpy(m_MusicFileNames[ART_OF_ESCAPISM], ART_OF_ESCAPISM_STR);
  
  strcpy(m_MusicFileNames[TYPICAL_DAY], TYPICAL_DAY_STR);
  
  strcpy(m_MusicFileNames[ALL_BEGINS_HERE ], ALL_BEGINS_HERE_STR);
  
  strcpy(m_MusicFileNames[SHES_A_GIFT ], SHES_A_GIFT_STR);
  
  strcpy(m_MusicFileNames[WELCOME ], WELCOME_STR);
  
  strcpy(m_MusicFileNames[NIGHT_OWL ], NIGHT_OWL_STR);
  
  strcpy(m_MusicFileNames[WITH_EASE ], WITH_EASE_STR);
  
  strcpy(m_MusicFileNames[BEING_FINE ], BEING_FINE_STR);
  
  strcpy(m_MusicFileNames[MOODY_BREAKFAST ], MOODY_BREAKFAST_STR);
  
  strcpy(m_MusicFileNames[THE_LAST_ONES ], THE_LAST_ONES_STR);
  
  strcpy(m_MusicFileNames[GOOD_GRIEF ], GOOD_GRIEF_STR);
  
  strcpy(m_MusicFileNames[PIANO_ROLL ], PIANO_ROLL_STR);
  
  strcpy(m_MusicFileNames[ENTHUSIAST ], ENTHUSIAST_STR);
}


void Mp3Window::InitializeFileTimes(void){
  m_MusicFileTimes[ENTHUSIAST ] =ENTHUSIAST_TIME*1000;
  m_MusicFileTimes[THE_LAST_ONES ] =THE_LAST_ONES_TIME*1000;
  m_MusicFileTimes[GOOD_GRIEF ] =GOOD_GRIEF_TIME*1000;
  m_MusicFileTimes[ALL_STAR_TRIO ] =ALL_STAR_TRIO_TIME*1000;
  m_MusicFileTimes[ART_OF_ESCAPISM ] =ART_OF_ESCAPISM_TIME*1000;
  m_MusicFileTimes[TYPICAL_DAY ] =TYPICAL_DAY_TIME*1000;
  m_MusicFileTimes[ALL_BEGINS_HERE ] =ALL_BEGINS_HERE_TIME*1000;
  m_MusicFileTimes[SHES_A_GIFT ] =SHES_A_GIFT_TIME*1000;
  m_MusicFileTimes[WELCOME ] =WELCOME_TIME*1000;
  m_MusicFileTimes[ NIGHT_OWL ] = NIGHT_OWL_TIME*1000;
  m_MusicFileTimes[WITH_EASE ] =WITH_EASE_TIME*1000;
  m_MusicFileTimes[BEING_FINE ] = BEING_FINE_TIME*1000;
  m_MusicFileTimes[MOODY_BREAKFAST ] =MOODY_BREAKFAST_TIME*1000;
  m_MusicFileTimes[PIANO_ROLL ] = PIANO_ROLL_TIME*1000;
}
