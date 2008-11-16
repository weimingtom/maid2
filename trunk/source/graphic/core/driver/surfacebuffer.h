#ifndef maid2_graphic_core_surfacebuffer_h
#define maid2_graphic_core_surfacebuffer_h

#include"../../../config/define.h"

#include<boost/smart_ptr.hpp>

#include"../../../auxiliary/mathematics.h"
#include"../../pixelformat.h"
#include"../../color.h"

#include"surfacebufferlockinfo.h"

namespace Maid
{
  /*!
      @class	SurfaceBuffer surfacebuffer.h
      @brief	サーフェスバッファの基本クラス
   */
  class SurfaceBuffer
  {
  public:
    virtual ~SurfaceBuffer(){}	//!<	デストラクタ

    //! サーフェスの大きさの取得
    /*! 
        @return	サーフェスの横幅、縦幅
     */
    virtual SIZE2DI GetSize() const=0;


    //! ピクセルフォーマットの取得
    /*! 
       @return	ピクセルフォーマット
     */
    virtual PIXELFORMAT GetPixelFormat() const=0;

    //! サーフェスのロック
    /*!
      サーフェスの内部アドレスにアクセスできるようにします
    \n	Lock() を呼び出した場合は必ず Unlock() も呼び出すようにすること
    \n	SurfaceBufferLock を使うと便利です
  	 
        @param	info	[ o]	ロックに成功するとこれに各種情報が入る
     */
    virtual void Lock( SurfaceBufferLockInfo& info )=0;

    //! サーフェスのアンロック
    /*!
        Lock()　でロックしたサーフェスを再び使えるようにアンロックします
    \n	SurfaceBufferLock を使うと便利です
     */
    virtual void Unlock()=0;

  };

  typedef boost::shared_ptr<SurfaceBuffer> SPSURFACEBUFFER;



  /*!
    @class	SurfaceBufferLock surfacebuffer.h
    @brief	SurfaceBuffer::Lock(), SurfaceBuffer::Unlock() を
  \n			自動的に行うクラス
   
    @code
    {
      SPSURFACEBUFFER pSurface( new ISurfaceXXXXX );
      SurfaceBufferLockInfo info;

      SurfaceBufferLock lock( pSurface.get(), info  );	//	これでロックされる

      //	ここでいろいろ行う	

    }	//	lock のデストラクタで pSurface->Unlock() される
  	
    @endcode
   */
  class SurfaceBufferLock
  {
  public:

    SurfaceBufferLock( SurfaceBuffer& Surface, SurfaceBufferLockInfo& info )
      : m_Surface( Surface )
    {
      m_Surface.Lock( info );
    }

    ~SurfaceBufferLock()	//!<	デストラクタ
    {
      m_Surface.Unlock();
    }
  private:
    SurfaceBuffer& m_Surface;
  };
}


#endif