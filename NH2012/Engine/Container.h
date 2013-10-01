#pragma once

#include <boost/shared_ptr.hpp>

#include "Id.h"

template<class T>
struct Container {
  
  typedef T Type;
  typedef std::map<Id<T>, std::shared_ptr<T> > Map;
  typedef typename Map::iterator Iterator;

  Map map;

  template<class V> bool isType()
  {
    return boost::is_same<V, T>::value;
  }

  T* get(Id<T> id)
  {
    if(map.count(id) > 0) return map.find(id)->second.get();
    return NULL;
  }

  void insert(Id<T> id, std::shared_ptr<T> item)
  {
    std::cout << "Added " << id.getName() << std::endl;
    map.insert(map.begin(), std::pair< Id<T>, std::shared_ptr<T> >(id, item));
  }

  //Remove the specified object from the container. The caller becomes the new owner (or deleted if returned value ignored).
  std::shared_ptr<T> remove(Id<T> id)
  {
    if(map.count(id) > 0)
    {
      std::shared_ptr<T> temp(map.find(id)->second);
      map.erase(id);
      return temp;
    }
    return std::shared_ptr<T>();
  }

  //TODO: Put keys inside values to make this function constant in time.
  std::shared_ptr<T> remove(T* value)
  {
    for(Iterator it = begin(); it != end(); ++it) 
    {
      if(it->second.get() == value)
      {
        std::shared_ptr<T> temp(it->second);
        map.erase(it->first);
        return temp;
      }
    }
    return std::shared_ptr<T>();
  }

  int count()
  {
    return map.size();
  }

  bool contains(Id<T> id)
  {
    return map.count(id) > 0;
  }

  Iterator begin()
  {
    return map.begin();
  }

  Iterator end()
  {
    return map.end();
  }

  void clear()
  {
    map.clear();
  }
};


