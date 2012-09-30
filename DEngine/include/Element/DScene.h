#ifndef DEV_SCENE_H
#define DEV_SCENE_H

#include <Element/DGroup.h>

namespace dev
{
  class Camera;
  class Mesh;

  class Scene
  {
  public:
    Scene();
    Scene(Camera* camera);
    virtual ~Scene();

    void SetActiveCamera(Camera* camera);
    Camera* GetActiveCamera();

    void AddElement(Element* element, bool update = true);
    void SetElementUpdate(Element* element, bool update);
    void RemoveElement(Element* element);

    virtual void Update();
    virtual void Draw(bool vSync);

  protected:
    void initScene();
    void addElementShaderPass(Element* element, const UINT& numPass, const ShaderStruct& shaderStruct);
    void removeElementShaderPass(Element* element, const UINT& numPass, const ShaderStruct& shaderStruct);
    Camera* _cameraActive;

    Elements::Set  _listUpdate, _listElements;
    static Matrix _matrix;

    VertexShader*   _currentVS;
    ComputeShader*  _currentCS;
    PixelShader*    _currentPS;

    struct DrawStruct
    {
      Elements::Set   elements;
      UINT            orderNum;
      VertexShader*   vs;
      ComputeShader*  cs;
      PixelShader*    ps;
    };
    typedef std::vector<DrawStruct> DrawStructVec;
    DrawStructVec _drawVec;

    friend Element;
  };
}

#endif