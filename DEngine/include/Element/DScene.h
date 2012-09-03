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
    void addElement(Element* element);
    void removeElement(Element* element);
    void addElementShaderPass(Element* element, const UINT& numPass, const ShaderStruct& shaderStruct);
    void removeElementShaderPass(Element* element, const UINT& numPass, const ShaderStruct& shaderStruct);
    Camera* _cameraActive;

    Elements::Set  _listUpdate, _listElements;
    static Matrix _matrix;

    VertexShader* _currentVS;
    PixelShader*  _currentPS;


    Buffer::ConstantBuffer* _wBuffer;
    Buffer::ConstantBuffer* _vpBuffer;

    struct DrawStruct
    {
      Elements::Set elements;
      UINT          orderNum;
      VertexShader* vs;
      PixelShader*  ps;
    };
    typedef std::vector<DrawStruct> DrawStructVec;
    DrawStructVec _drawVec;

    friend Element;
  };
}

#endif