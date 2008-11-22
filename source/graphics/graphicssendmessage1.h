#ifndef maid2_graphics_graphicssendmessage1_h
#define maid2_graphics_graphicssendmessage1_h

#include"../config/define.h"
#include"graphicssendmessagebase.h"
#include"core/driver/ivideodevice.h"

#include"color.h"

//	このヘッダには補助的なメッセージを定義する

namespace Maid
{
	namespace GraphicsSendMessage
	{
		struct ClearRenderTarget : public Base
		{
			ClearRenderTarget() : Base(CLEAR_RENDERTARGET){}

			COLOR_A32B32G32R32F	Color;	//	塗りつぶし色
		};

		struct ClearDepthStencil : public Base
		{
			ClearDepthStencil() : Base(CLEAR_DEPTHSTENCIL){}

			float	Depth;	//	塗りつぶし色
      unt32 Stencil;
		};

		struct ResetRenderTarget : public Base
		{
			ResetRenderTarget() : Base(RESETRENDERTARGET){}
		};

		struct SetRenderTarget : public Base
		{
			SetRenderTarget() : Base(SETRENDERTARGET){}

			enum TYPE
			{
				TYPE_SURFACE,	//	設定元はサーフェス
				TYPE_TEXTURE,	//	設定元はテクスチャ
			};

			OBJECTID	ColorID;
			TYPE      ColorType;
      
			OBJECTID	DepthStencilID;
		};

		struct SetTexture : public Base
		{
			SetTexture() : Base(SETTEXTURE){}

			OBJECTID	ID;
			int		    Stage;
		};

		struct SetVertex : public Base
		{
			SetVertex() : Base(SETVERTEX){}

			OBJECTID	ID;
			int       pos;
		};

		struct SetIndex : public Base
		{
			SetIndex() : Base(SETINDEX){}

			OBJECTID	ID;
		};

		struct SetVertexShader : public Base
		{
			SetVertexShader() : Base(SETVERTEXSHADER){}

			OBJECTID	ID;
		};

		struct SetVertexShaderConstF : public Base
		{
			SetVertexShaderConstF() : Base(SETVERTEXSHADER_CONSTF){}
			int pos;
			VECTOR4DF vec;
		};

		struct SetVertexDeclaration : public Base
		{
			SetVertexDeclaration() : Base(SETVERTEXDECLARATION){}

			OBJECTID	ID;
		};

		struct SetPixelShader : public Base
		{
			SetPixelShader() : Base(SETPIXELSHADER){}

			OBJECTID	ID;
		};





		struct SetViewPort : public Base
		{
			SetViewPort() : Base(SETVIEWPORT){}

			RECT2DI Screen;
			float	MinZ;
			float	MaxZ;
		};

		struct SetRenderState : public Base
		{
			SetRenderState() : Base(SETRENDERSTATE){}

			IVideoDevice::RENDERSTATE state;
			unt	Value;
		};

		struct SetSamplerState : public Base
		{
			SetSamplerState() : Base(SETSAMPLERSTATE){}

			int	Stage;
			IVideoDevice::SAMPLERSTATE State;
			unt32	Value;
		};


		struct CopyRenderTarget : public Base
		{
			CopyRenderTarget() : Base(COPYRENDERTARGET){}

			OBJECTID ID;
		};

		struct GetVideoInfo : public Base
		{
			GetVideoInfo() : Base(GETVIDEOINFO){}
		};
	}

}


#endif
