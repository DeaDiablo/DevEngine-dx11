#ifndef DEV_VERTICES_H
#define DEV_VERTICES_H

#include <Element/DVertexStruct.h>

namespace dev
{
  namespace Vertex
  {
    struct VPosition
    {
      Position position;
    };

    struct VPositionColor32
    {
      Position position;
      DWORD color;
    };

    struct VPositionColor128
    {
      Position position;
      Color color;
    };

    struct VPositionTexCoordUV
    {
      Position position;
      TexCoordUV texCoord;
    };

    struct VPositionTexCoordUVW
    {
      Position position;
      TexCoordUVW texCoord;
    };

    struct VPositionNormal
    {
      Position position;
      Normal normal;
    };

    struct VPositionNormalTexCoordUV
    {
      Position pos;
      Normal normal;
      TexCoordUV texCoord;
    };

    struct VPositionNormalTexCoordUVW
    {
      Position pos;
      Normal normal;
      TexCoordUVW texCoord;
    };
  }
}

#endif