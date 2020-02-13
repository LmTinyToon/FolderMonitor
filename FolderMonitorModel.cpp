#include <QDirIterator>
#include "FolderMonitorModel.h"

//  FolderMonitorModel - implementation
int FolderStatusModel::rowCount(const QModelIndex &parent) const
{
    return 2;
}

QVariant FolderStatusModel::data(const QModelIndex &index, int role) const
{
    return QVariant("test field");
}
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

//      FolderMonitorModel - Constructors/destructor
FolderMonitorModel::FolderMonitorModel() :
    m_folder_root(nullptr), m_folder_status_model()
{
    m_folder_root = new FolderItem(nullptr, "", "");
    for (const auto& item : QDir::drives())
    {
        m_folder_root->add_child(item.path(), item.path());
    }
}

//      FolderMonitorModel - public methods
FolderStatusModel& FolderMonitorModel::get_submodel(void)
{
    return m_folder_status_model;
}
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
