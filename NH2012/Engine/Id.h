
#include <string>
#include <boost/lexical_cast.hpp>


template<class T>
class Id
{
public:

  //Generate an id
  Id()
  {
    globalInstance++;
    this->instance = globalInstance;
  }

  //Initialise specifying the id
  Id(long instance)
  {
    if(instance > globalInstance) globalInstance = instance + 1;
    this->instance = instance;
  }

  long getInstance()
  {
    return instance;
  }

  bool isValid()
  {
    if(instance == 0) return false;
    return true;
  }

  //Gets the identifier name in the format "Object::Instance"
  std::string getName()
  {
    std::stringstream stream;
    stream << std::hex << instance;
    return std::string(typeid(T).name()) + "::" + stream.str();
  }

  bool Id<T>::operator==(const Id<T> &other) const {
    return getInstance() == other.getInstance();
  }

  bool Id<T>::operator!=(const Id<T> &other) const {
    return !(*this == other);
  }

  typedef T type;

private:
  static long globalInstance;
  long instance;

};


//Intialising the global instance number.
template <typename T>
long Id<T>::globalInstance = 1;//saving zero for invalid id

