/*!
  @file
  @brief トレースモジュール
 */

#ifndef maid2_auxiliary_debug_trace_h
#define maid2_auxiliary_debug_trace_h


#include"../../config/define.h"
#include"../macro.h"
#include"../streambuffer.h"

#include <ostream>


namespace Maid
{
  class Trace : public std::ostream
  {
  public:
    Trace( const char* szFile, int Line );

    void OnAlert();

  private:
    const std::string	m_FileName;
    const int	m_Line;
	  StreamBuffer	m_StreamBuf;

    static bool s_IsCreateConsole;
  };

}

#if defined(GLOBAL_TRACE) || defined(LOCAL_TRACE)
  #pragma COMPILERMSG("trace を有効にします")	
  #define MAID_TRACE( mess )	                \
          {                                   \
            Maid::Trace d(__FILE__,__LINE__); \
            d<<mess;                          \
            d.OnAlert();                      \
          }
#else
	#pragma COMPILERMSG("trace を無効にします")	
	#define MAID_TRACE( mess )
#endif

#endif
