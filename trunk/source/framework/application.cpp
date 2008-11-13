#include"application.h"
#include"../auxiliary/string.h"
#include"../auxiliary/debug/warning.h"
#include"../auxiliary/debug/assert.h"
#include"../auxiliary/exception.h"

namespace Maid
{

Application::Application()
{
  m_ExitCode = 0;
}

Application::~Application()
{
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! アプリケーションの起動
/*!
	@return アプリケーションの終了コード WinMain の返り値として使います

	@exception Exception 異常終了した場合
 */
int Application::Run()
{
	try
	{
    { //  本当に汎用的なものはここで初期化しておく
      String::Initialize();
    }


    OnSetup();

		while( IsValid() )
		{
			OnLoop();
		}

    OnCleanup();
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

void Application::OnExit( int ExitCode )
{
  LocalExit();
  m_ExitCode = ExitCode;
}

}