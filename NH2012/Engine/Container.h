#pragma once

#include <boost/shared_ptr.hpp>
#include <map>

#include "Id.h"

template<class T>
struct Container {
  typedef std::map< Id<T>, std::shared_ptr<T> > map;
  typedef T type;

  template<class V> bool isType()
  {
    return boost::is_same<V, T>::value;
  }

  std::shared_ptr<T> get(Id<T> id)
  {
    if(map.count(id) > 0) return map.find(id)->second;
    return NULL;
  }

  void insert(Id<T> id, std::shared_ptr<T> item)
  {
    map.insert(map.begin(), std::pair< Id<T>, std::shared_ptr<T> >(id, item));
  }

  void remove(Id<T> id)
  {
    if(map.count(id) > 0) map.erase(id);
  }

  int count()
  {
    return map.size();
  }
};


