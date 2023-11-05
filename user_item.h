#ifndef USER_ITEM_H
#define USER_ITEM_H

#include <QWidget>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui { class UserItemUI; }
QT_END_NAMESPACE

class UserItemWidget : public QWidget
{
    Q_OBJECT
public:
    UserItemWidget(QWidget *parent = nullptr);
    QString GetName() const;
    void SetName(const QString& name);
    void SetLastText(const QString& lastText);
    void SetUnreadCount(int count);
    ~UserItemWidget();

private:
    Ui::UserItemUI *ui;
};

#endif // USER_ITEM_H
