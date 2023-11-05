#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class RegistrationUI; }
QT_END_NAMESPACE

class MainWindow;
class QNetworkAccessManager;
class QNetworkReply;
class HttpClient;

class RegistrationWidget : public QWidget
{
    Q_OBJECT
public:
    RegistrationWidget(std::shared_ptr<HttpClient>, MainWindow* mainWindow, QWidget *parent = nullptr);
    ~RegistrationWidget();

private:
    void SetChatWindow();
    void SaveUserInfo(int userId, const QString& userName);

private slots:
    void Register();
    void RegisterUserReply(QNetworkReply *rep);
private:
     Ui::RegistrationUI *ui;
     MainWindow* m_mainWindow;
     std::shared_ptr<HttpClient> m_httpClient;
};

#endif // REGISTRATION_H
