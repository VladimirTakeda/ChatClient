#ifndef DIALOG_H
#define DIALOG_H

#include <QString>
#include <QList>
#include <QDateTime>

struct Message {
    QString text;
    bool isMyMessage;
    QDateTime time;
};

class DialogsManager;
class ChatWidget;

class Dialog
{
    friend DialogsManager;
    friend ChatWidget;
public:
    Dialog(int64_t chatId);
    void addMessage(const Message& msg);
    const QString & GetHtmlDialog() const;
private:
    uint64_t m_unreadCount;
    int64_t m_chatId;
    QList<Message> m_messages;
    QString m_htmlMessages;
};

#endif // DIALOG_H
