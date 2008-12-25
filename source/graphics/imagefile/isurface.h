/*!
    @file
    @brief  平面
 */

#ifndef maid2_graphics_imagefile_isurface_h
#define maid2_graphics_imagefile_isurface_h


#include"../../config/define.h"
#include"../../config/typedef.h"
#include"../../auxiliary/mathematics/size.h"
#include"../../auxiliary/mathematics/point.h"
#include"../pixelformat.h"
#include"../color.h"

#include<vector>
#include<boost/smart_ptr.hpp>

namespace Maid
{
  class ISurface
  {
  public:
    virtual ~ISurface(){}

		virtual void Create( const SIZE2DI& size, PIXELFORMAT fmt )=0;
		virtual void Destroy()=0;

		virtual SIZE2DI    GetSize() const=0;
		virtual PIXELFORMAT GetPixelFormat() const=0;
    virtual int   GetPitch() const=0;
    virtual bool  IsEmpty() const=0;

		virtual void* GetPlanePTR()=0;
		virtual void* GetCLUTPTR()=0;


		void* GetLinePTR( int y );
		void* GetPixelPTR( const POINT2DI& pos );
		void* GetColorPTR( int index );


    #pragma  COMPILERMSG( "この３つは別途転送クラスを作って、そっちで実装すべきだよな" )
    COLOR_R32G32B32A32F GetPixel( const POINT2DI& pos );
    void SetPixel( const POINT2DI& pos, const COLOR_R32G32B32A32F& col );

    void Fill( const COLOR_R32G32B32A32F& Color );
	};

  typedef	boost::shared_ptr<ISurface>	SPSURFACE;

}

#endif

