#ifndef maid2_sound_core_win32_directsound8_bufferdirectsound8_h
#define maid2_sound_core_win32_directsound8_bufferdirectsound8_h

#include"../../../../config/define.h"
#include"../../ibuffer.h"
#include"common.h"

namespace Maid { namespace Sound {

  class BufferDirectSound8 : public IBuffer
	{
	public:
    BufferDirectSound8( const CREATEBUFFERPARAM& param, const SPDIRECTSOUNDBUFFER& pBuffer );

    virtual void Lock( size_t begin, size_t Length, LOCKDATA& data );
    virtual void Unlock( LOCKDATA& data );

    virtual void Play( bool IsLoop );
    virtual void SetPosition( size_t pos );
    virtual void SetVolume( double vol );
    virtual void Stop();
    virtual bool IsPlay() const;
    virtual size_t GetPlayPosition()const;
    virtual size_t GetWritePosition()const;
    virtual double GetVolume()const;

    const SPDIRECTSOUNDBUFFER& GetBuffer() const { return m_pBuffer; }
  private:
    SPDIRECTSOUNDBUFFER m_pBuffer;

  };

}}


#endif
