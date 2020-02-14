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
    m_folders_view = new QTreeView();
    m_folders_view->setHeaderHidden(true);
    m_folders_view->setModel(&model);

    {
        //  selection changed signal connecting
        QItemSelectionModel* const selection_model = m_folders_view->selectionModel();
        void currentChanged(const QModelIndex &current, const QModelIndex &previous);
        QObject::connect(selection_model, &QItemSelectionModel::currentChanged,
            this, [=](const QModelIndex &current, const QModelIndex &previous)
            {
                Q_UNUSED(previous)
                emit this->folder_selection_changed(current);
            });
    }
    layout->addWidget(m_folders_view);

    m_folder_info = new QListWidget();
    layout->addWidget(m_folder_info);
    this->setLayout(layout);

    QObject::connect(&m_model, &FolderMonitorModel::statistics_ready,
                     this, &FolderMonitorView::on_statistics_ready,
                     Qt::ConnectionType::QueuedConnection);
}

QListWidget& FolderMonitorView::fodler_info_view(void)
{
    return *m_folder_info;
}

void FolderMonitorView::on_statistics_ready(const QModelIndex index, const QMap<QString, size_t> files, const size_t size)
{
    if (m_folders_view->currentIndex() != index)
        return;
    m_folder_info->clear();
    m_folder_info->addItem("Size : " + QString::number(size));
    for (auto it = files.cbegin(); it != files.cend(); ++it)
    {
        m_folder_info->addItem(it.key() + " : " + QString::number(it.value()));
    }
}
