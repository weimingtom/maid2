#ifndef maid2_graphics_inputlayout_h
#define maid2_graphics_inputlayout_h

/*!
    @file
    @brief  InputLayout
 */


#include"../config/define.h"
#include"../auxiliary/debug/assert.h"
#include"../auxiliary/globalpointer.h"
#include"../auxiliary/jobcachetemplate.h"
#include"../auxiliary/string.h"

#include"graphicsobjecttemplate.h"
#include"graphicscore.h"
#include"shadercompiler.h"

#include<vector>

namespace Maid
{
  namespace KEEPOUT
  {
    class ilInput : public IJobInput
    {
    public:
      ilInput(){}
      ilInput( const String& str, const std::vector<Graphics::INPUT_ELEMENT>& elem, const std::vector<unt08>& code, const Graphics::SPDEVICE& p )
        :Key(str), Element(elem), ByteCode(code), pDevice(p){}
      String Key;
      std::vector<Graphics::INPUT_ELEMENT>  Element;
      std::vector<unt08> ByteCode;
      Graphics::SPDEVICE pDevice;
    };

    inline bool operator < ( const ilInput& lhs, const ilInput& rhs ) 
    {
      return lhs.Key < rhs.Key;
    }

    class ilOutput : public IJobOutput
    {
    public:
      Graphics::SPINPUTLAYOUT pLayout;
    };

    class ilFunction : public IJobFunction
    {
    public:
      void Execute( const IJobInput& Input, IJobOutput& Output )
      {
        const ilInput&  in  = static_cast<const ilInput&>(Input);
        ilOutput& out = static_cast<ilOutput&>(Output);        

        out.pLayout = in.pDevice->CreateInputLayout(
            &(in.Element[0]), in.Element.size(),
            &(in.ByteCode[0]),in.ByteCode.size()
            );
      }
    };
  }


  class InputLayout 
    :public IInputLayout
    ,private GlobalPointer<GraphicsCore>
  {
  public:
    void Create( const Graphics::INPUT_ELEMENT* Element, int Count, const String& ShaderCode );
    bool IsCompiling()const;

  private:
    std::vector<Graphics::INPUT_ELEMENT>  m_Element;
    ShaderCompiler  m_Compiler;
    typedef JobCacheTemplate<KEEPOUT::ilInput,KEEPOUT::ilFunction,KEEPOUT::ilOutput> CACHE;
    CACHE m_Cache;
  };
}

#endif