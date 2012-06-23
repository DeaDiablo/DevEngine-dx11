#ifndef DEV_GROUP_H
#define DEV_GROUP_H

#include <Core/DInclude.h>
#include <Element/DElement.h>

namespace dev
{
  class Group :
    public Element
  {
  public:
    Group(Vec3 position, Vec3 rotation, Vec3 scale);
    Group();
    virtual ~Group();

    virtual void ClearList();
    virtual void AddElement(Element* element);
    virtual void InsertElement(UINT num, Element* element);
    virtual void SetElement(UINT num, Element* element);
    virtual void ReplaceElement(Element* element, Element* newElement);
    virtual void RemoveElement(Element* element);
    virtual void RemoveElement(UINT num);
    virtual void RemoveAllElements();

    inline UINT GetElementNum(Element* element) const
    {
      for (UINT i = 0; i < _elements.size(); ++i)
      {
        if (_elements[i] == element) 
          return i;
      }
      return static_cast<UINT>(_elements.size());
    }

    inline Element* GetElement(UINT num) const
    {
      if (num < _elements.size())
        return (*(_elements.begin() + num));
      return NULL;
    }

    virtual void Update(const Matrix& matrix);

    virtual inline Group* AsGroup() 
    {
      return this;
    }

    inline UINT GetSize() const
    {
      return (UINT)_elements.size();
    }

  protected:
    Element*    _element;
    ElementList _elements;
  };
}

#endif