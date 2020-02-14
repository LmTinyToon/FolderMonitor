#ifndef FOLDERMONITORVIEW_H_INCLUDED__
#define FOLDERMONITORVIEW_H_INCLUDED__

//  Standard includes
#include <QMainWindow>

//  FolderMonitorModel forward declaration
class FolderMonitorModel;

//  FolderMonitorView - main view of application
class FolderMonitorView final : public QWidget
{
    Q_OBJECT
public:
//  Constructors/destructor
/*
        FolderMonitorView constructor
        Params: model
*/
    FolderMonitorView(FolderMonitorModel& model);

private:
//  Members
//      Model of view
    FolderMonitorModel& m_model;
};

#endif // FOLDERMONITORVIEW_H_INCLUDED__
