#include "websocketclient.h"

#include <QJsonDocument>
#include <QJsonObject>

namespace WebSocket{

WebSocketClient::WebSocketClient(const QUrl &url, std::function<void(Message)> callBack, QObject* parent):
    QObject(parent),
    m_socket(),
    m_callBack(callBack)
{
    connect(&m_socket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::errorOccurred),
            [=](QAbstractSocket::SocketError error){
                qDebug() << "Error";
            });
    connect(&m_socket, &QWebSocket::connected, this, &WebSocketClient::OnNewConnection);
    connect(&m_socket, &QWebSocket::disconnected, this, &WebSocketClient::OnCloseConnection);
    connect(&m_socket, QOverload<const QList<QSslError>&>::of(&QWebSocket::sslErrors),
            this, &WebSocketClient::handle_ssl_errors);
    qDebug() << url.path();
    qDebug() << url.port();
    qDebug() << url.url();
    qDebug() << url.host();
    m_socket.open(url);
    qDebug() << m_socket.state();
}

void WebSocketClient::SendTextMessage(const QString& msg){
    qDebug() << msg;
    qDebug() << "Sended bytes : " << m_socket.sendTextMessage(msg);
}

void WebSocketClient::OnNewConnection(){
    qDebug() << "New Connection";

    connect(&m_socket, &QWebSocket::textMessageReceived, this, &WebSocketClient::OnTextMessageRecieved);
}

void WebSocketClient::OnCloseConnection(){
    qDebug() << "Closed Connection";
}

void WebSocketClient::OnTextMessageRecieved(QString message){
    QJsonDocument itemDoc = QJsonDocument::fromJson(message.toUtf8());
    QJsonObject rootObject = itemDoc.object();
    Message msg;
    msg.text = rootObject.value("content").toString();
    msg.userFrom = rootObject.value("user_from_id").toInt();
    msg.userTo = rootObject.value("user_to_id").toInt();
    msg.userNameFrom = rootObject.value("user_name_from").toString();
    msg.isMyMessage = rootObject.value("is_my_message").toBool();
    m_callBack(msg);
}

void WebSocketClient::handle_ssl_errors(const QList<QSslError> &errors){
    Q_UNUSED(errors);
    qDebug() << "SSL";
    m_socket.ignoreSslErrors();
}
}
