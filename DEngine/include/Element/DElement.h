#ifndef DEV_ELEMENT_H
#define DEV_ELEMENT_H

#include <vector>

#include <Core/DInclude.h>
#include <Core/DSettings.h>
#include <Math/DMatrix.h>
#include <Element/DProperty.h>
#include <DX/DShader.h>

namespace dev
{
  class Model;
  class Mesh;
  class Group;

  class Element
  {
  public:
    Element(const Vec3& position = Vec3(0, 0, 0), const Vec3& rotation = Vec3(0, 0, 0), const Vec3& scale = Vec3(1, 1, 1));
    virtual ~Element();

    void SetVisible(bool value);
    inline bool GetVisible() const
    {
      return _visible();
    }

    void SetMove(const float& x, const float& y, const float& z);
    void SetMove(const Vec3& value);

    void SetPosition(const float& x, const float& y, const float& z);
    void SetPosition(const Vec3& value);
    inline const Vec3& GetPosition() const
    {
      return _position();
    }

    void SetScale(const float& sx, const float& sy, const float& sz);
    void SetScale(const Vec3& value);
    inline const Vec3& GetScale() const
    {
      return _scale();
    }

    void SetRotation(const float& angleX, const float& angleY, const float& angleZ);
    void SetRotation(const Vec3& value);
    inline const Vec3& GetRotation() const
    {
      return _rotation();
    }

    void SetParent(Element* parent);
    inline const Element* GetParent() const
    {
      return _parent;
    }
    void ClearParent();

    inline Matrix GetMatrix() const
    {
      return _matrix;
    }

    virtual void Update(const Matrix& matrix);
    virtual void DrawElement();

    virtual inline Mesh* AsMesh() 
    {
      return NULL;
    }

    virtual inline Model* AsModel() 
    {
      return NULL;
    }

    virtual inline Group* AsGroup() 
    {
      return NULL;
    }

    virtual void SetVertexShader(UINT passNum, const wchar_t* path, VertexShader::TypeVertexShader type = VertexShader::VS_4_0, const char* funcName = defaultVSfunction);
    virtual void SetPixelShader(UINT passNum, const wchar_t* path, PixelShader::TypePixelShader type = PixelShader::PS_4_0, const char* funcName = defaultPSfunction);

    Buffer::BufferType GetBufferType() const
    {
      return _type;   
    }

    inline ShaderPassMap GetShaderPasses() 
    { 
      return _shaderPasses; 
    }

  protected:
    
    enum Message
    {
      UPDATE_BUFFER_TYPE = 0
    };
    virtual void updateParent(Message msg);

    virtual void setBufferType(Buffer::BufferType BT_Type);

    virtual void updateParameters();
    virtual void draw();
    virtual void returnParameters();

    Properties::Property<bool> _visible;
    Properties::Property<Vec3> _position;
    Properties::Property<Vec3> _scale;
    Properties::Property<Vec3> _rotation;

    Matrix              _matrix;
    Element*            _parent;

    ShaderPassMap       _shaderPasses;
    bool                _protectedShader;

    Buffer::BufferType      _type;
    Buffer::ConstantBuffer* _wBuffer;

    friend Group;
  };

  typedef std::vector<Element*> ElementVec;
}

#endif