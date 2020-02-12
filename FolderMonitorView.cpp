#include <QTreeView>
#include "FolderMonitorView.h"
#include "FolderMonitorModel.h"

//  FolderMonitorView - implementation
//      FolderMonitorView - constructors/destructors
FolderMonitorView::FolderMonitorView(FolderMonitorModel& model)
    : QMainWindow(), m_model(model)
{
    QTreeView* const folders_tree = new QTreeView(this);
    folders_tree->setModel(&model);
}

FolderMonitorView::~FolderMonitorView()
{

}
