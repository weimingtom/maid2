/*!
  @file
  @brief プロファイルモジュール
 */

#ifndef maid2_auxiliary_debug_profile_h
#define maid2_auxiliary_debug_profile_h


#include"../../config/define.h"
#include"../../config/typedef.h"
#include"../macro.h"

#include<string>
#include<boost/smart_ptr.hpp>

namespace Maid
{
  class Profile
  {
  public:
    Profile( const std::string& FileName, int Line );

    static void PrintLog( const std::string& FileName );

  private:
    class Impl;
    boost::shared_ptr<Impl>	m_pImpl;
  };
}

#if defined(GLOBAL_PROFILE) || defined(LOCAL_PROFILE)
  #pragma COMPILERMSG("profile を有効にします")	
  #define MAID_PROFILE_BASE( a )	              \
  Maid::Profile  d##a(__FUNCTION__,__LINE__) \

  #define MAID_PROFILE_BASE2(a)  MAID_PROFILE_BASE(a)
  #define MAID_PROFILE()  MAID_PROFILE_BASE2(__LINE__)

  #define MAID_PROFILE_PRINTLOG( name ) Maid::Profile::PrintLog( name )

#else
	#pragma COMPILERMSG("profile を無効にします")	
	#define MAID_PROFILE()
  #define MAID_PROFILE_PRINTLOG( name )
#endif

#endif
