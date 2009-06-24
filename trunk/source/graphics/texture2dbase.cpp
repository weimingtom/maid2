#include"texture2dbase.h"

namespace Maid
{

Texture2DBase::Texture2DBase()
  :m_VirtualSize(0,0)
  ,m_RealSize(0,0)
  ,m_TextureSize(0,0)
  ,m_VirtualFormat(PIXELFORMAT_NONE)
  ,m_RealFormat(PIXELFORMAT_NONE)
{

}


RECT2DF Texture2DBase::CalcUV( const RECT2DI& rc ) const
{
  const SIZE2DI& image = GetSize();
  const SIZE2DI& real  = GetRealSize();
  const SIZE2DI& texsize=GetTextureSize();

  const int x = rc.x * real.w / image.w;
  const int y = rc.y * real.h / image.h;
  const int w = rc.w * real.w / image.w;
  const int h = rc.h * real.h / image.h;

  const float ux = float(x) / float(texsize.w);
  const float vy = float(y) / float(texsize.h);
  const float uw = float(w) / float(texsize.w);
  const float vh = float(h) / float(texsize.h);

  return RECT2DF( POINT2DF(ux,vy), SIZE2DF(uw,vh) );
}


void Texture2DBase::Clear()
{
  IMaterial::Clear();
  ITexture2D::Clear();

  m_VirtualSize = SIZE2DI(0,0);
  m_RealSize    = SIZE2DI(0,0);
  m_TextureSize = SIZE2DI(0,0);
  m_VirtualFormat = PIXELFORMAT_NONE;
  m_RealFormat = PIXELFORMAT_NONE;
}


bool Texture2DBase::IsSetupped() const
{
  return ITexture2D::Get().get()!=NULL && IMaterial::Get().get()!=NULL;
}

void Texture2DBase::Setup( const Graphics::SPTEXTURE2D& pTex, const Graphics::SPMATERIAL& pMat, const SIZE2DI& Virtual, const SIZE2DI& real, PIXELFORMAT fmt )
{
  ITexture2D::Set(pTex);
  IMaterial::Set(pMat);

  m_VirtualSize = Virtual;
  m_RealSize    = real;
  m_TextureSize = pTex->GetParam().Size;

  m_VirtualFormat = fmt;
  m_RealFormat = pTex->GetParam().Format;
}


}

