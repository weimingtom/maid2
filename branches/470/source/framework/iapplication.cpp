#include"iapplication.h"
#include"../auxiliary/string.h"
#include"../auxiliary/debug/warning.h"
#include"../auxiliary/debug/assert.h"
#include"../auxiliary/exception.h"

namespace Maid
{

IApplication::IApplication()
  :m_ExitCode(0)
{
}

IApplication::~IApplication()
{
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! アプリケーションの起動
/*!
	@return アプリケーションの終了コード WinMain の返り値として使います

	@exception Exception 異常終了した場合
 */
int IApplication::Run()
{
  try
  {
    { //  本当に汎用的なものはここで初期化しておく
      String::Initialize();
    }

    {
      //  起動確認
      const bool ret = BootCheck();
      if( !ret ) { return 0; }
    }
    
    IOSDevice& OSDev = GetOSDevice();

    OSDev.Initialize();
    Initialize();

    while( OSDev.IsValid() )
    {
        OnLoop();
    }

    Finalize();
    OSDev.Finalize();
  }
  catch( Exception& e )
  {
    MAID_WARNING( "ライブラリ例外:" << e.GetLog() );
    throw;
  }
  catch(...)
  {
    MAID_WARNING( "システム例外" );
    throw;
  }

	return m_ExitCode;
}

void IApplication::OnExit( int ExitCode )
{
  IOSDevice& OSDev = GetOSDevice();
  OSDev.Exit();
  m_ExitCode = ExitCode;
}

//! アプリケーションが起動していいかチェックする
/*!
    @return 起動していいなら true
            起動がダメなら false
 */
bool IApplication::BootCheck() 
{
  return true; 
}

//! アプリケーションの初期化
/*!
 */
void IApplication::Initialize()
{
}

//! 処理の開始
/*!
 */
void IApplication::OnLoop()
{
  OnExit(0); 
}

//! アプリケーションの開放
/*!
 */
void IApplication::Finalize()
{

}

/*! @fn IOSDevice& IApplication::GetOSDevice()
    @brief ＯＳデバイスの取得

    @return OSデバイス
*/


}