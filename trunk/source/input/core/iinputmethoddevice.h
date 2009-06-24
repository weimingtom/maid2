#ifndef maid2_input_core_iinputmethoddevice_h
#define maid2_input_core_iinputmethoddevice_h

#include"../../config/define.h"
#include"../../auxiliary/string.h"

#include<boost/smart_ptr.hpp>

namespace Maid{ namespace Input {

	/*!
	 	@class	IInputMethodDevice iinputmethoddevice.h
	 	@brief	InputMethod クラス
	\n			いわゆるＦＥＰを管理するクラス
	\n			このクラスは描画を行わないのでほかのクラスで行う必要があります。
	 */

  class IInputMethodDevice
  {
  public:
    virtual ~IInputMethodDevice(){}

		virtual void Initialize()=0;
		virtual void Finalize()=0;

    //! FEP の起動
    /*!
     */
    virtual void Open() =0;

    //! FEP の終了
    /*!
     */
    virtual void Close()=0;


    //! FEP がうごいているか？
    /*!
 	    @return ＦＥＰが動いているなら true
     \n			うごいていないなら false
     */
    virtual bool IsOpen() const=0;

    //! 変換位置の取得
    /*!
 	    @return	変換位置
     */
    virtual int GetConvertPos() const=0;

    //! カーソル位置の取得
    /*!
 	    @return	カーソル位置
     */
    virtual int GetCursorPos() const=0;

    //! 変換候補数の取得
    /*!
 	    @return 変換候補数
     */
    virtual int GetCandidateCount() const=0;

    //! 現在選択中の変換候補の取得
    /*!
 	    @return 変換候補位置
     */
    virtual int GetCandidateSelect() const=0;

    //! 指定されたインデックスの変換候補を取得
    /*!
 	    @return 変換候補文字
     */
    virtual String GetCandidateString( int Index ) const=0;

    //! FEP入力文字列の取得
    /*!
 	    @return	入力中の文字列
     */
    virtual String GetCompString() const=0;


    //! 文字が確定された場合、に入っているバッファ
    /*!
      @param	buffer	[ o]  入力が確定した文字列
     */
    virtual void Flush( String& buffer )=0;
  };

	typedef boost::shared_ptr<IInputMethodDevice> SPINPUTMETHODDEVICE;
}}

#endif
