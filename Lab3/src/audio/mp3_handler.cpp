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

#include "mp3_handler.hpp"

MP3Handler::MP3Handler(status_message* i_pStatusMessage, char* i_pMediaPath){
  m_bTerminate = false;
  m_pStatusMessage = i_pStatusMessage;
  m_iCurrentSong = 0;
  strcpy(m_MediaPath, i_pMediaPath);
  strcat(m_MediaPath, "music/");
}

bool
MP3Handler::Setup(void){

  InitializeFilePaths();
  
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

  g_print("file %s",m_FilePaths[0]);
  g_object_set (G_OBJECT (m_Source), "location",
		m_FilePaths[m_iCurrentSong], NULL);

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
    GError *l_Err;
    gchar *l_pDebugInfo;
    
    switch (GST_MESSAGE_TYPE (m_InternalMsg)) {
    case GST_MESSAGE_ERROR:
      g_printerr("***Error ocurred***\n");
      l_bHandledErrors = true;
      gst_message_parse_error (m_InternalMsg, &l_Err, &l_pDebugInfo);
      g_printerr ("Error received from element %s: %s\n", GST_OBJECT_NAME (m_InternalMsg->src), l_Err->message);
      g_printerr ("Debugging information: %s\n", l_pDebugInfo ? l_pDebugInfo : "none");
      g_clear_error (&l_Err);
      g_free (l_pDebugInfo);
      m_bTerminate = true;
      break;
    case GST_MESSAGE_EOS:
      g_print ("End-Of-Stream reached.\n");
      gst_element_set_state (m_Pipeline, GST_STATE_NULL);
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
    m_iCurrentSong +=1;
    if (m_iCurrentSong >= LAST_FILE){
      m_iCurrentSong = LAST_FILE-1;
    }
    gst_element_set_state (m_Pipeline, GST_STATE_NULL);
    g_object_set (G_OBJECT (m_Source), "location",
		m_FilePaths[m_iCurrentSong], NULL);
    break;
  case BACKWARD:
    m_iCurrentSong -=1;
    if (m_iCurrentSong < 0){
      m_iCurrentSong = 0;
    }
    gst_element_set_state (m_Pipeline, GST_STATE_NULL);
    g_object_set (G_OBJECT (m_Source), "location",
		m_FilePaths[m_iCurrentSong], NULL);
    break;
  case QUIT:
     m_bTerminate = true;
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


void MP3Handler::InitializeFilePaths(void){
  strcpy(m_FilePaths[ALL_STAR_TRIO], m_MediaPath);
  strcat(m_FilePaths[ALL_STAR_TRIO], ALL_STAR_TRIO_STR);

  strcpy(m_FilePaths[ART_OF_ESCAPISM], m_MediaPath);
  strcat(m_FilePaths[ART_OF_ESCAPISM], ART_OF_ESCAPISM_STR);

  strcpy(m_FilePaths[TYPICAL_DAY], m_MediaPath);
  strcat(m_FilePaths[TYPICAL_DAY], TYPICAL_DAY_STR);

  strcpy(m_FilePaths[ALL_BEGINS_HERE ], m_MediaPath);
  strcat(m_FilePaths[ALL_BEGINS_HERE ], ALL_BEGINS_HERE_STR);

  strcpy(m_FilePaths[SHES_A_GIFT ], m_MediaPath);
  strcat(m_FilePaths[SHES_A_GIFT ], SHES_A_GIFT_STR);

  strcpy(m_FilePaths[WELCOME ], m_MediaPath);
  strcat(m_FilePaths[WELCOME ], WELCOME_STR);
  
  strcpy(m_FilePaths[NIGHT_OWL ], m_MediaPath);
  strcat(m_FilePaths[NIGHT_OWL ], NIGHT_OWL_STR);
  
  strcpy(m_FilePaths[WITH_EASE ], m_MediaPath);
  strcat(m_FilePaths[WITH_EASE ], WITH_EASE_STR);
  
  strcpy(m_FilePaths[BEING_FINE ], m_MediaPath);
  strcat(m_FilePaths[BEING_FINE ], BEING_FINE_STR);
  
  strcpy(m_FilePaths[MOODY_BREAKFAST ], m_MediaPath);
  strcat(m_FilePaths[MOODY_BREAKFAST ], MOODY_BREAKFAST_STR);
  
  strcpy(m_FilePaths[THE_LAST_ONES ], m_MediaPath);
  strcat(m_FilePaths[THE_LAST_ONES ], THE_LAST_ONES_STR);
  
  strcpy(m_FilePaths[GOOD_GRIEF ], m_MediaPath);
  strcat(m_FilePaths[GOOD_GRIEF ], GOOD_GRIEF_STR);
  
  strcpy(m_FilePaths[PIANO_ROLL ], m_MediaPath);
  strcat(m_FilePaths[PIANO_ROLL ], PIANO_ROLL_STR);
  
  strcpy(m_FilePaths[ENTHUSIAST ], m_MediaPath);
  strcat(m_FilePaths[ENTHUSIAST ], ENTHUSIAST_STR);

}
