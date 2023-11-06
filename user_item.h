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
    void SetLastTextTime(const QDateTime& lastTextLime);
    void IncrementUnreadCount();
    void ClearUnreadCount();
    ~UserItemWidget();

private:
    Ui::UserItemUI *ui;
};

#endif // USER_ITEM_H
