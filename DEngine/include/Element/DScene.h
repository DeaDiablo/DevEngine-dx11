#ifndef DEV_SCENE_H
#define DEV_SCENE_H

#include <Model/DMesh.h>
#include <Element/DGroup.h>
#include <Element/DCamera.h>

namespace dev
{
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
    ElementList _listUpdate;

    VertexShader* _currentVS;
    PixelShader*  _currentPS;

    Buffer::ConstantBuffer* _wBuffer;
    Buffer::ConstantBuffer* _vpBuffer;
    
    struct drawStruct
    {
      Mesh* mesh;
      VertexShader* vs;
      PixelShader*  ps;
      Layout* layout;
    };
    typedef std::vector<drawStruct> drawVec;
    drawVec _drawMesh;
  };
}

#endif