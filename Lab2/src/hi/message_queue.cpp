#include "hi/message_queue.hh"

MessageQueue::MessageQueue()
{
    m_u8ReadIndex = 0;
    m_u8WriteIndex = 0;
}
return_e MessageQueue::AddMessage(message_t i_stNewMessage)
{
    // Check if there is available space
    if (m_u8ReadIndex == (m_u8WriteIndex + 1) % MESSAGE_QUEUE_SIZE)
    {
        return RETURN_NO_SPACE;
    }
    // Add message
    m_u8WriteIndex = (m_u8WriteIndex + 1) % MESSAGE_QUEUE_SIZE;
    m_aMessageList[m_u8WriteIndex] = i_stNewMessage;
    return RETURN_OK;
}

return_e MessageQueue::PopMessage(message_t* o_stPopMessage)
{
    //Check if it is empty
    if (m_u8ReadIndex == m_u8WriteIndex)
    {
        return RETURN_EMPTY;
    }
    // Pop Message
    *o_stPopMessage = m_aMessageList[m_u8ReadIndex];
    m_u8ReadIndex = (m_u8ReadIndex + 1) % MESSAGE_QUEUE_SIZE;
    return RETURN_OK;
}

return_e MessageQueue::ReadMessage(message_t* o_stReadMessage)
{
    //Check if it is empty
    if (m_u8ReadIndex == m_u8WriteIndex)
    {
        return RETURN_EMPTY;
    }
    // Read Message
    *o_stReadMessage = m_aMessageList[m_u8ReadIndex];
    return RETURN_OK;
}
