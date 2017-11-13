#include <gst/gst.h>
#include <glib.h>
#include "mp3_handler.hpp"

int
main (int   argc,char *argv[]){
  // Initialization
  gst_init (&argc, &argv);

  MP3Handler l_handler;
  l_handler.Setup();
  l_handler.Loop();
  l_handler.Deinit();
  return 0;
}
