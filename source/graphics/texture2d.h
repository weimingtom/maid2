﻿#ifndef maid2_graphics_texture2d_h
#define maid2_graphics_texture2d_h

/*!
    @file
    @brief  2Dテクスチャ
 */


#include"../config/define.h"
#include"../auxiliary/globalpointer.h"
#include"../auxiliary/jobcachetemplate.h"
#include"../auxiliary/string.h"
#include"../auxiliary/functionresult.h"

#include"texture2dbase.h"

namespace Maid
{
  namespace KEEPOUT
  {
    class tex2dInput : public IJobInput
    {
    public:
      tex2dInput(){}
      tex2dInput( const String& name, GraphicsCore* c )
        :FileName(name), Core(c){}
      String FileName;
      GraphicsCore* Core;
    };

    inline bool operator < ( const tex2dInput& lha, const tex2dInput& rha ) 
    {
      return lha.FileName < rha.FileName; 
    }

    class tex2dOutput : public IJobOutput
    {
    public:
      tex2dOutput()
        :ImageSize(0,0)
        ,CreateSize(0,0)
        ,TextureSize(0,0)
      {}

      Graphics::SPTEXTURE2D pTexture;
      Graphics::SPMATERIAL  pMaterial;
      SIZE2DI ImageSize;    //  元画像の大きさ
      SIZE2DI CreateSize;   //  実際に作った大きさ
      SIZE2DI TextureSize;  //  テクスチャバッファの大きさ
      PIXELFORMAT ImageFormat;  //  画像ファイルのフォーマット
    };

    class tex2dFunction : public IJobFunction
    {
    public:
      void Execute( const IJobInput& Input, IJobOutput& Output );

    private:
      typedef std::map<String,String> CONVERTSETTING;

      void ReadConvertSetting( const String& filename, CONVERTSETTING& out );

      FUNCTIONRESULT LoadImage( const CONVERTSETTING& Element, std::vector<SurfaceInstance>& dst );
      FUNCTIONRESULT ConvertSubResource( const std::vector<SurfaceInstance>& src, std::vector<SurfaceInstance>& dst );
      void GenerateSublevel( std::vector<SurfaceInstance>& target, int level );

      void ReadName( const String& Tag, String& Element, String& Value );
      FUNCTIONRESULT LoadImageFile( const String& filename, std::vector<SurfaceInstance>& dst );
      int CalcMipLevels( const GraphicsCore& core, const CONVERTSETTING& setting, const SIZE2DI& size ) const;
    };
  }


  class Texture2D 
    :public Texture2DBase
    ,private GlobalPointer<GraphicsCore>
  {
  public:
    void LoadCommand( const String& Command );
    void LoadFile( const String& FileName );
    void LoadFile( const String& FileName, int MipmapLevel );
    bool IsLoading()const;
    void Delete();

    const SIZE2DI& GetImageSize()const;

    bool IsEmpty() const;
    String GetLoadText() const;

  private:
    void SendCommand( const String& Command );

  private:
    typedef JobCacheTemplate<KEEPOUT::tex2dInput,KEEPOUT::tex2dFunction, KEEPOUT::tex2dOutput> CACHE;
    CACHE m_Cache;

    String  m_LoadText;
  };


}

#endif