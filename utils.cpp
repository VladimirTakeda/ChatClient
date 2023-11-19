#include "utils.h"

#include <QSettings>
#include <QApplication>

int getCurrUserId(){
    QSettings settings(QApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat);
    qDebug() << settings.fileName();
    return settings.value("userId").toInt();
}

QString getCurrDeviceId(){
    QSettings settings(QApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat);
    qDebug() << settings.fileName();
    return settings.value("deviceId").toString();
}

QString getCurrUserName(){
    QSettings settings(QApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat);
    qDebug() << settings.fileName();
    return settings.value("currUserName").toString();
}
