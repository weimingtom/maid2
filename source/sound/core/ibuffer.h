#ifndef maid2_sound_core_ibuffer_h
#define maid2_sound_core_ibuffer_h

#include"../../config/define.h"

#include"common.h"

#include<boost/smart_ptr.hpp>

namespace Maid { namespace Sound {

  struct CREATEBUFFERPARAM
  {
    size_t    Length;   //  バッファサイズ（バイト単位）
    PCMFORMAT Format;
  };

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

    virtual void Lock( size_t begin, size_t Length, LOCKDATA& data )=0;
    virtual void Unlock( LOCKDATA& data )=0;

    virtual void Play( bool IsLoop )=0;
    virtual void SetPosition( size_t pos )=0;

    virtual void SetVolume( float vol )=0;
    virtual void Stop()=0;

    virtual bool IsPlay() const=0;
    virtual size_t GetPlayPosition()const=0;
    virtual size_t GetWritePosition()const=0;

    const CREATEBUFFERPARAM& GetParam() const { return m_Pamram; }
  private:
    CREATEBUFFERPARAM m_Pamram;
  };

  typedef	boost::shared_ptr<IBuffer>	SPBUFFER;

}}


#endif
