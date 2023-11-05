#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <QObject>
#include <QHash>
#include <qsslerror.h>

class QNetworkAccessManager;
class QNetworkReply;
class QNetworkRequest;

constexpr std::string_view createChatReq = "CreateChatReq";
constexpr std::string_view getUsersListReq = "GetUsersListReq";

class HttpClient : public QObject {
    Q_OBJECT
public:
    HttpClient(QObject* parent = nullptr);

public:
    void sendHttpRequest(QNetworkRequest&& request, QByteArray&& data, const std::vector<std::pair<std::string, QVariant>>& properties, std::function<void(QNetworkReply *)>);
private slots:
    void onRequestFinished(QNetworkReply* reply);

    void onSslErrors(QList<QSslError> errors);

private:
    QNetworkAccessManager* m_networkManager;
    std::unordered_map<QNetworkReply*, std::function<void(QNetworkReply *)>> m_reqToFunc;
    QHash<QNetworkReply*, std::string_view> m_requestNames;
};

#endif // HTTPCLIENT_H
