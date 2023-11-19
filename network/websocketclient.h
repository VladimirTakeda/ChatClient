#ifndef WEBSOCKETCLIENT_H
#define WEBSOCKETCLIENT_H


#include <QtWebSockets/QWebSocketServer>
#include <QtWebSockets/QWebSocket>

#include <QObject>
#include <QDebug>

namespace WebSocket{

struct Message{
    QString text;
    QString chatName;
    int userFrom;
    int chatTo;
    bool isMyMessage;
    QDateTime time;
};

class WebSocketClient : public QObject{
public:
    WebSocketClient(const QUrl &url, std::function<void(Message)> callBack, QObject* parent = nullptr);
    void SendTextMessage(const QString& msg);
private slots:
    void OnNewConnection();
    void OnCloseConnection();
    void OnTextMessageRecieved(QString message);
    void handle_ssl_errors(const QList<QSslError> &errors);
    QWebSocket m_socket;
    std::function<void(Message)> m_callBack;
};

}

#endif // WEBSOCKETCLIENT_H
