#include"soundobjectpcmstatic.h"
#include"../auxiliary/mathematics.h"


namespace Maid {

  /*!
      @class  SoundObjectPCMStatic soundobjectpcmstatic.h
      @brief  固定バッファを使って再生するクラス
  */

void SoundObjectPCMStatic::Initialize( const Sound::SPBUFFER& pBuffer )
{
  m_pBuffer = pBuffer;
  m_IsLoop = false;
}


void SoundObjectPCMStatic::Update()
{
  //  効果なし。

}

void SoundObjectPCMStatic::Play()
{
  m_pBuffer->Play(m_IsLoop);
}

void SoundObjectPCMStatic::Stop()
{
  m_pBuffer->Stop();
}

void SoundObjectPCMStatic::SetPosition( double time )
{
  const Sound::CREATEBUFFERPARAM& param = m_pBuffer->GetParam();

  const size_t pos = param.Format.CalcLength(time);

  m_pBuffer->SetPosition(pos);
}

void SoundObjectPCMStatic::SetVolume( double volume )
{
  //  強さから デシベルに変換する
  // http://e-words.jp/w/E38387E382B7E38399E383AB.html

  const double db = 20.0 * Math<double>::log10(volume);

  m_pBuffer->SetVolume(db);
}

bool SoundObjectPCMStatic::IsPlay()const
{
  return m_pBuffer->IsPlay();
}

double SoundObjectPCMStatic::GetPosition() const
{
  const size_t p = m_pBuffer->GetPlayPosition();

  const Sound::CREATEBUFFERPARAM& param = m_pBuffer->GetParam();

  const double ret = param.Format.CalcTime(p);

  return ret;
}

double SoundObjectPCMStatic::GetVolume() const
{
  //  デシベルから 強さに変換する
  // http://e-words.jp/w/E38387E382B7E38399E383AB.html

  const double db = m_pBuffer->GetVolume();

  if( db < -100.0f ) { return 0; }

  //  db = 20.0f * Math<float>::log10(ret);
  //  db/20.0f = Math<float>::log10(ret);
  // 10 の db/20.0f 乗が ret 

  return Math<double>::pow(10,db/20.0);
}

void SoundObjectPCMStatic::SetLoopState( bool IsLoop )
{
  m_IsLoop = IsLoop;

  if( m_pBuffer->IsPlay() )
  {
    m_pBuffer->Play(IsLoop);
  }
}

}


