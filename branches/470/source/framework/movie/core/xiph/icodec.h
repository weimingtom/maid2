#ifndef maid2_framework_movie_core_xiph_icodec_h
#define maid2_framework_movie_core_xiph_icodec_h

/*

  ogg の内部構造の解説

  ogg はファイルフォーマットみたいなもので RIFF だと思うとわかりやすい

  v１つあたりが１画面
  s１つあたりがサウンド単位

  v0,v1,v2,v3    s0,s1,s2,s3




  それぞれの単位に ogg packet(p) がついてる

  p+v0,p+v1,p+v2,p+v3  p+s0,p+s1,p+s2,p+s3




  packet の塊が ogg page

  page[pv0,pv1]page[pv2,pv3,,,]   page[ps0,ps1,ps2,ps3,,,]



  ogg page の塊がoggコンテナ（実ファイル）となっています
  pageの種類を区別するのにシリアルナンバーが振ってあります
  oggが勝手に割り振るので、固定してはいけません

  page,page,page,,,,



  ogg page を、パケット単位でアクセスできるようにしてるのが ogg stream

  stream.PageIn(page[pv0,pv1]);
  stream.PageIn(page[pv2,pv3]);
  stream[0] == pv0;
  stream[1] == pv1;
  stream[2] == pv2;
  stream[3] == pv3;


  そのた細かいことは
  http://d.hatena.ne.jp/Ko-Ta/searchdiary?of=7&word=*[theora]
  を参照！
*/


#include"../../../config/define.h"

#include"oggpacket.h"
#include"../isample.h"

#include <boost/smart_ptr.hpp>
#include <string>

namespace Maid { namespace Movie { namespace Xiph {

  class ICodec
  {
  public:
    virtual ~ICodec(){}

    virtual void Initialize()=0;
    virtual void Finalize()=0;

    //  デコード
    virtual void Decode( const OggPacket& NewPacket, SPSAMPLE& pOut )=0;
    virtual void Skip( const OggPacket& NewPacket )=0;

    //  現在の時間の取得
    virtual double GetTime()=0;

    //  このパケットをデコードしたときにすすむ時間を求める
    virtual double CalcPacketLength( const OggPacket& NewPacket )const=0;

    virtual std::string GetDebugString( const OggPacket& NewPacket )const=0;
  };

  typedef boost::shared_ptr<ICodec> SPCODEC;
}}}

#endif
