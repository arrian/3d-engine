#include "datawrapper.h"

DataWrapper::DataWrapper(const QString &data, QObject *parent)
  : QAbstractItemModel(parent)
{
  //QList<QVariant> rootData;
  //rootData << "Title" << "Summary";
  //rootItem = new DataWrapper(rootData);
}

DataWrapper::~DataWrapper()
{
  //delete rootItem;
}
