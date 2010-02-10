/*!
	@file
	@brief IDecoderを一括して管理するクラス
 */

#ifndef maid2_framework_movie_idecodermanager_h
#define maid2_framework_movie_idecodermanager_h


#include"../../config/define.h"
#include"../../auxiliary/functionresult.h"
#include"../../auxiliary/string.h"

#include"idecoder.h"
#include"isample.h"

#include<boost/smart_ptr.hpp>


namespace Maid { namespace Movie { 



  class IDecoderManager
  {
  public:
    virtual ~IDecoderManager(){}

    virtual FUNCTIONRESULT Initialize()=0;

    //! デコードするための準備が整ったか？
    /*!
        @return true  準備ＯＫ
                false まだまだ AddSource() が欲しい
    */
    virtual bool Setuped()const=0;

    //! デコードするためのデータを送る
    /*!
        内部でそれぞれのコーデックに振り分けます

        @param pData  [i ]  データ
        @param Length [i ]  長さ
    */
    virtual void AddSource( const SPSTORAGESAMPLE& buffer )=0;

    //! 内部で動いているデコーダのフォーマットを取得する
    /*!
        @param id   [i ]  欲しい種類（"FRAME1","PCM1", 等）
        @param pOut [ o]  フォーマット
    */
    virtual void GetFormat( const String& id, SPSAMPLEFORMAT& pOut )const=0;


    //! デコードされたデータを受け取る
    /*!
        @param id   [i ]  欲しい種類（"FRAME1","PCM1", 等）
        @param time [i ]  欲しい時間以降(<=)のデータ
        @param pOut [ o]  デコードされたデータ
    */
    virtual void FlushSample( const String& id, double time, DECODERSAMPLELIST& pOut )=0;

    //! 現在のデコード時間。複数ある場合は一番遅れてるもの
    /*!
        @return デコード時間
    */
    virtual double GetTime() const= 0;

    //! デコード前のデータが全部いっぱいかどうか。複数ある場合はすべていっぱいか？
    /*!
        @return true なら満タン。falseならどこかがない
    */
    virtual bool IsSourceFull() const=0;

    //! デコード後のデータが全部いっぱいかどうか。複数ある場合はすべていっぱいか？
    /*!
        @return true なら満タン。falseならどこかがない
    */
    virtual bool IsSampleFull() const= 0;

    //! 指定した時間にくるまで処理をスキップする状態になる
    /*!
        @return time  [i ]  目標時間
    */
    virtual void BeginSkipMode( double time ) = 0;

    //! すべてのデコードが終了したか？
    /*!
        @return true なら終了。falseなら続きがある
    */
    virtual bool IsDecodeEnd() const=0;

  };

  typedef boost::shared_ptr<IDecoderManager> SPDECODERMANAGER;

}}

#endif