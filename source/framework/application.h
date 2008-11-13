
/*!
	@file
	@brief 各ＯＳ単位の変換を行うヘッダ
 */

#ifndef framework_application_h
#define framework_application_h


#include"../config/define.h"

namespace Maid
{
  class _Application
	{
	public:
    _Application();
    virtual ~_Application();

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
