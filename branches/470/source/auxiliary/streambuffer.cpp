#include"streambuffer.h"


namespace Maid
{

  /*!
    @class	StreamBuffer streambuffer.h
    @brief	String を << できるようにあるクラス
  \n        デバッグ目的以外は禁止します
   */
  std::streambuf::int_type StreamBuffer::overflow(std::streambuf::int_type iChar) {
	  if (iChar != EOF) {
		  m_buffer += iChar;
	  }
	  return iChar;   // 出力が成功したことを示す eof 以外を返す。
  }

  const std::string& StreamBuffer::GetBuffer()const
  {
    return m_buffer;
  }

}