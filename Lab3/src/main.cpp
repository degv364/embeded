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

#include<iostream>

#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <thread>

//Internal includes
#include "file_names.hpp"
#include "messages.hpp"

#include "audio/audio_server.hpp"
#include "gui/container_window.hpp"

int main(int argc, char **argv){
  char* media_path;
  QApplication app(argc, argv);
  gst_init (&argc, &argv);
  
  if (argc != 2){
    std::cout<<"Usage: ./main <path_to_media>"<<std::endl;
    return 1;
  }
  media_path = argv[1];
  std::cout<<"Media Path: "<<media_path<<std::endl;
  
  
  // Initialize intercommunication message
  status_message l_stStatusMessage;
  l_stStatusMessage.RequiredAction = PAUSE;
  l_stStatusMessage.Handled = true;
  
  ContainerWindow container(&l_stStatusMessage, media_path);
  container.showFullScreen();
  
  std::thread l_AudioServer(AudioServer, &l_stStatusMessage);
  
  app.exec();
  l_AudioServer.join();
  return 0;
}

