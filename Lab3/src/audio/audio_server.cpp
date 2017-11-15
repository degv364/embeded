#include "audio_server.hpp"

void
AudioServer (status_message* i_pStatusMessage){
  // Initialization
  if (i_pStatusMessage == NULL){
    g_printerr("Got invalid message");
    return;
  }

  MP3Handler l_handler(i_pStatusMessage);
  l_handler.Setup();
  l_handler.Loop();
  l_handler.Deinit();
}
