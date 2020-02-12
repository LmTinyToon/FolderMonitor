#include "FolderMonitorView.h"
#include "FolderMonitorModel.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FolderMonitorModel model;
    FolderMonitorView w(model);
    w.show();

    return a.exec();
}
