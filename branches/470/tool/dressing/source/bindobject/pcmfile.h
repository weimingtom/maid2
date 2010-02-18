#ifndef tool_dressing_bindobject_pcmfile_h
#define tool_dressing_bindobject_pcmfile_h

#include"cppsoundtype.h"

class PcmFile
  :public CppSoundType
{
public:
  static void Register();

  virtual void Play( bool state );
  virtual bool IsPlay() const;
  virtual void  SetVolume( float vol );
  virtual float GetVolume()const;
  virtual void  SetPosition( float pos );
  virtual float GetPosition()const;

  Sqrat::string GetFileName() const;
  void SetFileName(const Sqrat::string& name);

private:
  void LoadingCheck()const;
private:
  Maid::GameSound m_Sound;

};




#endif
