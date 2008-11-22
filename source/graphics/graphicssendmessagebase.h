/*!	
	@file
	@brief	描画命令メッセージ 定義
*/

#ifndef maid2_graphics_graphicssendmessagebase_h
#define maid2_graphics_graphicssendmessagebase_h

#include"../config/define.h"

#include<boost/smart_ptr.hpp>
#include<vector>

#include"../auxiliary/mathematics.h"
#include"../auxiliary/string.h"

namespace Maid
{
  //	描画スレッドに投げる　メッセージクラス

  namespace GraphicsSendMessage
  {
    typedef uintptr_t OBJECTID;
    struct Base
    {
      //	描画メッセージ
      enum MESSAGE
      {
	      NONE,				//!<	何もしない

	      SCREENMODECHANGE,		//!<	スクリーンモード変更

	      CREATERENDERTARGETSURFACE,  //!<	レンダリングサーフェスの作成
	      CREATEDEPTHSTENCILSURFACE,  //!<	深度バッファの登録
	      CREATETEXTURE,		          //!<	テクスチャの登録
	      CREATERENDERTARGETTEXTURE,  //!<	レンダリングテクスチャの作成
	      CREATEVERTEX,		            //!<	頂点の登録
	      CREATEINDEX,		            //!<	インデックスの登録
	      CREATEVERTEXSHADER,	        //!<	バーテックスシェーダーの登録
	      CREATEVERTEXDECLARATION,    //!<	頂点定義の登録
	      CREATEPIXELSHADER,	        //!<	ピクセルシェーダーの登録

	      DELETESURFACE,              //!<	サーフェスの破棄
	      DELETETEXTURE,              //!<	テクスチャの破棄
	      DELETEVERTEX,               //!<	頂点の破棄
	      DELETEINDEX,                //!<	インデックスの破棄
	      DELETEVERTEXSHADER,	        //!<	バーテックスシェーダーの破棄
	      DELETEVERTEXDECLARATION,	  //!<	頂点定義の登録
	      DELETEPIXELSHADER,	        //!<	ピクセルシェーダーの破棄


	      CLEAR_RENDERTARGET, //!<	描画先のクリア
	      CLEAR_DEPTHSTENCIL, //!<	描画先のクリア

	      RESETRENDERTARGET,	//!<	レンダーターゲットのリセット
	      SETRENDERTARGET,	  //!<	レンダーターゲットのセット
	      SETTEXTURE,			    //!<	テクスチャのセット
	      SETVERTEX,			    //!<	頂点のセット
	      SETINDEX,			      //!<	インデックスのセット
	      SETVERTEXSHADER,	  //!<	バーテックスシェーダーのセット
	      SETPIXELSHADER,		  //!<	ピクセルシェーダーのセット
	      SETVERTEXDECLARATION,	//!<	頂点定義のセット

	      SETVERTEXSHADER_CONSTF,	//!<	const float セット

	      SETVIEWPORT,		//!<	ビューポートの設定
	      SETRENDERSTATE,		//!<	レンダーステート
	      SETSAMPLERSTATE,

	      DRAWPRIMITIVE,		    //!<	ポリゴンの描画
	      DRAWPRIMITIVEUP,	    //!<	ポリゴンの描画
	      DRAWINDEXPRIMITIVE,	  //!<	ポリゴンの描画
	      DRAWINDEXPRIMITIVEUP,	//!<	ポリゴンの描画

	      COPYRENDERTARGET,		//!<	システムメモリに転送
	      GETVIDEOINFO,		    //!<	ビデオ能力を取得
      };

      Base( MESSAGE mess ) : Message(mess){}

      const MESSAGE	Message;
      String GetMessage() const;
    };
  }

  typedef	boost::shared_ptr<GraphicsSendMessage::Base> SPGRAPHICSRENDERMESSAGE;
  typedef	std::vector<SPGRAPHICSRENDERMESSAGE>         GRAPHICSRENDERMESSAGEQUE;

  typedef boost::shared_ptr<std::vector<unt08> > SPBINARY;

}


#endif
