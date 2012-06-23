#ifndef DEV_CONST_BUFFERS_H
#define DEV_CONST_BUFFERS_H

#include <DX/DBuffer.h>

namespace dev
{
  namespace Buffer
  {
    //BufferSingleton 
    template<class B, class T, UINT size>
    class BufferSingleton
    {
    public:
      inline static T* Register()
      {
        if(!_element)
          _element = new T(size);
        _refCount++;
        return _element;
      }

      inline static T* Get()
      {
        return _element;
      }

      inline static void Unregister()
      { 
        _refCount--;
        if(_element && _refCount <= 0)
        {
          delete _element;
          _element = 0;
        }
      }
    private:
      static T* _element;
      static int _refCount;
    };

    template<class B, class T, UINT size>
    T* BufferSingleton<B, T, size>::_element = 0;

    template<class B, class T, UINT size>
    int BufferSingleton<B, T, size>::_refCount = 0;

    //const buffers    
    class WorldConstBuffer : public BufferSingleton<WorldConstBuffer, ConstantBuffer, sizeof(Matrix)>
    {};

    class ViewProjectionConstBuffer : public BufferSingleton<ViewProjectionConstBuffer, ConstantBuffer, sizeof(Matrix)>
    {};

  }
}

#endif
