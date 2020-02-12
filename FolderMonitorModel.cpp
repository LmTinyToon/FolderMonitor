#include "FolderMonitorModel.h"

//  FolderMonitorModel - implementation
//  FolderMonitorModel::FolderItem  - implementation
class FolderMonitorModel::FolderItem
{
public:
    FolderItem(FolderItem* parent, const QString& str) :
        m_parent(parent), m_str(str)
    {
    }

    const QString& str(void) const
    {
        return m_str;
    }

    size_t children_count(void) const
    {
        return m_children.size();
    }

    void add_child(FolderItem* const child)
    {
        m_children.push_back(child);
    }

    FolderItem* parent(void)
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
    QString m_str;
};

//      FolderMonitorModel - Constructors/destructor
FolderMonitorModel::FolderMonitorModel()
{
    m_folder_root = new FolderItem(nullptr, "Root");
    m_folder_root->add_child(new FolderItem(m_folder_root, "Item1"));
    m_folder_root->add_child(new FolderItem(m_folder_root, "Item2"));
}

//      FolderMonitorModel - public methods
QModelIndex FolderMonitorModel::index(int row, int column, const QModelIndex& parent) const
{
    FolderItem* item = m_folder_root;
    if (parent.isValid())
        item = static_cast<FolderItem*>(parent.internalPointer())->at(row);
    return createIndex(row, column, item);
}

int FolderMonitorModel::rowCount(const QModelIndex& parent) const
{
    const FolderItem* item = m_folder_root;
    if (parent.isValid())
        item = static_cast<const FolderItem*>(parent.internalPointer());
    return item->children_count();
}

int FolderMonitorModel::columnCount(const QModelIndex& parent) const
{
    return 1;
}

QModelIndex FolderMonitorModel::parent(const QModelIndex& index) const
{
    if (!index.isValid())
        return QModelIndex();

    FolderItem* const child_item = static_cast<FolderItem*>(index.internalPointer());
    FolderItem* const parent_item = child_item->parent();

    if (parent_item == nullptr)
        return QModelIndex();

    return createIndex(parent_item->parent()->index(parent_item), 0, parent_item);
}

QVariant FolderMonitorModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();
    return QVariant(static_cast<FolderItem*>(index.internalPointer())->str());
}
