#include "FolderMonitorController.h"

//  FolderMonitorController - implementation
FolderMonitorController::FolderMonitorController(void) :
    m_model(), m_view(*this, m_model)
{
    QObject::connect(&m_view, &FolderMonitorView::folder_selection_changed,
                     this, &FolderMonitorController::on_folder_selection_change);
}
//      FolderMonitorController - methods
void FolderMonitorController::run(void)
{
    m_view.show();
}

void FolderMonitorController::on_folder_selection_change(const QModelIndex& index)
{
    int k = 0;
    k = k + 1;
}
