#ifndef ioggbuffer_h
#define ioggbuffer_h


#include"../../source/config/define.h"
#include"../../source/graphics/surface.h"
#include"../../source/auxiliary/buffer.h"

#include <boost/smart_ptr.hpp>




class IOggDecodedBuffer
{
public:
};

typedef boost::shared_ptr<IOggDecodedBuffer> SPOGGDECODEDBUFFER;


class OggDecodedBufferImage : public IOggDecodedBuffer
{
public:
  Maid::SIZE2DI SizeY;  //  YUV の Y 平面の大きさ
  int           PitchY; //  Y平面のピッチ
  std::vector<unt08>  BitsY;  //  Y平面のデータ

  Maid::SIZE2DI SizeUV;  //  YUV の UV 平面の大きさ
  int           PitchUV; //  UV平面のピッチ
  std::vector<unt08>  BitsU;  //  U平面のデータ
  std::vector<unt08>  BitsV;  //  V平面のデータ
};

typedef boost::shared_ptr<OggDecodedBufferImage> SPOGGDECODEDBUFFERIMAGE;

class OggDecodedBufferPCM : public IOggDecodedBuffer
{
public:
  Maid::Buffer  WaveData;
};


typedef boost::shared_ptr<OggDecodedBufferPCM> SPOGGDECODEDBUFFERPCM;


struct OGGDECODEDINFO
{
  double BeginTime; //  開始時間（単位は秒）
  double EndTime;   //  終了時間（単位は秒）
  SPOGGDECODEDBUFFER pBuffer;

  //  BeginTime <= x < EndTime の範囲になる
};

#endif
