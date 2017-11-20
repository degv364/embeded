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

#ifndef RADIO_H
#define RADIO_H

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
// Included for debugging. Final product wont print to stdout
#include <iostream>

#include "def.hpp"

class RadioWindow : public QWidget{
  Q_OBJECT  
public:
  explicit RadioWindow(QRect i_screenSize , char *media_path,QWidget *parent = 0);

  QPushButton* returnButton(){return m_returnButton;};							      
				      
private slots:
  void goIncrease();
  void goDecrease();
  void toggleAMFM();
  
private:

  QRect transformResolution(int x, int y, int width, int height);
  void updateStationIndicator();

  float m_fCurrentFM;
  float m_fCurrentAM;
  bool m_bIsFM;
  

  QPushButton *m_returnButton;
  QPushButton *m_increaseButton;
  QPushButton *m_decreaseButton;
  QPushButton *m_toggleAMFMButton;

  QIcon m_fmIcon;
  QIcon m_amIcon;
  
  // This is QT5 specific. Can we use that with yocto?
  QRect m_screenSize;
  // FIXME: This button is for development
  QTextEdit *m_stationIndicator;
};

#endif
