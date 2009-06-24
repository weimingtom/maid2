/*!	@file
    @brief	maidライブラリの中で扱う文字列クラス
\n          maidの中ではこれ以外の文字列が存在することはありません
*/

#ifndef maid2_auxiliary_string_h
#define maid2_auxiliary_string_h

#include"../config/typedef.h"

#include<string>
#include<ostream>


namespace Maid
{


  /*!	
    @brief  文字列クラス
  \n			文字コードは UTF16 と一緒で、１文字当たり強制的に32bit使用してます。
  \n      UTF16 と違ってBOMがありません。
  \n			1文字ごとのエンディアンはコンパイラ依存です
  \n			そうゆうわけで物理メモリ上に存在しているときだけ有効な文字クラスです
  \n			ファイルに書き出すときは必ずSJISなどに変換すること
  \n			文字コード名は MSTRING としておきます。
  */
  class String : public std::basic_string<unt32>
  {
  public:
    String() : basic_string<unt32>(){}
    String( const std::basic_string<unt32>& str ) : basic_string<unt32>(str) {}

    enum
    {
      CODE_LF = 0x0000000A,         //!<	'\r'
      CODE_CR = 0x0000000D,         //!<	'\n'
      CODE_BACKSLASH = 0x0000005C,	//!<	'\\'
      CODE_SLASH = 0x0000002F,      //!<	'/'
      CODE_SPACE = 0x00000020,      //!<	' '
    };


    static void Initialize();

	  static bool IsSJIS1( char x );
    static bool IsUTF16H( wchar_t x );
    static bool IsUTF16L( wchar_t x );

	  static String ConvertUNICODEtoMAID( const std::wstring& str );
	  static String ConvertSJIStoMAID( const std::string& str );
	  static std::wstring ConvertMAIDtoUNICODE( const String& str );
	  static std::string ConvertMAIDtoSJIS( const String& str );

	  static std::wstring ConvertSJIStoUNICODE( const std::string& str );


	  static unt32 ConvertCharaMAIDtoUNICODE( unt32 str );
	  static unt32 ConvertCharaMAIDtoSJIS( unt32 str );

	  static int    CountByte( const String& str );
	  static int    AtoI( const String& str );
	  static float  AtoF( const String& str );
	  static bool   IsHankaku( unt32 c );
	  static bool   IsGaizi( unt32 c );
	  static unt32  MakeGaizi( unt32 c );

	  static String ItoA( int value );
	  static String FtoA( float value );
    static String PrintFormat( const char* format, ...);

	  static String ToLower( const String& str );
	  static String GetDirectory( const String& Path );
	  static String GetExtension( const String& Path );
	  static String GetFileName( const String& Path );

  };

  //!	ストリーム出力用オペレータ
  std::ostream&  operator<<(std::ostream& out, const String& a);

	//!	ソースに文字列を埋め込むときに変換するコンバータ
  #define MAIDTEXT( t ) (Maid::String::ConvertSJIStoMAID((t)))
}

#endif