#include "mp3_handler.hpp"

MP3Handler::MP3Handler(status_message* i_pStatusMessage){
  m_bTerminate = false;
  m_pStatusMessage = i_pStatusMessage;
}

bool
MP3Handler::Setup(void){
  g_print("Initializing Pipeline...\n");
  m_Pipeline = gst_pipeline_new("mp3_handler");
  m_Source   = gst_element_factory_make ("filesrc","file-source");
  m_Parser   = gst_element_factory_make ("mpegaudioparse","parser");
  m_Decoder  = gst_element_factory_make ("mpg123audiodec","decoder");
  m_Converter= gst_element_factory_make ("audioconvert","converter");
  m_Sink     = gst_element_factory_make ("pulsesink","audio-output");

  if (!m_Pipeline || !m_Source || !m_Parser || !m_Decoder || !m_Converter ||!m_Sink) {
    g_printerr ("One element could not be created. Exiting.\n");
    return false;
  }

  // FIXME: hard coded name
  g_object_set (G_OBJECT (m_Source), "location",
		"../media/music/Tours_-_01_-_Enthusiast.mp3", NULL);

  // Add elements to pipeline
  gst_bin_add_many (GST_BIN(m_Pipeline), m_Source, m_Parser,
		    m_Decoder, m_Converter, m_Sink, NULL);

  // Link the elements together
  gst_element_link_many(m_Source, m_Parser, m_Decoder,
			m_Converter, m_Sink, NULL);

  // Set internal bus
  m_Bus = gst_pipeline_get_bus (GST_PIPELINE (m_Pipeline));

  return true;
}

bool
MP3Handler::Loop(void){
  std::chrono::high_resolution_clock::time_point before_step;
  std::chrono::high_resolution_clock::time_point after_step;
  g_print("start Loop...");
  gst_element_set_state (m_Pipeline, GST_STATE_PAUSED);
  do {
    before_step = std::chrono::high_resolution_clock::now();
    if(!HandleExternalMessage()){
      g_printerr("Error handling external messages");
      return false;
    }
    if(!HandleInternalMessage()){
      g_printerr("Error handling internal messages");
      return false;
    }
    after_step = std::chrono::high_resolution_clock::now();
    // FXME: CHECK for types
    auto time_span = std::chrono::duration_cast<std::chrono::microseconds>(after_step - before_step);
    // FIXME: take into consideration time_span
    usleep(PERIOD);
  }while(!m_bTerminate);
  // Get ready for next stream
  g_print("Termianted loop.../n");
  m_bTerminate =false;
  return true; 
}

bool
MP3Handler::HandleInternalMessage(void){
  bool l_bHandledErrors=false;
  m_InternalMsg = gst_bus_pop (m_Bus);
  /* Parse message */
  if (m_InternalMsg != NULL) {
    GError *err;
    gchar *debug_info;
    
    switch (GST_MESSAGE_TYPE (m_InternalMsg)) {
    case GST_MESSAGE_ERROR:
      g_printerr("***Error ocurred***\n");
      l_bHandledErrors = true;
      gst_message_parse_error (m_InternalMsg, &err, &debug_info);
      g_printerr ("Error received from element %s: %s\n", GST_OBJECT_NAME (m_InternalMsg->src), err->message);
      g_printerr ("Debugging information: %s\n", debug_info ? debug_info : "none");
      g_clear_error (&err);
      g_free (debug_info);
      m_bTerminate = true;
      break;
    case GST_MESSAGE_EOS:
      g_print ("End-Of-Stream reached.\n");
      m_bTerminate = true;
      break;
    case GST_MESSAGE_STATE_CHANGED:
      /* We are only interested in state-changed messages from the pipeline */
      if (GST_MESSAGE_SRC (m_InternalMsg) == GST_OBJECT (m_Pipeline)) {
	GstState old_state, new_state, pending_state;
	gst_message_parse_state_changed (m_InternalMsg, &old_state, &new_state, &pending_state);
	g_print ("Pipeline state changed from %s to %s:\n",
		 gst_element_state_get_name (old_state), gst_element_state_get_name (new_state));
      }
      break;
    default:
      // Do not handle any other message
      g_printerr ("INFO: unhandled message\n");
      break;
    }
    gst_message_unref (m_InternalMsg);
  }
  return !l_bHandledErrors;
}
bool
MP3Handler::HandleExternalMessage(void){
  if (m_pStatusMessage->Handled) {
    return true;
  }
  g_printerr("Received Gui Command\n");
  switch (m_pStatusMessage->RequiredAction){
  case PLAY:
    gst_element_set_state (m_Pipeline, GST_STATE_PLAYING);
    break;
  case PAUSE:
    gst_element_set_state (m_Pipeline, GST_STATE_PAUSED);
    break;
  case STOP:
    gst_element_set_state (m_Pipeline, GST_STATE_NULL);
    break;
  case FORWARD:
  case BACKWARD:
    g_print("WIP: not implemented yet");
    break;
  default:
    g_printerr("Invalid action");
  }
  //FIXME: implement handler for Reset and FileID
  m_pStatusMessage->locker.lock();
  m_pStatusMessage->Handled = true;
  m_pStatusMessage->locker.unlock(); 
  return true;
}

bool
MP3Handler::Deinit(void){
  // Deinitialization
  g_print ("Deinitializing...\n");
  gst_object_unref (m_Bus);
  gst_element_set_state (m_Pipeline, GST_STATE_NULL);

  g_print ("Deleting Pipeline...\n");
  gst_object_unref (GST_OBJECT (m_Pipeline));

  return true;
}
