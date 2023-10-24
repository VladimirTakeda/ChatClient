#include "registration.h"
#include "mainwindow.h"

#include <QNetworkAccessManager>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkReply>

#include "./ui_registration.h"

RegistrationWidget::RegistrationWidget(MainWindow* mainWindow, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RegistrationUI)
    , m_mainWindow(mainWindow)
    , m_networkMgr(new QNetworkAccessManager(this))
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::pressed, this, &RegistrationWidget::Register);
    connect(m_networkMgr, &QNetworkAccessManager::finished,this, &RegistrationWidget::ReplyFinished);
}

RegistrationWidget::~RegistrationWidget()
{
}


void RegistrationWidget::Register()
{
    QNetworkRequest request;

    QJsonObject obj;
    obj["login"] = ui->lineEdit_2->text();
    obj["password"] = ui->lineEdit_2->text();
    QJsonDocument doc(obj);
    QByteArray data = doc.toJson();

    QUrl url;

    url.setScheme("http");
    url.setHost("localhost");
    url.setPath("/user/register");
    url.setPort(8080);
    request.setUrl(url);
    request.setRawHeader("Content-Type", "application/json");
    m_networkMgr->post(request, data);
}

void RegistrationWidget::ReplyFinished(QNetworkReply *reply){
    if (reply->error() == QNetworkReply::NoError) {
        qDebug() << "Success" << reply->readAll();
        SetChatWindow();
    }
    else {
        qDebug() << "Failure" <<reply->errorString();
    }
}

void RegistrationWidget::SetChatWindow()
{
    m_mainWindow->SetChatWidget();
}
