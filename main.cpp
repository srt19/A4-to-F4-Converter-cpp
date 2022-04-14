#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("A4 to F4 Converter");
    w.setWindowIcon(QIcon(":/img/favicon.png"));
    w.show();
    return a.exec();
}
