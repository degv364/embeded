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
  explicit Mp3Window(QRect i_screenSize, status_message* i_pStatusMessage,QWidget *parent = 0);

  QPushButton* returnButton(){return m_returnButton;};							      
				      
private slots:
  void goNext();
  void goPrev();
  void play_pause();
  void stop();
  void updateProgress();
  
private:

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
};

#endif
