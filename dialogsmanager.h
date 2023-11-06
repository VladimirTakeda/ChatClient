#ifndef DIALOGSMANAGER_H
#define DIALOGSMANAGER_H

#include "dialog.h"

class ChatWidget;

class DialogsManager
{
    friend ChatWidget;
public:
    DialogsManager();
    void CreateNewChat(int UserToId, const QString& userToName);
    void AddMessage(int userId, const Message& msg);
    const Dialog& GetDialog(int userId);
    bool IsDialogExist(int userId) const;
    void LoadFromMemory();
    void SaveToMemory() const;

private:
    void SaveDialogs() const;
    void SaveGuiDialogs() const;
    void LoadDialogs();
    void LoadGuiDialogs();

private:
    //std::unordered_map<int, int> m_UserToChatId;
    std::unordered_map<int32_t, Dialog> m_IdToDialog;
    std::unordered_map<int32_t, QString> m_IdToName;
};

#endif // DIALOGSMANAGER_H
