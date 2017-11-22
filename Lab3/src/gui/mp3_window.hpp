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

#ifndef MP3_H
#define MP3_H

#include <QWidget>
#include <QApplication>
#include <QPushButton>
#include <QProgressBar>
#include <QSlider>
#include <QTextEdit>
#include <QScreen>
#include <QLabel>
#include <QPixmap>
#include <QIcon>
#include <QTimer>
// Included for debugging. Final product wont print to stdout
#include <iostream>

#include "file_names.hpp"
#include "messages.hpp"

#include "def.hpp"

class Mp3Window : public QWidget{
  Q_OBJECT  
public:
  explicit Mp3Window(QRect i_screenSize, status_message* i_pStatusMessage,
		     char*  media_path, QWidget *parent = 0);

  QPushButton* ReturnButton(){return m_returnButton;};
  void stopSong(void){return stop();};						    
				      
private slots:
  void goNext();
  void goPrev();
  void play_pause();
  void stop();
  void updateProgress();
  
private:

  void InitializeFileNames(void);
  void InitializeFileTimes(void);
  QRect transformResolution(int x, int y, int width, int height);
  void updateSongIndicator();

  bool playing;

  int currentSong;
  float m_fProgress;
  int m_iShownProgress;

  QIcon m_playIcon;
  QIcon m_pauseIcon;
  //QIcon *m_likeIcon;
  //QIcon *m_likedIcon;

  QPushButton *m_returnButton;
  QPushButton *m_nextButton;
  QPushButton *m_prevButton;
  QPushButton *m_playButton;
  //QPushButton *m_pauseButton;
  QPushButton *m_stopButton;

  QTimer *m_timer;
  QProgressBar* m_songProgressBar;
  
  // This is QT5 specific. Can we use that with yocto?
  QRect m_screenSize;
  
  QTextEdit *m_songIndicator;

  status_message* m_pStatusMessage;

  char  m_MusicFileNames[LAST_FILE][MAX_FILE_NAME_SIZE];
  int m_MusicFileTimes[LAST_FILE];
};

#endif
