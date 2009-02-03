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

