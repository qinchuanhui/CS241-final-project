
#include "mainwindow.h"
#include <QApplication>
#include<QString>
#include<QDateTime>
#include<QDebug>
#include<QWindow>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    MainWindow r;
    r.setWindowTitle("Metro Assistant");
    r.resize(1400,930);
    r.show();

    return a.exec();
}

