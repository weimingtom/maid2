#ifndef maid2_sound_core_ibuffer_h
#define maid2_sound_core_ibuffer_h

#include"../../config/define.h"

#include"../common.h"

#include<boost/smart_ptr.hpp>

namespace Maid { namespace Sound {

  struct CREATEBUFFERPARAM
  {
    size_t    Length;   //  バッファサイズ（バイト単位）
    PCMFORMAT Format;
  };


  /*!
      @class  IBuffer ibuffer.h
      @brief  PCMサウンドバッファ
  \n          PCM専用になってます。MIDIはできません
  */
  class IBuffer
	{
	public:
    IBuffer( const CREATEBUFFERPARAM& p ):m_Pamram(p){}
    virtual ~IBuffer(){}

    struct LOCKDATA
    {
      void*   pData1;
      size_t  Data1Length;
      void*   pData2;
      size_t  Data2Length;
    };

    //! バッファをメモリ上にマップする
    /*!
 	      @param	begin   [i ]	マップする先頭位置（バイト単位）
 	      @param	Length	[i ]	ロックする長さ（バイト単位）
 	      @param	data    [ o]	マップされた情報が入ってる
     */
    virtual void Lock( size_t begin, size_t Length, LOCKDATA& data )=0;

    //! マップされていた領域を戻す
    /*!
 	      @param	data    [ o]	Lock()時に取得したデータ
     */
    virtual void Unlock( LOCKDATA& data )=0;

    //! バッファの再生を開始する
    /*!
 	      @param	IsLoop    [i ]	ループ再生するか？
     */
    virtual void Play( bool IsLoop )=0;

    //! 再生位置を変更
    /*!
 	      @param	pos    [i ]	場所（バイト単位）
     */
    virtual void SetPosition( size_t pos )=0;


    //! 再生ボリュームを変更
    /*!
        相対デシベルで指定します -100.0 等

 	      @param	vol    [i ]	 デシベル単位
     */
    virtual void SetVolume( double vol )=0;

    //! バッファの再生を停止する
    /*!
     */
    virtual void Stop()=0;

    //! バッファが再生しているか？
    /*!
        @return 再生しているなら true
     */
    virtual bool IsPlay() const=0;

    //! 現在の再生カーソル位置の取得
    /*!
     */
    virtual size_t GetPlayPosition()const=0;

    //! 現在の書き込めるカーソル位置の取得
    /*!
     */
    virtual size_t GetWritePosition()const=0;

    //! 現在のボリュームを取得
    /*!
     */
    virtual double GetVolume()const=0;

    const CREATEBUFFERPARAM& GetParam() const { return m_Pamram; }
  private:
    CREATEBUFFERPARAM m_Pamram;
  };

  typedef	boost::shared_ptr<IBuffer>	SPBUFFER;

}}


#endif
