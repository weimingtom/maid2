#ifndef maid2_framework_movie_core_isample_h
#define maid2_framework_movie_core_isample_h


#include"../../../config/define.h"
#include"../../../auxiliary/memorybuffer.h"
#include"../../../auxiliary/mathematics.h"
#include"../../../sound/common.h"

#include <boost/smart_ptr.hpp>
#include <vector>
#include <list>



namespace Maid { namespace Movie { 

  class ISample
  {
  public:
    virtual ~ISample(){}
  };

  typedef boost::shared_ptr<ISample> SPSAMPLE;

  class SampleFrame
    : public ISample
  {
  public:
    SIZE2DI       SizeY;        //  YUV の Y 平面の大きさ
    int           PitchY;       //  Y平面のピッチ
    std::vector<unt08>  BitsY;  //  Y平面のデータ

    SIZE2DI       SizeUV;       //  YUV の UV 平面の大きさ
    int           PitchUV;      //  UV平面のピッチ
    std::vector<unt08>  BitsU;  //  U平面のデータ
    std::vector<unt08>  BitsV;  //  V平面のデータ
  };

  typedef boost::shared_ptr<SampleFrame> SPSAMPLEFRAME;


  class SamplePCM
    : public ISample
  {
  public:
    typedef std::vector<float32>  BUFFER;
    std::vector<BUFFER> Data; //  １チャンネルごとに並んでる
  };

  typedef boost::shared_ptr<SamplePCM> SPSAMPLEPCM;


  class ISampleFormat
  {
  public:
    virtual ~ISampleFormat(){}
  };

  typedef boost::shared_ptr<ISampleFormat> SPSAMPLEFORMAT;


  class SampleFormatFrame
    : public ISampleFormat
  {
  public:
    SIZE2DI EncodedSize;    //  データとして入っている大きさ
    SIZE2DI DisplaySize;    //  表示しなくてはいけない大きさ
    POINT2DI DisplayOffset; //  配置座標
    int FpsNumerator;       //  FPS の分子
    int FpsDenominator;     //  FPS の分母
    int AspectNumerator;    //  アスペクト の分子
    int AspectDenominator;  //  アスペクト の分母

    enum PIXELFORMAT
    {
      PIXELFORMAT_YUV444,
      PIXELFORMAT_YUV422,
      PIXELFORMAT_YUV420,
    };

    PIXELFORMAT PixelFormat;
  };

  typedef boost::shared_ptr<SampleFormatFrame> SPSAMPLEFORMATFRAME;


  class SampleFormatPCM
    : public ISampleFormat
  {
  public:
    PCMFORMAT Format;
  };

  typedef boost::shared_ptr<SampleFormatPCM> SPSAMPLEFORMATPCM;


}}

#endif
