#include "popup.h"
#include "user_item.h"

#include <QPainter>
#include <QApplication>
#include <QScreen>
#include <QDebug>
#include <QHBoxLayout>

PopUp::PopUp(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint |
                   Qt::Tool |
                   Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_ShowWithoutActivating);

    animation.setTargetObject(this);
    animation.setPropertyName("popupOpacity");
    connect(&animation, &QAbstractAnimation::finished, this, &PopUp::hide);

    setStyleSheet("background-color: white;");
    setFixedSize(300, 70);
    setAutoFillBackground(true);
    setContentsMargins(0, 0, 0, 0);

    m_popup = new UserItemWidget();
    m_layout = new QHBoxLayout(this);
    m_layout->setContentsMargins(0, 0, 0, 0);

    m_layout->addWidget(m_popup);

    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &PopUp::hideAnimation);
}

void PopUp::setPopupText(const QString& name, const QString& text, int64_t unreadCount, const QDateTime& time)
{
    m_popup->SetLastTextTime(time);
    m_popup->SetUnreadCount(unreadCount);
    m_popup->SetName(name);
    m_popup->SetLastText(text);
    adjustSize();
}

void PopUp::show()
{
    setWindowOpacity(0.0);

    animation.setDuration(150);
    animation.setStartValue(0.0);
    animation.setEndValue(1.0);

    setGeometry(QApplication::primaryScreen()->availableGeometry().width() - 36 - width() + QApplication::primaryScreen() -> availableGeometry().x(),
                QApplication::primaryScreen()->availableGeometry().height() - 36 - height() + QApplication::primaryScreen() -> availableGeometry().y(),
                width(),
                height());
    QWidget::show();

    animation.start();
    timer->start(3000);
}

void PopUp::hideAnimation()
{
    timer->stop();
    animation.setDuration(1000);
    animation.setStartValue(1.0);
    animation.setEndValue(0.0);
    animation.start();
}

void PopUp::hide()
{
    if(getPopupOpacity() == 0.0){
        QWidget::hide();
    }
}

void PopUp::setPopupOpacity(float opacity)
{
    popupOpacity = opacity;

    setWindowOpacity(opacity);
}

float PopUp::getPopupOpacity() const
{
    return popupOpacity;
}
