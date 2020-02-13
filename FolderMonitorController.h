#ifndef FOLDERMONITORCONTROLLER_H_INCLUDED__
#define FOLDERMONITORCONTROLLER_H_INCLUDED__

//  Standard includes
#include "FolderMonitorModel.h"
#include "FolderMonitorView.h"

//  FolderMonitorController - controller for FolderMonitorModel/FolderMonitorView
class FolderMonitorController final : public QObject
{
public:
//  Constructors/destructor
/*
        FolderMonitorController
        Params: none
*/
    FolderMonitorController(void);

//  Methods
/*
        Runs controller
        Params: none
        Return: none
*/
    void run(void);

private slots:
//  Methods
/*
        On folder selection change handler
        Params: model index
        Return: none
*/
    void on_folder_selection_change(const QModelIndex& index);
private:
//  Members
//      Model
    FolderMonitorModel m_model;
//      View
    FolderMonitorView m_view;
};

#endif // FOLDERMONITORCONTROLLER_H_INCLUDED__
