#ifndef FOLDERINFORWORKERPRIVATE_H_INCLUDED__
#define FOLDERINFORWORKERPRIVATE_H_INCLUDED__

/*
    Note: (alex) that file is a workaround to implemented moc'ed FolderInforWorkerThread
    object within FolderMonitorModel.cpp. FolderInfoWorkerThread is "private" class.
*/

//  Standard includes
#include <QThread>
#include <QMutex>
#include <QDir>

//  Local includes
#include "FolderMonitorModel.h"

/*
        Worker to calculate information about folder in background.
        It can be managed to start new job or to be aborted.
*/
class FolderMonitorModel::FolderInfoWorkerThread : public QThread
{
    Q_OBJECT

public:
//  Constructors
/*
        FolderInfoWorkerThread constructor
        Params: model
*/
    FolderInfoWorkerThread(FolderMonitorModel& model);

//  Methods
/*
        Starts worker
        Params: none
        Return: none
*/
    void run() override;

/*
        Sets new job for worker
        Params: index
        Return: none
*/
    void set_new_index(const QModelIndex& index);

/*
        Stops thread
        Params: none
        Return: none
*/
    void stop();

private:
//  Private methods
/*
        Processes folder specified by path
        Params: index - job initiator, current folder path
        Return: none
*/
    void process(const QModelIndex& index, const QString& path);

/*
        Updates statistics from file info
        Params: file info
        Return: none
*/
    void update_file_info(const QFileInfo& file_info);

/*
        Checks, whether thread in "aborting" stage
        Params: none
        Return: check result
*/
    bool is_aborting(void) const;

/*
        Checks, whether new job is presented
        Params: none
        Return: check result
*/
    bool is_new_index_present(void) const;

/*
        Partially updates model
        Params: index - job initiator
        Return: none
*/
    void signal_update(const QModelIndex& index);

/*
        Clears statistics of folder
        Params: none
        Return: none
*/
    void clear();

//  Private members
//      Syncronizer for accesses to index
    static QMutex s_index_mutex;
//      Syncronizer for accesses to aborting flag
    static QMutex s_abort_mutex;
//      Flag, indicating aborting stage of thread
    bool m_is_aborting;
//      Job initiator index
    QModelIndex m_index;
//      Folder statistics
    FolderInfo m_folder_info;
//      Cached size of folder
    size_t m_cached_size;
//      Base model to notify
    FolderMonitorModel& m_model;
};

#endif // FOLDERINFORWORKERPRIVATE_H
