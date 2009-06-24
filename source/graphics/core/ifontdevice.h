/*!
	  @file
	  @brief フォント管理のベースクラス
 */

#ifndef framework_ifontdevice_h
#define framework_ifontdevice_h

#include"../config/define.h"

#include"../auxiliary/string.h"
#include"../auxiliary/mathematics/size.h"

#include"../graphics/surface.h"

#include<boost/smart_ptr.hpp>

namespace Maid { namespace Graphics {

  /*!	
      @class	IFont ifontdevice.h
      @brief	フォントハンドル
  */
  class IFont
  {
  public:
    IFont( const String& name, const SIZE2DI& size )
      :m_Name(name),m_Size(size){}
    virtual ~IFont(){}

    const String&  GetName() const { return m_Name; }
    const SIZE2DI& GetSize() const { return m_Size; }

  private:
    const String  m_Name; //  フォント名
    const SIZE2DI m_Size; //  半角の大きさ
  };

  typedef	boost::shared_ptr<IFont>	SPFONT;



  /*!	
      @class	IFontDevice ifontdevice.h
      @brief	フォントを作るデバイス
  */
  class IFontDevice
  {
  public:
    virtual ~IFontDevice(){}

    //! デバイスの初期化
    /*!	
    */
    virtual void Initialize() = 0;

    struct FONTINFO
    {
      String  Name; //!<  フォント名
    };

    //! デバイスに存在している作成可能なフォントを取得する
    /*!	
        @param  List  [ o]  フォント名
    */
    virtual void GetFontList( std::vector<FONTINFO>& List )=0;

    //! フォントを作成する
    /*!	
        @param  name  [i ]  作るフォント名
        @param  size  [o ]  フォントの大きさ
    */
    virtual SPFONT CreateFont( const String& name, const SIZE2DI& size )=0;

    //! システムに標準で使われているフォント名を取得する
    /*!	
        このフォントはかならず存在しています

        @return  フォント名
    */
    virtual String GetDefaultFontName()const=0;


    //! フォントをラスタライズする
    /*!	
        ラスタライズしたピクセルは 1.0f になっているものとします
        ラスタライズされていないピクセルは 0.0f になっているものとします

        @param  pFont     [i ]  フォント
        @param  FontCode  [i ]  ラスタライズする文字コード(FontCode)
        @param  surf      [ o]  ラスタライズ先。メモリは自動的に確保されます
    */    
    virtual void Rasterize( const SPFONT& pFont, unt32 FontCode, SurfaceInstance& surf )=0;
  };

  typedef	boost::shared_ptr<IFontDevice>	SPFONTDEVICE;

}}


#endif