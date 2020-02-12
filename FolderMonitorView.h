#ifndef FOLDERMONITORVIEW_H_INCLUDED__
#define FOLDERMONITORVIEW_H_INCLUDED__

//  Standard includes
#include <QMainWindow>

//  FolderMonitorModel forward declaration
class FolderMonitorModel;

//  FolderMonitorView - main view of application
class FolderMonitorView final : public QMainWindow
{
    Q_OBJECT
public:
//  Constructors/destructor
/*
        FolderMonitorView constructor
        Params: model
*/
    FolderMonitorView(FolderMonitorModel& model);

/*
        FolderMonitorView destructor
*/
    ~FolderMonitorView();
private:
    FolderMonitorModel& m_model;
};

#endif // FOLDERMONITORVIEW_H_INCLUDED__
