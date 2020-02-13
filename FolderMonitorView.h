#ifndef FOLDERMONITORVIEW_H_INCLUDED__
#define FOLDERMONITORVIEW_H_INCLUDED__

//  Standard includes
#include <QMainWindow>

//  FolderMonitorModel forward declaration
class FolderMonitorModel;
//  FolderMonitorController;
class FolderMonitorController;

//  FolderMonitorView - main view of application
class FolderMonitorView final : public QMainWindow
{
    Q_OBJECT
public:
//  Constructors/destructor
/*
        FolderMonitorView constructor
        Params: controller, model
*/
    FolderMonitorView(FolderMonitorController& controller,FolderMonitorModel& model);

/*
        FolderMonitorView destructor
*/
    ~FolderMonitorView();

signals:
/*
        Folder selection was changed
        Params: model index
        Return: none
*/
    void folder_selection_changed(const QModelIndex& index);
private:
//  Members
//      Controller of view
    FolderMonitorController& m_controller;
//      Model of view
    FolderMonitorModel& m_model;
};

#endif // FOLDERMONITORVIEW_H_INCLUDED__
