#include"rendertargetbase.h"
#include"../auxiliary/debug/warning.h"
#include"../auxiliary/debug/assert.h"
#include"../auxiliary/mathematics/point.h"

namespace Maid
{

  /*!
      @class  RenderTargetBase rendertargetbase.h
      @brief  描画対象となる平面をあらわす基本クラス
    \n        実際の大きさと異なるように振舞ってほしい場合、このクラスを継承して使うとベンリ
  */
RenderTargetBase::RenderTargetBase()
  :m_Size(0,0)
  ,m_RealSize(0,0)
  ,m_TextureSize(0,0)
  ,m_Area(DRAWAREA_LEFTTOP)
{

}


//! 現在の設定で描画範囲を計算する
/*!
 */
RECT2DI RenderTargetBase::CalcDrawArea() const
{
  const SIZE2DI& realsize    = m_RealSize;
  const SIZE2DI& texturesize = m_TextureSize;

  const int w = texturesize.w - realsize.w;
  const int h = texturesize.h - realsize.h;

  RECT2DI ret;

  ret.w = realsize.w;
  ret.h = realsize.h;

  switch( m_Area )
  {
  case DRAWAREA_CENTER:     { ret.x = w/2;  ret.y = h/2; }break;
  case DRAWAREA_LEFTTOP:    { ret.x =   0;  ret.y =   0; }break;
  case DRAWAREA_RIGHTTOP:   { ret.x =   w;  ret.y =   0; }break;
  case DRAWAREA_LEFTBOTTOM: { ret.x =   0;  ret.y =   h; }break;
  case DRAWAREA_RIGHTBOTTOM:{ ret.x =   w;  ret.y =   h; }break;
  }

  return ret;
}


//! 作っていたものを開放する
/*!
 */
void RenderTargetBase::Clear()
{
  IRenderTarget::Clear();

  m_Size = SIZE2DI(0,0);
  m_TextureSize = SIZE2DI(0,0);
}


//! すでに作成されているか？
/*!
    @return 作成されているなら true
 */
bool RenderTargetBase::IsSetupped() const
{
  return IRenderTarget::Get().get()!=NULL;
}

//! 各種設定しておく
/*!
    @param  pTarget     [i ]  要求があったときに差し出すターゲット
    @param  virtualsize [i ]  仮想的な大きさ
    @param  createsize  [i ]  実際の大きさ
    @param  area        [i ]  描画範囲の位置
 */
void RenderTargetBase::Setup( const Graphics::SPRENDERTARGET& pTarget, const SIZE2DI& virtualsize, const SIZE2DI& realsize, DRAWAREA area )
{
  MAID_ASSERT( pTarget->GetResource()->GetType()!=Graphics::IResource::TYPE_TEXTURE2D,
      "設定するリソースがテクスチャではありません" );
  IRenderTarget::Set(pTarget);

  const Graphics::CREATERETEXTURE2DPARAM param = static_cast<Graphics::ITexture2D*>(pTarget->GetResource().get())->GetParam();

  m_Size = virtualsize;
  m_RealSize  = realsize;
  m_TextureSize = param.Size;
  m_Format = param.Format;
  m_Area   = area;
}

//! 各種設定しておく
/*!
    @param  pTarget [i ]  要求があったときに差し出すターゲット
 */
void RenderTargetBase::Setup( const Graphics::SPRENDERTARGET& pTarget )
{
  MAID_ASSERT( pTarget->GetResource()->GetType()!=Graphics::IResource::TYPE_TEXTURE2D,
      "設定するリソースがテクスチャではありません" );
  const SIZE2DI size = static_cast<Graphics::ITexture2D*>(pTarget->GetResource().get())->GetParam().Size;

  Setup( pTarget, size, size, DRAWAREA_LEFTTOP );
}

}

