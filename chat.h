#ifndef CHATWIDGET_H
#define CHATWIDGET_H

#include <QWidget>
#include <memory>

class WebSocketClient;

QT_BEGIN_NAMESPACE
namespace Ui { class ChatUI; }
QT_END_NAMESPACE

class QNetworkAccessManager;
class QNetworkReply;

class ChatWidget : public QWidget
{
    Q_OBJECT
public:
    ChatWidget(QWidget *parent = nullptr);
    void SetUpWSConnection();
    ~ChatWidget();

private slots:
    void on_lineEdit_2_returnPressed();
    void LookingForPeople(const QString& prefix);
    void ReplyFinished(QNetworkReply *rep);


private:
    Ui::ChatUI *ui;
    QNetworkAccessManager* m_networkMgr;
    std::unique_ptr<WebSocketClient> m_client;
};

#endif // CHATWIDGET_H
