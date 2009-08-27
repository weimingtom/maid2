#include"iguislider.h"


namespace Maid
{
/*!
 	  @class	IGUISlider iguislider.h
 	  @brief	スライダ。ボタンをつまんで左右に動かせるアレね
 */ 
IGUISlider::IGUISlider()
  :m_ValueBegin(0)
  ,m_ValueEnd(1)
  ,m_Value(0)
  ,m_Rotat(0)
  ,m_IsButtonDown(false)
  ,m_ButtonLength(0)
  ,m_BarLength(1)
{

}


//! 範囲の初期値を設定する
/*!
    @param  min [i ]  初期値
 */ 
void	IGUISlider::SetValueBegin( int val )
{
  m_ValueBegin = val;

  if( IsBeginEnd() )
  {
    if( m_Value < m_ValueBegin ) { SetValue(m_ValueBegin); }
  }else
  {
    if( m_ValueBegin < m_Value ) { SetValue(m_ValueBegin); }
  }
}

//! 範囲の終了値を設定する
/*!
    @param  max [i ]  終了値
 */ 
void	IGUISlider::SetValueEnd( int val )
{
  m_ValueEnd = val;

  if( IsBeginEnd() )
  {
    if( m_ValueEnd < m_Value ) { SetValue(m_ValueEnd); }
  }else
  {
    if( m_Value < m_ValueEnd ) { SetValue(m_ValueEnd); }
  }
}

//! 設定されている最小値を取得する
/*!
    @return  最小値
 */ 
int IGUISlider::GetValueEnd() const
{
  return m_ValueEnd;
}

//! 設定されている最大値を取得する
/*!
    @return  最大値
 */ 
int IGUISlider::GetValueBegin() const
{
  return m_ValueBegin;
}

//! 新しい値を設定する
/*!
    @param Value  [i ]  新しい値
 */ 
void	IGUISlider::SetValue( int Value )
{
  if( m_Value==Value ) { return ; } //  無駄な発行はしない

/*
  GUIMESSAGE_SLIDER_CHANGEVALUE mess;
  mess.Value = Value;
  SendMessage( mess );
*/
  m_Value = Value;
}

//! 現在の値を取得する
/*!
    @return 現在の値
 */ 
int		IGUISlider::GetValue() const
{
  return m_Value;
}


//! スライダバーの長さを設定する
/*!
    @param len  [i ]  バーの長さ（初期値は１）
 */ 
void IGUISlider::SetBarLength( int len )
{
  MAID_ASSERT( len<1, "長さは１以上必要です" );
  m_BarLength = len;
}

//! スライダバーの長さを取得する
/*!
    @return バーの長さ（初期値は１）
 */ 
int IGUISlider::GetBarLength() const
{
  return m_BarLength;
}

//! ボタンの幅を設定する
/*!
    @param len  [i ]  ボタンの長さ（初期値は０）
 */ 
void IGUISlider::SetButtonLength( int len )
{
  MAID_ASSERT( len<0, "長さは０以上必要です" );
  m_ButtonLength = len;
}

//! ボタンの幅を取得する
/*!
    @return ボタンの幅（初期値は０）
 */ 
int IGUISlider::GetButtonLength() const
{
  return m_ButtonLength;
}

//! バーを回転させる
/*!
    これを設定することで斜めになっているスライダが作れます。

    @param rad  [i ]  傾き（ラジアン度）
 */ 
void IGUISlider::SetRotate( float rad )
{
  if( rad<DEGtoRAD(-90.0f) || DEGtoRAD(90.0f)<rad ) { m_Rotat = fmod(rad,90.0f); }
  else { m_Rotat = rad; }
}

//! バーの傾きを取得する
/*!
    @return 傾き（ラジアン度）
 */ 
float IGUISlider::GetRotate() const
{
  return m_Rotat;
}

//! ボタンが押されているか？（＝ドラッグ中か？）
/*!
    @return 押されているなら true
  \n        まだなら false
 */ 
bool IGUISlider::IsButtonDown() const
{
  return m_IsButtonDown;
}


//! 線分の方向が＋かマイナスか？
/*!
    @return ＋なら true
  \n        ーなら false
 */ 
bool  IGUISlider::IsBeginEnd() const
{
  return m_ValueBegin < m_ValueEnd;
}


bool IGUISlider::LocalIsCollision( const POINT2DI& pos ) const
{
  if( IsBarCollision( pos ) ) { return true; }

  const VECTOR2DI ButtonOffset = CalcButtonOffset();

  return IsButtonCollision( pos-ButtonOffset );
}

//! ボタンの位置を計算する
/*!
    @return ＧＵＩの位置からボタンの位置が離れている量
 */ 
VECTOR2DI IGUISlider::CalcButtonOffset() const
{
  const int range = GetBarLength() - GetButtonLength();

  if( IsBeginEnd() )
  {
    const int p = m_Value-m_ValueBegin;
    const int vec = m_ValueEnd-m_ValueBegin;
    const int scale = range;

    const int pos = (p * scale / vec) + GetButtonLength()/2;

    const float x = float(pos) * Math<float>::cos(m_Rotat);
    const float y = float(pos) * Math<float>::sin(m_Rotat);

    return VECTOR2DI(int(x),int(y));
  }

  const int p = m_ValueBegin - m_Value;
  const int vec = m_ValueBegin-m_ValueEnd;
  const int scale = range;

  const int pos = (p * scale / vec) + GetButtonLength()/2;

  const float x = float(pos) * Math<float>::cos(m_Rotat);
  const float y = float(pos) * Math<float>::sin(m_Rotat);

  return VECTOR2DI(int(x),int(y));
}

//! 指定した座標をスライダ上の値に変換する
/*!
    @param  pos [i ] 調べたい場所(GUI座標からの相対）
 */ 
int IGUISlider::CalcValue( const POINT2DI& pos ) const
{
  //  pos はスライダが配置してあるところからどれだけ離れているかが入っているので
  //  m_Rotat を使って逆算すれば、直線の位置になる。

  int line_pos; //  直線に戻したときの位置

  if( Math<float>::abs(m_Rotat) < DEGtoRAD(45.0f) )
  {
    line_pos = int( float(pos.x) / Math<float>::cos(m_Rotat) );
  }else
  {
    line_pos = int( float(pos.y) / Math<float>::sin(m_Rotat) );
  }
  
  int NewValue;

  {
    const int range = GetBarLength() - GetButtonLength();
    const int p   = line_pos - GetButtonLength()/2;
    const int vec = range;

    if( IsBeginEnd() )
    {
      const int scale   = m_ValueEnd-m_ValueBegin;
      NewValue = p * scale / vec + m_ValueBegin;
    }else
    {
      const int scale   = m_ValueBegin-m_ValueEnd;
      NewValue = m_ValueBegin - (p * scale / vec);

    }

  }

  {
    //  クリップする

    if( IsBeginEnd() )
    {
      if( NewValue   < m_ValueBegin ) { NewValue = m_ValueBegin; }
      if( m_ValueEnd < NewValue     ) { NewValue = m_ValueEnd;   }
    }else
    {
      if( m_ValueBegin < NewValue ) { NewValue = m_ValueBegin; }
      if( NewValue < m_ValueEnd   ) { NewValue = m_ValueEnd;   }
    }
/*
    if( NewValue   < m_ValueBegin ) { NewValue = m_ValueBegin; }
    if( m_ValueEnd < NewValue     ) { NewValue = m_ValueEnd;   }
*/
  }


  return NewValue;
}


IGUISlider::MESSAGERETURN IGUISlider::MessageExecuting( SPGUIPARAM& pParam )
{
  switch( pParam->Message )
  {
  case IGUIParam::MESSAGE_MOUSEDOWN:
    {     
      const GUIMESSAGE_MOUSEDOWN& m = static_cast<const GUIMESSAGE_MOUSEDOWN&>(*pParam);
      const POINT2DI pos  = CalcLocalPosition(m.Position);
      const POINT2DI bpos = pos - CalcButtonOffset();

      if( IsButtonCollision( bpos ) )
      {
        m_IsButtonDown = true;
        OnButtonDown(pos);
      }
      else if( IsBarCollision(pos) )
      {
        //  バーをクリックしたら、そこに移動してほしいよね。
        const int NewValue = CalcValue(pos);
        if( NewValue!=m_Value ) //  無駄な発行はしない
        {
          GUIMESSAGE_SLIDER_CHANGEVALUE mess;
          mess.Value = NewValue;
          SendMessage( mess );
        }
      }

    }break;

  case IGUIParam::MESSAGE_MOUSEUP:
    {
      const GUIMESSAGE_MOUSEUP& m = static_cast<const GUIMESSAGE_MOUSEUP&>(*pParam);

      if( m_IsButtonDown )
      {
        const POINT2DI pos = CalcLocalPosition(m.Position) - CalcButtonOffset();
        m_IsButtonDown = false;
        OnButtonUp(pos);
      }
    }break;

  case IGUIParam::MESSAGE_MOUSEMOVE:
    {
      const GUIMESSAGE_MOUSEMOVE& p = static_cast<const GUIMESSAGE_MOUSEMOVE&>(*pParam);
      const POINT2DI ButtonOffset = CalcLocalPosition(p.Position);

      //  ボタンをつかんだときだけボタンの再配置を行う
      if( !m_IsButtonDown ) { break; }

      const int NewValue = CalcValue( ButtonOffset );

      if( NewValue!=m_Value )//  無駄な発行はしない
      {
        GUIMESSAGE_SLIDER_CHANGEVALUE mess;
        mess.Value = NewValue;
        SendMessage( mess );
      }
    }break;

  case IGUIParam::MESSAGE_SLIDER_CHANGEVALUE:
    {
      const GUIMESSAGE_SLIDER_CHANGEVALUE& m = static_cast<const GUIMESSAGE_SLIDER_CHANGEVALUE&>(*pParam);

      //  変化ないなら破棄だよね。
      if( m_Value==m.Value ) { return MESSAGERETURN_NONE; }

      SetValue( m.Value );
    //  m_Value = m.Value;
    }break;

  case IGUIParam::MESSAGE_SETKEYBORDFOCUS:
    {
      const GUIMESSAGE_SETKEYBORDFOCUS& m = static_cast<const GUIMESSAGE_SETKEYBORDFOCUS&>(*pParam);

      //  フォーカスが外れたら、移動モードをとめる
      if( !m.IsFocus )
      {
        m_IsButtonDown = false;
      }
    }break;
  }

  return IGUIParts::MessageExecuting( pParam );
}


//! マウスカーソルがバーの中に入っているか？
/*!
    継承先では傾きがあることを考慮すること
    傾きは GetRotate() が使えます

    @param  pos [i ]  調べる座標（ＧＵＩ座標）

    @return カーソルが入っているなら true
 */ 
bool IGUISlider::IsBarCollision( const POINT2DI& pos ) const{ return false; }

//! マウスカーソルがボタンの中に入っているか？
/*!
    継承先では傾きがあることを考慮すること
    傾きは GetRotate() が使えます

    @param  pos [i ]  調べる座標（ＧＵＩ座標）

    @return カーソルが入っているなら true
 */ 
bool IGUISlider::IsButtonCollision( const POINT2DI& pos ) const{ return false; }


//! ボタンが押された（==ドラッグが開始された）時に呼ばれる
/*!
    @param  pos [i ]  ボタンを中心とした座標
 */ 
void IGUISlider::OnButtonDown( const POINT2DI& pos ){}


//! ボタンがあがった（==ドラッグが終了した）時に呼ばれる
/*!
    @param  pos [i ]  ボタンを中心とした座標
 */ 
void IGUISlider::OnButtonUp( const POINT2DI& pos ){}

}
