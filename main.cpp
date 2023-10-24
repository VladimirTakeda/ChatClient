#include "mainwindow.h"

#include <QApplication>
#include <QHBoxLayout>
#include <QSettings>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.resize(1158, 757);
    QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(w.sizePolicy().hasHeightForWidth());
    w.setSizePolicy(sizePolicy);

    w.SetCentralWidet();
    w.show();
    return a.exec();
}
