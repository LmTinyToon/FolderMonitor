#include <QTreeView>
#include <QListWidget>
#include <QHBoxLayout>
#include "FolderMonitorView.h"
#include "FolderMonitorModel.h"

//  FolderMonitorView - implementation
//      FolderMonitorView - constructors/destructors
FolderMonitorView::FolderMonitorView(FolderMonitorModel& model)
    : QWidget(), m_model(model)
{
    QBoxLayout* const layout = new QHBoxLayout();
    QTreeView* const folders_view = new QTreeView();
    folders_view->setHeaderHidden(true);
    folders_view->setModel(&model);
    layout->addWidget(folders_view);

    QListWidget* const folder_stats_view = new QListWidget();
    layout->addWidget(folder_stats_view);

    QObject::connect(folders_view->selectionModel(), &QItemSelectionModel::currentChanged,
        this, [=](const QModelIndex &current, const QModelIndex& previous)
        {
            Q_UNUSED(previous)
            folder_stats_view->clear();
            folder_stats_view->addItem("Not computed yet");
            m_model.request_statistics(current);
        });


    QObject::connect(&m_model, &FolderMonitorModel::statistics_update,
        this, [=](const QModelIndex index, const FolderMonitorModel::FolderInfo folder_stats)
        {
            if (folders_view->currentIndex() != index)
                return;
            folder_stats_view->clear();
            folder_stats_view->addItem(QString::number(folder_stats.files_count) + " items");
            folder_stats_view->addItem("Totalling " + QString::number(folder_stats.files_size) + " bytes");
            folder_stats_view->addItem(QString::number(folder_stats.subdirs_count) + " subdirectories");
            for (auto it = folder_stats.files_stats.cbegin(); it != folder_stats.files_stats.cend(); ++it)
            {
                folder_stats_view->addItem("\"" + it.key() + "\" : " + QString::number(it.value().files_size / it.value().files_count));
            }
        },
        Qt::ConnectionType::QueuedConnection);


    this->setLayout(layout);
}
