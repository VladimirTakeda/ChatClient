#include "websocketclient.h"

WebSocketClient::WebSocketClient(const QUrl &url, QObject* parent):
    QObject(parent),
    m_socket()
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
}

void WebSocketClient::handle_ssl_errors(const QList<QSslError> &errors){
    Q_UNUSED(errors);
    qDebug() << "SSL";
    m_socket.ignoreSslErrors();
}
