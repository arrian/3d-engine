#include "datamodel.h"

#include <QtGui>

DataModel::DataModel(const QString &data, QObject *parent)
  : QAbstractItemModel(parent)
{
  QList<QVariant> rootData;
  rootData << "Data";
  rootItem = new DataItem(rootData);

  QList<QVariant> columnData1;
  columnData1 << "test" << "more";
  QList<QVariant> columnData2;
  columnData2 << "test2" << "more2";
  QList<QVariant> columnData3;
  columnData3 << "test3" << "more3";
  QList<QVariant> columnData4;
  columnData4 << "test4" << "more4";

  DataItem* subNode = new DataItem(columnData1,rootItem);
  subNode->appendChild(new DataItem(columnData1,rootItem));

  rootItem->appendChild(subNode);
  rootItem->appendChild(new DataItem(columnData2,rootItem));
  rootItem->appendChild(new DataItem(columnData3,rootItem));
  rootItem->appendChild(new DataItem(columnData4,rootItem));
}

DataModel::~DataModel()
{
  delete rootItem;
}

int DataModel::columnCount(const QModelIndex &parent) const
{
  if (parent.isValid())
      return static_cast<DataItem*>(parent.internalPointer())->columnCount();
  else
      return rootItem->columnCount();
}

QVariant DataModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
      return QVariant();

  if (role != Qt::DisplayRole)
      return QVariant();

  DataItem *item = static_cast<DataItem*>(index.internalPointer());

  return item->data(index.column());
}

Qt::ItemFlags DataModel::flags(const QModelIndex &index) const
{
  if (!index.isValid())
      return 0;

  return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant DataModel::headerData(int section, Qt::Orientation orientation,
                            int role) const
{
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
      return rootItem->data(section);

  return QVariant();
}

QModelIndex DataModel::index(int row, int column, const QModelIndex &parent)
          const
{
  if (!hasIndex(row, column, parent))
      return QModelIndex();

  DataItem *parentItem;

  if (!parent.isValid())
      parentItem = rootItem;
  else
      parentItem = static_cast<DataItem*>(parent.internalPointer());

  DataItem *childItem = parentItem->child(row);
  if (childItem)
      return createIndex(row, column, childItem);
  else
      return QModelIndex();
}

QModelIndex DataModel::parent(const QModelIndex &index) const
{
  if (!index.isValid())
      return QModelIndex();

  DataItem *childItem = static_cast<DataItem*>(index.internalPointer());
  DataItem *parentItem = childItem->parent();

  if (parentItem == rootItem)
      return QModelIndex();

  return createIndex(parentItem->row(), 0, parentItem);
}

int DataModel::rowCount(const QModelIndex &parent) const
{
  DataItem *parentItem;
  if (parent.column() > 0)
      return 0;

  if (!parent.isValid())
      parentItem = rootItem;
  else
      parentItem = static_cast<DataItem*>(parent.internalPointer());

  return parentItem->childCount();
}

void DataModel::setupModelData(const QStringList &lines, DataItem *parent)
{
  QList<DataItem*> parents;
  QList<int> indentations;
  parents << parent;
  indentations << 0;

  int number = 0;

  while (number < lines.count()) {
      int position = 0;
      while (position < lines[number].length()) {
          if (lines[number].mid(position, 1) != " ")
              break;
          position++;
      }

      QString lineData = lines[number].mid(position).trimmed();

      if (!lineData.isEmpty()) {
          // Read the column data from the rest of the line.
          QStringList columnStrings = lineData.split("\t", QString::SkipEmptyParts);
          QList<QVariant> columnData;
          for (int column = 0; column < columnStrings.count(); ++column)
              columnData << columnStrings[column];

          if (position > indentations.last()) {
              // The last child of the current parent is now the new parent
              // unless the current parent has no children.

              if (parents.last()->childCount() > 0) {
                  parents << parents.last()->child(parents.last()->childCount()-1);
                  indentations << position;
              }
          } else {
              while (position < indentations.last() && parents.count() > 0) {
                  parents.pop_back();
                  indentations.pop_back();
              }
          }

          // Append a new item to the current parent's list of children.
          parents.last()->appendChild(new DataItem(columnData, parents.last()));
      }

      number++;
  }
}



