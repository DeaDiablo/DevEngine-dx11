#ifndef DEV_INCLUDE_H
#define DEV_INCLUDE_H

//STL 
#include <vector>
#include <set>
#include <map>

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

namespace dev
{
  class OutputClass;
  class Element;
  
  namespace Elements
  {
    typedef std::set<Element*>      Set;
    typedef std::vector<Element*>   Vec;
  };

  namespace OutputClasses
  {
    typedef std::set<OutputClass*>  Set;
  }
}

#endif