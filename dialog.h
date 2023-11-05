#ifndef DIALOG_H
#define DIALOG_H

#include <QString>
#include <QList>

struct Message {
    QString text;
    bool isMyMessage;
};

class DialogsManager;

class Dialog
{
    friend DialogsManager;
public:
    Dialog(int64_t contactId);
    void addMessage(const Message& msg);
    const QString & GetHtmlDialog() const;
private:
    int64_t m_contactId;
    QList<Message> m_messages;
    QString m_htmlMessages;
};

#endif // DIALOG_H
