#ifndef FOLDERMONITORMODEL_H_INCLUDED__
#define FOLDERMONITORMODEL_H_INCLUDED__

//  Standard includes
#include <QAbstractItemModel>
#include <memory>

//  FolderMonitorModel - model to represent file system folders
class FolderMonitorModel final : public QAbstractItemModel
{
    Q_OBJECT
public:
//  Constructors/destructor
/*
        FolderMonitorModel constructor
        Params: none
*/
    FolderMonitorModel(void);

/*
        FolderMonitoModel destructor
*/
    ~FolderMonitorModel(void);

//  Public methods
/*
        Gets index from specified coordinates and parent index
        Params: row, column, parent index
        Return: index
*/
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const final override;

/*
        Gets row count of indices
        Params: parent index
        Return: row count
*/
    int rowCount(const QModelIndex& parent = QModelIndex()) const final override;

/*
        Gets column count of indices
        Params: parent index
        Return: column count
*/
    int columnCount(const QModelIndex& parent = QModelIndex()) const final override;

/*
        Gets parent index
        Params: index
        REturn: parent index
*/
    QModelIndex parent(const QModelIndex& index) const final override;

/*
        Gets data from index
        Params: index, role
        Return: data
*/
    QVariant data(const QModelIndex& index, int role) const final override;

/*
        Request statistics of specified model index
        Params: index
        Return: none
*/
    void request_statistics(const QModelIndex& index);
signals:
/*
        Statistic of model index update event
        Params: index, files info, overall size of model index
        Return: none
*/
    void statistics_update(const QModelIndex index, const QMap<QString, size_t> files, const size_t size);

private:
//  Private classes
//      Folder root class forward declaration
    class FolderItem;
//  Thread to perform collecting statistics of folder
    class FolderInfoWorkerThread;

/*
        Gets folder item from model index
        Params: index
        Return: folder item
*/
    FolderItem* get(const QModelIndex& index) const;

//      Folder root
    std::unique_ptr<FolderItem> m_folder_root;
//      Background worker
    std::unique_ptr<FolderInfoWorkerThread> m_worker;
};

#endif // FOLDERMONITORMODEL_H_INCLUDED__
