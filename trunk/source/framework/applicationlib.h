
/*!
	@file
	@brief アプリケーションの雛形
 */

#ifndef framework_applicationlib_h
#define framework_applicationlib_h


#include"../config/define.h"

namespace Maid
{
  class ApplicationLib
	{
	public:
    ApplicationLib();
    virtual ~ApplicationLib();

    int Run();

		void OnExit( int ExitCode );

  protected:
		virtual void LocalExit()=0;
		virtual void OnSetup()=0;
		virtual void OnLoop()=0;
		virtual void OnCleanup()=0;
		virtual bool IsValid()=0;

  private:
    int m_ExitCode;
	};
}

#endif
