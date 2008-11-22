/*!
 	@file
 	@brief	テクスチャバッファベースクラス
 */

#ifndef maid2_graphic_core_itexturebuffer_h
#define maid2_graphic_core_itexturebuffer_h

#include"../../config/define.h"
#include"../../auxiliary/mathematics.h"

#include<boost/smart_ptr.hpp>

#include"../pixelformat.h"
#include"isurfacebuffer.h"



namespace Maid
{
  /*!
      @class	ITextureBuffer itexturebuffer.h
      @brief	テクスチャバッファの基本クラス
   */
  class ITextureBuffer
  {
  public:
    virtual ~ITextureBuffer(){}	//!<	デストラクタ

    //! テクスチャの大きさの取得
    /*! 
        @return	大きさ
     */
    virtual SIZE2DI GetSize() const=0;

    //! ピクセルフォーマットの取得
    /*! 
        @return	ピクセルフォーマット
     */
    virtual PIXELFORMAT GetPixelFormat() const=0;

    //! サーフェスが全部で何枚あるか
    /*! 
        @return	枚数
     */
    virtual int GetSurfaceLevel() const=0;


    //! 指定したサーフェスの取得
    /*! 
        @return	指定したサーフェス
     */
    virtual ISurfaceBuffer& GetSurface( int Level ) = 0;
  };

  typedef boost::shared_ptr<ITextureBuffer> SPTEXTUREBUFFER;
}

#endif

