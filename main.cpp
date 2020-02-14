#include <QApplication>
#include "FolderMonitorModel.h"
#include "FolderMonitorView.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FolderMonitorModel model;
    FolderMonitorView view(model);
    view.show();
    return a.exec();
}
