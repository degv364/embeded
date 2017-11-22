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
  explicit Mp3Window(QRect i_ScreenSize, status_message* i_pStatusMessage,
		     char*  i_pMediaPath, QWidget *parent = 0);

  QPushButton* ReturnButton(){return m_ReturnButton;};
  void StopSong(void){return Stop();};						    
				      
private slots:
  void GoNext();
  void GoPrev();
  void PlayPause();
  void Stop();
  void UpdateProgress();
  
private:

  void InitializeFileNames(void);
  void InitializeFileTimes(void);
  QRect TransformResolution(int x, int y, int width, int height);
  void UpdateSongIndicator();

  bool m_bPlaying;

  int m_iCurrentSong;
  float m_fProgress;
  int m_iShownProgress;

  QIcon m_PlayIcon;
  QIcon m_PauseIcon;

  QPushButton *m_ReturnButton;
  QPushButton *m_NextButton;
  QPushButton *m_PrevButton;
  QPushButton *m_PlayButton;
  QPushButton *m_StopButton;

  QTimer *m_Timer;
  QProgressBar* m_SongProgressBar;
  
  // This is QT5 specific. Can we use that with yocto?
  QRect m_ScreenSize;
  
  QTextEdit *m_SongIndicator;

  status_message* m_pStatusMessage;

  char  m_MusicFileNames[LAST_FILE][MAX_FILE_NAME_SIZE];
  int m_MusicFileTimes[LAST_FILE];
};

#endif
