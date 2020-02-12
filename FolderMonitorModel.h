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

/*    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& index) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
*/
};

#endif // FOLDERMONITORMODEL_H_INCLUDED__
