#include <QDirIterator>
#include <QThreadPool>
#include "FolderMonitorModel.h"

//  FolderMonitorModel - implementation
//  FolderMonitorModel::FolderItem  - implementation
class FolderMonitorModel::FolderItem
{
public:
    FolderItem(FolderItem* parent, const QString& path, const QString& name) :
        m_parent(parent), m_path(path), m_name(name)
    {
    }

    QString name(void) const
    {
        return m_name;
    }

    size_t children_count(void) const
    {
        return m_children.size();
    }

    FolderItem* add_child(const QString& path, const QString& name)
    {
        FolderItem* const item = new FolderItem(this, path, name);
        m_children.push_back(item);
        return item;
    }

    void expand_childs(void)
    {
        QDirIterator it(get_path(), QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
        while (it.hasNext())
        {
            it.next();
            this->add_child(it.filePath(), it.fileName());
        }
    }

    QString get_path(void)
    {
        return m_path;
    }

    FolderItem* parent(void) const
    {
        return m_parent;
    }
    size_t index(const FolderItem* const item) const
    {
        return std::distance(m_children.begin(), std::find(m_children.begin(), m_children.end(), item));
    }

    FolderItem* at(const size_t index) const
    {
        return m_children.at(index);
    }

private:
    std::vector<FolderItem*> m_children;
    FolderItem* m_parent;
    QString m_path;
    QString m_name;
};

//  FolderMonitorModel::FolderStatsTask implementation
class FolderMonitorModel::FolderStatsTask : public QRunnable
{
public:
    FolderStatsTask(FolderMonitorModel& model, const QModelIndex& index) :
        m_index(index), m_file_stats(), m_size(), m_model(model)
    {
    }

    void run() override
    {
        process(m_model.get(m_index)->get_path());
        emit m_model.statistics_ready(m_index, m_file_stats, m_size);
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
            emit m_model.statistics_ready(m_index, m_file_stats, m_size);
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
    m_folder_root = new FolderItem(nullptr, "", "");
    for (const auto& item : QDir::drives())
    {
        m_folder_root->add_child(item.path(), item.path());
    }
}

 FolderMonitorModel::~FolderMonitorModel()
 {

 }

//      FolderMonitorModel - public methods
QModelIndex FolderMonitorModel::index(int row, int column, const QModelIndex& parent) const
{
    return createIndex(row, column, get(parent)->at(row));
}

int FolderMonitorModel::rowCount(const QModelIndex& parent) const
{
    if (get(parent)->children_count() == 0)
    {
        get(parent)->expand_childs();
    }
    return get(parent)->children_count();
}

int FolderMonitorModel::columnCount(const QModelIndex& parent) const
{
    return 1;
    //return get(parent)->children_count() != 0 ? 1 : 0;
}

QModelIndex FolderMonitorModel::parent(const QModelIndex& index) const
{
    if (!index.isValid() || get(index)->parent() == m_folder_root)
        return QModelIndex();
    FolderItem* item = get(index);
    FolderItem* parent_item = item->parent();
    return createIndex(parent_item->parent()->index(parent_item), 0, parent_item);
}

QVariant FolderMonitorModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();
    return QVariant(get(index)->name());
}

QVariant FolderMonitorModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return QVariant("HEADER");

    return QVariant();
}

//  FolderMonitorModel - private methods
FolderMonitorModel::FolderItem* FolderMonitorModel::get(const QModelIndex& index) const
{
    if (!index.isValid())
        return m_folder_root;
    return static_cast<FolderItem*>(index.internalPointer());
}

void FolderMonitorModel::request_statistics(const QModelIndex& index)
{
    QThreadPool::globalInstance()->start(new FolderStatsTask(*this, index));
}
