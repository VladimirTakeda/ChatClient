#include "user_item.h"

#include "./ui_dialog_item.h"

UserItemWidget::UserItemWidget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::UserItemUI)
{
    ui->setupUi(this);
}

QString UserItemWidget::GetName() const
{
    return ui->label_4->text();
}

UserItemWidget::~UserItemWidget()
{

}

void UserItemWidget::SetName(const QString& name)
{
    ui->label_4->setText(name);
    ui->label_4->update();
}

void UserItemWidget::SetLastText(const QString& lastText)
{
    ui->label_3->setText(lastText);
    ui->label_3->update();
}

void UserItemWidget::SetUnreadCount(int count)
{
    ui->label_5->setText(QString::number(count));
    ui->label_5->setStyleSheet("background-color: gray; color: white; border-radius: 10px; padding: 5px;");
}
