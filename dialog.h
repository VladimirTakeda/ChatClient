#ifndef DIALOG_H
#define DIALOG_H

#include <QString>
#include <QList>

struct Message {
    QString text;
    bool isMyMessage;
};

class Dialog
{
public:
    Dialog();
    void addMessage(const Message& msg);
    const QString & GetHtmlDialog() const;
private:
    QString m_contactName;
    QList<Message> m_messages;
    QString m_htmlMessages;
};

#endif // DIALOG_H
