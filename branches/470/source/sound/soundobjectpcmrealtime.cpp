#include"soundobjectpcmrealtime.h"
#include"../auxiliary/debug/assert.h"
#include"../auxiliary/debug/warning.h"
#include"../auxiliary/mathematics.h"

#include<mmsystem.h>

namespace Maid {

  /*!
      @class  SoundObjectPCMStream soundobjectpcmrealtime.h
      @brief  ストリーム書き込みで再生するクラス
  */
SoundObjectPCMRealTime::SoundObjectPCMRealTime()
  :ISoundObject(TYPE_REALTIME)
{

}

void SoundObjectPCMRealTime::Initialize(  const Sound::SPBUFFER& pBuffer, const SPPCMBUFFERLIST& pDecoder )
{
  m_pBuffer = pBuffer;
  m_pDecoder= pDecoder;
 
  m_PlayPosition  = 0;
  m_OldPosition   = 0;
  m_WritePosition = 0;
  UpdateBuffer();
}

void SoundObjectPCMRealTime::Update()
{
  const size_t bufferpos = m_pBuffer->GetPlayPosition();
  const size_t delta_len = CalcLength( m_OldPosition, bufferpos );

  m_PlayPosition += delta_len;
  m_OldPosition = bufferpos;

  UpdateBuffer();
  m_pDecoder->ClearData(m_PlayPosition);
}


void SoundObjectPCMRealTime::Play()
{
  if( IsPlay() ){ return ; }

  UpdateBuffer();
  m_pBuffer->Play(true);
}

void SoundObjectPCMRealTime::Stop()
{
  m_pBuffer->Stop();
}

void SoundObjectPCMRealTime::SetPosition( double time )
{
  //  常に進んでるのでスルー
}

void SoundObjectPCMRealTime::SetVolume( double volume )
{
  //  強さから デシベルに変換する
  // http://e-words.jp/w/E38387E382B7E38399E383AB.html

  const double db = 20.0 * Math<double>::log10(volume);

  m_pBuffer->SetVolume(db);
}

void SoundObjectPCMRealTime::SetLoopState( bool IsLoop )
{
  //  常に進んでるのでスルー
}

bool SoundObjectPCMRealTime::IsPlay()const
{
  return m_pBuffer->IsPlay();
}

double SoundObjectPCMRealTime::GetPosition() const
{
  const size_t pos = m_PlayPosition + CalcLength( m_OldPosition, m_pBuffer->GetPlayPosition() );

  return m_pBuffer->GetParam().Format.CalcTime(pos);
}

double SoundObjectPCMRealTime::GetVolume() const
{
  //  デシベルから 強さに変換する
  // http://e-words.jp/w/E38387E382B7E38399E383AB.html

  const double db = m_pBuffer->GetVolume();

  if( db < -100.0f ) { return 0; }

  //  db = 20.0f * Math<float>::log10(ret);
  //  db/20.0f = Math<float>::log10(ret);
  // 10 の db/20.0f 乗が ret 

  return Math<double>::pow(10,db/20.0f);
}





void SoundObjectPCMRealTime::UpdateBuffer()
{
  const size_t UPDATESIZE = CalcUpdateScape();

  if( m_pDecoder->GetLength()==0 ) { return ; }  //  データがないときはスルー

  if( m_WritePosition < m_PlayPosition )
  {
    //  書き込んだ位置より、すでに再生していたら、シークする
    //m_PlayPosition + UPDATESIZE から書き込みを開始する
    const size_t pos = m_PlayPosition + std::max(m_PlayPosition-m_WritePosition, UPDATESIZE);

    m_pDecoder->SetPosition( pos );
    m_WritePosition = pos;
  }


  {
    //  書き込まれたデータが一定量たまっていたら何もしない
    if( UPDATESIZE < (m_WritePosition - m_PlayPosition) ) { return ; }
  }

  //  リングバッファにデータを流すのはしんどいので
  //  いったん作ってからやる
  std::vector<unt08> tmp(UPDATESIZE,0);

  {
    const size_t len = m_pDecoder->Read( &(tmp[0]), tmp.size() );
    if( len==0 ) { return ; }
    if( len<tmp.size() ) { tmp.resize(len); }
  }

  //  そんでもってながしこむ～
  {
    const size_t pos = m_WritePosition % m_pBuffer->GetParam().Length;

    Sound::IBuffer::LOCKDATA dat;
    m_pBuffer->Lock( pos, tmp.size(), dat );
    memcpy( dat.pData1, &(tmp[0]), dat.Data1Length );
    if( dat.pData2!=NULL ) { memcpy( dat.pData2, &(tmp[dat.Data1Length]), dat.Data2Length ); }
    m_pBuffer->Unlock( dat );
  }

  m_WritePosition += tmp.size();
}

size_t SoundObjectPCMRealTime::CalcLength( size_t prev, size_t now )const
{
  size_t ret;
  
  if( prev<=now ) { ret = now-prev; }
  else { ret = m_pBuffer->GetParam().Length - prev + now; }

  return ret;
}

size_t SoundObjectPCMRealTime::CalcUpdateScape()const
{
  //  更新間隔を求める
  MAID_ASSERT( m_pBuffer.get()==NULL, "初期化されていません" );
  return m_pBuffer->GetParam().Length / 4;
}

}


