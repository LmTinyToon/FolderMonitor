#ifndef FOLDERMONITORMODEL_H_INCLUDED__
#define FOLDERMONITORMODEL_H_INCLUDED__

//  Standard includes
#include <QAbstractItemModel>

//  FolderMonitorModel - model to represent file system folders
class FolderMonitorModel final : public QAbstractItemModel
{
public:
//  Constructors/destructor
/*
        FolderMonitorModel constructor
        Params: none
*/
    FolderMonitorModel();

//  Public methods
/*
    TODO: (alex) add comment
*/
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const final override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const final override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const final override;
    QModelIndex parent(const QModelIndex& index) const final override;
    QVariant data(const QModelIndex& index, int role) const final override;

/*    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QModelIndex parent(const QModelIndex& index) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
*/
private:
//  Private classes
//      Folder root class forward declaration
    class FolderItem;
//      Item type typedef
    typedef FolderItem* item_type;

/*
        Gets folder item from model index
        Params: index
        Return: folder item
*/
    FolderItem* get(const QModelIndex& index) const;

//      Folder root
    FolderItem* m_folder_root;
};

#endif // FOLDERMONITORMODEL_H_INCLUDED__
