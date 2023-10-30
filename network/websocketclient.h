#ifndef WEBSOCKETCLIENT_H
#define WEBSOCKETCLIENT_H


//Qt includes
//WebSocket
#include <QtWebSockets/QWebSocketServer>
#include <QtWebSockets/QWebSocket>
//Util
#include <QObject>
#include <QDebug>

struct Message{
    QString text;
    int userFrom;
    int userTo;
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

#endif // WEBSOCKETCLIENT_H
