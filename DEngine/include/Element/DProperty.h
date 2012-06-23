#ifndef DEV_PROPERTY_H
#define DEV_PROPERTY_H

namespace dev
{
  namespace Properties
  {
    template<class T>
    class Property
    {
    public:
      Property() {}
      virtual ~Property() {}

      virtual void Save() {}
      virtual void Load() {}

      void operator = (const T& value) 
      {
        _value = value;
      }

      void operator += (const T& value) 
      {
        _value += value;
      }

      inline const T& operator () () const
      {
        return _value;
      }

    protected:
      T _value;
    };
  }
}

#endif