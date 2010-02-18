#ifndef maid2_framework_core_xiph_oggpage_h
#define maid2_framework_core_xiph_oggpage_h


#include"../../../config/define.h"
#include"../istoragesample.h"

#include<ogg/ogg.h>
#include<vector>

namespace Maid { namespace Movie { namespace Xiph {

  class OggPage
    : public IStorageSample
  {
  public:
    OggPage( ogg_page& src );
    virtual unt GetSize();

    bool IsBeginOfStream() const;
    bool IsEndOfStream() const;
    bool IsContinued() const;

    int GetSerialNo() const;
    int GetPageNo() const;
    int GetPacketCount() const;

    ogg_int64_t GetGranulePosition() const;

    std::string GetDebugString() const;

    const ogg_page& Get() const;
  private:
    ogg_page  m_Page;
    std::vector<unsigned char> m_Header;
    std::vector<unsigned char> m_Body;
  };

  typedef boost::shared_ptr<OggPage> SPOGGPAGE;

}}}

#endif
