#include "dialog.h"

Dialog::Dialog(int64_t contactId) : m_contactId(contactId)
{
}

void Dialog::addMessage(const Message& msg)
{
    m_messages.push_back(msg);
    if (msg.isMyMessage){
        m_htmlMessages.append("<div style='text-align: left;'><span style='background-color: #3498db; color: #fff;'>" + msg.text + "</span></div>");
    }
    else{
        m_htmlMessages.append("<div style='text-align: right;'><span style='background-color: #3498db; color: #fff;'>" + msg.text + "</span></div>");
    }
}

const QString& Dialog::GetHtmlDialog() const
{
    return m_htmlMessages;
}
