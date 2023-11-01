#include "dialogsmanager.h"

DialogsManager::DialogsManager()
{

}

void DialogsManager::AddMessage(int userId, const Message& msg)
{
    m_dialogs[userId].addMessage(msg);
}

const Dialog& DialogsManager::GetDialog(int userId)
{
    return  m_dialogs[userId];
}
