#include <Element/DScene.h>

#include <Model/DMesh.h>
#include <Element/DGroup.h>
#include <Element/DCamera.h>
#include <Math/DMatrix.h>
#include <DX/DirectX.h>

using namespace dev;

Scene::Scene() :
  _cameraActive(NULL)
{
  _matrix = Matrix::identity();
  _wBuffer = Buffer::WorldConstBuffer::Register();
  _vpBuffer = Buffer::ViewProjectionConstBuffer::Register();
}

Scene::Scene(Camera* camera) :
  _cameraActive(camera)
{
  _matrix = Matrix::identity();
  _wBuffer = Buffer::WorldConstBuffer::Register();
  _vpBuffer = Buffer::ViewProjectionConstBuffer::Register();
}

Scene::~Scene()
{
  Buffer::WorldConstBuffer::Unregister();
  Buffer::ViewProjectionConstBuffer::Unregister();
}

void Scene::SetActiveCamera(Camera* camera)
{
  _cameraActive = camera;
}

Camera* Scene::GetActiveCamera()
{
  return _cameraActive;
}

void Scene::AddElement(Element* element)
{
  _listUpdate.push_back(element);
  addElement(element);
}

void Scene::RemoveElement(Element* element)
{
  for (ElementVec::iterator i = _listUpdate.begin(); i != _listUpdate.end(); ++i)
    if ((*i) == element)
      _listUpdate.erase(i);

  removeElement(element);
}

void Scene::Update()
{
  if (_cameraActive)
    _cameraActive->Update();

  for (ElementVec::iterator i = _listUpdate.begin(); i != _listUpdate.end(); ++i)
    (*i)->Update(_matrix);
}

void Scene::Draw(bool vSync)
{
  //_currentVS = NULL;
  //_currentPS = NULL;

  DX.ClearAllRenderTargets();
  DX.ClearAllDepthStencil();

  for (DrawStructVec::iterator i = _drawVec.begin(); i != _drawVec.end(); ++i)
  {
    DrawStruct& ds = (*i);
    
    if(_currentVS != ds.vs)
    {
      if (!ds.vs->SetShader())
        continue;
      _vpBuffer->SetAsVSSource(0);
      _wBuffer->SetAsVSSource(1);

      _currentVS = ds.vs;
    }

    if(_currentPS != ds.ps)
    {
      if (!ds.ps->SetShader())
        continue;
      _currentPS = ds.ps;
    }

    ds.element->DrawElement();
  }

  DX.Present(vSync);
}

void Scene::addElement(Element* element)
{
  if (!element)
    return;

  ShaderPassMap shaderPasses = element->GetShaderPasses();
  if (shaderPasses.empty())
    return;

  for (ShaderPassMap::iterator i = shaderPasses.begin(); i != shaderPasses.end(); ++i)
  {
    DrawStruct newElement;
    newElement.element  = element;
    newElement.orderNum = (*i).second->GetOrderPass();
    newElement.vs       = (*i).second->GetVertexShader();
    newElement.ps       = (*i).second->GetPixelShader();

    for (DrawStructVec::iterator i = _drawVec.begin(); i != _drawVec.end(); ++i)
    {
      DrawStruct& ds = (*i);
      if (ds.orderNum >= newElement.orderNum)
      {
        if (ds.orderNum == newElement.orderNum)
        {
          if (ds.ps <= newElement.ps)
          {
            if (ds.ps == newElement.ps)
            {
              if (ds.vs <= newElement.vs)
              {
                if (ds.vs == newElement.vs)
                {
                  if ((ds.element->GetPosition() - _cameraActive->GetPosition()).Length() >= 
                    (element->GetPosition() - _cameraActive->GetPosition()).Length())
                  {
                    _drawVec.insert(i, newElement);
                    return;
                  }
                }
                else
                {
                  _drawVec.insert(i, newElement);
                  return;
                }
              }
            }
            else
            {
              _drawVec.insert(i, newElement);
              return;
            }
          }
        }
        else
        {
          _drawVec.insert(i, newElement);
          return;
        }
      }
    }
    _drawVec.push_back(newElement);
  }
}

void Scene::removeElement(Element* element)
{
  if (!element)
    return;

  for (DrawStructVec::iterator i = _drawVec.begin(); i != _drawVec.end(); ++i)
    if ((*i).element == element)
    {
      _drawVec.erase(i);
      return;
    }
}

