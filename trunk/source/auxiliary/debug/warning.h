/*!
  @file
  @brief 警告用クラス
 */

#ifndef maid2_auxiliary_debug_warning_h
#define maid2_auxiliary_debug_warning_h


#include"../../config/define.h"
#include"../macro.h"
#include"../streambuffer.h"

#include <ostream>


namespace Maid
{
  class Warning : public std::ostream
  {
  public:
    Warning( const char* szFile, int Line );

    void OnAlert( int level );

  private:
    const std::string	m_FileName;
    const int	m_Line;
	  StreamBuffer	m_StreamBuf;
  };

}

#ifdef USE_WARNING
  #pragma COMPILERMSG("warning を有効にします")	
  #define MAID_WARNING_N( level, mess )	          \
          {                                       \
            Maid::Warning d(__FILE__,__LINE__);   \
            d<<mess;                              \
            d.OnAlert(level);                     \
          }

  #define MAID_WARNING( mess )    MAID_WARNING_N(0,mess)

#else
	#pragma COMPILERMSG("warning を無効にします")	
	#define MAID_WARNING_N( level, mess )
	#define MAID_WARNING( mess )
#endif

#endif
