#include"texture2dbase.h"

namespace Maid
{

Texture2DBase::Texture2DBase()
  :m_Size(0,0),m_CreateSize(0,0),m_TextureSize(0,0)
{

}

void Texture2DBase::Clear()
{
  IMaterial::Clear();
  ITexture2D::Clear();

  m_Size = SIZE2DI(0,0);
  m_CreateSize  = SIZE2DI(0,0);
  m_TextureSize = SIZE2DI(0,0);
}


bool Texture2DBase::IsSetupped() const
{
  return ITexture2D::Get().get()!=NULL && IMaterial::Get().get()!=NULL;
}


void Texture2DBase::Setup( const Graphics::SPTEXTURE2D& pTex, const Graphics::SPMATERIAL& pMat, const SIZE2DI& def, const SIZE2DI& create )
{
  ITexture2D::Set(pTex);
  IMaterial::Set(pMat);

  m_Size = def;
  m_CreateSize  = create;
  m_TextureSize = pTex->GetParam().Size;
}


}

