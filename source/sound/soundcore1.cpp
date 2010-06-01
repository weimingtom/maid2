#include"soundcore.h"


namespace Maid {

  /*!
      @class  SoundCore soundcore.h
      @brief  ライブラリで一番えらいコア部分
              デバイスを管理しています
  */


//  こちらに書く関数はゲームスレッドが使うものとします
//  2.cppに書くのはサウンド専用スレッドが行います
//  3.cppに書くのはいろんなスレッドが使います


SoundCore::SoundCore()
{

}

SoundCore::~SoundCore()
{

}


//! 初期化
/*!
    初期化に使う設定

 	  @param	param      [i ]	設定
 */
void SoundCore::Initialize( const INITIALIZEPARAM& param )
{
  GlobalPointer<SoundCore>::Set();

  m_CreateCount = 0;
  m_Param = param;

  if( m_Param.IsBackGround )
  {
    m_AccessThread.SetFunc( MakeThreadObject(&SoundCore::ThreadFunction,this) );
    m_AccessThread.Execute();
    m_AccessThread.SetProcesserMask( param.ProcesserMask );
  }else
  {
    m_Param.pDevice->Initialize();
  }
}


//! 解放
/*!
 */
void SoundCore::Finalize()
{
  if( m_Param.IsBackGround )
  {
    m_AccessThread.Close();
  }else
  {
    m_Param.pDevice->Finalize();
  }
  m_Param.pDevice.reset();
}


//! フレーム毎の更新
/*!
 */
void SoundCore::Update()
{
  if( m_Param.IsBackGround ) { return ; }

  for( int i=0; i<(int)m_MessageQue.size(); ++i )
  {
    const SPSOUNDMESSAGE& pMess = m_MessageQue[i];
    ExecuteMessage( *pMess );
  }
  m_MessageQue.clear();
}


//! サウンドオブジェクトを作成する
/*!
    ハンドルといったほうがいいかも

    @return オブジェクト
 */
SPSOUNDOBJECTINFO    SoundCore::CreateObjectInfo()
{
  ThreadMutexLocker lock(m_ObjectMutex);
  const unt ret = m_CreateCount;
  ++m_CreateCount;
  return SPSOUNDOBJECTINFO( new SoundObjectInfo(ret) );
}


//! サウンドオブジェクトにメッセージを送る
/*!
    @param  pInfo [i ] 送信先
    @param  pMess [i ] 送るメッセージ
 */
void SoundCore::PostMessage( const SPSOUNDOBJECTINFO& pInfo, const SPSOUNDMESSAGE& pMess )
{
  ThreadMutexLocker lock(m_MessageMutex);
  pMess->pInfo = pInfo;
  pMess->pInfo->m_MessageCount += 1;
  m_MessageQue.push_back(pMess);
}





}


