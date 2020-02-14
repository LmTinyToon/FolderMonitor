#include <QDirIterator>
#include <QLinkedList>
#include <QThread>
#include "FolderMonitorModel.h"

//  FolderMonitorModel - implementation
//  FolderMonitorModel::FolderItem  - implementation
class FolderMonitorModel::FolderItem
{
public:
//  Constructors/destructor
/*
        Folder item constructor
        Params: parent item, item path, item name
*/
    FolderItem(FolderItem* parent, const QString& path, const QString& name) :
        m_parent(parent), m_path(path), m_name(name), m_modified(false)
    {
    }


//  Folder item destructor
    ~FolderItem(void)
    {
        for (size_t i = 0; i < m_children.size(); ++i)
        {
            delete m_children[i];
        }
        m_children.clear();
    }

/*
        Name of item
        Params: none
        Return: name
*/
    const QString& local_name(void) const
    {
        return m_name;
    }

/*
        Path of item
        Params: none
        Return: name
*/
    const QString& get_path(void) const
    {
        return m_path;
    }

/*
        Count of children
        Params: none
        Return: count of children
*/
    size_t children_count(void)
    {
        ensure_state();
        return m_children.size();
    }

/*
        Adds child to item
        Params: child path, child name
        Return: child item
*/
    FolderItem* add_child(const QString& path, const QString& name)
    {
        FolderItem* const item = new FolderItem(this, path, name);
        m_children.push_back(item);
        m_modified = true;
        return item;
    }

/*
        Gets parent of folder item
        Params: none
        Return: folder item
*/
    FolderItem* parent(void) const
    {
        return m_parent;
    }

/*
        Gets index of item
        Params: child item
        Return: index
*/
    size_t index(const FolderItem* const item) const
    {
        return std::distance(m_children.begin(), std::find(m_children.begin(), m_children.end(), item));
    }

/*
        Gets item at specified index
        Params: item index
        Return: item
*/
    FolderItem* at(const size_t index) const
    {
        return m_children.at(index);
    }

private:
//  Private methods
/*
        Ensures state of children
        Params: none
        Return: none
*/
    void ensure_state(void)
    {
        if (m_modified)
            return;
        m_modified = true;
        QDirIterator it(get_path(), QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
        while (it.hasNext())
        {
            it.next();
            this->add_child(it.filePath(), it.fileName());
        }
    }

//  Members
//      Mofidication flag (should we syncronize children or not)
    bool m_modified;
//      Children collection
    std::vector<FolderItem*> m_children;
//      Parent item
    FolderItem* m_parent;
//      Item path
    QString m_path;
//      Item name
    QString m_name;
};

//  FolderMonitorModel::FolderInfoWorkerThread implementation
class FolderMonitorModel::FolderInfoWorkerThread : public QThread
{
    Q_OBJECT
public:
/*
        Data structure to hold job for worker
*/
    struct Job
    {
        //  Query kind
        enum class Query
        {
            CalcInfo,   //  New job
            Exit        //  Termination
        };

        /*
            Job constructor
            Params: query, index
        */
        Job(const Query _query, const QModelIndex& _index) :
            query(_query), index(_index)
        {
        }

        //  Public members
        //      Query
        const Query query;
        //      Index
        const QModelIndex index;
    };

    FolderInfoWorkerThread(FolderMonitorModel& model) :
        m_model(model)
    {
    }

    void run() override
    {
        for (;;)
        {
            Job job;
            if (!peek_job(job))
            {
                sleep(3);
                continue;
            }
            if (job.query == Job::Query::Exit)
                return;
            process(m_model.get(job.index)->get_path());
        }
    }
private:
    void process(const QString& path)
    {
        QDirIterator it1(path, QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
        while (it1.hasNext())
        {
            it1.next();
            update_file_info(it1.fileInfo());
        }
        QDirIterator it2(path, QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
        while (it2.hasNext())
        {
            it2.next();
            process(it2.filePath());
        }
        if (m_size - m_cached_size > 1024 * 1024 * 100)
        {
            m_cached_size = m_size;
            emit m_model.statistics_update(m_index, m_file_stats, m_size);
        }
    }
    void update_file_info(const QFileInfo& file_info)
    {
        const QString& suffix = file_info.suffix().toLower();
        const size_t size = file_info.size();
        auto iterator = m_file_stats.find(suffix);
        if (iterator == m_file_stats.end())
            iterator = m_file_stats.insert(suffix, 0);
        iterator.value() += size;
        m_size += size;
    }

    bool peek_job(Job& job)
    {
        job_mutex.lock();
        bool result = !m_jobs.empty();
        while (!m_jobs.empty())
        {
            if ((job = m_jobs.first()).query == Job::Query::Exit)
                break;
            m_jobs.removeFirst();
        }
        job_mutex.unlock();
        return result;
    }

    static QMutex job_mutex;
    QLinkedList<Job> m_jobs;
    QModelIndex m_index;
    QMap<QString, size_t> m_file_stats;
    size_t m_size;
    size_t m_cached_size;
    FolderMonitorModel& m_model;
};

//      FolderMonitorModel - Constructors/destructor
FolderMonitorModel::FolderMonitorModel() :
    m_folder_root(nullptr)
{
    m_folder_root = std::make_unique<FolderItem>(nullptr, "", "");
    for (const auto& item : QDir::drives())
    {
        m_folder_root->add_child(item.path(), item.path());
    }
}

//      FolderMonitorModel - public methods
QModelIndex FolderMonitorModel::index(int row, int column, const QModelIndex& parent) const
{
    return createIndex(row, column, get(parent)->at(row));
}

int FolderMonitorModel::rowCount(const QModelIndex& parent) const
{
    return static_cast<int>(get(parent)->children_count());
}

int FolderMonitorModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return 1;
}

QModelIndex FolderMonitorModel::parent(const QModelIndex& index) const
{
    if (!index.isValid() || get(index)->parent() == m_folder_root.get())
        return QModelIndex();
    FolderItem* item = get(index);
    FolderItem* parent_item = item->parent();
    return createIndex(static_cast<int>(parent_item->parent()->index(parent_item)), 0, parent_item);
}

QVariant FolderMonitorModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();
    return QVariant(get(index)->local_name());
}

//  FolderMonitorModel - private methods
FolderMonitorModel::FolderItem* FolderMonitorModel::get(const QModelIndex& index) const
{
    if (!index.isValid())
        return m_folder_root.get();
    return static_cast<FolderItem*>(index.internalPointer());
}

void FolderMonitorModel::request_statistics(const QModelIndex& index)
{
    QThreadPool::globalInstance()->start(new FolderStatsTask(*this, index));
}
