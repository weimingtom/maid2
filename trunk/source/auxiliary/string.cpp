#include"string.h"
#include"macro.h"

#include"../import/babel/babel.h"

#include <stdarg.h>

namespace Maid
{
  std::ostream& operator<<(std::ostream& out, const String& a)
  {
    out << String::ConvertMAIDtoSJIS(a);
    return out;
  }

  void String::Initialize()
  {
    babel::init_babel();
  }


  //!	指定した1byteがSJISかどうか判断する
  /*!	
	    @retval true SJISである
	    @retval false SJISでない
  */
  bool String::IsSJIS1( char x )
  {
    return ((unsigned char)((x^0x20)-0xA1)<=0x3B);
  }

  //!	指定した1byteがUTF16 の4byte文字かどうか判断する
  /*!	
	    @retval true 4byte文字である
	    @retval false 4byte文字でない
  */
  bool String::IsUTF16H( wchar_t x )
  {
    // http://homepage1.nifty.com/nomenclator/unicode/ucs_utf.htm
    // UTF16は(0xD800-0xDBFFと0xDC00-0xDFFF)を組み合わせて４byte文字を作ることがある。
    // U+D800からU+DBFFまでの文字を上位サロゲート（high surrogate）
    // U+DC00からU+DFFFまでの文字を下位サロゲート（low surrogate）

    const unt32 c = (unt32)x;
    if( 0xd800 <= c  &&
                  c <= 0xDBFF )
    {
      return true;
    }

    return false;
  }

  //!	指定した1byteがUTF16 の4byte文字かどうか判断する
  /*!	
	    @retval true 4byte文字である
	    @retval false 4byte文字でない
  */
  bool String::IsUTF16L( wchar_t x )
  {
    // http://homepage1.nifty.com/nomenclator/unicode/ucs_utf.htm
    // UTF16は(0xD800-0xDBFFと0xDC00-0xDFFF)を組み合わせて４byte文字を作ることがある。
    // U+D800からU+DBFFまでの文字を上位サロゲート（high surrogate）
    // U+DC00からU+DFFFまでの文字を下位サロゲート（low surrogate）

    const unt32 c = (unt32)x;
    if( 0xDC00 <= c  &&
                  c <= 0xDFFF )
    {
      return true;
    }

    return false;
  }



  //!	UTF16 -> MSTRING への変換
  /*!	
	    @return MSTRINGになった文字列
  */
  String String::ConvertUNICODEtoMAID( const std::wstring& str )
  {
    String ret;

    for( int i=0; i<(int)str.length(); ++i )
    {
      const wchar_t x = str[i];

      if( IsUTF16H(x) )
      {
        // U' = yyyyyyyyyyxxxxxxxxxx (UCS-4 ＝ U から0x10000だけ差し引いた数）
        // W1 = 110110yyyyyyyyyy     (上位サロゲート：D800..DBFFになる）
        // W2 = 110111xxxxxxxxxx     (下位サロゲート：DC00..DFFFになる）

        const unt32 h = (unt32)ConvertEndian16((unt16)str[i+0]);
        const unt32 l = (unt32)ConvertEndian16((unt16)str[i+1]);

        const unt32 hh = h & 0x3FF;
        const unt32 ll = l & 0x3FF;

        const unt32 d = ((h<<10)|(l)) + 0x010000;
        ret.push_back(d);
        ++i;
      }else
      {
        ret.push_back((unt32)x);
      }
    }

	  return ret;
  }

  //!	sjis -> MSTRING への変換
  /*!	
      @return MSTRINGになった文字列
  */
	String String::ConvertSJIStoMAID( const std::string& str )
  {
    std::wstring source = babel::sjis_to_unicode(str);
	  return ConvertUNICODEtoMAID( source );
  }

  //!	MSTRING -> UTF16 への変換
  /*!	
	    @return UTF16になった文字列
  */
	std::wstring String::ConvertMAIDtoUNICODE( const String& str )
  {
    std::wstring ret;
    for( int i=0; i<(int)str.length(); ++i )
    {
      const unt32 x = str[i];

      if( x >= 0x010000 )
      {
        const unt32 u = x-0x010000;
        const unt32 h = ((u&0x000FFC00)>>10) | (0xD800);
        const unt32 l = ((u&0x000003FF) | (0xDC00) );
        ret.push_back( (wchar_t)ConvertEndian16(h) );
        ret.push_back( (wchar_t)ConvertEndian16(l) );
      }else
      {
        ret.push_back( (wchar_t)x );
      }
    }

    return ret;
  }

  //!	MSTRING -> SJIS への変換
  /*!	
	    @return SJISになった文字列
  */
	std::string String::ConvertMAIDtoSJIS( const String& str )
  {
    const std::wstring source = ConvertMAIDtoUNICODE( str );

    return babel::unicode_to_sjis(source);
  }
  
  std::wstring String::ConvertSJIStoUNICODE( const std::string& str )
  {
    return babel::sjis_to_unicode(str);
  }


  unt32 String::ConvertCharaMAIDtoSJIS( unt32 c )
  {
	  std::string ret;
	  if( IsHankaku(c) )
	  {
      return (unt32)c;
	  }
	  else if( IsGaizi(c) )
	  {
		  //	外字の場合は表示できないのでバグっぽくしておく
      return (unt32)' ';
	  }
	  else
	  {
      String a;
      a.push_back(c);
		  ret = ConvertMAIDtoSJIS(a);
	  }

    const unt32 h = (unt32)ret[0];
    const unt32 l = (unt32)ret[1];

	  return (h<<8)|l;
  }

  unt32 String::ConvertCharaMAIDtoUNICODE( unt32 c )
  {
	  std::wstring ret;
	  if( IsHankaku(c) )
	  {
      return (unt32)c;
	  }
	  else if( IsGaizi(c) )
	  {
		  //	外字の場合は表示できないのでバグっぽくしておく
      return (unt32)' ';
	  }
	  return c;
  }

  //!	半角文字か？
  /*!
 	    @param	c	[i ]	調べる文字
 	    @return	半角なら true
   */
  bool  String::IsHankaku( unt32 c )
  {
    //  半角チェックはここを見る http://ash.jp/code/unitbl1.htm
    if( c<=0x7F ) { return true; }
    if( c==0x203E ) { return true; }
    if( 0xFF61<=c && c<=0xFF9F ) { return true; }

	  return false;
  }

  //!	外字か？
  /*!
 	    @param	c	[i ]	調べる文字
 	    @return	半角なら true
   */
  bool  String::IsGaizi( unt32 c )
  {
	  return IsBitOn(c,31);
  }

  //!	外字コードを作る
  /*!
 	    @param	c	[i ]	作りたい番号
 	    @return	外字コード
   */
  unt32 String::MakeGaizi( unt32 c )
  {
	  return BitOn( c, 31 );
  }

  //!	半角を１、全角を２とした文字の総数を調べる
  /*!
 	    @return	バイト数
   */
  int		String::CountByte( const String& str )
  {
	  int ret = 0;
	  for( int i=0; i<(int)str.length(); ++i )
	  {
		  const unt32 a = str[i];

		  if( IsHankaku(a) ){ ret += 1;	}
		  else			        { ret += 2;	}
	  }

	  return ret;
  }

  //!	すべて小文字化する
  /*!
 	  @param	str	[i ]	小文字化する文字列
 	  @return	小文字化された文字列
   */
  String String::ToLower( const String& str )
  {
	  String ret;
	  ret.resize( str.length() );
	  for( int i=0; i<(int)str.length(); ++i )
	  {
		  const unt32 a = str[i];

		  if( IsHankaku(a) ){ ret[i] = ::tolower((char)a);	}
		  else			  { ret[i] = a;	}
	  }

	  return ret;
  }


  //!	パスからディレクトリ名を割り出す
  /*!
 	  "hoge/huga" だったら "hoge" が返ります ( / は付きません )
   
 	  @param	Path	[i ]	抜き出したいディレクトリ名
   
 	  @return	ディレクトリ名
   */
  String String::GetDirectory( const String& Path )
  {
	  //	パス名を後ろから見ていって、 / or \ が見つかったら切り取って終了
	  for( int i=int(Path.length())-1; i>=0; --i )
	  {
		  const unt32 c = Path[i];

		  if( c=='\\' || c=='/' ) 
		  {
			  return Path.substr(0,i);
		  }
	  }

	  return String();
  }


  //!	パスから拡張子を割り出す
  /*!
 	  "hoge/huga.bmp" だったら "bmp" が返ります ( . は付きません )
 	  "hoge/huga" だったら "" が返ります ( 拡張子なしと判定します )
   
 	  @param	Path	[i ]	抜き出したいディレクトリ名
   
 	  @return	拡張子
   */
  String String::GetExtension( const String& Path )
  {
	  for( int i=0; i<(int)Path.length(); ++i )
	  {
		  const unt32 a = Path[i];

		  if( a == '.'  )
		  {
			  return Path.substr(i+1);
		  }
	  }

	  return String();
  }




  //!	パスからファイル名を割り出す
  /*!
 	  "hoge/huga" だったら "huga" が返ります
   
 	  @param	Path	[i ]	抜き出したいディレクトリ名
   
 	  @return	ファイル名
   */
  String String::GetFileName( const String& Path )
  {
	  //	パス名を後ろから見ていって、 / or \ が見つかったら切り取って終了
	  for( int i=int(Path.length())-1; i>=0; --i )
	  {
		  const wchar_t c = Path[i];

		  if( c=='\\' || c=='/' ) 
		  {
			  return Path.substr(i+1);
		  }
	  }

	  return Path;
  }

  //!	文字を整数に変換する
  /*!
 	  @param	str	[i ]	変換する文字列
 	  @return	整数
   */
  int   String::AtoI( const String& str )
  {
	  return ::atoi( ConvertMAIDtoSJIS(str).c_str() );
  }

  //!	文字を浮動小数点に変換する
  /*!
 	  @param	str	[i ]	変換する文字列
 	  @return	浮動小数点
   */
  float String::AtoF( const String& str )
  {
	  return (float)::atof( ConvertMAIDtoSJIS(str).c_str() );
  }

  //!	文字を浮動小数点に変換する
  /*!
 	  @param	str	[i ]	変換する文字列
 	  @return	浮動小数点
   */
  String String::ItoA( int value )
  {
	  char buf[256];
	  ::sprintf_s( buf, "%d", value );

	  return ConvertSJIStoMAID(buf);
  }

  //!	浮動小数点を文字列に変換する
  /*!
 	  @param	value	[i ]	変換したい値
 	  @return	文字列
   */
  String String::FtoA( float value )
  {
	  char buf[256];
	  ::sprintf_s( buf, "%f", value );

	  return ConvertSJIStoMAID(buf);
  }

  //!	sprintf の String版
  /*!
 	  @param	format	[i ]	整形
 	  @return	文字列
   */
  String String::PrintFormat( const char* format, ...)
  {
    va_list args;
    char buf[1024];

    va_start(args,format);
    vsprintf_s(buf, format, args);
//    va_end(ap);

    return ConvertSJIStoMAID(buf);
  }

}



