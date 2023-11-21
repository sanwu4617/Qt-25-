#include "mainwindow.h"

#include <QApplication>
#include "Event.h"

int main(int argc, char *argv[])
{
    setEventData();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
