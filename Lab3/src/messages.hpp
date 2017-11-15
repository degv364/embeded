#ifndef MESSAGES_H
#define MESSAGES_H

#include <mutex>
#include "file_names.hpp"

typedef enum action_e{
  PLAY = 0,
  PAUSE,
  STOP,
  FORWARD,
  BACKWARD,
  LAST_ACTION
}action_e;

typedef struct status_message{
  action_e RequiredAction;
  file_e FileId;
  std::mutex locker;
  bool Handled;
} status_message;


#endif
