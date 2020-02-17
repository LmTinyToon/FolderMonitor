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
//  FolderInfo
    struct FolderInfo
    {
        //  Default constructor (to explicitly initialize members)
        FolderInfo(void) :
            files_count(0), subdirs_count(0), files_size(0), files_stats()
        {
        }

        //  Statistics for group files
        struct FileGroupStats
        {
            /*
                FileGroupStats constructor
                Params: files size, files count
            */
            FileGroupStats(const size_t _files_size = 0, const size_t _files_count = 0) :
                files_size(_files_size), files_count(_files_count)
            {
            }

            //  Members
            //  Files size
            size_t files_size;
            //  Files count
            size_t files_count;
        };

        //  Overall files count
        size_t files_count;
        //  Subdirectories count
        size_t subdirs_count;
        //  Overall files size
        size_t files_size;
        //  Files groups statistics
        QMap<QString, FileGroupStats> files_stats;
    };

//  Constructors/destructor
/*
        FolderMonitorModel constructor
        Params: none
*/
    FolderMonitorModel(void);

/*
        FolderMonitoModel destructor
*/
    ~FolderMonitorModel(void) override;

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
        Params: index, folder statistics
        Return: none
*/
    void statistics_update(const QModelIndex index, const FolderMonitorModel::FolderInfo folder_stats);

private:
//  Private classes
//      Folder root class forward declaration
    class FolderItem;
//      FolderInfoTask class forward declaration
    class FolderInfoTask;

/*
        Gets folder item from model index
        Params: index
        Return: folder item
*/
    FolderItem* get(const QModelIndex& index) const;

//      Folder root
    std::unique_ptr<FolderItem> m_folder_root;
//      Current task id
    volatile size_t m_current_task_id;
};
Q_DECLARE_METATYPE(FolderMonitorModel::FolderInfo)

#endif // FOLDERMONITORMODEL_H_INCLUDED__
