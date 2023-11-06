#include "user_item.h"

#include "./ui_dialog_item.h"

#include <QDateTime>

namespace {
    const QString timeFormat = "h:mm AP";
}

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

void UserItemWidget::SetLastTextTime(const QDateTime& lastTextLime)
{
    ui->label->setText(lastTextLime.toString(timeFormat));
}

void UserItemWidget::IncrementUnreadCount()
{
    if (ui->label_5->text().isEmpty())
        ui->label_5->setText(QString::number(1));
    else
        ui->label_5->setText(QString::number(ui->label_5->text().toInt() + 1));
    ui->label_5->setStyleSheet("background-color: gray; color: white; border-radius: 10px; padding: 5px;");
}

void UserItemWidget::ClearUnreadCount()
{
    ui->label_5->clear();
    ui->label_5->setStyleSheet("");
}
