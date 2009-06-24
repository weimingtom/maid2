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
  ogg_page* p = const_cast<ogg_page*>(&m_Page);
  return ogg_page_bos(p)!=0;
}

bool OggPage::IsEndOfStream() const
{
  ogg_page* p = const_cast<ogg_page*>(&m_Page);
  return ogg_page_eos(p)!=0;
}

bool OggPage::IsContinued() const
{
  ogg_page* p = const_cast<ogg_page*>(&m_Page);
  return ogg_page_continued(p)!=0;
}


int OggPage::GetSerialNo() const
{
  ogg_page* p = const_cast<ogg_page*>(&m_Page);
  return ogg_page_serialno(p);
}

int OggPage::GetPageNo() const
{
  ogg_page* p = const_cast<ogg_page*>(&m_Page);
  return ogg_page_pageno(p);
}

int OggPage::GetPacketCount() const
{
  ogg_page* p = const_cast<ogg_page*>(&m_Page);
  return ogg_page_packets(p);
}


ogg_int64_t OggPage::GetGranulePosition() const
{
  ogg_page* p = const_cast<ogg_page*>(&m_Page);
  return ogg_page_granulepos(p);
}


void OggPage::Copy( std::vector<unsigned char>& header, std::vector<unsigned char>& body, const OggPage& SrcPage )
{
  const ogg_page& src = SrcPage.m_Page;

  header.resize( src.header_len );
  ::memcpy( &(header[0]), src.header, src.header_len );

  body.resize( src.body_len );
  ::memcpy( &(body[0]), src.body, src.body_len );
  
  m_Page.header = &(header[0]);
  m_Page.header_len = src.header_len;
  m_Page.body = &(body[0]);
  m_Page.body_len = src.body_len;
}



  std::string OggPage::GetDebugString() const
  {
    std::string Binary_h;
    {
      const int size = std::min(8,(int)m_Page.header_len);
      for( int i=0; i<size; ++i )
      {
        char buf[256];
        sprintf( buf, "%02x.", m_Page.header[i] );
        Binary_h += buf;
      }
    }

    std::string Binary_b;
    {
      const int size = std::min(8,(int)m_Page.body_len);
      for( int i=0; i<size; ++i )
      {
        char buf[256];
        sprintf( buf, "%02x.", m_Page.body[i] );
        Binary_b += buf;
      }
    }

    std::string Text;
    {
      ogg_page* p = const_cast<ogg_page*>(&m_Page);
      const int serial = GetSerialNo();
      const int page = GetPageNo();
      const int granu = GetGranulePosition();
      const int packet = GetPacketCount();
      const int bos = ogg_page_bos(p);
      const int eos = ogg_page_eos(p);
      const int continued = ogg_page_continued(p);
      const int header_len = m_Page.header_len;
      const int body_len = m_Page.body_len;

      char buf[1024];
      sprintf( buf, "serial:%06d no:%06d granule:%06d packet:%03d continued:%0d bos:%03d eos:%03d header_len:%06d body_len:%06d",
        serial, page, granu, packet, continued, bos, eos, header_len, body_len );
      Text += buf;
    }

    return Text + " header:" + Binary_h+ " body:" + Binary_b;
 }

}}