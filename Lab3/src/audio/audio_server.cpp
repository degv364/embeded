#include "audio_server.hpp"

void
AudioServer (void){
  // Initialization
  //gst_init (&argc, &argv);

  MP3Handler l_handler;
  l_handler.Setup();
  l_handler.Loop();
  l_handler.Deinit();
}
