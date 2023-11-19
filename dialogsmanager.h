#ifndef DIALOGSMANAGER_H
#define DIALOGSMANAGER_H

#include "dialog.h"

#include <unordered_set>

class ChatWidget;

class DialogsManager
{
    friend ChatWidget;
public:
    DialogsManager();
    void CreateNewChat(std::optional<int> userId, int chatId, const QString& userToName);
    void AddMessage(int userId, const Message& msg);
    const Dialog& GetDialog(int userId);
    bool IsChatExist(int chatId) const;
    bool IsDialogWithUserExist(int userId) const;
    void LoadFromMemory();
    void SaveToMemory() const;

private:
    void SaveDialogs() const;
    void SaveGuiDialogs() const;
    void LoadDialogs();
    void LoadGuiDialogs();

private:
    //std::unordered_map<int, int> m_UserToChatId;
    std::unordered_set<int> m_users; // users, we have dialog with
    std::unordered_map<int32_t, Dialog> m_IdToDialog;
    std::unordered_map<int32_t, QString> m_IdToName;
};

#endif // DIALOGSMANAGER_H
