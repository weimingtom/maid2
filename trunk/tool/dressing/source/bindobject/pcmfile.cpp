#include"stdafx.h"
#include"pcmfile.h"

#include"../app.h"

using namespace Maid;
using namespace Sqrat;

void PcmFile::Register()
{
  RootTable().Bind(
    _SC("PcmFile"), 
    DerivedClass<PcmFile,CppSoundType>()
    .Prop(_SC("FileName"), &PcmFile::GetFileName, &PcmFile::SetFileName)
  );
}

void PcmFile::Play( bool state )
{
  if( m_Sound.IsEmpty()   ) { return ; }
  if( m_Sound.IsLoading() ) { return ; }
  LoadingCheck();

  if( state ) { m_Sound.Play(); }
  else { m_Sound.Pause(); }

}

bool PcmFile::IsPlay() const
{
  if( m_Sound.IsEmpty() ) { return false; }
  LoadingCheck();

  return m_Sound.IsPlay();
}

void  PcmFile::SetVolume( float vol )
{
  if( m_Sound.IsEmpty() ) { return ; }
  LoadingCheck();

  m_Sound.SetVolume( vol, 0.0f );
}

float PcmFile::GetVolume()const
{
  if( m_Sound.IsEmpty() ) { return 0.0f; }
  LoadingCheck();

  return (float)m_Sound.GetPosition();
}

void  PcmFile::SetPosition( float pos )
{
  if( m_Sound.IsEmpty() ) { return ; }
  LoadingCheck();
  m_Sound.SetPosition( pos );

}

float PcmFile::GetPosition()const
{
  if( m_Sound.IsEmpty() ) { return 0.0f; }
  LoadingCheck();

  return (float)m_Sound.GetPosition();
}

Sqrat::string PcmFile::GetFileName() const
{
  const String str = m_Sound.GetFileName();
  const Sqrat::string ret = String::ConvertMAIDtoUNICODE(str);

  return ret;
}

void PcmFile::SetFileName(const Sqrat::string& name)
{
  const String str = String::ConvertUNICODEtoMAID(name);
  const String dir = GetApp().GetCurrentDirectry();


  m_Sound.Load( GameSound::TYPE_SE,  dir + MAIDTEXT("\\") + str );
}


void PcmFile::LoadingCheck() const
{
  if( m_Sound.IsEmpty() ) { return ; }

  while( true )
  {
    if( !m_Sound.IsLoading() ) { return ; }
  }
}




