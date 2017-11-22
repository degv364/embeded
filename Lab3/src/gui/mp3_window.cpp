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



Mp3Window::Mp3Window(QRect i_ScreenSize,  status_message* i_pStatusMessage,
		     char*  i_pMediaPath, QWidget *parent):
  QWidget(parent){
  m_ScreenSize = i_ScreenSize;
  char l_cPathToFile[MAX_PATH_LENGTH];

  m_pStatusMessage = i_pStatusMessage;
  InitializeFileNames();
  InitializeFileTimes();

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
						  BIG_BUTTON_HEIGHT-50));

  // ForwardButton
  strcpy(l_cPathToFile, i_pMediaPath);
  strcat(l_cPathToFile, "icons/forward.png");
  QIcon l_ForwardIcon(l_cPathToFile);
  m_NextButton = new QPushButton(this);
  m_NextButton->setIcon(l_ForwardIcon);
  m_NextButton->setIconSize(QSize(LITTLE_BUTTON_WIDTH, LITTLE_BUTTON_HEIGHT));
  //m_nextButton->setFlat(true);
  m_NextButton->setGeometry(TransformResolution(1040,
						480,
						LITTLE_BUTTON_WIDTH,
						LITTLE_BUTTON_HEIGHT));

  // Backward Button
  strcpy(l_cPathToFile, i_pMediaPath);
  strcat(l_cPathToFile, "icons/back.png");
  QIcon l_BackwardIcon(l_cPathToFile);
  m_PrevButton = new QPushButton(this);
  m_PrevButton->setIcon(l_BackwardIcon);
  m_PrevButton->setIconSize(QSize(LITTLE_BUTTON_WIDTH, LITTLE_BUTTON_HEIGHT));
  //m_prevButton->setFlat(true);
  m_PrevButton->setGeometry(TransformResolution(280,
						480,
						LITTLE_BUTTON_WIDTH,
						LITTLE_BUTTON_HEIGHT));
  // Play Button
  m_bPlaying = false;
  strcpy(l_cPathToFile, i_pMediaPath);
  strcat(l_cPathToFile, "icons/play.png");
  m_PlayIcon = QIcon(l_cPathToFile);
  strcpy(l_cPathToFile, i_pMediaPath);
  strcat(l_cPathToFile, "icons/pause.png");
  m_PauseIcon = QIcon(l_cPathToFile);
  m_PlayButton = new QPushButton(this);
  m_PlayButton->setIcon(m_PlayIcon);
  m_PlayButton->setIconSize(QSize(LITTLE_BUTTON_WIDTH, LITTLE_BUTTON_HEIGHT));
  //m_playButton->setCheckable(true);
  m_PlayButton->setGeometry(TransformResolution(640,
						480,
						LITTLE_BUTTON_WIDTH,
						LITTLE_BUTTON_HEIGHT));

  // Sop Button
  strcpy(l_cPathToFile, i_pMediaPath);
  strcat(l_cPathToFile, "icons/stop.png");
  QIcon l_StopIcon(l_cPathToFile);
  m_StopButton = new QPushButton(this);
  m_StopButton->setIcon(l_StopIcon);
  m_StopButton->setIconSize(QSize(LITTLE_BUTTON_WIDTH, LITTLE_BUTTON_HEIGHT));
  //m_stopButton->setFlat(true);
  m_StopButton->setGeometry(TransformResolution(680,
						480,
						LITTLE_BUTTON_WIDTH,
						LITTLE_BUTTON_HEIGHT));


  // Song progress
  m_SongProgressBar = new QProgressBar(this);
  m_SongProgressBar->setGeometry(TransformResolution(280,440,800,40));
  m_SongProgressBar->setRange(0,100);
  m_SongProgressBar->setValue(0);

  // timer
  m_fProgress = 0;
  m_Timer = new QTimer(this);
  // FIXME: revisit this interval in the future
  m_Timer->start(DEFAULT_TIMER_INTERVAL);

  // Info
  m_iCurrentSong = 0;
  m_SongIndicator = new QTextEdit("default", this);
  m_SongIndicator->setGeometry(TransformResolution(280,
						   160,
						   800,
						   280));
  
  m_SongIndicator->setReadOnly(true);
  m_SongIndicator->setFontPointSize(20);
  m_SongIndicator->setAlignment(Qt::AlignCenter);
  // FIXME: show name of last played song
  m_SongIndicator->setText(m_MusicFileNames[0]);

  

  // Do the connection
  connect(m_NextButton, SIGNAL(clicked()), this, SLOT(GoNext()) );
  connect(m_PrevButton, SIGNAL(clicked()), this, SLOT(GoPrev()) );
  connect(m_PlayButton, SIGNAL(clicked()), this, SLOT(PlayPause()) );
  connect(m_StopButton, SIGNAL(clicked()), this, SLOT(Stop()) );
  connect(m_Timer, SIGNAL(timeout()), this, SLOT(UpdateProgress()) );
  
}

// To simplify the fact that this should work with many resolutions
// Lets assume that we always use a 1366x768, and use thisfunction
// to get the real values.
QRect
Mp3Window::TransformResolution(int x, int y, int width, int height){
  // THis function should only be called once we have the resolution
  float new_x = ((float) x)/DEFAULT_SCREEN_WIDTH*((float)m_ScreenSize.width());
  float new_y = ((float) y)/DEFAULT_SCREEN_HEIGHT*((float)m_ScreenSize.height());

  float new_width = ((float) width)/DEFAULT_SCREEN_WIDTH*((float)m_ScreenSize.width());
  float new_height = ((float) height)/DEFAULT_SCREEN_HEIGHT*((float)m_ScreenSize.height());

  return QRect((int)new_x, (int)new_y, (int)new_width, (int)new_height);
}

// Slots

void Mp3Window::GoNext(){
  m_iCurrentSong++;
  m_fProgress = 0;
  m_bPlaying = false;
  if (m_iCurrentSong >= LAST_FILE){
    m_iCurrentSong = LAST_FILE-1;
  }
  // send  message
  m_pStatusMessage->locker.lock();
  m_pStatusMessage->RequiredAction = FORWARD;
  m_pStatusMessage->Handled = false;
  m_pStatusMessage->locker.unlock();
  // Update display
  UpdateSongIndicator();
}

void Mp3Window::GoPrev(){
  m_iCurrentSong--;
  m_fProgress = 0;
  m_bPlaying = false;
  if (m_iCurrentSong < 0){
    m_iCurrentSong = 0;
  }
    // send  message
  m_pStatusMessage->locker.lock();
  m_pStatusMessage->RequiredAction = BACKWARD;
  m_pStatusMessage->Handled = false;
  m_pStatusMessage->locker.unlock();
  // Update display
  UpdateSongIndicator();
}

void Mp3Window::PlayPause(){
  m_bPlaying = !m_bPlaying;
  // send message
  m_pStatusMessage->locker.lock();
  m_pStatusMessage->RequiredAction = (m_bPlaying)? PLAY:PAUSE;
  m_pStatusMessage->Handled = false;
  m_pStatusMessage->locker.unlock();
  //update gui
  UpdateSongIndicator();
}

void Mp3Window::Stop(){
  m_bPlaying = false;
  m_fProgress = 0;
  // send message
  m_pStatusMessage->locker.lock();
  m_pStatusMessage->RequiredAction = STOP;
  m_pStatusMessage->Handled = false;
  m_pStatusMessage->locker.unlock();
  //update gui
  UpdateSongIndicator();
}


void Mp3Window::UpdateSongIndicator(){
  QString newIndicator;
  if (m_bPlaying){
    m_PlayButton->setIcon(m_PauseIcon);
    m_PlayButton->setIconSize(QSize(LITTLE_BUTTON_WIDTH, LITTLE_BUTTON_HEIGHT));
  }
  else {
    m_PlayButton->setIcon(m_PlayIcon);
    m_PlayButton->setIconSize(QSize(LITTLE_BUTTON_WIDTH, LITTLE_BUTTON_HEIGHT));
  }
  newIndicator = m_MusicFileNames[m_iCurrentSong];
  m_SongIndicator->setText(newIndicator);
}

void Mp3Window::UpdateProgress(){
  if (m_bPlaying){
    m_fProgress+=DEFAULT_TIMER_INTERVAL;
  }
  m_iShownProgress = m_fProgress*(100.0/(m_MusicFileTimes[m_iCurrentSong]));

  if (m_iShownProgress>=100){
    m_bPlaying = false;
    m_fProgress = 0;
    UpdateSongIndicator();
  }
  m_SongProgressBar->setValue(m_iShownProgress);
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
