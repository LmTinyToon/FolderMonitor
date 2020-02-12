#include <QTreeView>
#include <QVBoxLayout>
#include "FolderMonitorView.h"
#include "FolderMonitorModel.h"

//  FolderMonitorView - implementation
//      FolderMonitorView - constructors/destructors
FolderMonitorView::FolderMonitorView(FolderMonitorModel& model)
    : QMainWindow(), m_model(model)
{

    auto mainLayout = new QVBoxLayout();
    this->setLayout(mainLayout);
    QTreeView* const folders_tree = new QTreeView(this);
    folders_tree->setModel(&model);
    mainLayout->addWidget(folders_tree);
}

FolderMonitorView::~FolderMonitorView()
{

}
