#include "dialogsmanager.h"

#include <fstream>
#include <string>

#include <QString>

DialogsManager::DialogsManager()
{

}

void DialogsManager::CreateNewChat(int UserToId, const QString &userToName)
{
    //m_UserToChatId[UserToId] = chatId;
    m_IdToName[UserToId] = userToName;
    m_IdToDialog.emplace(UserToId, Dialog(UserToId));
}

void DialogsManager::AddMessage(int userId, const Message& msg)
{
    m_IdToDialog.at(userId).addMessage(msg);
}

const Dialog& DialogsManager::GetDialog(int userId)
{
    return  m_IdToDialog.at(userId);
}

bool DialogsManager::IsDialogExist(int userId) const
{
    return m_IdToDialog.count(userId);
}

//TODO rewrite to binary format
void DialogsManager::LoadFromMemory()
{
    LoadDialogs();
    LoadGuiDialogs();
}

//TODO rewrite to binary format
void DialogsManager::SaveToMemory() const
{
    SaveDialogs();
    SaveGuiDialogs();
}

void DialogsManager::SaveDialogs() const
{
    std::ofstream outFile("dialogs", std::ios::trunc);
    if (outFile.is_open()) {
        outFile << static_cast<uint64_t>(m_IdToDialog.size());
        for (const auto& [dialogId, dialog] : m_IdToDialog) {
            outFile << static_cast<uint64_t>(dialogId);
            outFile << static_cast<uint64_t>(dialog.m_messages.size());
            for (const Message& msg : dialog.m_messages) {
                outFile << msg.isMyMessage << msg.text.toStdString();
            }
        }
        outFile.close();
    } else {

    }
}
void DialogsManager::SaveGuiDialogs() const
{
    std::ofstream outFile("data.txt", std::ios::binary);
    if (outFile.is_open()) {
        int32_t size = m_IdToName.size();
        outFile.write(reinterpret_cast<const char*>(&size), sizeof(size));

        // Записываем элементы контейнера
        for (const auto& [key, value] : m_IdToName) {
            outFile.write(reinterpret_cast<const char*>(&key), sizeof(key));

            QByteArray utf8Value = value.toUtf8();
            int32_t valueLength = utf8Value.size();
            outFile.write(reinterpret_cast<const char*>(&valueLength), sizeof(valueLength));
            outFile.write(utf8Value.constData(), valueLength);
        }

        // Закрываем файл
        outFile.close();
    } else {
        // Обработка ошибки открытия файла
    }
}
void DialogsManager::LoadDialogs()
{
    std::ifstream inFile("dialogs");
    if (inFile.is_open()) {
        m_IdToDialog.clear();

        uint64_t dialogsCount = 0;
        inFile >> dialogsCount;

        for (int i = 0; i < dialogsCount; i++){
            int64_t dialogId = 0;
            int64_t messagesCount = 0;
            inFile >> dialogId >> messagesCount;

            Dialog currentDialog(dialogId);

            bool isMyMessage;
            std::string text;

            for (int j = 0; j < messagesCount; j++) {
                inFile >> isMyMessage >> text;
                currentDialog.addMessage({QString::fromStdString(text), isMyMessage});
            }

            if (!currentDialog.m_messages.empty()) {
                m_IdToDialog.emplace(dialogId, currentDialog);
            }
        }

        inFile.close();
    }
}
void DialogsManager::LoadGuiDialogs()
{
    std::ifstream inFile("data.txt", std::ios::binary);
    if (inFile.is_open()) {
        int32_t size;
        inFile.read(reinterpret_cast<char*>(&size), sizeof(size));

        for (int i = 0; i < size; i++) {
            int32_t key;
            inFile.read(reinterpret_cast<char*>(&key), sizeof(key));

            int32_t valueLength;
            inFile.read(reinterpret_cast<char*>(&valueLength), sizeof(valueLength));

            QByteArray utf8Value(valueLength, '\0');
            inFile.read(utf8Value.data(), valueLength);
            QString value = QString::fromUtf8(utf8Value.constData());

            m_IdToName[key] = value;
        }

        // Закрываем файл
        inFile.close();
    } else {
        // Обработка ошибки открытия файла
    }
}
