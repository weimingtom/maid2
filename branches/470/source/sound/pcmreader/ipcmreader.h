#ifndef maid2_sound_pcmreader_ipcmreader_h
#define maid2_sound_pcmreader_ipcmreader_h 


#include"../../config/define.h"
#include"../common.h"


#include<boost/smart_ptr.hpp>

namespace Maid
{
  /*!
      @class  IPCMReader ipcmreader.h
      @brief  各種データからＰＣＭデータに変換させる基本クラス
  */
  class IPCMReader
  {
  public:
    virtual ~IPCMReader(){};

    enum INITIALIZERESULT
    {
      INITIALIZERESULT_OK =0,
      INITIALIZERESULT_ERROR,
    };

    //! デコーダの初期化
    /*!
 	      @return 初期化に成功したら INITIALIZERESULT_OK
 	              失敗なら INITIALIZERESULT_ERROR
     */
    virtual INITIALIZERESULT Initialize()=0;


    //! デコーダの開放
    /*!
     */
    virtual void Finalize()=0;


    //! 指定した分だけデコードする
    /*!
 	      @param	pDst  [i ]	書き込み先
 	      @param	size	[i ]	書き込む長さ
     */
    virtual size_t  Read( void* pDst, size_t size )=0;


    //! デコード開始位置を変更する
    /*!
        @param  Offset  [i ]  開始位置（バイト単位）
     */
    virtual void    SetPosition( size_t Offset )=0;

    //! 現在の位置を取得する
    /*!
        @return  位置（バイト単位）
     */
    virtual size_t  GetPosition()		const=0;

    //! デコードできる長さを取得する
    /*!
        @return  長さ（バイト単位）
     */
    virtual size_t  GetLength()			const=0;

    //! 設定されているデータフォーマットを調べる
    /*!
        @return  フォーマット
     */
    virtual PCMFORMAT GetFormat() const=0;
  };

  typedef	boost::shared_ptr<IPCMReader>	SPPCMREADER;

}

#endif