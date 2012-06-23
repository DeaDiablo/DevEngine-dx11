#include <Element/DScene.h>

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
  for (ElementList::iterator i = _listUpdate.begin(); i != _listUpdate.end(); ++i)
    if ((*i) == element)
      _listUpdate.erase(i);

  removeElement(element);
}

void Scene::Update()
{
  if (_cameraActive)
    _cameraActive->Update();

  for (ElementList::iterator i = _listUpdate.begin(); i != _listUpdate.end(); ++i)
    (*i)->Update(_matrix);
}

void Scene::Draw(bool vSync)
{
  _currentVS = NULL;
  _currentPS = NULL;

  DX->ClearFrame();

  for (drawVec::iterator i = _drawMesh.begin(); i != _drawMesh.end(); ++i)
  {
    drawStruct& m = (*i);
    
    if(_currentVS != m.vs)
    {
      m.vs->SetShader();
      m.layout->SetLayout();
      _vpBuffer->SetAsVSSource(0);
      _wBuffer->SetAsVSSource(1);

      _currentVS = m.vs;
    }

    if(_currentPS != m.ps)
    {
      m.ps->SetShader();
      _currentPS = m.ps;
    }

    m.mesh->DrawElement();
  }

  DX->Present(vSync);
}

void Scene::addElement(Element* element)
{
  Group* group = element->AsGroup();
  if (group)
  {
    for(UINT i = 0; i < group->GetSize(); i++)
      addElement(group->GetElement(i));
    return;
  }

  Mesh* mesh = element->AsMesh();
  if (mesh)
  {
    drawStruct newMesh;
    newMesh.mesh = mesh;
    newMesh.vs = mesh->GetVertexShader();
    newMesh.ps = mesh->GetPixelShader();
    newMesh.layout = mesh->GetLayout();
    
    for (drawVec::iterator i = _drawMesh.begin(); i != _drawMesh.end(); ++i)
    {
      drawStruct& m = (*i);
      if (m.mesh->GetOrderNum() >= mesh->GetOrderNum())
      {
        if (m.mesh->GetOrderNum() == mesh->GetOrderNum())
        {
          if (m.ps <= newMesh.ps)
          {
            if (m.ps == newMesh.ps)
            {
              if (m.vs <= newMesh.vs)
              {
                if (m.vs == newMesh.vs)
                {
                  if ((m.mesh->GetPosition() - _cameraActive->GetPosition()).Length() >= 
                      (mesh->GetPosition() - _cameraActive->GetPosition()).Length())
                  {
                    _drawMesh.insert(i, newMesh);
                    return;
                  }
                }
                else
                {
                  _drawMesh.insert(i, newMesh);
                  return;
                }
              }
            }
            else
            {
              _drawMesh.insert(i, newMesh);
              return;
            }
          }
        }
        else
        {
          _drawMesh.insert(i, newMesh);
          return;
        }
      }
    }

    _drawMesh.push_back(newMesh);
  }
}

void Scene::removeElement(Element* element)
{
  Group* group = element->AsGroup();
  if (group)
  {
    for(UINT i = 0; i < group->GetSize(); i++)
      removeElement(group->GetElement(i));
    return;
  }

  Mesh* mesh = element->AsMesh();
  if (mesh)
  {
    for (drawVec::iterator i = _drawMesh.begin(); i != _drawMesh.end(); ++i)
      if ((*i).mesh == mesh)
        _drawMesh.erase(i);
  }
}

