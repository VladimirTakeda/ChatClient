#include "chat.h"

#include "user_item.h"
#include "dialogsmanager.h"

#include "network/websocketclient.h"
#include "network/httpclient.h"

#include <QNetworkAccessManager>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QSettings>
#include <QJsonArray>
#include <QDir>

#include "./ui_chat.h"

UserInfo::UserInfo(int userId, QString&& userNickname) : userId(userId), userLogin(std::move(userNickname)){

}

namespace {
int getCurrUserId(){
    QSettings settings(QDir(QApplication::applicationDirPath()).filePath("settings.ini"));
    qDebug() << settings.fileName();
    return settings.value("userId").toInt();
}

std::vector<UserInfo> ParseUsers(QByteArray reply)
{
    qDebug() << reply;
    QJsonDocument itemDoc = QJsonDocument::fromJson(reply);
    QJsonObject rootObject = itemDoc.object();
    QJsonArray array = rootObject.value("users").toArray();
    std::vector<UserInfo> answer;
    for (const QJsonValue & value : array) {
        QJsonObject obj = value.toObject();
        answer.emplace_back(obj["id"].toInt(), obj["nickname"].toString());
    }
    return answer;
}
}

ChatWidget::ChatWidget(std::shared_ptr<HttpClient> httpClient, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatUI)
    , m_networkMgr(new QNetworkAccessManager(this))
    , m_httpClient(std::move(httpClient))
    , m_dialogsManager(new DialogsManager())
{
    ui->setupUi(this);
    connect(ui->lineEdit, &QLineEdit::textEdited, this, &ChatWidget::LookingForPeople);
    connect(ui->listWidget_2, &QListWidget::itemClicked, this, &ChatWidget::SetDialog);
}

ChatWidget::~ChatWidget()
{
}

void ChatWidget::SetUpWSConnection(){

    QString url = QString("ws://localhost:8080/create?user_id=%1").arg(getCurrUserId());
    m_client.reset(new WebSocket::WebSocketClient(QUrl(url), std::bind(&ChatWidget::GetNewMessage, this, std::placeholders::_1)));
}

void ChatWidget::GetNewMessage(WebSocket::Message mgs)
{
    m_dialogsManager->AddMessage(mgs.userFrom, {mgs.text, false});
    UpdateTextBrowser();
}

void ChatWidget::UpdateTextBrowser()
{
    ui->textBrowser->clear();
    ui->textBrowser->setHtml(m_dialogsManager->GetDialog(m_CurrDialogUserId).GetHtmlDialog());
}

void ChatWidget::on_lineEdit_2_returnPressed()
{
    qDebug() << "here";
    QJsonObject obj;
    obj["content"] = ui->lineEdit_2->text();
    obj["user_from_id"] = getCurrUserId();
    obj["user_to_id"] = m_CurrDialogUserId;

    m_dialogsManager->AddMessage(m_CurrDialogUserId, {ui->lineEdit_2->text(), true});
    UpdateTextBrowser();

    QJsonDocument doc(obj);

    m_client->SendTextMessage(doc.toJson(QJsonDocument::Compact));

    //QString formattedMessage = "<span style='background-color: #3498db; color: #fff;'>" + ui->lineEdit_2->text() + "</span>";
    ui->lineEdit_2->clear();
}

void ChatWidget::LookingForPeople(const QString& prefix)
{
    if (prefix.isEmpty() && ui->stackedWidget->currentIndex() == 1){
        ui->stackedWidget->setCurrentIndex(0);
        return;
    }

    if (!prefix.isEmpty() && ui->stackedWidget->currentIndex() == 0)
        ui->stackedWidget->setCurrentIndex(1);

    QNetworkRequest request;

    QJsonObject obj;
    obj["search_prefix"] = ui->lineEdit->text();
    obj["this_user_id"] = getCurrUserId();
    QJsonDocument doc(obj);
    QByteArray data = doc.toJson();

    QUrl url;

    url.setScheme("http");
    url.setHost("localhost");
    url.setPath("/get_users_by_prefix");
    url.setPort(8080);
    request.setUrl(url);
    request.setRawHeader("Content-Type", "application/json");

    m_httpClient->sendHttpRequest(std::move(request), std::move(data), std::bind(&ChatWidget::LookingForPeopleReply, this, std::placeholders::_1));
}

void ChatWidget::LookingForPeopleReply(QNetworkReply *reply){
    if (reply->error() == QNetworkReply::NoError) {
        SetSearchResults(ParseUsers(reply->readAll()));
    }
    else {
        qDebug() << "Failure" <<reply->errorString();
    }
}

void ChatWidget::SetSearchResults(const std::vector<UserInfo>& results)
{
    ui->listWidget_2->clear();
    for (const UserInfo& userInfo : results){
        QListWidgetItem *contactItem = new QListWidgetItem(ui->listWidget_2);
        auto user = new UserItemWidget();
        user->SetName(userInfo.userLogin);
        user->SetLastText("");
        contactItem->setData(Qt::UserRole, userInfo.userId);
        contactItem->setSizeHint(user->sizeHint());
        ui->listWidget_2->addItem(contactItem);
        ui->listWidget_2->setItemWidget(contactItem, user);
    }
}

void ChatWidget::SetDialog(QListWidgetItem * clickedItem)
{
    ui->stackedWidget_2->setCurrentIndex(1);
    ui->label_4->setText(clickedItem->text());
    m_CurrDialogUserId = clickedItem->data(Qt::UserRole).toInt();
    UpdateTextBrowser();
    if (ui->textBrowser->toPlainText().isEmpty()){
        SendCreateDialogReq(getCurrUserId(), clickedItem->data(Qt::UserRole).toInt());
    }
}

void ChatWidget::SendCreateDialogReq(int fromUser, int toUser){
    QNetworkRequest request;

    QJsonObject obj;
    obj["from_user"] = fromUser;
    obj["to_user"] = toUser;
    QJsonDocument doc(obj);
    QByteArray data = doc.toJson();

    QUrl url;

    url.setScheme("http");
    url.setHost("localhost");
    url.setPath("/chats");
    url.setPort(8080);
    request.setUrl(url);
    request.setRawHeader("Content-Type", "application/json");

    m_httpClient->sendHttpRequest(std::move(request), std::move(data), std::bind(&ChatWidget::CreateChatReply, this, std::placeholders::_1));
    lastUserId = toUser;
}

void ChatWidget::CreateChatReply(QNetworkReply *reply){
    if (reply->error() == QNetworkReply::NoError) {
        QJsonDocument itemDoc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject rootObject = itemDoc.object();
        m_UserToChatId[lastUserId] = rootObject.value("chatId").toInt();
    }
    else {
        qDebug() << "Failure" <<reply->errorString();
    }
}
