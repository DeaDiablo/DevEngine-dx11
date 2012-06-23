#include <DX/DBuffer.h>

#include <Core/DLog.h>
#include <Element/DVertices.h>
#include <DX/DirectX.h>

using namespace dev::Buffer;

//declarations
static D3D11_INPUT_ELEMENT_DESC vertexP[] =
{
  { "POSITION", 0,    DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

static D3D11_INPUT_ELEMENT_DESC vertexPC32[] =
{
  { "POSITION", 0,    DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  { "COLOR",    0,      DXGI_FORMAT_R8G8B8A8_UINT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

static D3D11_INPUT_ELEMENT_DESC vertexPC128[] =
{
  { "POSITION", 0,    DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

static D3D11_INPUT_ELEMENT_DESC vertexPTuv[] =
{
  { "POSITION", 0,    DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  { "TEXCOORD", 0,       DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

static D3D11_INPUT_ELEMENT_DESC vertexPTuvw[] =
{
  { "POSITION", 0,    DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  { "TEXCOORD", 0,    DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

static D3D11_INPUT_ELEMENT_DESC vertexPN[] =
{
  { "POSITION", 0,    DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  { "NORMAL",   0,    DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

static D3D11_INPUT_ELEMENT_DESC vertexPNTuv[] =
{
  { "POSITION", 0,    DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  { "NORMAL",   0,    DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  { "TEXCOORD", 0,       DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

static D3D11_INPUT_ELEMENT_DESC vertexPNTuvw[] =
{
  { "POSITION", 0,    DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  { "NORMAL",   0,    DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  { "TEXCOORD", 0,    DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

const D3D11_INPUT_ELEMENT_DESC* Declaration::GetLayout(BufferType VB_type)
{
  switch(VB_type)
  {
  case (VB_POS):
    return vertexP;
  case (VB_POS_COL_32):
    return vertexPC32;
  case (VB_POS_COL_128):
    return vertexPC128;
  case (VB_POS_TEX_UV):
    return vertexPTuv;
  case (VB_POS_TEX_UVW):
    return vertexPTuvw;
  case (VB_POS_NORM):
    return vertexPN;
  case (VB_POS_NORM_TEX_UV):
    return vertexPNTuv;
  case (VB_POS_NORM_TEX_UVW):
    return vertexPNTuvw;
  }

  return NULL;
}

UINT Declaration::GetSizeLayout(BufferType VB_type)
{
  switch(VB_type)
  {
  case (VB_POS):
    return ARRAYSIZE(vertexP);
  case (VB_POS_COL_32):
    return ARRAYSIZE(vertexPC32);
  case (VB_POS_COL_128):
    return ARRAYSIZE(vertexPC128);
  case (VB_POS_TEX_UV):
    return ARRAYSIZE(vertexPTuv);
  case (VB_POS_TEX_UVW):
    return ARRAYSIZE(vertexPTuvw);
  case (VB_POS_NORM):
    return ARRAYSIZE(vertexPN);
  case (VB_POS_NORM_TEX_UV):
    return ARRAYSIZE(vertexPNTuv);
  case (VB_POS_NORM_TEX_UVW):
    return ARRAYSIZE(vertexPNTuvw);
  }

  return NULL;
}

UINT Declaration::GetSizeBuffer(BufferType BT_type)
{
  switch(BT_type)
  {
  case (BT_NONE):
    return 1;
  case (IB_16):
    return 2;
  case (IB_32):
    return 4;
  case (VB_POS):
    return sizeof(Vertex::VPosition);
  case (VB_POS_COL_32):
    return sizeof(Vertex::VPositionColor32);
  case (VB_POS_COL_128):
    return sizeof(Vertex::VPositionColor128);
  case (VB_POS_TEX_UV):
    return sizeof(Vertex::VPositionTexCoordUV);
  case (VB_POS_TEX_UVW):
    return sizeof(Vertex::VPositionTexCoordUVW);
  case (VB_POS_NORM):
    return sizeof(Vertex::VPositionNormal);
  case (VB_POS_NORM_TEX_UV):
    return sizeof(Vertex::VPositionNormalTexCoordUV);
  case (VB_POS_NORM_TEX_UVW):
    return sizeof(Vertex::VPositionNormalTexCoordUVW);
  }

  return NULL;
}

Buffer::Buffer() :
  _buffer(NULL),
  _count(NULL),
  _sizeSrc(NULL),
  _type(BT_NONE)
{
  init();
}

Buffer::Buffer(LPVOID src, UINT count, BufferType BT_Type) :
  _buffer(NULL),
  _count(NULL),
  _sizeSrc(NULL),
  _type(BT_NONE)
{
  init();
  SetBuffer(src, count, BT_Type);
}

void Buffer::init()
{
  ZeroMemory(&_bufferDesc, sizeof(_bufferDesc));
  _bufferDesc.Usage = D3D11_USAGE_DEFAULT;
  _bufferDesc.ByteWidth = 0;
  _bufferDesc.BindFlags = 0;
  _bufferDesc.CPUAccessFlags = 0;
}


Buffer::~Buffer()
{
  Destroy();
}

void Buffer::Destroy()
{
  _type = BT_NONE;
  _sizeSrc = NULL;
  _count = NULL;
  if (_buffer)
  {
    _buffer->Release();
    _buffer = NULL;
  }
}

void Buffer::SetBuffer(LPVOID src, UINT count, BufferType BT_Type)
{
  Destroy();
  _type  = BT_Type;
  _count = count;
  _sizeSrc = Declaration::GetSizeBuffer(BT_Type);
  _bufferDesc.ByteWidth = _sizeSrc * _count;
  if (src != NULL)
  {
    ZeroMemory(&_data, sizeof(_data));
    _data.pSysMem = src;
    if (FAILED(DX_DEVICE->CreateBuffer(&_bufferDesc, &_data, &_buffer)))
    {
      Log::GetLog()->WriteToLog(L"Buffer not created (DVertex.cpp)");
    }
  }
  else
    if (FAILED(DX_DEVICE->CreateBuffer(&_bufferDesc, NULL, &_buffer)))
    {
      Log::GetLog()->WriteToLog(L"Buffer not created (DVertex.cpp)");
    }
}

//Vertex buffer
VertexBuffer::VertexBuffer() :
  Buffer(),
  _offset(NULL)
{
  init();
}

VertexBuffer::VertexBuffer(LPVOID src, UINT count, BufferType BT_Type) :
  Buffer(),
  _offset(NULL)
{
  init();
  SetBuffer(src, count, BT_Type);
}

VertexBuffer::~VertexBuffer()
{
}

void VertexBuffer::init()
{
  Buffer::init();
  _bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
}

void VertexBuffer::SetBuffer(LPVOID src, UINT count, BufferType BT_Type)
{
  if (BT_Type > VB_POINTER)
    return;
  Buffer::SetBuffer(src, count, BT_Type);
}

void VertexBuffer::SetAsSource()
{
  if (!_buffer)
    return;

  DX_CONTEXT->IASetVertexBuffers( 0, 1, &_buffer, &_sizeSrc, &_offset );
}

//Index buffer
IndexBuffer::IndexBuffer() :
  Buffer()
{
  init();
}

IndexBuffer::IndexBuffer(LPVOID src, UINT count, BufferType BT_Type) :
  Buffer()
{
  init();
  SetBuffer(src, count, BT_Type);
}

IndexBuffer::~IndexBuffer()
{
}

void IndexBuffer::init()
{
  Buffer::init();
  _bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
}

void IndexBuffer::SetAsSource()
{
  if (!_buffer)
    return;

  DX_CONTEXT->IASetIndexBuffer(_buffer, (DXGI_FORMAT)_type, 0);
}

//Index buffer
ConstantBuffer::ConstantBuffer() :
  Buffer()
{
}

ConstantBuffer::ConstantBuffer(LPVOID src, UINT size) :
  Buffer()
{
  init();
  SetBuffer(src, size);
}

ConstantBuffer::ConstantBuffer(UINT size) :
  Buffer()
{
  init();
  SetBuffer(NULL, size);
}

ConstantBuffer::~ConstantBuffer()
{
}

void ConstantBuffer::init()
{
  Buffer::init();
  _bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
}

void ConstantBuffer::SetBuffer(LPVOID src, UINT size)
{
  Buffer::SetBuffer(src, size, BT_NONE);
}

void ConstantBuffer::SetAsVSSource(UINT num)
{
  if (!_buffer || num >= D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT)
    return;

  DX_CONTEXT->VSSetConstantBuffers(num, 1, &_buffer);
}

void ConstantBuffer::SetAsPSSource(UINT num)
{
  if (!_buffer || num >= D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT)
    return;

  DX_CONTEXT->PSSetConstantBuffers(num, 1, &_buffer);
}

void ConstantBuffer::SetAsSource()
{
}

void ConstantBuffer::UpdateResource(LPVOID src)
{
  if (!_buffer)
    return;

  DX_CONTEXT->UpdateSubresource(_buffer, 0, NULL, src, 0, 0);
}
