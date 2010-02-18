#ifndef maid2_sound_pcmreader_custompcmreader_h
#define maid2_sound_pcmreader_custompcmreader_h 


#include"../../config/define.h"
#include"../../config/typedef.h"
#include"../common.h"
#include"ipcmreader.h"


#include<boost/smart_ptr.hpp>

#include<vector>

namespace Maid
{
  /*!
      @class  CustomPCMReader custompcmreader.h
      @brief  IPCMReader を指定したデコード順に書き換えるクラス
  */
  class CustomPCMReader
  {
  public:
    struct JUMPPOINT
    {
	    size_t	Boot;
	    size_t	Jump;
    };

    typedef std::vector<JUMPPOINT>  JUMPPOINTLIST;

    void Initialize( const SPPCMREADER& pReader );
    void Initialize( const SPPCMREADER& pReader, const JUMPPOINTLIST& list );

    void Finalize();

    size_t    Read( void* pDst, size_t size );
    void      SetPosition( size_t pos );
    size_t    GetPosition()	const;
    size_t    GetLength()   const;
    PCMFORMAT GetFormat()   const;

    const JUMPPOINTLIST& GetJumpList()const;

  private:
    SPPCMREADER   m_pReader;
    JUMPPOINTLIST m_JumpPointList;
    size_t        m_Position;
  };

  typedef	boost::shared_ptr<CustomPCMReader>	SPCUSTOMPCMREADER;

}

#endif