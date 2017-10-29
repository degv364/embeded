#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QApplication>
#include <QPushButton>
#include <QProgressBar>
#include <QSlider>
#include <QTextEdit>


class Window : public QWidget{
  Q_OBJECT  
public:
  explicit Window(QWidget *parent = 0);
				      
private slots:
  void radioSelected();
  void mp3Selected();
  
private:
  QPushButton *m_radioButton;
  QPushButton *m_mp3Button;
  // FIXME: This button is for development
  QPushButton *m_Quit;
  QTextEdit *m_TextMessage;
};

#endif
