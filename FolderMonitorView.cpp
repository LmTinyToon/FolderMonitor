#include <QTreeView>
#include <QVBoxLayout>
#include "FolderMonitorView.h"
#include "FolderMonitorModel.h"

//  FolderMonitorView - implementation
//      FolderMonitorView - constructors/destructors
FolderMonitorView::FolderMonitorView(FolderMonitorModel& model)
    : QMainWindow(), m_model(model)
{
    QTreeView* const folders_tree = new QTreeView();
    folders_tree->setHeaderHidden(true);
    folders_tree->setModel(&model);
    this->setCentralWidget(folders_tree);
}

FolderMonitorView::~FolderMonitorView()
{

}
