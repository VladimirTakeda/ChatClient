#ifndef CHATWIDGET_H
#define CHATWIDGET_H

#include <QWidget>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui { class ChatUI; }
QT_END_NAMESPACE

struct UserInfo
{
    UserInfo(int, QString&&);
    QString userLogin;
    int userId;
};

class QNetworkAccessManager;
class QNetworkReply;
class QListWidgetItem;
class HttpClient;
class PopUp;

namespace WebSocket{
class Message;
class WebSocketClient;
}

class DialogsManager;

class ChatWidget : public QWidget
{
    Q_OBJECT
public:
    ChatWidget(std::shared_ptr<HttpClient>, QWidget *parent = nullptr);
    void SetUpWSConnection();
    void LoadDialogs();
    void SaveDialogs() const;
    ~ChatWidget();

private:
    void AddNewWidgetDialog(int chatId, const QString& name, bool needSetItem);
    void UpdateWidgetDialog(int chatId, const QString& lastMessage, uint64_t unreadCount, const QDateTime& localMsgTime);
    void AddMessageToWidgetDialog(int userId, const QString &lastMessage, bool NeedIncrement, const QDateTime& localMsgTime);
    void SetExistingDialogs();
    void SetSearchResults(const std::vector<UserInfo>& results);
    void SendCreateDialogReq(int fromUser, int toUser, const QString& toUserName);
    void GotNewMessage(WebSocket::Message mgs);
    void UpdateTextBrowser(int selectedContactId);

private slots:
    void on_lineEdit_2_returnPressed();
    void LookingForPeople(const QString& prefix);
    void LookingForPeopleReply(QNetworkReply *rep);
    void CreateChatReply(QNetworkReply *rep);
    void SetDialog(QListWidgetItem *);
    void SetNewDialog(QListWidgetItem *);
    void OnGotNotification(const QString& name, const QString& text, int64_t unreadCount, const QDateTime& time);

private:
    Ui::ChatUI *ui;

    std::unordered_map<int, QListWidgetItem*> m_idToDialogWidget;

    std::unique_ptr<WebSocket::WebSocketClient> m_client;
    std::shared_ptr<HttpClient> m_httpClient;

    std::unordered_map<QString, int> m_DialogsToId;
    std::unique_ptr<DialogsManager> m_dialogsManager;

    PopUp *m_popUp;
};

#endif // CHATWIDGET_H
