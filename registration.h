#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class RegistrationUI; }
QT_END_NAMESPACE

class MainWindow;
class QNetworkAccessManager;
class QNetworkReply;

class RegistrationWidget : public QWidget
{
    Q_OBJECT
public:
    RegistrationWidget(MainWindow* mainWindow, QWidget *parent = nullptr);
    ~RegistrationWidget();

private:
    void SetChatWindow();

private slots:
    void Register();
    void ReplyFinished(QNetworkReply *rep);
private:
    QNetworkAccessManager* m_networkMgr;
     Ui::RegistrationUI *ui;
     MainWindow* m_mainWindow;
};

#endif // REGISTRATION_H
