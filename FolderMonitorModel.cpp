#include <QDirIterator>
#include <QThreadPool>
#include "FolderMonitorModel.h"

//  Static constants
static size_t MEGABYTE = 1024 * 1024;

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
        m_modified(false), m_parent(parent), m_path(path), m_name(name)
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
        const QString& path = get_path();
        if (!QDir(path).exists())
            return;
        QDirIterator it(path, QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
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

//  FolderMonitorModel::FolderInfoTask
class FolderMonitorModel::FolderInfoTask final : public QRunnable
{
public:
//  Constructors/destructor
/*
        FolderInfoTask constructor
        Params: model, index
*/
    FolderInfoTask(FolderMonitorModel& model, const QModelIndex& index) :
        m_model(model), m_task_id(m_model.m_current_task_id),
        m_current_id(m_model.m_current_task_id),
        m_index(index), m_folder_info(), m_cached_size(0)
    {
    }

/*
        Runs task
        Params: none
        Return: none
*/
    void run() final override
    {
        process(m_model.get(m_index)->get_path());
        if (is_actual())
        {
            emit m_model.statistics_update(m_index, m_folder_info);
        }
    }
private:
//  Private methods
/*
        Checks, whether task is actual
        Params: none
        Return: check result
*/
    bool is_actual(void) const
    {
        return m_task_id == m_current_id;
    }

/*
        Collects statistics of folder specified by path
        Params: folder path
        Return: none
*/
    void process(const QString& path)
    {
        if (!QDir(path).exists())
            return;
        QDirIterator it1(path, QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
        while (it1.hasNext() && is_actual())
        {
            it1.next();
            update_file_info(it1.fileInfo());
        }
        QDirIterator it2(path, QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
        while (it2.hasNext() && is_actual())
        {
            it2.next();
            process(it2.filePath());
            m_folder_info.subdirs_count  += 1;
        }
        if (m_folder_info.files_size - m_cached_size > 100 * MEGABYTE && is_actual())
        {
            m_cached_size = m_folder_info.files_size;
            emit m_model.statistics_update(m_index, m_folder_info);
        }
    }

/*
        Updates file info statistics
        Params: file info
        Return: none
*/
    void update_file_info(const QFileInfo& file_info)
    {
        const QString& suffix = file_info.suffix().toLower();
        const size_t size = file_info.size();
        m_folder_info.files_size += size;
        m_folder_info.files_count += 1;
        auto iterator = m_folder_info.files_stats.find(suffix);
        if (iterator == m_folder_info.files_stats.end())
            iterator = m_folder_info.files_stats.insert(suffix, FolderInfo::FileGroupStats(0, 0));
        iterator.value().files_size += size;
        iterator.value().files_count += 1;
    }

//  Private members
//      Base model to notify
    FolderMonitorModel& m_model;
//      Current task id
    const size_t m_task_id;
//      Current model id
    volatile const size_t& m_current_id;
//      Job initiator index
    const QModelIndex m_index;
//      Folder statistics
    FolderInfo m_folder_info;
//      Cached size of folder
    size_t m_cached_size;
};

//      FolderMonitorModel - Constructors/destructor
FolderMonitorModel::FolderMonitorModel() :
    m_folder_root(nullptr), m_current_task_id(0)
{
    m_folder_root.reset(new FolderItem(nullptr, "", ""));
    for (const auto& item : QDir::drives())
    {
        m_folder_root->add_child(item.path(), item.path());
    }
}

FolderMonitorModel::~FolderMonitorModel(void)
{
    //  Minor tricky to optimize performance (stop thread before destructor of folder)
    ++m_current_task_id;
    if (QThreadPool::globalInstance()->activeThreadCount() != 0)
    {
        QThreadPool::globalInstance()->waitForDone();
    }
    m_folder_root.reset();
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
    ++m_current_task_id;
    QThreadPool::globalInstance()->start(new FolderInfoTask(*this, index));
}
