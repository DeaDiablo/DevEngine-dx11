#include <Model/DPlane.h>

#include <Element/DVertices.h>

using namespace dev;

Plane::Plane(const wchar_t* name, const Vec2& leftTop, const Vec2& rightBottom) :
  Mesh(name)
{
  SetPlain(leftTop, rightBottom);
}

Plane::~Plane()
{
}

void Plane::SetPlain(const Vec2& leftTop, const Vec2& rightBottom)
{
  Vertex::VPosition arrayVertex[] = 
  {
    Vertex::Position(leftTop.x,      rightBottom.y,  0.0f),
    Vertex::Position(rightBottom.x,  rightBottom.y,  0.0f),
    Vertex::Position(rightBottom.x,  leftTop.y,      0.0f),
    Vertex::Position(leftTop.x,      leftTop.y,      0.0f)
  };

  const unsigned short arrayIndex[] =
  {
    0,1,2,    2,3,0
  };

  SetVertices((LPVOID)arrayVertex, 4, Buffer::VB_POS);
  SetIndexes((LPVOID)arrayIndex, 6, Buffer::IB_16);
}

PlaneIdentity::PlaneIdentity(const wchar_t* name) : 
  Plane(name, Vec2(-1.0f, -1.0f), Vec2(1.0f, 1.0f))
{
}

PlaneIdentity::~PlaneIdentity()
{

}
