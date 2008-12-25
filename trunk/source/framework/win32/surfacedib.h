/*!
 	@file
 	@brief	描画平面クラス DIB Ver.
 */

#ifndef framework_win32_surfacedib_h
#define framework_win32_surfacedib_h

#include"../../config/define.h"
#include"../../config/Win32.h"

#include"../../graphics/imagefile/isurface.h"

namespace Maid
{
  class SurfaceDIB : public ISurface
  {
  public:
    SurfaceDIB();
    virtual ~SurfaceDIB();

		virtual void Create( const SIZE2DI& size, PIXELFORMAT fmt );
		virtual void Destroy();

		virtual SIZE2DI    GetSize() const;
		virtual PIXELFORMAT GetPixelFormat() const;
    virtual int   GetPitch() const;
    virtual bool  IsEmpty() const;

		virtual void* GetPlanePTR();
		virtual void* GetCLUTPTR();

    HBITMAP GetHBITMAP() const;

  private:
    HBITMAP     m_hDIB;   //!<	DIBハンドル
    void*       m_pPlane; //!<	展開されるプレーン
    SIZE2DI     m_Size;   //!<	m_pPlaneの横幅
    PIXELFORMAT	m_Format;	//!<	m_pPlaneのピクセルフォーマット
    int         m_Pitch;  //!<	横１ラインのバイト数
  };

}

#endif
