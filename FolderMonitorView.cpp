#include <QTreeView>
#include <QListWidget>
#include <QHBoxLayout>
#include "FolderMonitorView.h"
#include "FolderMonitorModel.h"
#include "FolderMonitorController.h"

//  FolderMonitorView - implementation
//      FolderMonitorView - constructors/destructors
FolderMonitorView::FolderMonitorView(FolderMonitorController& controller,
                                     FolderMonitorModel& model)
    : QWidget(), m_controller(controller), m_model(model)
{
    QBoxLayout* const layout = new QHBoxLayout();
    QTreeView* const folders_tree = new QTreeView();
    folders_tree->setHeaderHidden(true);
    folders_tree->setModel(&model);

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
    layout->addWidget(folders_tree);

    m_folder_info = new QListWidget();
    layout->addWidget(m_folder_info);
    this->setLayout(layout);
}

QListWidget& FolderMonitorView::fodler_info_view(void)
{
    return *m_folder_info;
}

FolderMonitorView::~FolderMonitorView()
{

}
