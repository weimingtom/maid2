/*!
    @file
    @brief  平面
 */

#ifndef maid2_graphics_imagefile_surface_h
#define maid2_graphics_imagefile_surface_h


#include"../../config/define.h"
#include"../../config/typedef.h"
#include"../../auxiliary/mathematics/size.h"
#include"../../auxiliary/mathematics/point.h"
#include"../pixelformat.h"
#include"../color.h"

#include<vector>

namespace Maid
{
  class Surface
  {
  public:
    Surface();
		void Create( const SIZE2DI& size, PIXELFORMAT fmt );
		void Create( const SIZE2DI& size, PIXELFORMAT fmt, int Pitch );
		void Destroy();

		const SIZE2DI&    GetSize() const;
		const PIXELFORMAT GetPixelFormat() const;
    int   GetPitch() const;

		void* GetPlanePTR();
		void* GetCLUTPTR();

		void* GetLinePTR( int y );
		void* GetPixelPTR( const POINT2DI& pos );
		void* GetColorPTR( int index );

    COLOR_R32G32B32A32F GetPixel( const POINT2DI& pos );
    void SetPixel( const POINT2DI& pos, const COLOR_R32G32B32A32F& col );

    bool IsEmpty() const;

    void Fill( const COLOR_R32G32B32A32F& Color );
	private:
		std::vector<unt08>	m_Plane;
		std::vector<unt08>	m_CLUT;

		SIZE2DI		  m_Size;
		PIXELFORMAT m_PixelFormat;
    int  m_Pitch;
    bool m_IsCreated;
	};
}

#endif

