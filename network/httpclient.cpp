#include "httpclient.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>

HttpClient::HttpClient(QObject* parent) : QObject(parent) {
    m_networkManager = new QNetworkAccessManager(this);
    connect(m_networkManager, &QNetworkAccessManager::finished, this, &HttpClient::onRequestFinished);
}

void HttpClient::sendHttpRequest(QNetworkRequest&& request, QByteArray&& data, std::function<void(QNetworkReply *)> callBack) {
    QNetworkReply* reply = m_networkManager->post(request, data);

    m_reqToFunc[reply] = callBack;

    connect(reply, &QNetworkReply::sslErrors, this, &HttpClient::onSslErrors);
}

void HttpClient::onRequestFinished(QNetworkReply* reply) {
    if (reply->error() == QNetworkReply::NoError) {
        m_reqToFunc[reply](reply);
    } else {
        qDebug() << "HTTP request error" << ": " << reply->errorString();
    }
}

void HttpClient::onSslErrors(QList<QSslError> errors) {
    for (const QSslError& error : errors) {
        qDebug() << "SSL error: " << error.errorString();
    }
}
