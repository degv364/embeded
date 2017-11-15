#ifndef MESSAGES_H
#define MESSAGES_H

#include <mutex>
#include "file_names.hpp"

typedef struct status_message{
  bool Play;
  bool Reset;
  file_e FileId;
  std::mutex locker;
  bool Handled;
} status_message;


#endif
