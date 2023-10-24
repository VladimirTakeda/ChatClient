#include "mainwindow.h"
#include "chat.h"
#include "unregister.h"
#include "registration.h"

#include <QSettings>
#include <QStackedLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget* centralWidget = new QWidget( this );
    this->setCentralWidget( centralWidget );

    m_layout = new QStackedLayout( centralWidget );

    m_chatWidget = new ChatWidget(centralWidget);
    m_unregisterWidget = new UnregisterWidget(this, centralWidget);
    m_registrationWidget = new RegistrationWidget(this, centralWidget);

    m_layout->addWidget( m_chatWidget );
    m_layout->addWidget( m_unregisterWidget );
    m_layout->addWidget( m_registrationWidget );
}

void MainWindow::SetCentralWidet()
{
    QSettings settings("MySoft", "Star Runner");

    bool IsRegistered = settings.value("registered", false).toBool();

    QWidget *widget = nullptr;
    bool type = true;
    if (IsRegistered){
        m_layout->setCurrentWidget(m_chatWidget);
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
}

MainWindow::~MainWindow()
{
}

