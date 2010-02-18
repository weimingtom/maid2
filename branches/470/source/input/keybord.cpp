#include"keybord.h"
#include"../auxiliary/debug/assert.h"
#include"../auxiliary/string.h"


namespace Maid
{
	/*!
	 	@class	CKeybord CKeybord.h
	 	@brief	キーボード入力を管理するクラス
	 */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! コンストラクタ
/*!
 */
Keybord::Keybord()
{

}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! デストラクタ
/*!
 */
Keybord::~Keybord()
{
  Finalize();
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! キーボードの初期化
/*!
 	@param	pKey	[i ]	新しく設定するドライバ
 */
void Keybord::Initialize( const Input::SPKEYBORDDEVICE& pKey )
{
	m_pKeybordDevice  = pKey;

  m_pKeybordDevice->Initialize();
	m_ButtonList.resize( m_pKeybordDevice->GetButtonnMax() );
}

void Keybord::Finalize()
{
  if( m_pKeybordDevice.get()==NULL ) { return ; }
  m_pKeybordDevice->Finalize();
  m_pKeybordDevice.reset();
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! キーボードの状態を更新する
/*!
 	フレーム毎に１回呼び出すこと
 */
void Keybord::Update()
{
	std::vector<bool> KeyList(m_ButtonList.size());
	m_pKeybordDevice->IsDown(KeyList);

	for( int i=0; i<(int)KeyList.size(); ++i )
	{
		//	半角/全角　ボタンは無効にしておく
		//	OS側でＩＭＥのフックとかしててメンドイ
		if( i==0xE5 ) { continue; }

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
//! キーボードの状態の初期化
/*!
 */
void Keybord::Flash()
{
  for( int i=0; i<(int)m_ButtonList.size(); ++i )
  {
    m_ButtonList[i] = BUTTONSTATE();
  }
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ボタンが押されているか調べる
/*!
 	@param	Index	[i ]	取得したいボタン
 
 	@return 押されているか？
 */
bool Keybord::IsDown( unt08 Index ) const
{
	MAID_ASSERT( !(Index<m_ButtonList.size()), "ボタンの指定がおかしいです " << Index << " "<< m_ButtonList.size() );
	return m_ButtonList[Index].DownCount!=0;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 指定したボタンが押された瞬間か調べる
/*!
 	@param	Index	[i ]	取得したいボタン
 
 	@return 押されているか？
 */
bool Keybord::IsIn( unt08 Index ) const
{
	MAID_ASSERT( !(Index<m_ButtonList.size()), "ボタンの指定がおかしいです " << Index << " "<< m_ButtonList.size() );
	return m_ButtonList[Index].DownCount==1;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 指定したボタンが離された瞬間か調べる
/*!
 	@param	Index	[i ]	取得したいボタン
 
 	@return 押されているか？
 */
bool Keybord::IsOut( unt08 Index ) const
{
	MAID_ASSERT( !(Index<m_ButtonList.size()), "ボタンの指定がおかしいです " << Index << " "<< m_ButtonList.size() );
	return m_ButtonList[Index].IsOut;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 指定したボタンが押されて一定時間経過したか調べる
/*!
 	@param	Index	[i ]	取得したいボタン
 	@param	Frame	[i ]	リピート間隔
 
 	@return 押されているか？
 */
bool Keybord::IsRepeat( unt08 Index, unt Frame  ) const
{
	MAID_ASSERT( !(Index<m_ButtonList.size()), "ボタンの指定がおかしいです " << Index << " "<< m_ButtonList.size() );
	return (m_ButtonList[Index].DownCount%Frame)==1;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 指定したボタンが押されて一定時間経過したか調べる
/*!
 	@param	Index		[i ]	取得したいボタン
 	@param	Frame		[i ]	リピート間隔
 	@param	BeginFrame	[i ]	リピートチェックを開始する時間
 
 	@return 押されているか？
 */
bool Keybord::IsRepeat( unt08 Index, unt Frame, unt BeginFrame  ) const
{
	MAID_ASSERT( !(Index<m_ButtonList.size()), "ボタンの指定がおかしいです " << Index << " "<< m_ButtonList.size() );

	const BUTTONSTATE& state = m_ButtonList[Index];

	if( state.DownCount<BeginFrame )
	{
		return state.DownCount==1;
	}

	return ((state.DownCount-BeginFrame)%Frame)==0;
}


}

