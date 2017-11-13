#include <gst/gst.h>
#include <glib.h>
#include <chrono>
#include <unistd.h>

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

  bool HandleInternalMessage(void);
  bool HandleExternalMessage(void);
  bool LoopStep(void);

public:
  MP3Handler(void);

  bool Setup(void);

  bool Loop(void);

  bool Deinit(void);
};

#endif
