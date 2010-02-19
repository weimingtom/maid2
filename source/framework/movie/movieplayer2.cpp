//#define LOCAL_PROFILE


#include"movieplayer.h"

#include"../../auxiliary/mathematics.h"
#include"../../auxiliary/debug/warning.h"
#include"../../auxiliary/debug/profile.h"
#include"../../auxiliary/debug/assert.h"

#include"core/xiph/oggdecodermanagersingle.h"
#include"core/xiph/oggfile.h"
#include"core/storagereadermultithread.h"

namespace Maid {

//  ここの実装はデコードスレッドが使う関数を書いておきます
//  ゲーム側が使う関数は 1.cpp を参照



unt MoviePlayer::ThreadFunction( volatile ThreadController::BRIGEDATA& brige )
{
  while( true )
  {
    if( brige.IsExit ) { break; }
    switch( m_State )
    {
    case STATE_INITIALIZING: { Init(brige); }break;
    case STATE_SEEKING: { Seek(brige); }break;
    case STATE_WORKING: { Work(brige); }break;
    case STATE_END:     { ThreadController::Sleep(5); }break;
    }
  }

  m_State = STATE_EMPTY;

  return 0;
}

void MoviePlayer::Init(volatile ThreadController::BRIGEDATA& state)
{
  {
    Movie::SPSTORAGEREADER pSingle( new Movie::Xiph::OggFile(m_FileName) );
    m_pStorage.reset( new Movie::StorageReaderMultiThread(pSingle, 50) );
    m_pManager.reset( new Movie::Xiph::OggDecoderManagerSingle() );
  }

  m_pStorage->Initialize();
  m_pManager->Initialize();

  while( true )
  {
    if( state.IsExit ) { break; }

    ThreadController::Sleep(1);
    if( m_pStorage->IsEnd() ) { break; }
    if( m_pManager->Setuped() ) { break; }
    Movie::SPSTORAGESAMPLE pSample;
    m_pStorage->Read( pSample );
    if( pSample.get()==NULL ) { continue; }
    m_pManager->AddSource( pSample );
  }


  {
    {
      Movie::SPSAMPLEFORMAT pFormat;
      m_pManager->GetFormat( DECODERID_FRAME1, pFormat );

      if( pFormat.get()!=NULL )
      {
        const Movie::SampleFormatFrame& fmt = *boost::shared_static_cast<Movie::SampleFormatFrame>(pFormat);

        m_FileInfo.IsImage = true;
        m_FileInfo.Image.EncodedSize    = fmt.EncodedSize;
        m_FileInfo.Image.DisplaySize    = fmt.DisplaySize;
        m_FileInfo.Image.DisplayOffset  = fmt.DisplayOffset;
        m_FileInfo.Image.FpsNumerator   = fmt.FpsNumerator;
        m_FileInfo.Image.FpsDenominator = fmt.FpsDenominator;
        m_FileInfo.Image.AspectNumerator= fmt.AspectNumerator;
        m_FileInfo.Image.AspectDenominator = fmt.AspectDenominator;
        m_FileInfo.Image.PixelFormat    = (FILEINFO::FRAME::PIXELFORMAT)fmt.PixelFormat;
      }
    }

    {
      Movie::SPSAMPLEFORMAT pFormat;
      m_pManager->GetFormat( DECODERID_PCM1, pFormat );

      if( pFormat.get()!=NULL )
      {
        const Movie::SampleFormatPCM& fmt = *boost::shared_static_cast<Movie::SampleFormatPCM>(pFormat);

        m_FileInfo.IsPCM = true;
        m_FileInfo.Pcm.Format = fmt.Format;
      }
    }
  }
  //  初期化が終了したらシークへ
  m_State = STATE_SEEKING;
}


void MoviePlayer::Seek(volatile ThreadController::BRIGEDATA& state)
{
  //  名前はシークですが、バッファリングも行う
  const double time = m_Timer.Get();
  if( 0<time )
  {
    m_pManager->BeginSkipMode( time );
  }

  //  サンプルが埋まるまで入れ続ける
  while( true )
  {
    if( state.IsExit ) { break; }
    if( m_pStorage->IsEnd() ) { break; }
    if( m_pManager->IsSampleFull() ) { break; }

    Update();
  }

  //  デコード前が埋まるまで入れ続ける
  while( true )
  {
    if( state.IsExit ) { break; }
    if( m_pStorage->IsEnd() ) { break; }
    if( m_pManager->IsSourceFull() ) { break; }
    Update();
  }

  //  ストレージが埋まるまで待つ
  while( true )
  {
    if( state.IsExit ) { break; }
    if( m_pStorage->IsEnd() ) { break; }
    if( m_pStorage->IsCacheFull() ) { break; }
    ThreadController::Sleep(1);
  }

  m_State = STATE_WORKING;
}

void MoviePlayer::Work(volatile ThreadController::BRIGEDATA& state)
{
  while( true )
  {
    if( state.IsExit ) { break; }
    {
      //  再生が遅れていたら、スキップ
      const double now = m_Timer.Get();
      const double dec = m_pManager->GetTime();
      if( dec < now )
      {
        m_pManager->BeginSkipMode( now );
      }
    }

    if( m_pStorage->IsEnd() )
    {
      if( m_pManager->IsDecodeEnd() ) { break; }
      ThreadController::Sleep(1);
    }
    Update();
  }

  m_State = STATE_END;
}


void MoviePlayer::Update()
{
  const bool IsSourceFull = m_pManager->IsSourceFull();  //  サンプルがいっぱい?
  const bool IsSampleFull = m_pManager->IsSampleFull();   //  キャッシュが全部いっぱい?
    ThreadController::Sleep(0);

  if( IsSourceFull && IsSampleFull )
  {
    //  待ち
    ThreadController::Sleep(1);
  }else
  {
    if( IsSourceFull ) 
    {
      ThreadController::Sleep(1);
    }else
    {
      /*サンプル追加*/ 
      Movie::SPSTORAGESAMPLE pSample;
      m_pStorage->Read( pSample );
      if( pSample.get()!=NULL )
      {
        m_pManager->AddSource( pSample );
      }
    }

    if( IsSampleFull )
    {
      ThreadController::Sleep(1);
    }else
    {
      /*デコード*/ 
      Movie::Xiph::OggDecoderManagerSingle& manage = static_cast<Movie::Xiph::OggDecoderManagerSingle&>(*m_pManager);
      manage.Update();
    }
  }
}



}