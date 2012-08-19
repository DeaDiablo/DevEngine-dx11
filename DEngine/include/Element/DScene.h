#ifndef DEV_SCENE_H
#define DEV_SCENE_H

#include <Element/DElement.h>

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

    void AddElement(Element* element);
    void RemoveElement(Element* element);

    virtual void Update();
    virtual void Draw(bool vSync);

  protected:
    void addElement(Element* element);
    void removeElement(Element* element);

    Camera* _cameraActive;
    
    Matrix      _matrix;
    ElementVec  _listUpdate;

    VertexShader* _currentVS;
    PixelShader*  _currentPS;

    Buffer::ConstantBuffer* _wBuffer;
    Buffer::ConstantBuffer* _vpBuffer;
    
    struct DrawStruct
    {
      Element*      element;
      UINT  orderNum;
      VertexShader* vs;
      PixelShader*  ps;
    };
    typedef std::vector<DrawStruct> DrawStructVec;
    DrawStructVec _drawVec;
  };
}

#endif