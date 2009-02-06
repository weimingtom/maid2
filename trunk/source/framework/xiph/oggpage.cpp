#include"oggpage.h"

#include"../../auxiliary/macro.h"

#pragma COMPILERMSG("各種関数が const ogg_page* じゃないせいでコンパイルできないので、強引にキャストしてます。もうね（ｒｙ")	

namespace Maid { namespace Xiph {

OggPage::OggPage()
{
  ZERO( &m_Page, sizeof(m_Page) );
}

bool OggPage::IsBeginOfStream() const
{
  ogg_page* p = (ogg_page*)&m_Page;
  return ogg_page_bos(p)!=0;
}

bool OggPage::IsEndOfStream() const
{
  ogg_page* p = (ogg_page*)&m_Page;
  return ogg_page_eos(p)!=0;
}

int OggPage::GetSerialNo() const
{
  ogg_page* p = (ogg_page*)&m_Page;
  return ogg_page_serialno(p);
}

int OggPage::GetPageNo() const
{
  ogg_page* p = (ogg_page*)&m_Page;
  return ogg_page_pageno(p);
}

ogg_int64_t OggPage::GetGranulePosition() const
{
  ogg_page* p = (ogg_page*)&m_Page;

  return (size_t)ogg_page_granulepos(p);
}


}}