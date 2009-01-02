#include"soundobjectpcmstream.h"


namespace Maid {

void SoundObjectPCMStream::Initialize( const Sound::SPBUFFER& pBuffer, const SPPCMDECODER& pDecoder )
{
  m_pBuffer = pBuffer;
  m_pDecoder= pDecoder;
  m_IsLoopPlay = false;
}

void SoundObjectPCMStream::Update()
{

}

void SoundObjectPCMStream::Play()
{
  m_pBuffer->Play(true);
}

void SoundObjectPCMStream::Stop()
{
  m_pBuffer->Stop();
}

void SoundObjectPCMStream::SetPosition( float time )
{
  const Sound::CREATEBUFFERPARAM& param = m_pBuffer->GetParam();
  const size_t pos = param.Format.CalcLength(time);
}

void SoundObjectPCMStream::SetVolume( float volume )
{
  m_pBuffer->SetVolume(volume);
}

void SoundObjectPCMStream::SetJumpPoint( const std::vector<JUMPPOINT>& list )
{
  m_JumpList = list;
}

void SoundObjectPCMStream::SetLoopState( bool IsLoop )
{
  m_IsLoopPlay = IsLoop;
}


}


