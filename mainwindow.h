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
class HttpClient;

class MainWindow : public QMainWindow, std::enable_shared_from_this<MainWindow>
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void SetCentralWidet();
    void SetRegistrationWidget();
    void SetChatWidget();
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    void SaveMetaData();

private:
    std::shared_ptr<HttpClient> m_httpClient;
    ChatWidget* m_chatWidget;
    UnregisterWidget* m_unregisterWidget;
    RegistrationWidget* m_registrationWidget;
    QStackedLayout* m_layout;
};
#endif // MAINWINDOW_H
