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
	    if( lhs.Element.size() < rhs.Element.size() ) { return true; }
	    else if( lhs.Element.size() > rhs.Element.size() ) { return false; }

      for( int i=0; i<(int)lhs.Element.size(); ++i )
      {
        const Graphics::INPUT_ELEMENT& a = lhs.Element[i];
        const Graphics::INPUT_ELEMENT& b = rhs.Element[i];

	      if( a.SemanticName < b.SemanticName ) { return true;  }
	      else if( a.SemanticName > b.SemanticName ) { return false; }

	      if( a.SemanticIndex < b.SemanticIndex ) { return true;  }
	      else if( a.SemanticIndex > b.SemanticIndex ) { return false; }

	      if( a.Type < b.Type ) { return true;  }
	      else if( a.Type > b.Type ) { return false; }

	      if( a.SlotNo < b.SlotNo ) { return true;  }
	      else if( a.SlotNo > b.SlotNo ) { return false; }

	      if( a.Offset < b.Offset ) { return true;  }
	      else if( a.Offset > b.Offset ) { return false; }

	      if( a.Method < b.Method ) { return true;  }
	      else if( a.Method > b.Method ) { return false; }
      }

      return false; 
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