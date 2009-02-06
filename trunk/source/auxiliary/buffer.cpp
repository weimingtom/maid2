#include"buffer.h"

#include"debug/assert.h"

namespace Maid
{



static char TEMPORARY[] = "DEADBEEF"; //  初期化してないときorSize==0のときにGetPointer() した場合に返すバッファ。安全策

Buffer::Buffer()
:m_IsCreated(false)
{
/*
  m_IsCreated はいらないようにも感じるけど
  サイズが０で初期化されているときもあるので、一応用意しておく
*/

}

void Buffer::Initialize()
{
  Finalize();
  m_IsCreated = true;
}

void Buffer::Finalize()
{
  m_Image.clear();
  m_IsCreated = false;
}


void Buffer::Resize( size_t size )
{
  if( !IsCreated() ) { Initialize(); }
  m_Image.resize( size );
}

void* Buffer::GetPointer( size_t size )
{
  return _GetPointer(size);
}

const void* Buffer::GetPointer( size_t size )const
{
  return const_cast<Buffer*>(this)->_GetPointer(size);
}

size_t Buffer::GetSize() const
{
  return m_Image.size();
}

bool Buffer::IsCreated() const
{
  return m_IsCreated;
}


void* Buffer::_GetPointer( size_t size )
{
  MAID_ASSERT( !IsCreated(), "初期化されていません" );
  if( !IsCreated() ) { return TEMPORARY; }

  if( m_Image.empty() ) { return TEMPORARY; }

  MAID_ASSERT( !(size<m_Image.size()), "指定された長さがありません " << size );
  return &(m_Image[size]);
}

}



