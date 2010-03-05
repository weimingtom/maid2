#include"deviced3d09.h"

#include"vertexd3d09.h"
#include"indexd3d09.h"
#include"constantd3d09.h"
#include"texture2dd3d09.h"
#include"rendertargetd3d09.h"
#include"depthstencild3d09.h"
#include"materiald3d09.h"
#include"inputlayoutd3d09.h"
#include"rasterizerstated3d09.h"
#include"samplerstated3d09.h"
#include"depthstencilstated3d09.h"
#include"blendstated3d09.h"
#include"vertexshaderd3d09.h"
#include"pixelshaderd3d09.h"
#include"pixelshaderd3d09fixed.h"


#include"../../../../auxiliary/debug/trace.h"


namespace Maid { namespace Graphics {


IBuffer* DeviceD3D09::CreateVertex( const CREATEBUFFERPARAM& param, const SUBRESOURCE* data )
{
  const SPD3D09DEVICE& pDev = m_pDevice;
  const UINT  length = param.Length;
  const DWORD usage = 0;
  const DWORD fvf = 0;
  const D3DPOOL pool = D3DPOOL_MANAGED;

  IDirect3DVertexBuffer9* p=NULL;
  {
    const HRESULT ret = pDev->CreateVertexBuffer( length, usage, fvf, pool, &p, NULL );
    if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::CreateVertexBuffer()")); goto MAID_ERROR; }
  }

  if( data!=NULL )
  {
    VOID* pDst = NULL;

    {
      const HRESULT ret = p->Lock( 0, 0, &pDst, 0 );
      if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DVertexBuffer9::Lock()")); goto MAID_ERROR; }
    }

    ::memcpy( pDst, data->pData, data->Pitch );

    {
      const HRESULT ret = p->Unlock();
      if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DVertexBuffer9::Unlock()")); goto MAID_ERROR; }
    }
  }


  return new VertexD3D09( param, p );


MAID_ERROR:
  if( p!=NULL ) { p->Release(); }
  return NULL;
}

IBuffer* DeviceD3D09::CreateIndex( const CREATEBUFFERPARAM& param, const SUBRESOURCE* data )
{
  const SPD3D09DEVICE& pDev = m_pDevice;
  const UINT len = param.Length;
	const DWORD use = 0;
  const D3DFORMAT fmt = D3DFMT_INDEX16;
	const D3DPOOL pool = D3DPOOL_MANAGED;

  IDirect3DIndexBuffer9* p=NULL;
  {
    const HRESULT ret = pDev->CreateIndexBuffer( len, use, fmt, pool, &p, NULL );
    if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::CreateIndexBuffer()")); goto MAID_ERROR; }
  }

  if( data!=NULL )
  {
    VOID* pDst = NULL;

    {
      const HRESULT ret = p->Lock( 0, 0, &pDst, 0 );
      if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DIndexBuffer9::Lock()")); goto MAID_ERROR; }
    }

    ::memcpy( pDst, data->pData, data->Pitch );

    {
      const HRESULT ret = p->Unlock();
      if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DIndexBuffer9::Unlock()")); goto MAID_ERROR; }
    }
  }

  return new IndexD3D09( param, p );


MAID_ERROR:
  if( p!=NULL ) { p->Release(); }
  return NULL;
}

IBuffer* DeviceD3D09::CreateConstant( const CREATEBUFFERPARAM& param, const SUBRESOURCE* data )
{
  return new ConstantD3D09( param );
}


SPBUFFER DeviceD3D09::CreateBuffer( const CREATEBUFFERPARAM& param, const SUBRESOURCE* data )
{
  //  D3D09でVertex,Index,Constant,はひとつのバッファにまとめれないので、チェック処理入れる

  IBuffer* pBuffer=NULL;
  if( IsFlag(param.BindFlags,RESOURCEBINDFLAG_VERTEX) )
  {
    pBuffer = CreateVertex( param, data );
  }
  else if( IsFlag(param.BindFlags,RESOURCEBINDFLAG_CONSTANT) )
  {
    pBuffer = CreateConstant( param, data );
  }
  else if( IsFlag(param.BindFlags,RESOURCEBINDFLAG_INDEX) )
  {
    pBuffer = CreateIndex( param, data );
  }

  return SPBUFFER(pBuffer);
}

SPTEXTURE2D DeviceD3D09::CreateRenderTargetTexture( const CREATERETEXTURE2DPARAM& param )
{
  const SPD3D09DEVICE& pDev = m_pDevice;
  const SIZE2DI size = param.Size;

  const UINT    mip  = param.MipLevels;
  DWORD   usage = D3DUSAGE_RENDERTARGET;
  D3DPOOL pool = D3DPOOL_DEFAULT;

  SPD3D09TEXTURE pTex;
  SPD3D09SURFACE pSurf;

  {
    IDirect3DTexture9* p=NULL;
    const HRESULT ret = pDev->CreateTexture( size.w, size.h, mip, usage, PIXELFORMATtoD3DFORMAT(param.Format), pool, &p, NULL );
	  if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::CreateTexture()")); goto MAID_TEXERROR; }
    pTex.reset(p);
  }

  {
    IDirect3DSurface9* p;
    const HRESULT ret = pTex->GetSurfaceLevel( 0, &p );
	  if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::GetSurfaceLevel()")); goto MAID_TEXERROR; }
    pSurf.reset(p);
  }

  return SPTEXTURE2D(new Texture2DD3D09( param, pTex, pSurf ));

MAID_TEXERROR:
  return SPTEXTURE2D();
}

SPTEXTURE2D DeviceD3D09::CreateRenderTarget( const CREATERETEXTURE2DPARAM& param )
{
  const SPD3D09DEVICE& pDev = m_pDevice;
  const SIZE2DI size = param.Size;
  const D3DFORMAT fmt = PIXELFORMATtoD3DFORMAT(param.Format);
  const D3DMULTISAMPLE_TYPE sample = SAMPLETYPEtoD3DMULTISAMPLE_TYPE(param.Sample.Count);
  const DWORD quality = param.Sample.Quality;
  const BOOL  lock = FALSE;

  SPD3D09SURFACE pTarget;
  {
    IDirect3DSurface9* p=NULL;
    const HRESULT ret = pDev->CreateRenderTarget( size.w, size.h, fmt, sample, quality,  lock, &p, NULL );
	  if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::CreateRenderTarget()")); goto MAID_TEXERROR; }
    pTarget.reset(p);
  }
  return SPTEXTURE2D(new Texture2DD3D09( param, pTarget, false ));

MAID_TEXERROR:
  return SPTEXTURE2D();
}


SPTEXTURE2D DeviceD3D09::CreateDepthStencil( const CREATERETEXTURE2DPARAM& param )
{
  const SPD3D09DEVICE& pDev = m_pDevice;
  const SIZE2DI size = param.Size;
  const D3DFORMAT fmt = PIXELFORMATtoD3DFORMAT(param.Format);
  const D3DMULTISAMPLE_TYPE sample = SAMPLETYPEtoD3DMULTISAMPLE_TYPE(param.Sample.Count);
  const DWORD quality = param.Sample.Quality;
  const BOOL  discard = FALSE;

  SPD3D09SURFACE pTarget;
  {
    IDirect3DSurface9* p=NULL;

    const HRESULT ret = pDev->CreateDepthStencilSurface( size.w, size.h, fmt, sample, quality,  discard, &p, NULL );
	  if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::CreateRenderTarget()")); goto MAID_TEXERROR; }
    pTarget.reset(p);
  }
  return SPTEXTURE2D(new Texture2DD3D09( param, pTarget, false ));

MAID_TEXERROR:
  return SPTEXTURE2D();
}

SPTEXTURE2D DeviceD3D09::CreateTexture( const CREATERETEXTURE2DPARAM& param, const SUBRESOURCE* pSub )
{
  const SPD3D09DEVICE& pDev = m_pDevice;
  const SIZE2DI size = param.Size;
  const UINT    level  = param.MipLevels;
  DWORD   usage = 0;
  D3DPOOL pool = D3DPOOL_MANAGED;

  CREATERETEXTURE2DPARAM nowparam = param;

  switch( param.Usage )
  {
  case RESOURCEUSAGE_IMMUTABLE: { pool = D3DPOOL_MANAGED;   }break;
  case RESOURCEUSAGE_DYNAMIC: { pool = D3DPOOL_MANAGED;   }break;
  case RESOURCEUSAGE_DEFAULT: { pool = D3DPOOL_MANAGED;   }break;
  default: { MAID_ASSERT( true, "未実装 " << param.Usage ); }break;
  }

  SPD3D09TEXTURE  pTex;
  {
    IDirect3DTexture9* p=NULL;

    const HRESULT ret = pDev->CreateTexture( size.w, size.h, level, usage, PIXELFORMATtoD3DFORMAT(param.Format), pool, &p, NULL );
    if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::CreateTexture()")); goto MAID_TEXERROR; }
    pTex.reset(p);
  }

  if( pSub!=NULL )
  {
    for( int i=0; i<(int)level; ++i )
    {
      const SUBRESOURCE& data = pSub[i];

      D3DLOCKED_RECT rc;

      D3DSURFACE_DESC DstDesc;

      {
        const HRESULT ret = pTex->GetLevelDesc( i, &DstDesc );
        if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DTexture9::GetLevelDesc()")); goto MAID_TEXERROR; }
      }

      {
        const HRESULT ret = pTex->LockRect( i, &rc, NULL, 0 );
        if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DTexture9::LockRect()")); goto MAID_TEXERROR; }
      }

      {
        const size_t len = GetPixelBPP(param.Format) / 8 *  DstDesc.Width;

        unt08* pDst = (unt08*)rc.pBits;
        unt08* pSrc = (unt08*)data.pData;
        for( int y=0; y<(int)DstDesc.Height; ++y )
        {
          ::memcpy( pDst, pSrc, len );
          pDst += rc.Pitch;
          pSrc += data.Pitch;
        }
      }
      {
        const HRESULT ret = pTex->UnlockRect( i );
        if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DTexture9::UnlockRect()")); goto MAID_TEXERROR; }
      }
    }
  }

  nowparam.MipLevels = pTex->GetLevelCount();

  return SPTEXTURE2D(new Texture2DD3D09( nowparam, pTex ));

MAID_TEXERROR:
  return SPTEXTURE2D();
}


SPTEXTURE2D DeviceD3D09::CreateTexture2D( const CREATERETEXTURE2DPARAM& param, const SUBRESOURCE* data )
{
  if( param.Usage==RESOURCEUSAGE_STAGING )
  {
	  const int w = param.Size.w;
	  const int h = param.Size.h;
	  const D3DFORMAT fmt = PIXELFORMATtoD3DFORMAT(param.Format);

    SPD3D09SURFACE  pTex;
    {
		  IDirect3DSurface9* p=NULL;
		  const HRESULT ret = m_pDevice->CreateOffscreenPlainSurface( w, h, fmt, D3DPOOL_SYSTEMMEM, &p, NULL );
		  if( FAILED(ret) ) { MAID_THROWEXCEPTION(MAIDTEXT("IDirect3DDevice9::CreateOffscreenPlainSurface()")); return SPTEXTURE2D(); }
      pTex.reset(p);
    }

    return SPTEXTURE2D(new Texture2DD3D09( param, pTex, false ));
  }
  else if( IsFlag(param.BindFlags,RESOURCEBINDFLAG_DEPTHSTENCIL) )
  {
    return CreateDepthStencil( param );
  }
  else if( IsFlag(param.BindFlags,RESOURCEBINDFLAG_RENDERTARGET) )
  {
    if( IsFlag(param.BindFlags,RESOURCEBINDFLAG_MATERIAL) )
    {
      return CreateRenderTargetTexture( param );
    }else
    {
      return CreateRenderTarget( param );
    }
  }
  else
  {
    return CreateTexture( param, data );
  }

  return SPTEXTURE2D();
}

SPRENDERTARGET DeviceD3D09::CreateRenderTarget( const SPRESOURCE& resource, const CREATERENDERTARGETPARAM* param )
{
  MAID_ASSERT( resource->GetType()!=IResource::TYPE_TEXTURE2D, "テクスチャ以外は対応していません" );

  CREATERENDERTARGETPARAM p;

  if( param==NULL )
  {
    Texture2DD3D09* pTex = static_cast<Texture2DD3D09*>(resource.get());
    const CREATERETEXTURE2DPARAM& texparam = pTex->GetParam();

    p.Format    = texparam.Format;
    p.Dimension = CREATERENDERTARGETPARAM::DIMENSION_TEXTURE2D;
  }else
  {
    p = *param;
  }

  return SPRENDERTARGET( new RenderTargetD3D09( resource, p ) );
}

SPDEPTHSTENCIL DeviceD3D09::CreateDepthStencil( const SPRESOURCE& resource, const CREATEDEPTHSTENCILPARAM* param )
{
  MAID_ASSERT( resource->GetType()!=IResource::TYPE_TEXTURE2D, "テクスチャ以外は対応していません" );

  CREATEDEPTHSTENCILPARAM p;

  if( param==NULL )
  {
    Texture2DD3D09* pTex = static_cast<Texture2DD3D09*>(resource.get());
    const CREATERETEXTURE2DPARAM& texparam = pTex->GetParam();

    p.Format    = texparam.Format;
    p.Dimension = CREATEDEPTHSTENCILPARAM::DIMENSION_TEXTURE2D;
  }else
  {
    p = *param;
  }

  return SPDEPTHSTENCIL( new DepthStencilD3D09( resource, p ) );
}

SPMATERIAL DeviceD3D09::CreateMaterial( const SPRESOURCE& resource, const CREATEMATERIALPARAM* param )
{
  MAID_ASSERT( resource->GetType()!=IResource::TYPE_TEXTURE2D, "テクスチャ以外は対応していません" );

  CREATEMATERIALPARAM p;

  if( param==NULL )
  {
    Texture2DD3D09* pTex = static_cast<Texture2DD3D09*>(resource.get());
    const CREATERETEXTURE2DPARAM& texparam = pTex->GetParam();

    p.Format    = texparam.Format;
    p.Dimension = CREATEMATERIALPARAM::DIMENSION_TEXTURE2D;
  }else
  {
    p = *param;
  }

  return SPMATERIAL( new MaterialD3D09(resource,p) );
}

SPVERTEXSHADER DeviceD3D09::CreateVertexShader( const void* pData, size_t Length )
{
  const SPD3D09DEVICE& pDev = m_pDevice;
  IDirect3DVertexShader9* pShader=NULL;

  const DWORD ver = m_GetShaderVersion( (DWORD*)pData );


  const HRESULT ret = pDev->CreateVertexShader( (DWORD*)pData, &pShader );
  if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::CreateVertexShader()")); goto MAID_LABEL; }

  return SPVERTEXSHADER(new VertexShaderD3D09(pShader,ver));

MAID_LABEL:
  return SPVERTEXSHADER();
}

SPPIXELSHADER DeviceD3D09::CreatePixelShader( const void* pData, size_t Length )
{
  const SPD3D09DEVICE& pDev = m_pDevice;
  PixelShaderD3D09* p = NULL;

  if( Length==4 )
  { //  標準機能を使う
    DWORD no = 0;
    memcpy( &no, pData, Length  );

    //  使うんだけど、ハードが対応してたらそれにする
    //  ps 2.0 しか使わない
    if( 0x0200 <= (m_DeviceCaps.PixelShaderVersion&0xFFFF) )
    {
      std::vector<unt08> code;
      CreateDefaultPixelShader( no, code );

      IDirect3DPixelShader9* pShader;

      const HRESULT ret = pDev->CreatePixelShader( (DWORD*)&(code[0]), &pShader );
      if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::CreatePixelShader()")); goto MAID_LABEL; }

      p = new PS_SHADERCODE(pShader);

    }else
    {
      p = new PS_FIXED(no);
    }
  }else
  {
    IDirect3DPixelShader9* pShader;

    const HRESULT ret = pDev->CreatePixelShader( (DWORD*)pData, &pShader );
    if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::CreatePixelShader()")); goto MAID_LABEL; }

    p = new PS_SHADERCODE(pShader);
  }

  return SPPIXELSHADER(p);

MAID_LABEL:
  return SPPIXELSHADER();
}


SPSAMPLERSTATE DeviceD3D09::CreateSamplerState( const SAMPLERSTATEPARAM& Option )
{
  SAMPLERSTATELIST list;

  list.push_back( SAMPLERSTATE( D3DSAMP_ADDRESSU, ADDRESStoD3DTEXTUREADDRESS(Option.AddressU) ) );
  list.push_back( SAMPLERSTATE( D3DSAMP_ADDRESSV, ADDRESStoD3DTEXTUREADDRESS(Option.AddressV) ) );
  list.push_back( SAMPLERSTATE( D3DSAMP_ADDRESSW, ADDRESStoD3DTEXTUREADDRESS(Option.AddressW) ) );

  list.push_back( SAMPLERSTATE( D3DSAMP_MAGFILTER, FILTERtoD3DTEXTUREFILTERTYPE(Option.MagFilter) ) );
  list.push_back( SAMPLERSTATE( D3DSAMP_MINFILTER, FILTERtoD3DTEXTUREFILTERTYPE(Option.MinFilter) ) );
  list.push_back( SAMPLERSTATE( D3DSAMP_MIPFILTER, FILTERtoD3DTEXTUREFILTERTYPE(Option.MipFilter) ) );

  {
    const COLOR_A08R08G08B08I col = PIXELCONVERTtoI(Option.BorderColor);

    list.push_back( SAMPLERSTATE( D3DSAMP_BORDERCOLOR, *((DWORD*)&col) ) );
  }
/*
  //  いらない感じするのでスルー
  list.push_back( SAMPLERSTATE( D3DSAMP_MIPMAPLODBIAS, Option.MipLODBias ) );
  list.push_back( SAMPLERSTATE( D3DSAMP_MAXMIPLEVEL, Option.MaxLOD ) );
  list.push_back( SAMPLERSTATE( D3DSAMP_MAXANISOTROPY, Option.MaxAnisotropy ) );
*/

  return SPSAMPLERSTATE( new SamplerStateD3D09( Option, list ));
}

SPRASTERIZERSTATE DeviceD3D09::CreateRasterizerState( const RASTERIZERSTATEPARAM& Option )
{
  std::vector<RENDERSTATE> v;

  {
    RENDERSTATE s;
    s.Type = D3DRS_FILLMODE;
    switch( Option.Fill )
    {
    case RASTERIZERSTATEPARAM::FILL_WIREFRAME: { s.Value = D3DFILL_WIREFRAME; }break;
    case RASTERIZERSTATEPARAM::FILL_SOLID: { s.Value = D3DFILL_SOLID; }break;
    }

    v.push_back( s );
  }

  {
    RENDERSTATE s;
    s.Type = D3DRS_CULLMODE;
    switch( Option.Culling )
    {
    case RASTERIZERSTATEPARAM::CULLING_NONE:  { s.Value = D3DCULL_NONE; }break;
    case RASTERIZERSTATEPARAM::CULLING_BACK:  { s.Value = D3DCULL_CCW; }break;
    case RASTERIZERSTATEPARAM::CULLING_FRONT: { s.Value = D3DCULL_CW ; }break;
    }

    v.push_back( s );
  }
  {
    RENDERSTATE s;
    s.Type =  D3DRS_MULTISAMPLEANTIALIAS;
    s.Value= booltoBOOL( Option.MultiSample );
    v.push_back( s );
  }

  return SPRASTERIZERSTATE(new RasterizerStateD3D09(Option,v));
}

SPDEPTHSTENCILSTATE DeviceD3D09::CreateDepthStencilState( const DEPTHSTENCILSTATEPARAM& Option )
{
  std::vector<RENDERSTATE> v;
  {
    RENDERSTATE s;
    s.Type = D3DRS_ZENABLE;
    s.Value= booltoBOOL( Option.DepthTest );
    v.push_back( s );
  }
  {
    RENDERSTATE s;
    s.Type = D3DRS_ZWRITEENABLE;
    s.Value= booltoBOOL( Option.DepthTest );
    v.push_back( s );
  }
  {
    RENDERSTATE s;
    s.Type = D3DRS_ZFUNC;
    s.Value= CMPTYPEtoD3DCMPFUNC( Option.DepthFunc );
    v.push_back( s );
  }

  {
    RENDERSTATE s;
    s.Type = D3DRS_STENCILENABLE;
    s.Value= booltoBOOL( Option.StencilTest );
    v.push_back( s );
  }

  return SPDEPTHSTENCILSTATE( new DepthStencilStateD3D09(v) );
}


SPBLENDSTATE DeviceD3D09::CreateBlendState( const BLENDSTATEPARAM& Option )
{
  //  D3D9だと ８つあるRenderTargetは全部同じ設定でしかレンダリングできない。
  //  このクラス内でごにょごにょすればうまく出来そうだけど、めんどいので
  //  Target[0]以外は破棄してしまう。

  const BLENDSTATEPARAM::TARGET& t = Option.Target[0];
  std::vector<RENDERSTATE> v;

  v.push_back( RENDERSTATE(D3DRS_ALPHABLENDENABLE,booltoBOOL(t.Enable)) );

  v.push_back( RENDERSTATE(D3DRS_SRCBLEND, TARGETBLENDtoD3DBLEND(t.ColorSrc)) );
  v.push_back( RENDERSTATE(D3DRS_DESTBLEND,TARGETBLENDtoD3DBLEND(t.ColorDst)) );
  v.push_back( RENDERSTATE(D3DRS_BLENDOP,  TARGETOPERATIONtoD3DBLENDOP(t.ColorOp)) );

  v.push_back( RENDERSTATE(D3DRS_SRCBLENDALPHA, TARGETBLENDtoD3DBLEND(t.AlphaSrc)) );
  v.push_back( RENDERSTATE(D3DRS_DESTBLENDALPHA,TARGETBLENDtoD3DBLEND(t.AlphaDst)) );
  v.push_back( RENDERSTATE(D3DRS_BLENDOPALPHA,  TARGETOPERATIONtoD3DBLENDOP(t.AlphaOp)) );

  v.push_back( RENDERSTATE(D3DRS_COLORWRITEENABLE,  t.WriteMask ) );

  return SPBLENDSTATE(new BlendStateD3D09( Option, v ));
}

SPINPUTLAYOUT DeviceD3D09::CreateInputLayout( const INPUT_ELEMENT* Element, int Count, const void* pShaderBytecodeWithInputSignature, size_t BytecodeLength )
{
  std::vector<D3DVERTEXELEMENT9>  elem( Count );

  for( int i=0; i<Count; ++i )
  {
    const INPUT_ELEMENT& src = Element[i];
    D3DVERTEXELEMENT9&   dst = elem[i];

    dst.Stream = src.SlotNo;
    dst.Offset = src.Offset;
    dst.Type = INPUT_ELEMENTTYPEtoD3DDECLTYPE(src.Type);
    dst.Method = INPUT_ELEMENTMETHODtoD3DDECLMETHOD(src.Method);

    dst.Usage = INPUT_ELEMENTSemanticNametoD3DDECLUSAGE(src.SemanticName.c_str());
    dst.UsageIndex = src.SemanticIndex;
  }

  {
    const D3DVERTEXELEMENT9 end = D3DDECL_END();
    elem.push_back( end );
  }

  IDirect3DVertexDeclaration9* p=NULL;

  const HRESULT ret = m_pDevice->CreateVertexDeclaration( &(elem[0]), &p );
  if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::CreateVertexDeclaration()")); goto MAID_ERROR; }

  return SPINPUTLAYOUT( new InputLayoutD3D09(p) );

MAID_ERROR:
  return SPINPUTLAYOUT();
}

bool DeviceD3D09::CompileShaderLanguage( const String& Source, CODETYPE type, std::vector<unt08>& Binary, String& ErrorMessage )
{
  const std::string SrcData = String::ConvertMAIDtoSJIS(Source);

  //  固定機能を使う場合は言語サイズが4バイト以下かつ
  //  全部数字であることが条件なのでそれを元に作る
  //  いろいろしらべたけど、シェーダーバイナリの最小は32byteっぽいので、これなら自由にやれるはず
  if( SrcData.length()<=4 )
  {
    const unt32 no = atoi( SrcData.c_str() );

    if( 100<=no && no<=199 )
    {
      //  vertex shader はここで作る
      CreateDefaultVertexShader( no, Binary );
      return true;
    }
    else if( 200<=no && no<=299 ) 
    {
      //  PixelShader の場合は CreatePixelShader で作ってもらう
      Binary.resize( sizeof(no) );
      memcpy( &(Binary[0]), &no, sizeof(no)  );
      return true;
    }
    else { MAID_ASSERT( true, "このデバイスでは対応していません" ); }
  }

  //  そうでない場合は普通にやる
  LPD3DXBUFFER  pShader=NULL;
  LPD3DXBUFFER  pErrorMsgs=NULL;
  HRESULT ret = S_OK;

  std::vector<std::string>  Profile;
  {
    switch( type )
    {
    case CODETYPE_VERTEXSHADER:
      {
        Profile.push_back("vs_2_0");
        Profile.push_back("vs_2_a");
        Profile.push_back("vs_3_0");
      }break;
    case CODETYPE_PIXELSHADER: 
      {
        Profile.push_back("ps_2_0");
        Profile.push_back("ps_2_a");
        Profile.push_back("ps_2_b");
        Profile.push_back("ps_3_0");
      }break;
    }
  }

  for( int i=0; i<(int)Profile.size(); ++i )
  {
    ret = m_ShaderCompilerLast(
      SrcData.c_str(),
      SrcData.length(),
      NULL,
      NULL,
      "main",
      Profile[i].c_str(),
      0,
      &pShader,
      &pErrorMsgs,
      NULL
      );

    if( FAILED(ret) )
    {
      std::string str = (char*)pErrorMsgs->GetBufferPointer();
      pErrorMsgs->Release();
    }
    if( SUCCEEDED(ret) ) { break; }
  }

  if( FAILED(ret) ) 
  {
    std::string str = (char*)pErrorMsgs->GetBufferPointer();
    ErrorMessage    = String::ConvertSJIStoMAID(str);
    pErrorMsgs->Release();
    goto MAID_ERROR;  
  }

/*  //  逆アセ用コメント
  {
    LPD3DXBUFFER pDis;

    HRESULT a = D3DXDisassembleShader(
	   (DWORD*)pShader->GetBufferPointer(),
	  FALSE,
	  NULL,
	  &pDis
	);
  const char* text = (char*)pDis->GetBufferPointer();

  MAID_WARNING( text );
  pDis->Release();
  }
//*/
  const int len = pShader->GetBufferSize();

  Binary.resize( pShader->GetBufferSize() );
  memcpy( &(Binary[0]), pShader->GetBufferPointer(), pShader->GetBufferSize()  );

MAID_ERROR:
   if( pShader!=NULL ) { pShader->Release(); }
   if( pErrorMsgs!=NULL ) { pErrorMsgs->Release(); }
   return SUCCEEDED(ret);
}



}}


