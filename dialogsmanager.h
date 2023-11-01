#ifndef DIALOGSMANAGER_H
#define DIALOGSMANAGER_H

#include "dialog.h"

class DialogsManager
{
public:
    DialogsManager();
    void AddMessage(int userId, const Message& msg);
    const Dialog& GetDialog(int userId);
    void LoadFromMemory();
    void SaveToMemory();
    std::unordered_map<int, Dialog> m_dialogs;
};

#endif // DIALOGSMANAGER_H
