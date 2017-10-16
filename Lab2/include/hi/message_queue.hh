#ifndef MESSAGE_QUEUE_
#define MESSAGE_QUEUE_

#include "common_def.hh"
#include "hi/hi_def.hh"
#include "msp.h"

//FIXME: find a better way to choose size, maybe inheritance?
#define MESSAGE_QUEUE_SIZE 10 //5

class MessageQueue
{
private:
    message_t m_aMessageList[MESSAGE_QUEUE_SIZE];
    uint8_t m_u8ReadIndex;
    uint8_t m_u8WriteIndex;

public:
    MessageQueue();
    return_e AddMessage(message_t i_stNewMessage);
    return_e PopMessage(message_t* o_stPopMessage);
    return_e ReadMessage(message_t* o_stReadMessage);
};

#endif
