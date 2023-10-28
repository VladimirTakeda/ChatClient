#include "registration.h"
#include "mainwindow.h"
#include "network/httpclient.h"

#include <QNetworkAccessManager>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QSettings>
#include <QDir>

#include "./ui_registration.h"

RegistrationWidget::RegistrationWidget(std::shared_ptr<HttpClient> httpClient, MainWindow* mainWindow, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RegistrationUI)
    , m_mainWindow(mainWindow)
    , m_httpClient(std::move(httpClient))
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::pressed, this, &RegistrationWidget::Register);
}

RegistrationWidget::~RegistrationWidget()
{
}


void RegistrationWidget::Register()
{
    QNetworkRequest request;

    QJsonObject obj;
    obj["login"] = ui->lineEdit_3->text();
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

    m_httpClient->sendHttpRequest(std::move(request), std::move(data), std::bind(&RegistrationWidget::RegisterUserReply, this, std::placeholders::_1));
}

void RegistrationWidget::RegisterUserReply(QNetworkReply *reply){
    if (reply->error() == QNetworkReply::NoError) {
        QJsonDocument itemDoc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject rootObject = itemDoc.object();
        SaveUserId(rootObject.value("userId").toInt());
        SetChatWindow();
    }
    else {
        qDebug() << "Failure" <<reply->errorString();
    }
}

void RegistrationWidget::SaveUserId(int userId)
{
    QSettings settings(QDir(QApplication::applicationDirPath()).filePath("settings.ini"));
    settings.setValue("userId", userId);
}

void RegistrationWidget::SetChatWindow()
{
    m_mainWindow->SetChatWidget();
}
