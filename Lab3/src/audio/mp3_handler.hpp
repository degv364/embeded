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
