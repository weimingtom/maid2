/*!
  @file
  @brief Win32 用アサート関数
 */

#ifndef maid2_auxiliary_debug_assert_h
#define maid2_auxiliary_debug_assert_h


#include"../../config/define.h"
#include"../macro.h"
#include"../streambuffer.h"

#include <ostream>


namespace Maid
{
  class Assert : public std::ostream
  {
  public:
    Assert( const char* szFile, int Line );

    bool OnAlert();

  private:
    const std::string	m_FileName;
    const int	m_Line;
	  StreamBuffer	m_StreamBuf;
  };

}

#if defined(GLOBAL_ASSERT) || defined(LOCAL_ASSERT)
  #pragma COMPILERMSG("assert を有効にします")	
  #define MAID_ASSERT( check, mess )	              \
          {                                         \
            if( check )                             \
            {                                       \
              Maid::Assert d(__FILE__,__LINE__);    \
              d<<mess;                              \
              if( d.OnAlert() ) { _asm int 3 }     \
            }                                       \
          }                                         \

#else
	#pragma COMPILERMSG("assert を無効にします")	
	#define MAID_ASSERT( check, mess )
#endif

#endif
