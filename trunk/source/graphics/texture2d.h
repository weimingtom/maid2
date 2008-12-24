#ifndef maid2_graphics_texture2d_h
#define maid2_graphics_texture2d_h

/*!
    @file
    @brief  2Dテクスチャ
 */


#include"../config/define.h"
#include"../auxiliary/globalpointer.h"
#include"../auxiliary/jobcachetemplate.h"
#include"../auxiliary/string.h"

#include"graphicsobjecttemplate.h"
#include"core/core.h"

namespace Maid
{
  namespace KEEPOUT
  {
    class tex2dInput : public IJobInput
    {
    public:
      tex2dInput(){}
      tex2dInput( const String& name, const Graphics::Core& c )
        :FileName(name), Core(c){}
      String FileName;
      const Graphics::Core& Core;
    };

    inline bool operator < ( const tex2dInput& lha, const tex2dInput& rha ) 
    {
      return lha.FileName < rha.FileName; 
    }

    class tex2dOutput : public IJobOutput
    {
    public:
      Graphics::SPTEXTURE2D pTexture;
    };

    class tex2dFunction : public IJobFunction
    {
    public:
      void Execute( const IJobInput& Input, IJobOutput& Output );
    };
  }


  class Texture2D 
    :public ITexture2D
    ,public GlobalPointer<Graphics::Core>
  {
  public:
    void LoadFile( const String& FileName );
    bool IsCompiling()const;

  private:
    typedef JobCacheTemplate<KEEPOUT::tex2dInput,KEEPOUT::tex2dFunction, KEEPOUT::tex2dOutput> CACHE;
    CACHE m_Cache;
  };
}

#endif