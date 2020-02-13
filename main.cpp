#include <QApplication>
#include "FolderMonitorController.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FolderMonitorController controller;
    controller.run();
    return a.exec();
}
