#include "unregister.h"

#include "mainwindow.h"

#include "./ui_unregister.h"

UnregisterWidget::UnregisterWidget(MainWindow *mainWindow, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::UnregisteredUI)
    , m_mainWindow(mainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::pressed, this, &UnregisterWidget::SetRegistrationWindow);
}

UnregisterWidget::~UnregisterWidget()
{
    qDebug() << "Hello";
}

void UnregisterWidget::SetRegistrationWindow()
{
    m_mainWindow->SetRegistrationWidget();
}
