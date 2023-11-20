#include "chat.h"

#include "user_item.h"
#include "dialogsmanager.h"

#include "network/websocketclient.h"
#include "network/httpclient.h"

#include "utils.h"

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

namespace{

std::vector<UserInfo> ParseUsers(QByteArray reply)
{
    qDebug() << reply;
    QJsonDocument itemDoc = QJsonDocument::fromJson(reply);
    QJsonObject rootObject = itemDoc.object();
    QJsonArray array = rootObject.value("users").toArray();
    std::vector<UserInfo> answer;
    int currUserId = getCurrUserId();
    for (const QJsonValue & value : array) {
        QJsonObject obj = value.toObject();
        if (obj["id"].toInt() != currUserId)
            answer.emplace_back(obj["id"].toInt(), obj["nickname"].toString());
    }
    return answer;
}

}

ChatWidget::ChatWidget(std::shared_ptr<HttpClient> httpClient, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatUI)
    , m_httpClient(std::move(httpClient))
    , m_dialogsManager(new DialogsManager())
{
    ui->setupUi(this);
    connect(ui->lineEdit, &QLineEdit::textEdited, this, &ChatWidget::LookingForPeople);
    connect(ui->listWidget_2, &QListWidget::itemClicked, this, &ChatWidget::SetNewDialog);
    connect(ui->listWidget, &QListWidget::itemClicked, this, &ChatWidget::SetDialog);
}

ChatWidget::~ChatWidget()
{
}

void ChatWidget::AddNewWidgetDialog(int chatId, const QString& name, bool needSetItem)
{
    QListWidgetItem *contactItem = new QListWidgetItem(ui->listWidget);
    auto newUser = new UserItemWidget();
    newUser->SetName(name);
    contactItem->setData(Qt::UserRole, chatId);
    contactItem->setSizeHint(newUser->sizeHint());
    ui->listWidget->addItem(contactItem);
    ui->listWidget->setItemWidget(contactItem, newUser);
    if (needSetItem)
        contactItem->setSelected(true);
    m_idToDialogWidget.emplace(chatId, contactItem);
    ui->stackedWidget->setCurrentIndex(0);
}

void ChatWidget::UpdateWidgetDialog(int userId, const QString &lastMessage, uint64_t unreadCount, const QDateTime& localMsgTime)
{
    UserItemWidget *itemWidget = qobject_cast<UserItemWidget*>(ui->listWidget->itemWidget(m_idToDialogWidget[userId]));
    itemWidget->SetUnreadCount(unreadCount);
    itemWidget->SetLastText(lastMessage);
    itemWidget->SetLastTextTime(localMsgTime);
}

void ChatWidget::AddMessageToWidgetDialog(int userId, const QString &lastMessage, bool NeedIncrement, const QDateTime& localMsgTime)
{
    UserItemWidget *itemWidget = qobject_cast<UserItemWidget*>(ui->listWidget->itemWidget(m_idToDialogWidget[userId]));
    if (NeedIncrement){
        itemWidget->IncrementUnreadCount();
        m_dialogsManager->m_IdToDialog.at(userId).m_unreadCount++;
    }
    itemWidget->SetLastText(lastMessage);
    itemWidget->SetLastTextTime(localMsgTime);
}

void ChatWidget::SetUpWSConnection(){
    QString url = QString("ws://localhost:8080/create?user_id=%1&device_id=%2").arg(getCurrUserId()).arg(getCurrDeviceId());
    m_client.reset(new WebSocket::WebSocketClient(QUrl(url), std::bind(&ChatWidget::GetNewMessage, this, std::placeholders::_1)));
}

void ChatWidget::LoadDialogs()
{
    m_dialogsManager->LoadFromMemory();
    for (const auto& [userId, value]: m_dialogsManager->m_IdToDialog){
        AddNewWidgetDialog(userId, m_dialogsManager->m_IdToName[userId], false);
        UpdateWidgetDialog(userId, value.m_messages.back().text, value.m_unreadCount, value.m_messages.back().time);
    }
}

void ChatWidget::SaveDialogs() const
{
    m_dialogsManager->SaveToMemory();
}

void ChatWidget::GetNewMessage(WebSocket::Message msg)
{
    // MayCreateNewDialog
    if (!m_dialogsManager->IsChatExist(msg.chatTo)){
        m_dialogsManager->CreateNewChat(msg.userFrom, msg.chatTo, msg.chatName);
        AddNewWidgetDialog(msg.chatTo, msg.chatName, false);
    }
    m_dialogsManager->AddMessage(msg.chatTo, {msg.text, msg.isMyMessage, msg.time});
    //if it's current dialog then update otherwise no
    bool IsSelectedDialog = false;
    if (auto currItem = ui->listWidget->currentItem(); currItem != nullptr && currItem->data(Qt::UserRole).toInt() == msg.chatTo)
        IsSelectedDialog = true;

    if (IsSelectedDialog)
        UpdateTextBrowser(msg.chatTo);

    AddMessageToWidgetDialog(msg.chatTo, msg.text, !IsSelectedDialog, msg.time);
}

void ChatWidget::UpdateTextBrowser(int selectedContactId)
{
    ui->textBrowser->clear();
    ui->textBrowser->setHtml(m_dialogsManager->GetDialog(selectedContactId).GetHtmlDialog());
}

void ChatWidget::on_lineEdit_2_returnPressed()
{
    qDebug() << "here";
    QJsonObject obj;
    obj["content"] = ui->lineEdit_2->text();
    obj["user_from_id"] = getCurrUserId();
    obj["chat_to_id"] = ui->listWidget->currentItem()->data(Qt::UserRole).toInt();
    obj["chat_name"] = getCurrUserName();

    QJsonDocument doc(obj);

    m_client->SendTextMessage(doc.toJson(QJsonDocument::Compact));

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

    m_httpClient->sendHttpRequest(std::move(request), std::move(data), {}, std::bind(&ChatWidget::LookingForPeopleReply, this, std::placeholders::_1));
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

void ChatWidget::SetExistingDialogs()
{

}

void ChatWidget::SetDialog(QListWidgetItem * clickedItem)
{
    UserItemWidget *itemWidget = qobject_cast<UserItemWidget*>(ui->listWidget->itemWidget(clickedItem));

    itemWidget->ClearUnreadCount();
    m_dialogsManager->m_IdToDialog.at(clickedItem->data(Qt::UserRole).toInt()).m_unreadCount = 0;

    ui->stackedWidget_2->setCurrentIndex(1);
    ui->label_4->setText(itemWidget->GetName());
    UpdateTextBrowser(clickedItem->data(Qt::UserRole).toInt());
}

void ChatWidget::SetNewDialog(QListWidgetItem * clickedItem)
{
    ui->lineEdit->clear();

    UserItemWidget *itemWidget = qobject_cast<UserItemWidget*>(ui->listWidget_2->itemWidget(clickedItem));

    ui->stackedWidget_2->setCurrentIndex(1);
    ui->label_4->setText(itemWidget->GetName());
    if (!m_dialogsManager->IsDialogWithUserExist(clickedItem->data(Qt::UserRole).toInt())){
        SendCreateDialogReq(getCurrUserId(), clickedItem->data(Qt::UserRole).toInt(), itemWidget->GetName());
    }
    else{
        UpdateTextBrowser(clickedItem->data(Qt::UserRole).toInt());
    }
}

void ChatWidget::SendCreateDialogReq(int fromUser, int toUser, const QString& toUserName){
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

    std::vector<std::pair<std::string, QVariant>> properties = {{"toUserId", toUser}, {"toUserName", toUserName}};
    m_httpClient->sendHttpRequest(std::move(request), std::move(data), properties, std::bind(&ChatWidget::CreateChatReply, this, std::placeholders::_1));
}

void ChatWidget::CreateChatReply(QNetworkReply *reply){
    if (reply->error() == QNetworkReply::NoError) {
        // CreateNewDialog
        QJsonDocument itemDoc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject rootObject = itemDoc.object();
        m_dialogsManager->CreateNewChat(reply->property("toUserId").toInt(), rootObject.value("chatId").toInt(), reply->property("toUserName").toString());
        AddNewWidgetDialog(rootObject.value("chatId").toInt(), reply->property("toUserName").toString(), true);
    }
    else {
        qDebug() << "Failure" <<reply->errorString();
    }
}
