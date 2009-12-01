#include"soundfile.h"

#include"soundmessage1.h"
#include"../storage/filereadersync.h"
#include"../auxiliary/debug/warning.h"


namespace Maid {

  /*!
      @class  SoundFile soundfile.h
      @brief  ファイルから読み込んで操作できるサウンド
  */


  namespace KEEPOUT
  {
  void SoundFileFunction::Execute( const IJobInput& Input, IJobOutput& Output )
  {
    const SoundFileInput&  in  = static_cast<const SoundFileInput&>(Input);
    SoundFileOutput& out = static_cast<SoundFileOutput&>(Output);

    SPMEMORYBUFFER  pFileImage( new MemoryBuffer );

    SPPCMREADER pReader;
    {
      FileReaderSync hFile;

      if( hFile.Open(in.FileName) ) { MAID_WARNING( "ファイルが開けません " << in.FileName ); return; }

      pFileImage->Resize( hFile.GetSize() );

      void* pData = pFileImage->GetPointer(0);
      const size_t size = pFileImage->GetSize();

      hFile.Read( pData, size );

      pReader = in.pCore->CreatePCMReader(pData,size);
      if( pReader.get()==NULL ) { MAID_WARNING( "デコーダの作成に失敗 " << in.FileName ); return; }

      const IPCMReader::INITIALIZERESULT ret = pReader->Initialize();
      if( ret==IPCMReader::INITIALIZERESULT_ERROR ) { MAID_WARNING( "デコーダの作成に失敗 " << in.FileName ); return; }
    }

    CustomPCMReader::JUMPPOINTLIST  jump;
    for( int i=0; i<(int)in.JumpList.size(); ++i )
    {
      const JUMPPOINT src = in.JumpList[i];
      CustomPCMReader::JUMPPOINT dst;

      dst.Boot = pReader->GetFormat().CalcLength( src.Boot );
      dst.Jump = pReader->GetFormat().CalcLength( src.Jump );
      jump.push_back( dst );
    }

    SPSOUNDMESSAGE pMessage;
    
    switch( in.Type )
    {
    case SoundFileInput::LOADTYPE_AUTO:
      {
        const size_t len = pReader->GetLength();
        const double time = pReader->GetFormat().CalcTime(len);

        //  10秒未満ならstatic
        if( time < 10.0 ) {  pMessage = CreateStatic( pFileImage, pReader ); }
        else { pMessage = CreateStream( pFileImage, pReader, jump ); }

      }break;
    case SoundFileInput::LOADTYPE_STATIC:
      {
         pMessage = CreateStatic( pFileImage, pReader );
      }break;
    case SoundFileInput::LOADTYPE_STREAM:
      {
         pMessage = CreateStream( pFileImage, pReader, jump );
      }break;
    }
   
//    out.pInfo    = in.pCore->CreateObjectInfo();
    out.pMessage = pMessage;
  }


  SPSOUNDMESSAGE SoundFileFunction::CreateStatic( const SPMEMORYBUFFER& pFileImage, const SPPCMREADER& pDecorder )
  {
    SPMEMORYBUFFER  pPCMData( new MemoryBuffer );

    pPCMData->Resize( pDecorder->GetLength() );
    pDecorder->Read( pPCMData->GetPointer(0), pDecorder->GetLength() );

    boost::shared_ptr<SoundMessage::CreatePCMStatic> pMessage( new SoundMessage::CreatePCMStatic );
    pMessage->pData    = pPCMData;

    Sound::CREATEBUFFERPARAM& param = pMessage->Param;

    param.Format = pDecorder->GetFormat();
    param.Length = pDecorder->GetLength();

    return pMessage;
  }

  SPSOUNDMESSAGE SoundFileFunction::CreateStream( const SPMEMORYBUFFER& pFileImage, const SPPCMREADER& pDecorder, const CustomPCMReader::JUMPPOINTLIST& jump )
  {
    boost::shared_ptr<SoundMessage::CreatePCMStream> pMessage( new SoundMessage::CreatePCMStream );
    pMessage->pData    = pFileImage;
    pMessage->pDecoder.reset( new CustomPCMReader );
    pMessage->pDecoder->Initialize( pDecorder, jump );

    Sound::CREATEBUFFERPARAM& param = pMessage->Param;

    param.Format = pDecorder->GetFormat();
    param.Length = param.Format.CalcLength(2.0f);

    return pMessage;
  }

  }

SoundFile::CACHE::INFOMAP  SoundFile::CACHE::s_InfoMap;
ThreadMutex  SoundFile::CACHE::s_Mutex;

#pragma COMPILERMSG("現在の実装だと再生バッファ（ファイルイメージではない）の共有ができていません")	
#pragma COMPILERMSG("これはSoundCore側で対応させる必要がある")	


SoundFile::SoundFile()
  :m_IsPlay(false)
  ,m_IsLoopPlay(false)
  ,m_Volume(1)
  ,m_Position(0)
{

}

SoundFile::SoundFile( const SoundFile& obj )
  :m_IsPlay(false)
  ,m_IsLoopPlay(false)
  ,m_Volume(1)
  ,m_Position(0)
{
  if( !obj.m_Cache.IsEmpty() )
  {
    Destroy();
    m_Cache.Start( obj.m_Cache.GetInput() );

    LoadCheck();
  }
}


SoundFile::~SoundFile()
{
  Destroy();
}


//! ファイルから読み込む
/*!
    ストリーム再生か、固定再生かは自動的に決断されます

    @param	filename      [i ]	読み込むファイル名
 */
void SoundFile::LoadFile( const String& filename )
{
  Destroy();
  m_Cache.Start( KEEPOUT::SoundFileInput(filename, GlobalPointer<SoundCore>::Get(), KEEPOUT::SoundFileInput::LOADTYPE_AUTO, SOUNDJUMPPOINTLIST() ) );

  LoadCheck();
}


//! ストリーム再生するファイルとして読み込む
/*!
    @param	filename      [i ]	読み込むファイル名
 */
void SoundFile::LoadFileStream( const String& filename )
{
  LoadFileStream( filename, SOUNDJUMPPOINTLIST() );
}

//! ストリーム再生するファイルとして読み込む
/*!
    @param	filename      [i ]	読み込むファイル名
 */
void SoundFile::LoadFileStream( const String& filename, const SOUNDJUMPPOINTLIST& List )
{
  Destroy();
  m_Cache.Start( KEEPOUT::SoundFileInput(filename, GlobalPointer<SoundCore>::Get(), KEEPOUT::SoundFileInput::LOADTYPE_STREAM, List ) );
  LoadCheck();
}



//! 固定再生するファイルとして読み込む
/*!
    @param	filename      [i ]	読み込むファイル名
 */
void SoundFile::LoadFileStatic( const String& filename )
{
  Destroy();
  m_Cache.Start( KEEPOUT::SoundFileInput(filename, GlobalPointer<SoundCore>::Get(), KEEPOUT::SoundFileInput::LOADTYPE_STATIC, SOUNDJUMPPOINTLIST() ) );
  LoadCheck();
}


//! 読み込んでるバッファを破棄する
/*!
 */
void SoundFile::Destroy()
{
  if( m_pInfo.get()==NULL ) { return ; }

  boost::shared_ptr<SoundMessage::DeleteObject> pMess( new SoundMessage::DeleteObject );
  GlobalPointer<SoundCore>::Get()->PostMessage(m_pInfo, pMess );

  m_Cache.Reset();
  m_pInfo.reset();

  m_IsPlay = false;
  m_IsLoopPlay = false;
  m_Position = 0;
  m_Volume = 1;
}

//! 読み込み中か？
/*!
    読み込み中なら true
 */
bool SoundFile::IsLoading()const
{
  if( IsEmpty() ) { return false; }
  SoundFile* pThis = const_cast<SoundFile*>(this);
  pThis->LoadCheck();

  return m_pInfo.get()==NULL;
}

bool SoundFile::IsEmpty() const
{
  return m_Cache.IsEmpty();
}



void SoundFile::LoadCheck()
{
  if( IsEmpty() ) { return ; }
  if( m_pInfo.get()!=NULL ) { return ; }

  //  処理が終わるまでは忙しい
  if( m_Cache.IsExecuting() ) { return ; }

  {
    //  メッセージの使いまわしは厳禁なので、コピーして使う
    //  そもそも SoundCore側でやる処理なので、ここに書いてあるのはおかしい
    const KEEPOUT::SoundFileOutput& out = m_Cache.GetOutput();

    SoundCore* pCore = GlobalPointer<SoundCore>::Get();
    m_pInfo = pCore->CreateObjectInfo();

    SPSOUNDMESSAGE pMessage;
    switch( out.pMessage->Message )
    {
    case SoundMessage::Base::CREATE_PCMSTATIC:
      {
        const SoundMessage::CreatePCMStatic& m = static_cast<const SoundMessage::CreatePCMStatic&>(*(out.pMessage));
        pMessage.reset( new SoundMessage::CreatePCMStatic(m) );
      }break;

    case SoundMessage::Base::CREATE_PCMSTREAM:
      {
        const SoundMessage::CreatePCMStream& m = static_cast<const SoundMessage::CreatePCMStream&>(*(out.pMessage));
        pMessage.reset( new SoundMessage::CreatePCMStream(m) );
      }break;
    }

    pCore->PostMessage(m_pInfo, pMessage );
  }

  //  最初は０秒、ボリューム１倍にしておく
  SetPosition( m_Position );
  SetVolume( m_Volume );
  SetLoopState( m_IsLoopPlay );
  if( m_IsPlay ) { Play(); }
}

//! 再生を開始する
/*!
    再生開始場所の設定は SetPosition() で行います
 */
void SoundFile::Play()
{
  m_IsPlay = true;
  if( m_pInfo.get()==NULL ) { return ; }
  boost::shared_ptr<SoundMessage::Play> pMess( new SoundMessage::Play );
  GlobalPointer<SoundCore>::Get()->PostMessage(m_pInfo, pMess );

  //  ここで値を更新しているのは
  //  SoundMessage::Play を投げることで、その処理が終わるまで m_pInfo->IsMessageExecuting() がtrueをかえす
  //  すると GetPosition() が古いままになってしまうので、ここで更新しておく
  //  なお、 m_IsPlay を更新しないのは、Play() を呼ばれたら true になることを期待してるから
  m_Position = m_pInfo->GetPosition();
}

//! 再生位置の設定
/*!
    再生開始場所の設定は SetPosition() で行います
 */
void SoundFile::SetPosition( double time )
{
  m_Position = time;
  if( m_pInfo.get()==NULL ) { return ; }

  boost::shared_ptr<SoundMessage::SetPosition> pMess( new SoundMessage::SetPosition );
  pMess->Position = time;
  GlobalPointer<SoundCore>::Get()->PostMessage(m_pInfo, pMess );

  //  ここで値を更新しているのは
  //  Play() と同じ理由
  m_IsPlay = m_pInfo->IsPlay();
}

//! 再生バッファをとめる
/*!
 */
void SoundFile::Stop()
{
  m_IsPlay = false;
  if( m_pInfo.get()==NULL ) { return ; }
  boost::shared_ptr<SoundMessage::Stop> pMess( new SoundMessage::Stop );
  GlobalPointer<SoundCore>::Get()->PostMessage(m_pInfo, pMess );
  //  ここで値を更新しているのは
  //  Play() と同じ理由
  m_Position = m_pInfo->GetPosition();
  m_IsPlay = m_pInfo->IsPlay();
}



//! ループ設定
/*!
 */
void SoundFile::SetLoopState( bool IsLoop )
{
  m_IsLoopPlay = IsLoop;
  if( m_pInfo.get()==NULL ) { return ; }

  boost::shared_ptr<SoundMessage::SetLoopState> pMess( new SoundMessage::SetLoopState );
  pMess->IsLoop = IsLoop;
  GlobalPointer<SoundCore>::Get()->PostMessage(m_pInfo, pMess );
  //  ここで値を更新しているのは
  //  Play() と同じ理由
  m_Position = m_pInfo->GetPosition();
  m_IsPlay = m_pInfo->IsPlay();
}


//! ボリュームの設定
/*!
    @param  Volume  [i ]  新しいボリューム( 0.0f:無音  1.0f:等倍)
 */
void SoundFile::SetVolume( double Volume )
{
  m_Volume = Volume;
  if( m_pInfo.get()==NULL ) { return ; }

  boost::shared_ptr<SoundMessage::SetVolume> pMess( new SoundMessage::SetVolume );
  pMess->Volume = Volume;
  GlobalPointer<SoundCore>::Get()->PostMessage(m_pInfo, pMess );
  //  ここで値を更新しているのは
  //  Play() と同じ理由
  m_Position = m_pInfo->GetPosition();
  m_IsPlay = m_pInfo->IsPlay();
}




bool SoundFile::IsPlay() const
{
  if( m_pInfo.get()==NULL ) { return m_IsPlay; }

  if( m_pInfo->IsMessageExecuting() ) { return m_IsPlay; }

  return m_pInfo->IsPlay();
}

double SoundFile::GetPosition() const
{
  if( m_pInfo.get()==NULL ) { return m_Position; }

  if( m_pInfo->IsMessageExecuting() ) { return m_Position; }

  return m_pInfo->GetPosition();
}

double SoundFile::GetVolume() const
{
  return m_Volume;
}

String SoundFile::GetFileName() const
{
  if( IsEmpty() ) { return String(); }

  const KEEPOUT::SoundFileInput& in = m_Cache.GetInput();

  return in.FileName;
}


SoundFile& SoundFile::operator=(const SoundFile &obj)
{
  if( !obj.m_Cache.IsEmpty() )
  {
    Destroy();
    m_Cache.Start( obj.m_Cache.GetInput() );

    LoadCheck();
  }

  return *this;
}

}


