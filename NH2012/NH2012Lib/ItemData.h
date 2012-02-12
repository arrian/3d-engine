#pragma once

#include <OgreString.h>

#include "Attributes.h"

/*! Item data not related to graphical representation.*/
class ItemData
{
public:
  ItemData(void);
  virtual ~ItemData(void);

  friend bool operator==(const ItemData& x, const ItemData& y);
protected:
  /*! Gold value of the item.*/
  int value;

  /*! Item name.*/
  Ogre::String name;

  /*! Blessed/uncursed/cursed.*/
  ItemAttribute::BUC buc;

  /*! Quest/normal item.*/
  ItemAttribute::Status status;

  /*! eg. book,weapon etc.
      Maps directly to the items.dat file
  */
  ItemAttribute::Type type;
  
  /*! Item's 'special abilities'.*/
  std::vector<GeneralAttribute::Intrinsics> intrinsics;
};

