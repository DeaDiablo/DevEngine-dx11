#include <Element/DGroup.h>

using namespace dev;

Group::Group(Vec3 position, Vec3 rotation, Vec3 scale) :
  Element(position, rotation, scale)
{
  ClearList();
}

Group::Group() :
  Element()
{
  ClearList();
}

Group::~Group()
{
  for (Elements::Vec::iterator i = _elements.begin(); i != _elements.end(); ++i)
    delete (*i);
  ClearList();
}

void Group::ClearList()
{
  _elements.clear();
}

void Group::AddElement(Element* element)
{
  InsertElement((UINT)_elements.size(), element);
}

void Group::InsertElement(UINT num, Element* element)
{
  if (!element)
    return;

  if (GetElementNum(element) != _elements.size())
    return;

  if (num >= _elements.size())
    _elements.push_back(element);
  else
    _elements.insert(_elements.begin() + num, element);
  element->setParent(this);
}

void Group::SetElement(UINT num, Element* element)
{
  if (!element)
    return;

  if (num < _elements.size())
  {
    Element* oldElement = _elements[num];
    _elements[num] = element;
    oldElement->clearParent();
    element->setParent(this);
  }
}

void Group::ReplaceElement(Element* element, Element* newElement)
{
  if (!element || !newElement)
    return;

  UINT num = GetElementNum(element);
  
  if (num != _elements.size())
  {
    element->clearParent();
    _elements[num] = newElement;
    newElement->setParent(this);
  }
}

void Group::RemoveElement(Element* element)
{
  if (!element)
    return;

  UINT num = GetElementNum(element);
  if (num != _elements.size())
  {
    _elements.erase(_elements.begin() + num);
    element->clearParent();
  }
}

void Group::RemoveElement(UINT num)
{
  if (num < _elements.size())
  {
    Element* element = GetElement(num);
    element->clearParent();
    _elements.erase(_elements.begin() + num);
  }
}

void Group::RemoveAllElements()
{
  for (Elements::Vec::iterator i = _elements.begin(); i != _elements.end(); ++i)
    (*i)->clearParent();
  _elements.clear();
}

void Group::Update(const Matrix& matrix)
{
  if (!_visible())
    return;

  Element::Update(matrix);

  for (UINT i = 0; i < _elements.size(); ++i)
    _elements[i]->Update(_matrix);
}

void Group::DrawElement()
{
  if (!_visible())
    return;

  for (UINT i = 0; i < _elements.size(); ++i)
  {
    Element* element = _elements[i];
    if (element->GetShaderPasses().empty())
      element->DrawElement();
  }
}

void Group::updateParent(Message msg)
{
  switch(msg)
  {
  case UPDATE_BUFFER_TYPE:
    {
      for (Elements::Vec::iterator i = _elements.begin(); i != _elements.end(); ++i)
      {
        if ((*i)->GetShaderPasses().empty() && (*i)->GetBufferType() != Buffer::BT_NONE)
        {
          setBufferType((*i)->GetBufferType());
          break;
        }
      }
      break;
    }
  }
  Element::updateParent(msg);
}

void Group::setScene(Scene* scene)
{
  Element::setScene(scene);

  for (UINT i = 0; i < _elements.size(); ++i)
    _elements[i]->setScene(scene);
}
