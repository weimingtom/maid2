/*!
  @file
  @brief 文字列ストリームを保持するバッファ
 */

#ifndef maid2_auxiliary_streambuffer_h
#define maid2_auxiliary_streambuffer_h


#include"../config/define.h"
#include"macro.h"

#include <streambuf>

namespace Maid
{
    class StreamBuffer : public std::streambuf 
    {
    public:
      virtual int_type overflow(int_type iChar = EOF);
      const std::string& GetBuffer()const;
    private:
      std::string m_buffer;
    };
}

#endif