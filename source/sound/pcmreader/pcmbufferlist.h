#ifndef maid2_sound_pcmreader_pcmbufferlist_h
#define maid2_sound_pcmreader_pcmbufferlist_h 

#include"../../config/define.h"
#include"../../auxiliary/memorybuffer.h"
#include"../../auxiliary/thread.h"
#include"ipcmreader.h"

#include<set>

namespace Maid
{
  /*!
      @class  PCMBufferList pcmbufferlist.h
      @brief  ばらばらになってるPCMを連続したものとして扱うクラス
  */
  class PCMBufferList
    : public IPCMReader
  {
  public:
    PCMBufferList( const PCMFORMAT& fmt );

    virtual INITIALIZERESULT Initialize();
    virtual void Finalize();

    virtual size_t  Read( void* pDst, size_t size );
    virtual void    SetPosition( size_t Offset );
    virtual size_t  GetPosition()		const;
    virtual size_t  GetLength()			const;

    virtual PCMFORMAT GetFormat() const;

    void Create( size_t time, const void* pData, size_t Size );
    void ClearData( size_t time );

  private:
    ThreadMutex m_Mutex;

    struct BUFFERINFO
    {
      size_t       Time;
      MemoryBuffer Buffer;
    };

    typedef boost::shared_ptr<BUFFERINFO> SPBUFFERINFO;
    struct SPBUFFERINFO_less
    {
      bool operator()(const SPBUFFERINFO& lhs, const SPBUFFERINFO& rhs ) const
      {
        return lhs->Time < rhs->Time;
      }
    };


    typedef std::set<SPBUFFERINFO,SPBUFFERINFO_less> BUFFERLIST;
    BUFFERLIST m_BufferList;

    const PCMFORMAT m_Format;
    size_t  m_Position;
  };

  typedef	boost::shared_ptr<PCMBufferList>	SPPCMBUFFERLIST;

}

#endif