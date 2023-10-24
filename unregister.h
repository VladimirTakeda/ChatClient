#ifndef UNREGISTER_H
#define UNREGISTER_H

#include <QWidget>
#include <memory>

class MainWindow;

QT_BEGIN_NAMESPACE
namespace Ui { class UnregisteredUI; }
QT_END_NAMESPACE

class UnregisterWidget : public QWidget
{
    Q_OBJECT
public:
    UnregisterWidget(MainWindow* mainWindow, QWidget *parent = nullptr);
    ~UnregisterWidget();

private:
    Ui::UnregisteredUI *ui;
    MainWindow* m_mainWindow;

private slots:
    void SetRegistrationWindow();
};

#endif // UNREGISTER_H
