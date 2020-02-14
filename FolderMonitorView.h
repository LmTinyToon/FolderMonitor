#ifndef FOLDERMONITORVIEW_H_INCLUDED__
#define FOLDERMONITORVIEW_H_INCLUDED__

//  Standard includes
#include <QMainWindow>
#include <QListWidget>
#include <QTreeView>

//  FolderMonitorModel forward declaration
class FolderMonitorModel;
//  FolderMonitorController;
class FolderMonitorController;

//  FolderMonitorView - main view of application
class FolderMonitorView final : public QWidget
{
    Q_OBJECT
public:
//  Constructors/destructor
/*
        FolderMonitorView constructor
        Params: controller, model
*/
    FolderMonitorView(FolderMonitorController& controller,FolderMonitorModel& model);

    QListWidget& fodler_info_view(void);

signals:
/*
        Folder selection was changed
        Params: model index
        Return: none
*/
    void folder_selection_changed(const QModelIndex& index);
private slots:
    void on_statistics_ready(const QModelIndex index, const QMap<QString, size_t> files, const size_t size);
private:
//  Members
//      Controller of view
    FolderMonitorController& m_controller;
//      Model of view
    FolderMonitorModel& m_model;

    QListWidget* m_folder_info;
    QTreeView* m_folders_view;
};

#endif // FOLDERMONITORVIEW_H_INCLUDED__
