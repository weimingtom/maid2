//#define LOCAL_PROFILE


#include"movieplayer.h"

#include"../../auxiliary/mathematics.h"
#include"../../auxiliary/debug/warning.h"
#include"../../auxiliary/debug/profile.h"
#include"../../auxiliary/debug/assert.h"

#include"core/xiph/oggdecodermanager.h"
#include"core/xiph/oggfile.h"

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
    case STATE_SEEKING:
      {
        Seek();
      }break;

    case STATE_WORKING: 
      {
        Work();
      }break;

    case STATE_END:
      {
        ::Sleep(5);
      }break;
    }
  }

  m_State = STATE_EMPTY;

  return 0;
}

void MoviePlayer::Seek()
{
  //  名前はシークですが、初期化コードもある。

  m_pStorage.reset( new Movie::Xiph::OggFile(m_FileName) );
  m_pManager.reset( new Movie::Xiph::OggDecoderManager() );

  m_pStorage->Initialize();
  m_pManager->Initialize();
  m_pManager->BeginSkipMode( m_SeekTarget );

  //  数パケットにわたって初期化データが入っていることがあって
  //  初期化が終わるまではキャッシュがたまらないので、同じwhile()でも微妙に処理が変わってしまう

  while( true )
  {
    if( m_pStorage->IsEnd() ) { break; }
    if( m_pManager->IsSourceFull() ) { break; }
    Movie::SPSTORAGESAMPLE pSample;
    m_pStorage->Read( pSample );
    if( pSample.get()==NULL ) { break; }
    m_pManager->AddSource( pSample );
  }

  while( true )
  {
    if( m_pStorage->IsEnd() ) { break; }
    if( m_pManager->IsSampleFull() ) { break; }
    if( m_pManager->IsSourceFull() ) { ::Sleep(1); continue; }

    Movie::SPSTORAGESAMPLE pSample;
    m_pStorage->Read( pSample );
    if( pSample.get()==NULL ) { break; }
    m_pManager->AddSource( pSample );
  }

  {
    //  サンプルがいっぱいになってる==フォーマットがわかってるはずなので、それを取得する

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

  m_SeekTarget = -1.0;
  m_State = STATE_WORKING;
}

void MoviePlayer::Work()
{
  while( true )
  {
    if( m_pStorage->IsEnd() ) { break; }
    if( m_pManager->IsSourceFull() ) { ::Sleep(1); break; }
    if( m_pManager->IsSampleFull() ) { break; }

    Movie::SPSTORAGESAMPLE pSample;
    m_pStorage->Read( pSample );
    if( pSample.get()==NULL ) { break; }
    m_pManager->AddSource( pSample );
  }

  if( 0.0<=m_SeekTarget )
  {
    //  シーク命令が来ていたら、再生成してやり過ごす
    m_State = STATE_SEEKING;
  }else
  {
    if( m_pManager->IsDecodeEnd() )
    {
      m_State = STATE_END;
    }
    ::Sleep(1);
  }
}

}