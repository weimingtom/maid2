#ifndef maid2_sound_isoundobject_h
#define maid2_sound_isoundobject_h

#include"../config/define.h"
#include<boost/smart_ptr.hpp>
#include"soundmessagebase.h"

namespace Maid {

  class ISoundObject
	{
	public:

    enum TYPE
    {
      TYPE_NULL,
      TYPE_STATIC,
      TYPE_STREAM,
    };
    ISoundObject( TYPE t ):m_Type(t){}

    virtual ~ISoundObject(){}

    virtual void Update()=0;
    virtual void Play()=0;
    virtual void Stop()=0;
    virtual void SetPosition( float time )=0;
    virtual void SetVolume( float volume )=0;
    virtual void SetJumpPoint( const std::vector<JUMPPOINT>& list )=0;
    virtual void SetLoopState( bool IsLoop )=0;

    TYPE GetType() const { return m_Type; }
  private:
    TYPE  m_Type;
  };

  typedef	boost::shared_ptr<ISoundObject>	SPSOUNDOBJECT;

}


#endif
