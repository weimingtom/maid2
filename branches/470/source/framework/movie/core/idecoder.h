/*!
	@file
	@brief デコードの状態を管理するクラス
 */

#ifndef maid2_framework_movie_core_idecoder_h
#define maid2_framework_movie_core_idecoder_h


#include"../../../config/define.h"
#include"../../../auxiliary/functionresult.h"

#include"isample.h"
#include"istoragesample.h"

#include<boost/smart_ptr.hpp>

#include<list>

namespace Maid { namespace Movie { 

  struct DECODERSAMPLE
  {
    double BeginTime; //  開始時間（単位は秒）
    double EndTime;   //  終了時間（単位は秒）
    SPSAMPLE pSample;

    //  BeginTime <= x < EndTime の範囲になる
    DECODERSAMPLE()
      :BeginTime(0)
      ,EndTime(0)
    {
    }
  };

  typedef std::list<DECODERSAMPLE> DECODERSAMPLELIST;

  #define DECODERID_FRAME1 MAIDTEXT("FRAME1")
  #define DECODERID_PCM1 MAIDTEXT("PCM1")

  class IDecoder
  {
  public:
    virtual ~IDecoder(){}

    virtual void Initialize()=0;
    virtual void Finalize()=0;

    virtual void AddSource( const SPSTORAGESAMPLE& buffer )=0;


    virtual void FlushSample( double time, DECODERSAMPLELIST& pOut )=0;


    //  デコード前のデータが多すぎて AddData()をちょっと待って欲しいか
    virtual bool IsSourceFull() const= 0;

    //  デコード後のデータを大量に持っているか？
    virtual bool IsSampleFull() const= 0;

    //  現在デコードし終わった時間
    virtual double GetTime() const = 0;

    //  指定した時間までデコードをスキップする
    virtual void BeginSkipMode( double targettime )=0;

    //  デコードが全部終了したか？
    //  キャッシュが残っててもfalseです
    virtual bool IsDecodeEnd() const=0;
  };

  typedef boost::shared_ptr<IDecoder> SPDECODER;

}}

#endif