#include "dialogsmanager.h"

#include <fstream>
#include <string>

#include <QString>

#include <QFile>

DialogsManager::DialogsManager()
{

}

void DialogsManager::CreateNewChat(std::optional<int> userId, int chatId, const QString &userToName)
{
    //m_UserToChatId[UserToId] = chatId;
    if (userId)
        m_users.insert(userId.value());
    m_IdToName[chatId] = userToName;
    m_IdToDialog.emplace(chatId, Dialog(chatId));
}

void DialogsManager::AddMessage(int chatId, const Message& msg)
{
    m_IdToDialog.at(chatId).addMessage(msg);
}

const Dialog& DialogsManager::GetDialog(int chatId)
{
    return  m_IdToDialog.at(chatId);
}

bool DialogsManager::IsChatExist(int chatId) const
{
    return m_IdToDialog.count(chatId);
}

bool DialogsManager::IsDialogWithUserExist(int userId) const
{
    return m_users.count(userId);
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
    QFile outFile("dialogs");
    if (outFile.open(QIODevice::WriteOnly)) {
        QDataStream out(&outFile);

        out << static_cast<uint64_t>(m_users.size());
        for (int32_t elem : m_users)
            out << elem;

        out << static_cast<uint64_t>(m_IdToDialog.size());

        for (const auto& [dialogId, dialog] : m_IdToDialog) {
            out << static_cast<uint64_t>(dialogId);
            out << dialog.m_unreadCount;
            out << static_cast<uint64_t>(dialog.m_messages.size());

            for (const Message& msg : dialog.m_messages) {
                out << msg.isMyMessage;

                QByteArray text = msg.text.toUtf8();
                uint64_t msgSize = static_cast<uint64_t>(text.size());
                out << msgSize;
                out.writeRawData(text.constData(), msgSize);
                out << msg.time;
            }
        }

        outFile.close();
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
    QFile inFile("dialogs");
    if (inFile.open(QIODevice::ReadOnly)) {
        m_IdToDialog.clear();

        QDataStream in(&inFile);

        uint64_t usersCount = 0;
        in >> usersCount;
        for (uint64_t i = 0; i < usersCount; i++){
            int32_t elem;
            in >> elem;
            m_users.insert(elem);
        }

        uint64_t dialogsCount = 0;
        in >> dialogsCount;

        for (uint64_t i = 0; i < dialogsCount; i++){
            uint64_t dialogId = 0;
            uint64_t messagesCount = 0;
            in >> dialogId;
            Dialog currentDialog(dialogId);
            in >> currentDialog.m_unreadCount;
            in >> messagesCount;

            bool isMyMessage;

            for (int j = 0; j < messagesCount; j++) {
                in >> isMyMessage;
                uint64_t size;
                in >> size;

                QByteArray text;
                text.resize(size);
                in.readRawData(text.data(), size);
                QDateTime time;
                in >> time;

                currentDialog.addMessage({QString::fromUtf8(text), isMyMessage, time});
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
