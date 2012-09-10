#ifndef DEV_CONST_BUFFERS_H
#define DEV_CONST_BUFFERS_H

#include <DX/DBuffer.h>
#include <Math/DMatrix.h>

namespace dev
{
  namespace Buffer
  {
    //BufferSingleton 
    template<class B, class T, UINT size>
    class BufferSingleton
    {
    public:
      inline static T& Instance()
      {
        static T element(size);
        return element;
      }
    };

    #define WORLD_BUFFER            Buffer::WorldConstBuffer::Instance()
    #define VIEW_PROJECTION_BUFFER  Buffer::ViewProjectionConstBuffer::Instance()
    #define EYE_POSITION_BUFFER     Buffer::EyePositionConstBuffer::Instance()

    //const buffers    
    class WorldConstBuffer : public BufferSingleton<WorldConstBuffer, ConstantBuffer, sizeof(Matrix)>
    {};

    class ViewProjectionConstBuffer : public BufferSingleton<ViewProjectionConstBuffer, ConstantBuffer, sizeof(Matrix)>
    {};

    class EyePositionConstBuffer : public BufferSingleton<EyePositionConstBuffer, ConstantBuffer, sizeof(Vec4)>
    {};

  }
}

#endif
