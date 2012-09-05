#include <Element/DScene.h>

#include <Model/DMesh.h>
#include <Element/DGroup.h>
#include <Element/DCamera.h>
#include <Math/DMatrix.h>
#include <DX/DirectX.h>

using namespace dev;

Matrix Scene::_matrix = Matrix::identity();

Scene::Scene() :
  _cameraActive(NULL)
{
}

Scene::Scene(Camera* camera) :
  _cameraActive(camera)
{
}

Scene::~Scene()
{
}

void Scene::SetActiveCamera(Camera* camera)
{
  _cameraActive = camera;
}

Camera* Scene::GetActiveCamera()
{
  return _cameraActive;
}

void Scene::AddElement(Element* element, bool update)
{
  element->setScene(this);
  _listElements.insert(element);
  if (update)
    _listUpdate.insert(element);
  addElement(element);
}


void Scene::SetElementUpdate(Element* element, bool update)
{
  if (_listElements.find(element) == _listElements.end())
    return;

  if (update)
    _listUpdate.insert(element);
  else
    _listUpdate.erase(element);
}

void Scene::RemoveElement(Element* element)
{
  element->setScene(NULL);
  _listElements.erase(element);
  _listUpdate.erase(element);
  removeElement(element);
}

void Scene::Update()
{
  if (_cameraActive)
    _cameraActive->Update();

  for (Elements::Set::iterator i = _listUpdate.begin(); i != _listUpdate.end(); ++i)
    (*i)->Update(_matrix);
}

void Scene::Draw(bool vSync)
{
  DX.ClearAllRenderTargets();
  DX.ClearAllDepthStencil();

  for (DrawStructVec::iterator i = _drawVec.begin(); i != _drawVec.end(); ++i)
  {
    DrawStruct& ds = (*i);
    
    if(_currentVS != ds.vs)
    {
      if (ds.vs && ds.vs->SetShader())
        _currentVS = ds.vs;
    }

    if(_currentPS != ds.ps)
    {
      if (ds.ps && ds.ps->SetShader())
        _currentPS = ds.ps;
    }

    for(Elements::Set::iterator i = ds.elements.begin(); i != ds.elements.end(); i++)
      (*i)->DrawElement();
  }

  DX.Present(vSync);
}

void Scene::addElement(Element* element)
{
  if (!element)
    return;

  ShaderPassMap& shaderPasses = element->GetShaderPasses();
  if (shaderPasses.empty())
    return;

  for (ShaderPassMap::iterator i = shaderPasses.begin(); i != shaderPasses.end(); ++i)
    addElementShaderPass(element, (*i).first, (*i).second);
}

void Scene::removeElement(Element* element)
{
  if (!element)
    return;

  ShaderPassMap& shaderPasses = element->GetShaderPasses();
  if (shaderPasses.empty())
    return;

  for (ShaderPassMap::iterator i = shaderPasses.begin(); i != shaderPasses.end(); ++i)
    removeElementShaderPass(element, (*i).first, (*i).second);
}

void Scene::addElementShaderPass(Element* element, const UINT& numPass, const ShaderStruct& shaderStruct)
{
  DrawStruct newElement;
  newElement.elements.insert(element);
  newElement.orderNum = numPass;
  newElement.vs       = shaderStruct.vs;
  newElement.ps       = shaderStruct.ps;
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
                ds.elements.insert(element);
                continue;
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

void Scene::removeElementShaderPass(Element* element, const UINT& numPass, const ShaderStruct& shaderStruct)
{
  for (DrawStructVec::iterator i = _drawVec.begin(); i != _drawVec.end(); ++i)
  {
    if ((*i).orderNum == numPass && (*i).ps == shaderStruct.ps && (*i).vs == shaderStruct.vs)
    {
      (*i).elements.erase(element);
      if ((*i).elements.empty())
        _drawVec.erase(i);
      return;
    }
  }
}
