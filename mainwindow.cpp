#include "mainwindow.h"
#include "chat.h"
#include "unregister.h"
#include "registration.h"
#include "network/httpclient.h"

#include <QSettings>
#include <QStackedLayout>
#include <QApplication>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    m_httpClient(new HttpClient(this))
{
    QWidget* centralWidget = new QWidget( this );
    this->setCentralWidget( centralWidget );

    m_layout = new QStackedLayout( centralWidget );

    m_chatWidget = new ChatWidget(m_httpClient, centralWidget);
    m_unregisterWidget = new UnregisterWidget(this, centralWidget);
    m_registrationWidget = new RegistrationWidget(m_httpClient, this, centralWidget);

    m_layout->addWidget( m_chatWidget );
    m_layout->addWidget( m_unregisterWidget );
    m_layout->addWidget( m_registrationWidget );
}

void MainWindow::SetCentralWidet()
{
    QSettings settings(QDir(QApplication::applicationDirPath()).filePath("settings.ini"));

    bool IsRegistered = settings.value("registered", false).toBool();

    QWidget *widget = nullptr;
    bool type = true;
    if (IsRegistered){
        SetChatWidget();
    }
    else{
        m_layout->setCurrentWidget(m_unregisterWidget);
    }
}

void MainWindow::SetRegistrationWidget(){
    m_layout->setCurrentWidget(m_registrationWidget);
}

void MainWindow::SetChatWidget(){
    m_layout->setCurrentWidget(m_chatWidget);
    m_chatWidget->SetUpWSConnection();
}

MainWindow::~MainWindow()
{
}

