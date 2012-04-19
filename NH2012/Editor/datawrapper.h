#ifndef DATAWRAPPER_H
#define DATAWRAPPER_H

#include <QAbstractItemModel>

class DataWrapper : public QAbstractItemModel
{
  Q_OBJECT

 public:
     DataWrapper(const QString &data, QObject *parent = 0);
     ~DataWrapper();

     /*
     QVariant data(const QModelIndex &index, int role) const;
     Qt::ItemFlags flags(const QModelIndex &index) const;
     QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
     QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
     QModelIndex parent(const QModelIndex &index) const;
     int rowCount(const QModelIndex &parent = QModelIndex()) const;
     int columnCount(const QModelIndex &parent = QModelIndex()) const;
     */
 private:
     //void setupModelData(const QStringList &lines, TreeItem *parent);

     //TreeItem *rootItem;
};

#endif // DATAWRAPPER_H
