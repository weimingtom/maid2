#ifndef oggpage_h
#define oggpage_h


#include"../../source/config/define.h"
#include"../../source/auxiliary/macro.h"

#include<ogg/ogg.h>

class OggBuffer;
class OggStream;
class OggPage
{
  friend OggBuffer;
  friend OggStream;
public:

  OggPage();

  bool IsBeginOfStream() const;
  bool IsEndOfStream() const;

  int GetSerialNo() const;
  int GetPageNo() const;

  ogg_int64_t GetGranulePosition() const;


private:
  ogg_page  m_Page;
};


#endif
