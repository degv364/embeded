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
  explicit RadioWindow(QRect i_screenSize,QWidget *parent = 0);
				      
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
