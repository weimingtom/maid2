#ifndef ioggbuffer_h
#define ioggbuffer_h


#include"../../source/config/define.h"
#include"../../source/graphics/surface.h"

#include <boost/smart_ptr.hpp>


class IOggBuffer
{
public:
  double BeginTime;
};

typedef boost::shared_ptr<IOggBuffer> SPOGGBUFFER;


class OggBufferImage : public IOggBuffer
{
public:
  SIZE2DI SizeY;  //  YUV の Y 平面の大きさ
  int     PitchY; //  Y平面のピッチ
  std::vector<unt08>  BitsY;  //  Y平面のデータ

  SIZE2DI SizeUV;  //  YUV の UV 平面の大きさ
  int     PitchUV; //  UV平面のピッチ
  std::vector<unt08>  BitsU;  //  U平面のデータ
  std::vector<unt08>  BitsV;  //  V平面のデータ
};


class OggBufferPCM : public IOggBuffer
{
public:
  std::vector<unt08>  Buffer;
};


#endif
