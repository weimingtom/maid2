#ifndef maid2_framework_gui_guislider_h
#define maid2_framework_gui_guislider_h


#include"../../config/define.h"
#include"../../auxiliary/mathematics/line1d.h"
#include"iguiparts.h"


namespace Maid
{
  class GUISlider : public IGUIParts
  {
  public:
    GUISlider();

		void	SetValueRange( int min, int max );
		void	SetValue( int Value );
		int		GetValue() const;

    void SetRotate( float rad );
    float GetRotate() const;

    bool IsButtonDown() const;
  protected:
    virtual MESSAGERETURN MessageExecuting( SPGUIPARAM& pParam );
    virtual bool LocalIsCollision( const POINT2DI& pos ) const;

    virtual bool IsBarCollision( const POINT2DI& pos, float rad ) const= 0;
    virtual bool IsButtonCollision( const POINT2DI& pos, float rad ) const= 0;

    virtual void OnButtonDown( const POINT2DI& pos ){}
    virtual void OnButtonUp( const POINT2DI& pos ){}

    virtual LINE1D_TEMPLATE<int> GetButtonRange() const=0;

    VECTOR2DI CalcButtonOffset() const;
  private:

    int m_ValueMin;
    int m_ValueMax;
    int m_Value;
    float m_Rotat;

    bool m_IsButtonDown;//!<	スライダを動かしている最中か？
  };


}
#endif