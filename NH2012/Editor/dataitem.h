#pragma once

#include <QList>
#include <QVariant>

class DataItem
{
public:
  DataItem(const QList<QVariant> &data, DataItem *parent = 0);
  ~DataItem();

  void appendChild(DataItem *child);

  DataItem *child(int row);
  int childCount() const;
  int columnCount() const;
  QVariant data(int column) const;
  int row() const;
  DataItem *parent();

private:
  QList<DataItem*> childItems;
  QList<QVariant> itemData;
  DataItem *parentItem;
};

