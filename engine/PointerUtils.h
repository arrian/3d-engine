#pragma once

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>


/** Get a shared pointer from a weak pointer or returns an empty pointer.
*/
template<class T> boost::shared_ptr<T> getSharedFromWeak(boost::weak_ptr<T> weak, std::string debugMessage = "")
{
  try
  {
    return weak.lock();
  }
  catch(boost::bad_weak_ptr b)
  {
#ifdef _DEBUG
    std::cout << debugMessage << std::endl;
#endif
  }
  return boost::shared_ptr<T>();
}

template<class T> boost::weak_ptr<T> getWeakFromShared(boost::shared_ptr<T> shared)
{
  return boost::weak_ptr<T>(shared);
}

