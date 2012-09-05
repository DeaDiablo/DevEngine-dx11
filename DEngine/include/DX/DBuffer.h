#ifndef DEV_VERTEX_H
#define DEV_VERTEX_H

#include <Core/DInclude.h>
#include <Element/DVertexStruct.h>

namespace dev
{
  namespace Buffer
  {
    enum BufferType
    {
      BT_NONE             = 0,
      
      //vertex buffer
      VB_POS              = 1,
      VB_POS_TEX_UV       = 2,
      VB_POS_NORM         = 3,
      VB_POS_NORM_TEX_UV  = 4,
      VB_POS_COL_32       = 5,
      VB_POS_COL_128      = 6,
      VB_POS_TEX_UVW      = 7,
      VB_POS_NORM_TEX_UVW = 8,
      
      VB_POINTER,

      //index buffer
      IB_16    = DXGI_FORMAT_R16_UINT,
      IB_32    = DXGI_FORMAT_R32_UINT,
    };
    
    namespace Declaration
    {
      const D3D11_INPUT_ELEMENT_DESC* GetLayout(BufferType VB_type);
      UINT GetSizeLayout(BufferType VB_type);
      UINT GetSizeBuffer(BufferType BT_type);
    }

    class Buffer
    {
    public:
      Buffer();
      Buffer(LPVOID src, UINT count, BufferType BT_Type);
      virtual ~Buffer();
      void Destroy();

      virtual void SetBuffer(LPVOID src, UINT count, BufferType BT_Type);

      virtual void SetAsSource() = 0;
      inline int GetCount() const
      {
        return _count;
      }

      inline const BufferType& GetType()
      {
        return _type;
      }

    protected:
      virtual void init();

      BufferType                _type;
      int                       _count;
      UINT                      _sizeSrc;
      D3D11_SUBRESOURCE_DATA    _data;
      D3D11_BUFFER_DESC         _bufferDesc;
      ID3D11Buffer*             _buffer;
    };

    //Vertex buffer
    class VertexBuffer :
      public Buffer
    {
    public:
      VertexBuffer();
      VertexBuffer(LPVOID src, UINT count, BufferType BT_Type);
      virtual ~VertexBuffer();

      virtual void SetBuffer(LPVOID src, UINT count, BufferType BT_Type);
      virtual void SetAsSource();

    protected:

      UINT _offset;
      virtual void init();
    };

    //Index buffer
    class IndexBuffer :
      public Buffer
    {
    public:
      IndexBuffer();
      IndexBuffer(LPVOID src, UINT count, BufferType BT_Type);
      virtual ~IndexBuffer();

      virtual void SetAsSource();

    protected:
      virtual void init();
    };

    //Constant buffer
    class ConstantBuffer :
      public Buffer
    {
    public:
      ConstantBuffer();
      ConstantBuffer(LPVOID src, UINT size);
      ConstantBuffer(UINT size);
      virtual ~ConstantBuffer();

      virtual void SetBuffer(LPVOID src, UINT size);
      virtual inline ID3D11Buffer*& GetBuffer() { return _buffer; }

      virtual void SetAsSource();
      virtual void UpdateResource(LPVOID src);

    protected:
      virtual void init();
    };
  }
}

#endif