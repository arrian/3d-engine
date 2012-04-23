#include "dataitem.h"

#include <QStringList>

DataItem::DataItem(const QList<QVariant> &data, DataItem *parent)
{
  parentItem = parent;
  itemData = data;
}

DataItem::~DataItem()
{
  qDeleteAll(childItems);
}

void DataItem::appendChild(DataItem *item)
{
  childItems.append(item);
}

DataItem *DataItem::child(int row)
{
  return childItems.value(row);
}

int DataItem::childCount() const
{
  return childItems.count();
}

int DataItem::columnCount() const
{
  return itemData.count();
}

QVariant DataItem::data(int column) const
{
  return itemData.value(column);
}

DataItem *DataItem::parent()
{
  return parentItem;
}

int DataItem::row() const
{
  if (parentItem) return parentItem->childItems.indexOf(const_cast<DataItem*>(this));

  return 0;
}

