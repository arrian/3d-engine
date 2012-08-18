#ifndef DATAWRAPPER_H
#define DATAWRAPPER_H


#include <QAbstractItemModel>
#include "dataitem.h"
#include "../Engine/DataManager.h"

class DataModel : public QAbstractItemModel
{
  Q_OBJECT

 public:
     DataModel(DataManager* dataManager, QObject *parent = 0);
     ~DataModel();

     QVariant data(const QModelIndex &index, int role) const;
     Qt::ItemFlags flags(const QModelIndex &index) const;
     QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
     QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
     QModelIndex parent(const QModelIndex &index) const;
     int rowCount(const QModelIndex &parent = QModelIndex()) const;
     int columnCount(const QModelIndex &parent = QModelIndex()) const;
 private:
     void setupModelData(const QStringList &lines, DataItem *parent);

     DataItem *rootItem;
};

#endif // DATAWRAPPER_H
