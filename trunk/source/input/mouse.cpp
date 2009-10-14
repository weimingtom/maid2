#include"mouse.h"
#include"../auxiliary/collision.h"
#include"../auxiliary/debug/assert.h"


namespace Maid
{
	/*!
	 	@class	Mouse mouse.h
	 	@brief	マウス入力を管理するクラス
	 */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! コンストラクタ
/*!
 */
Mouse::Mouse() : m_ClipRange(0,0,800,600), m_Mapping(0,0,800,600)
{

}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! デストラクタ
/*!
 */
Mouse::~Mouse()
{
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! マウスの初期化
/*!
 	  @param	pDevice	[i ]	新しく設定するドライバ
 */
void Mouse::Initialize( const Input::SPMOUSEDEVICE& pDevice )
{

	m_pDevice = pDevice;

  m_pDevice->Initialize();
  m_PosList = std::vector<int>( m_pDevice->GetPositionMax(), 0 );
  m_ButtonList = std::vector<BUTTONSTATE>( m_pDevice->GetButtonMax() );

  m_PrevList = m_PosList;
}

void Mouse::Finalize()
{
  if( m_pDevice.get()==NULL ) { return ; }
  
  m_pDevice->Finalize();
  m_pDevice.reset();
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! マウスの状態の初期化
/*!
 */
void Mouse::Flash()
{
  m_PrevList = m_PosList;
  for( int i=0; i<(int)m_ButtonList.size(); ++i )
  {
    m_ButtonList[i] = BUTTONSTATE();
  }
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! マウスの状態を更新する
/*!
 *	フレーム毎に１回呼び出すこと
 */
void Mouse::Update()
{
	std::vector<bool> KeyList(m_ButtonList.size());
	m_PrevList = m_PosList;
	m_pDevice->GetState( m_PosList, KeyList );

	m_RealPos.x = m_PosList[0];
	m_RealPos.y = m_PosList[1];

	//	m_Mapping にマッピングする
	{
		int x = m_PosList[0];
		int y = m_PosList[1];

		x -= m_ClipRange.x;
		y -= m_ClipRange.y;

		x = x * m_Mapping.w / m_ClipRange.w;
		y = y * m_Mapping.h / m_ClipRange.h;

		x += m_Mapping.x;
		y += m_Mapping.y;

		m_PosList[0] = x;
		m_PosList[1] = y;
	}

	for( unt i=0; i<KeyList.size(); ++i )
	{
		const bool IsDown  = KeyList[i];
		BUTTONSTATE& state = m_ButtonList[i];

		if( IsDown )
		{
			state.DownCount++;	//	オーバーフローすることもあるけど・・・・いいやヾ(´▽｀*)ゝ
		}else
		{
			state.IsOut = state.DownCount!=0;
			state.DownCount = 0;
		}
	}
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! Ｘ軸の値を取得する
/*!
 	@return Ｘ値
 */
int Mouse::GetX() const
{
	return GetPos(0);
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! Ｙ軸の値を取得する
/*!
	@return Ｙ値
 */
int Mouse::GetY() const
{
	return GetPos(1);
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ホイールの回転量を取得する
/*!
	@return ホイールの回転量
 */
int Mouse::GetWheel() const
{
	return GetPos(2);
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 各軸の移動量を取得する
/*!
 	@param	Index	[i ]	取得したい軸
  
	@return 移動量
 */
int Mouse::GetPos( int Index ) const
{
	MAID_ASSERT( !(Index<(int)m_PosList.size()), "軸の指定がおかしいです " << Index << " " << m_PosList.size() );
	return m_PosList[Index];
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! Ｘ軸の前フレームとの差を取得する
/*!
	@return Ｘ値
 */
int Mouse::GetDeltaX() const
{
	return GetDelta(0);
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! Ｙ軸の前フレームとの差を取得する
/*!
	@return Ｙ値
 */
int Mouse::GetDeltaY() const
{
	return GetDelta(1);
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ホイールの回転量(前フレームとの差)を取得する
/*!
	@return ホイールの回転量
 */
int Mouse::GetDeltaWheel() const
{
	return GetDelta(2);
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 各軸の前フレームとの差を取得する
/*!
 	@param	Index	[i ]	取得したい軸
  
	@return 移動量
 */
int Mouse::GetDelta( int Index ) const
{
	MAID_ASSERT( !(Index<(int)m_PosList.size()), "軸の指定がおかしいです " << Index << " " << m_PosList.size() );
	return m_PosList[Index]-m_PrevList[Index];
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 左ボタンが押されているか調べる
/*!
	@return 押されているか？
 */
bool Mouse::IsDownL() const
{
	return IsDown(0);
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 右ボタンが押されているか調べる
/*!
	@return 押されているか？
 */
bool Mouse::IsDownR() const
{
	return IsDown(1);
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! マウスホイールボタンが押されているか調べる
/*!
	@return 押されているか？
 */
bool Mouse::IsDownWheel() const
{
	return IsDown(2);
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ボタンが押されているか調べる
/*!
 *	\param	Index	[i ]	取得したいボタン
 *
	@return 押されているか？
 */
bool Mouse::IsDown( int Index ) const
{
	MAID_ASSERT( !(Index<(int)m_ButtonList.size()), "軸の指定がおかしいです " << Index << " " << m_ButtonList.size() );
	return m_ButtonList[Index].DownCount!=0;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 左ボタンが押された瞬間か調べる
/*!
	@return 押されているか？
 */
bool Mouse::IsInL() const
{
	return IsIn(0);
}
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 右ボタンが押された瞬間か調べる
/*!
	@return 押されているか？
 */
bool Mouse::IsInR() const
{
	return IsIn(1);
}
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ホイールボタンが押された瞬間か調べる
/*!
	@return 押されているか？
 */
bool Mouse::IsInWheel() const
{
	return IsIn(2);
}
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 指定したボタンが押された瞬間か調べる
/*!
	@return 押されているか？
 */
bool Mouse::IsIn( int Index ) const
{
	MAID_ASSERT( !(Index<(int)m_ButtonList.size()), "軸の指定がおかしいです " << Index << " " << m_ButtonList.size() );
	return m_ButtonList[Index].DownCount==1;
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 左ボタンが離された瞬間か調べる
/*!
	@return 押されているか？
 */
bool Mouse::IsOutL() const
{
	return IsOut(0);
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 右ボタンが離された瞬間か調べる
/*!
	@return 押されているか？
 */
bool Mouse::IsOutR() const
{
	return IsOut(1);
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ホイールボタンが離された瞬間か調べる
/*!
	@return 押されているか？
 */
bool Mouse::IsOutWheel() const
{
	return IsOut(2);
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 指定したボタンが離された瞬間か調べる
/*!
	@return 押されているか？
 */
bool Mouse::IsOut( int Index ) const
{
	MAID_ASSERT( !(Index<(int)m_ButtonList.size()), "軸の指定がおかしいです " << Index << " " << m_ButtonList.size() );
	return m_ButtonList[Index].IsOut;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 左ボタンがリピートされた瞬間か調べる
/*!
 	@param	Frame	[i ]	リピート間隔
 
	@return 押されているか？
 */
bool Mouse::IsRepeatL( unt Frame ) const
{
	return IsRepeat(0, Frame);
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 右ボタンがリピートされた瞬間か調べる
/*!
 	@param	Frame	[i ]	リピート間隔
 
	@return 押されているか？
 */
bool Mouse::IsRepeatR( unt Frame ) const
{
	return IsRepeat(1, Frame);
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ホイールボタンがリピートされた瞬間か調べる
/*!
 	@param	Frame	[i ]	リピート間隔
 
	@return 押されているか？
 */
bool Mouse::IsRepeatWheel( unt Frame ) const
{
	return IsRepeat(2, Frame);
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 指定したボタンがリピートされた瞬間か調べる
/*!
 	@param	Index	[i ]	取得したいボタン
 	@param	Frame	[i ]	リピート間隔
 
	@return 押されているか？
 */
bool Mouse::IsRepeat( int Index, unt Frame  ) const
{
	return (m_ButtonList[Index].DownCount%Frame)==1;
}





/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! マウスの動ける範囲の設定
/*!
 	@param	Range	[i ]	マウスの範囲
 */
void Mouse::SetClipRange( const RECT2DI& Range )
{
	m_ClipRange = Range;
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! SetClipRangeで設定した座標をどんな座標で変換するか？
/*!
 	たとえば SetClipRange( 0,0,1024,768 ) で、SetMapping(0,0,800,600)　にした場合
 	GetX(), GetY() で得られる値は 0,0,800,600 の範囲になる
 
 	@param	map	[i ]	座標の出力範囲
 */
void Mouse::SetMapping( const RECT2DI& map )
{
	m_Mapping = map;
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! マウスが指定している矩形内に入っているか？
/*!
	@return はいっているなら true
\n			はいっていないなら false
 */
bool Mouse::IsClipIn() const
{
	return Collision<int>::IsPointToRect( m_RealPos.x, m_RealPos.y,  m_ClipRange );
}

const RECT2DI& Mouse::GetClipRange() const
{
  return m_ClipRange;
}

const RECT2DI& Mouse::GetMapping() const
{
  return m_Mapping;
}


}

