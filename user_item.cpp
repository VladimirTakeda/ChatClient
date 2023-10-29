#include "user_item.h"

#include "./ui_dialog_item.h"

UserItemWidget::UserItemWidget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::UserItemUI)
{
    ui->setupUi(this);
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
