
/*!
 	@file
 	@brief	マウス入力を管理するクラス
 */

#ifndef maid2_input_mouser_h
#define maid2_input_mouser_h

#include"../config/define.h"
#include"../config/typedef.h"
#include"../auxiliary/mathematics/point.h"
#include"../auxiliary/mathematics/rect.h"
#include"core/imousedevice.h"

#include<vector>

namespace Maid
{
  class Mouse
  {
  public:
    Mouse();
    virtual ~Mouse();

    void Initialize( const Input::SPMOUSEDEVICE& pDevice );
    void Finalize();

    void Update();

    int	GetX() const;
    int	GetY() const;
    int	GetWheel() const;
    int	GetPos( int Index ) const;
    int	GetDeltaX() const;
    int	GetDeltaY() const;
    int	GetDeltaWheel() const;
    int	GetDelta( int Index ) const;

    bool	IsDownL() const;
    bool	IsDownR() const;
    bool	IsDownWheel() const;
    bool	IsDown( int Index ) const;
    bool	IsInL() const;
    bool	IsInR() const;
    bool	IsInWheel() const;
    bool	IsIn( int Index ) const;

    bool	IsOutL() const;
    bool	IsOutR() const;
    bool	IsOutWheel() const;
    bool	IsOut( int Index ) const;

    bool	IsRepeatL( unt Frame ) const;
    bool	IsRepeatR( unt Frame ) const;
    bool	IsRepeatWheel( unt Frame ) const;
    bool	IsRepeat( int Index, unt Frame ) const;

    void SetClipRange( const RECT2DI& Range );
    void SetMapping( const RECT2DI& map );
    bool IsClipIn() const;

    const RECT2DI& GetClipRange() const;
    const RECT2DI& GetMapping() const;

  protected:
    const Input::SPMOUSEDEVICE& GetDevice() { return m_pDevice; }

  private:

    struct BUTTONSTATE
    {
	    BUTTONSTATE() :DownCount(0),IsOut(false){}
	    unt		DownCount;
	    bool	IsOut;
    };

    std::vector<int>      m_PrevList;
    std::vector<int>      m_PosList;
    std::vector<BUTTONSTATE>  m_ButtonList;
    Input::SPMOUSEDEVICE      m_pDevice;

    POINT2DI  m_RealPos;
    RECT2DI   m_ClipRange;
    RECT2DI   m_Mapping;
  };
}

#endif