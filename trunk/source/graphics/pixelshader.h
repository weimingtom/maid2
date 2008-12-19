#ifndef maid2_graphics_pixelshader_h
#define maid2_graphics_pixelshader_h

/*!
    @file
    @brief  pixelshader
 */


#include"../config/define.h"
#include"../auxiliary/debug/assert.h"
#include"../auxiliary/globalpointer.h"

#include"../storage/storageobjecttemplate.h"

#include"graphicsobjecttemplate.h"
#include"core/core.h"

namespace Maid
{

  class PixelShaderBase
      :public IPixelShader
      ,public GlobalPointer<Graphics::Core>
  {
  public:
    struct OPTION
    {
      Graphics::hPixelShader hObject;
    };

  protected:
		void CreateOption( const IStorageFunction* param, OPTION& option  );
		void LocalSetup( const OPTION& option  );
		void LocalClear();
		void DeleteOption( const OPTION& option );
    IStorageFunction* CreateFunction( const String& id );
    String ConvertID( const String& id );
  };	
  
  typedef StorageObjectTemplate<String,PixelShaderBase::OPTION,PixelShaderBase> PixelShader;
	
}

#endif