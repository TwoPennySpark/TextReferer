#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("windows-1251")); //изменения
    setlocale(LC_ALL, "rus");

    MainWindow w;
    w.show();

    return a.exec();
}
