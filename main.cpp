#include "mainwindow.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    Q_INIT_RESOURCE(images);
    QApplication MyApplication(argc, argv);
    MainWindow MyMainWindow;
    MyMainWindow.show();
    return MyApplication.exec();
}
