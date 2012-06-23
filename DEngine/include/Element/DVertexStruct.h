#ifndef DEV_VERTEX_STRUCT_H
#define DEV_VERTEX_STRUCT_H

#include <Math/DVec4.h>

namespace dev
{
  namespace Vertex
  {
    //position struct
    struct Position
    {
      Position() : 
        x(0), y(0), z(0) {}

      Position(float X, float Y, float Z) : 
        x(X), y(Y), z(Z) {}

      Position(const Vec3& vec) : 
        x(vec.x), y(vec.y), z(vec.z) {}

      Position& operator= (const Position& position) 
      { 
        x = position.x;
        y = position.y;
        z = position.z;
      }

      Position& operator= (const Vec3& vec) 
      { 
        x = vec.x;
        y = vec.y;
        z = vec.z;
      }

      float x, y, z;
    };

    //color struct
    struct Color
    {
      Color() : 
        r(0), g(0), b(0), a(0) {}
      
      Color(float R, float G, float B, float A) : 
        r(R), g(G), b(B), a(A) {}

      Color(const Vec4& vec) : 
        r(vec.x), g(vec.y), b(vec.z), a(vec.w) {}
      
      Color& operator= (const Color& color) 
      { 
        r = color.r;
        g = color.g;
        b = color.b;
        a = color.a;
      }

      Color& operator= (const Vec4& vec) 
      { 
        r = vec.x;
        g = vec.y;
        b = vec.z;
        a = vec.w;
      }


      float r, g, b, a;
    };

    struct Normal
    {
      Normal() : 
        nx(0), ny(0), nz(0) {}

      Normal(float NX, float NY, float NZ) : 
        nx(NX), ny(NY), nz(NZ) {}


      Normal(const Vec3& vec) : 
        nx(vec.x), ny(vec.y), nz(vec.z) {}

      Normal& operator= (const Normal& normal) 
      { 
        nx = normal.nx;
        ny = normal.ny;
        nz = normal.nz;
      }

      Normal& operator= (const Vec3& vec) 
      { 
        nx = vec.x;
        ny = vec.y;
        nz = vec.z;
      }
      
      float nx, ny, nz;
    };

    struct TexCoordUV
    {
      TexCoordUV() : 
        tu(0), tv(0) {}

      TexCoordUV(float TU, float TV) : 
        tu(TU), tv(TV) {}

      TexCoordUV(const Vec2& vec) : 
        tu(vec.x), tv(vec.y) {}

      TexCoordUV& operator= (const TexCoordUV& texCoordUV) 
      { 
        tu = texCoordUV.tu;
        tv = texCoordUV.tv;
      }

      TexCoordUV& operator= (const Vec2& vec) 
      { 
        tu = vec.x;
        tv = vec.y;
      }

      float tu, tv;
    };

    struct TexCoordUVW
    {
      TexCoordUVW() : 
        tu(0), tv(0), tw(0) {}

      TexCoordUVW(float TU, float TV, float TW) : 
        tu(TU), tv(TV), tw(TW) {}

      TexCoordUVW(const Vec3& vec) : 
        tu(vec.x), tv(vec.y), tw(vec.z) {}

      TexCoordUVW& operator= (const TexCoordUVW& texCoordUVW) 
      { 
        tu = texCoordUVW.tu;
        tv = texCoordUVW.tv;
        tw = texCoordUVW.tw;
      }

      TexCoordUVW& operator= (const Vec3& vec) 
      { 
        tu = vec.x;
        tv = vec.y;
        tw = vec.z;
      }

      float tu, tv, tw;
    };
  }
}

#endif