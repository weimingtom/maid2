#ifndef maid2_sound_soundobjectnull_h
#define maid2_sound_soundobjectnull_h

#include"../config/define.h"
#include"isoundobject.h"

namespace Maid {

  /*!
      @class  SoundObjectNULL soundobjectnull.h
      @brief  すべての動作をスルーするクラス
              エラったときに使う
  */
  //  なにもしないクラスより、無音再生っぽい挙動するほうがいいかもしれない
  class SoundObjectNULL : public ISoundObject
  {
  public:
    SoundObjectNULL():ISoundObject(TYPE_NULL){}
    void Update(){}
    void Play(){}
    void Stop(){}
    void SetPosition( double time ){}
    void SetVolume( double volume ){}
    void SetLoopState( bool IsLoop ){}
    bool IsPlay()const {return false;}
    double GetPosition() const {return 0;}
    double GetVolume() const{return 0.0f;}
  };

}


#endif
