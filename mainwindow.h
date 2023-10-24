#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class ChatWidget;
class UnregisterWidget;
class RegistrationWidget;
class QStackedLayout;

class MainWindow : public QMainWindow, std::enable_shared_from_this<MainWindow>
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void SetCentralWidet();
    void SetRegistrationWidget();
    void SetChatWidget();
    ~MainWindow();

private:
    ChatWidget* m_chatWidget;
    UnregisterWidget* m_unregisterWidget;
    RegistrationWidget* m_registrationWidget;
    QStackedLayout* m_layout;
};
#endif // MAINWINDOW_H
