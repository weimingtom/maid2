/*!
	@file
	@brief oggコンテナファイルを読み込むクラス
 */

#ifndef maid2_framework_movie_core_oggfile_h
#define maid2_framework_movie_core_oggfile_h


#include"../../../../config/define.h"
#include"../../../../storage/filereadersync.h"
#include"../istoragereader.h"

#include<ogg/ogg.h>

namespace Maid { namespace Movie { namespace Xiph {

  class OggFile
    : public IStorageReader
  {
  public:
    OggFile( const String& filename );
    virtual void Initialize();
    virtual void Finalize();

    virtual void Read( SPSTORAGESAMPLE& pSample );
    virtual bool IsEnd() const;
    virtual bool IsCacheEmpty() const;
    virtual bool IsCacheFull() const;

  private:
    String          m_FileName;
    FileReaderSync  m_hFile;
    ogg_sync_state  m_State;

    size_t  m_ReadSize;
    bool    m_IsEnd;
  };

}}}


#endif