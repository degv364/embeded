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

#include <stdio.h>
#include <string.h>
#include <gst/gst.h>
#include <glib.h>
#include <chrono>
#include <unistd.h>

#include "file_names.hpp"
#include "messages.hpp"

#ifndef MP3_HANDLER_H
#define MP3_HANDLER_H

#define PERIOD 1000

class MP3Handler{
private:
  GstElement *m_Pipeline;
  GstElement *m_Source;
  GstElement *m_Parser;
  GstElement *m_Decoder;
  GstElement *m_Converter;
  GstElement *m_Sink;

  GstBus *m_Bus;
  bool m_bTerminate;
  GstMessage* m_InternalMsg;

  status_message * m_pStatusMessage;

  char m_FilePaths[LAST_FILE][MAX_FILE_NAME_SIZE];
  int m_iCurrentSong;

  bool HandleInternalMessage(void);
  bool HandleExternalMessage(void);
  bool LoopStep(void);
  void InitializeFilePaths(void);

public:
  MP3Handler(status_message* i_pStatusMessage);

  bool Setup(void);

  bool Loop(void);

  bool Deinit(void);
};

#endif
