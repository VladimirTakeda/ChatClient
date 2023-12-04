#pragma once

#include <QWidget>
#include <QLabel>
#include <QPropertyAnimation>
#include <QTimer>


class QHBoxLayout;
class UserItemWidget;

class PopUp : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(float popupOpacity READ getPopupOpacity WRITE setPopupOpacity)

    void setPopupOpacity(float opacity);
    float getPopupOpacity() const;

public:
    explicit PopUp(QWidget *parent = 0);

public slots:
    void setPopupText(const QString& name, const QString& text, int64_t unreadCount, const QDateTime& time);
    void show();

private slots:
    void hideAnimation();
    void hide();

private:
    UserItemWidget* m_popup;
    QHBoxLayout* m_layout;
    QPropertyAnimation animation;
    float popupOpacity;
    QTimer *timer;
};
