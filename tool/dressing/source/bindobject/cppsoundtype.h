#ifndef tool_dressing_bindobject_cppsoundtype_h
#define tool_dressing_bindobject_cppsoundtype_h



class MyApp;
class CppSoundType
  : public Maid::GlobalPointer<MyApp>
{
public:
  static void Register();

  virtual ~CppSoundType();

  virtual void Play( bool state )=0;
  virtual bool IsPlay() const=0;
  virtual void  SetVolume( float vol )= 0;
  virtual float GetVolume()const= 0;
  virtual void  SetPosition( float pos )= 0;
  virtual float GetPosition()const= 0;

protected:
  MyApp& GetApp();

};




#endif
