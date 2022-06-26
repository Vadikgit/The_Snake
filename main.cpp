#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    //w.resize(w.app_weight, w.app_height);
    w.resize(w.app_size, w.app_size);

    w.setWindowTitle("The Snake");
    w.setWindowIcon(QIcon("snake2.png"));
    //w.setWindowIcon(QIcon("snake1.png"));
    w.show();

    return a.exec();
}
