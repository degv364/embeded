#include "hi/task.hh"

Task::Task()
{
    m_bIsFinished = false;
    Incoming = MessageQueue();
    Outgoing = MessageQueue();
}

return_e
Task::ReceiveMessage(message_t i_stNewMessage){
  if (i_stNewMessage.receiver != this->GetTaskName() ||
      i_stNewMessage.data == 0){
    return RETURN_BAD_PARAM;
  }
  return Incoming.AddMessage(i_stNewMessage);
}

return_e
Task::PopMessage(message_t* o_stMessage){
  return_e rt = RETURN_OK;
  rt = Outgoing.PopMessage(o_stMessage);
  if (rt != RETURN_OK){
    return rt;
  }
  if (o_stMessage->sender != m_eTaskName ||
      o_stMessage->data == 0){
    return RETURN_FAIL;
  }
  return RETURN_OK;
}
