#include <QTreeView>
#include <QVBoxLayout>
#include "FolderMonitorView.h"
#include "FolderMonitorModel.h"
#include "FolderMonitorController.h"

//  FolderMonitorView - implementation
//      FolderMonitorView - constructors/destructors
FolderMonitorView::FolderMonitorView(FolderMonitorController& controller,
                                     FolderMonitorModel& model)
    : QMainWindow(), m_controller(controller), m_model(model)
{
    QTreeView* const folders_tree = new QTreeView();
    folders_tree->setHeaderHidden(true);
    folders_tree->setModel(&model);
    this->setCentralWidget(folders_tree);
    {
        //  selection changed signal connecting
        QItemSelectionModel* const selection_model = folders_tree->selectionModel();
        void currentChanged(const QModelIndex &current, const QModelIndex &previous);
        QObject::connect(selection_model, &QItemSelectionModel::currentChanged,
            this, [=](const QModelIndex &current, const QModelIndex &previous)
            {
                Q_UNUSED(previous)
                emit this->folder_selection_changed(current);
            });
    }
}

FolderMonitorView::~FolderMonitorView()
{

}
