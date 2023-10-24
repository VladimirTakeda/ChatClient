#include "chat.h"

#include "websocketclient.h"

#include <QNetworkAccessManager>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkReply>

#include "./ui_chat.h"

ChatWidget::ChatWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatUI)
    , m_networkMgr(new QNetworkAccessManager(this))
{
    ui->setupUi(this);
    connect(ui->lineEdit, &QLineEdit::textEdited, this, &ChatWidget::LookingForPeople);
    connect(m_networkMgr, &QNetworkAccessManager::finished,this, &ChatWidget::ReplyFinished);
}

ChatWidget::~ChatWidget()
{
}

void ChatWidget::SetUpWSConnection(){
    m_client.reset(new WebSocketClient(QUrl("ws://localhost:8080/echo")));
}

void ChatWidget::on_lineEdit_2_returnPressed()
{
    m_client->SendTextMessage(ui->lineEdit_2->text());
    QString formattedMessage = "<span style='background-color: #3498db; color: #fff;'>" + ui->lineEdit_2->text() + "</span>";
    ui->textBrowser->append(formattedMessage);
    ui->lineEdit_2->clear();
}

void ChatWidget::LookingForPeople(const QString& prefix)
{
    QNetworkRequest request;

    QJsonObject obj;
    obj["prefix"] = ui->lineEdit_2->text();
    obj["this_user"] = ui->lineEdit_2->text();
    QJsonDocument doc(obj);
    QByteArray data = doc.toJson();

    QUrl url;

    url.setScheme("http");
    url.setHost("localhost");
    url.setPath("/user/get_users_by_prefix");
    url.setPort(8080);
    request.setUrl(url);
    request.setRawHeader("Content-Type", "application/json");
    m_networkMgr->post(request, data);
}

void ChatWidget::ReplyFinished(QNetworkReply *reply){
    if (reply->error() == QNetworkReply::NoError) {
        qDebug() << "Success" << reply->readAll();
    }
    else {
        qDebug() << "Failure" <<reply->errorString();
    }
}

