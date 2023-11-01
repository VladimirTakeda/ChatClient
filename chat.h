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
    ~ChatWidget();

private:
    void SetSearchResults(const std::vector<UserInfo>& results);
    void SendCreateDialogReq(int fromUser, int toUser);
    void GetNewMessage(WebSocket::Message mgs);
    void UpdateTextBrowser();

private slots:
    void on_lineEdit_2_returnPressed();
    void LookingForPeople(const QString& prefix);
    void LookingForPeopleReply(QNetworkReply *rep);
    void CreateChatReply(QNetworkReply *rep);
    void SetDialog(QListWidgetItem *);

private:
    Ui::ChatUI *ui;
    QNetworkAccessManager* m_networkMgr;
    std::unique_ptr<WebSocket::WebSocketClient> m_client;
    int lastUserId = 0;
    std::unordered_map<int,int> m_UserToChatId;
    std::shared_ptr<HttpClient> m_httpClient;
    std::unordered_map<QString, int> m_DialogsToId;
    std::unique_ptr<DialogsManager> m_dialogsManager;
    int m_CurrDialogUserId;
};

#endif // CHATWIDGET_H
