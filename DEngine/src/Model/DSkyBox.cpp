#include <Model/DSkyBox.h>

#include <Element/DVertices.h>

using namespace dev;

SkyBox::SkyBox(const wchar_t* name, const wchar_t* texture) :
  Mesh(name),
  _skyDSState(DX.CreateDepthStencilState(TRUE, D3D11_DEPTH_WRITE_MASK_ALL, D3D11_COMPARISON_LESS_EQUAL)),
  _oldDSState(NULL),
  _camera(NULL)
{
  Vertex::VPosition arrayVertex[] = 
  {
    Vertex::Position( 1.0f,  -1.0f,  -1.0f),
    Vertex::Position( 1.0f,   1.0f,  -1.0f),
    Vertex::Position(-1.0f,   1.0f,  -1.0f),
    Vertex::Position(-1.0f,  -1.0f,  -1.0f),

    Vertex::Position(-1.0f,  -1.0f,  -1.0f),
    Vertex::Position(-1.0f,   1.0f,  -1.0f),
    Vertex::Position(-1.0f,   1.0f,   1.0f),
    Vertex::Position(-1.0f,  -1.0f,   1.0f),

    Vertex::Position(-1.0f,  -1.0f,   1.0f),
    Vertex::Position(-1.0f,   1.0f,   1.0f),
    Vertex::Position( 1.0f,   1.0f,   1.0f),
    Vertex::Position( 1.0f,  -1.0f,   1.0f),

    Vertex::Position( 1.0f,  -1.0f,   1.0f),
    Vertex::Position( 1.0f,   1.0f,   1.0f),
    Vertex::Position( 1.0f,   1.0f,  -1.0f),
    Vertex::Position( 1.0f,  -1.0f,  -1.0f),

    Vertex::Position( 1.0f,  -1.0f,  -1.0f),
    Vertex::Position(-1.0f,  -1.0f,  -1.0f),
    Vertex::Position(-1.0f,  -1.0f,   1.0f),
    Vertex::Position( 1.0f,  -1.0f,   1.0f),

    Vertex::Position( 1.0f,   1.0f,   1.0f),
    Vertex::Position(-1.0f,   1.0f,   1.0f),
    Vertex::Position(-1.0f,   1.0f,  -1.0f),
    Vertex::Position( 1.0f,   1.0f,  -1.0f)
  };
  
  const unsigned short arrayIndex[] =
  {
    0,1,2,    2,3,0,
    4,5,6,    6,7,4,
    8,9,10,   10,11,8,
    12,13,14, 14,15,12,
    16,17,18, 18,19,16,
    20,21,22, 22,23,20
  };

  SetVertices((LPVOID)arrayVertex, 24, Buffer::VB_POS);
  SetIndexes((LPVOID)arrayIndex, 36, Buffer::IB_16);
  SetVertexShader(BACKGROUND_PASS_NUM, L"shaders/skybox.vs");
  SetPixelShader(BACKGROUND_PASS_NUM, L"shaders/skybox.ps");
  SetTexture(texture);
}

SkyBox::~SkyBox()
{
}

void SkyBox::SetCamera(Camera* camera)
{
  _camera = camera;
}

void SkyBox::updateParameters()
{
  Mesh::updateParameters();
  _oldDSState = DX.GetActiveDepthStencilState();
  DX.SetDepthStencilState(_skyDSState);
}

void SkyBox::returnParameters()
{
  DX.SetDepthStencilState(_oldDSState);
  Mesh::returnParameters();
}

void SkyBox::Update(const Matrix& matrix)
{
  if (_camera)
    SetPosition(_camera->GetPosition());
  Mesh::Update(matrix);
}
