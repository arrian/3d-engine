


template<class T>
class Id
{
public:

  Id()
  {
    globalInstance++;
    this->instance = globalInstance;
  }

  Id(long instance)
  {
    if(instance > globalInstance) globalInstance = instance + 1;
    this->instance = instance;
  }

  long getInstance()
  {
    return instance;
  }

  typedef T type;

private:
  static long globalInstance;
  long instance;

};


//Intialising the global instance number.
template <typename T>
long Id<T>::globalInstance = 0;

