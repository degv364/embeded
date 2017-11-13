#include <thread>
#include <audio_server.hpp>

int main(int   argc, char *argv[]){
  gst_init (&argc, &argv);
  std::thread l_AudioServer(AudioServer);
  l_AudioServer.join();
  return 0;
}
