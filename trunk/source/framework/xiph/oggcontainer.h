#ifndef oggcontainer_h
#define oggcontainer_h


#include"../../source/config/define.h"
#include"../../storage/filereadersync.h"
#include"../../auxiliary/string.h"

#include<ogg/ogg.h>

#include"oggpage.h"

namespace Maid { namespace Xiph {

  class OggContainer
  {
  public:
    OggContainer();
    ~OggContainer();

    void Initialize( const String& FileName );
    void Finalize();

    const OggPage& GetCurrentPage()const;
    void NextPage();

    bool IsEnd() const;

  private:
    void UpdateBuffer();

  private:
    FileReaderSync  m_hFile;
    ogg_sync_state  m_State;

    OggPage m_CurrentPage;
  };

}}

#endif
