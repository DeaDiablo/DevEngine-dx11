#ifndef DEV_INCLUDE_H
#define DEV_INCLUDE_H

//debug include
#ifdef _DEBUG
  #define _CRTDBG_MAP_ALLOC
  #include <stdlib.h>
  #include <crtdbg.h>
#endif

#define DESTROY_DX( object ) \
        if (object)          \
        {                    \
          object->Release(); \
          object = NULL;     \
        }

#include <D3D11.h>
#include <D3DX11.h>
#include <D3DCompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")

#endif