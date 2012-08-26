#ifndef DEV_SINGLETON_H
#define DEV_SINGLETON_H

namespace dev
{
  template<class T>
  class Singleton
  {
  public:
    inline static T& Instance()
    {
      static T instance;
      return instance;
    }
  };
}

#endif
