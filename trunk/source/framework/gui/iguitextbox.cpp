#include"iguitextbox.h"
#include"../../input/keybord.h"
#include"../../auxiliary/debug/warning.h"

namespace Maid
{
/*!
 	  @class	IGUITextBox iguitextbox.h
 	  @brief	いわゆるエディットボックス。１行しか対応してません
 */ 
IGUITextBox::IGUITextBox()
  :m_CursorPos(0)
  ,m_TextLengthByte(0)
  ,m_IMCursorPos(0)
  ,m_IsIMOpen(false)
{

}


//! テキストの設定
/*!
    @param  text [i ] 新しいテキスト
 */ 
void IGUITextBox::SetText( const String& text )
{
  m_InputText = text;
}

//! テキストの取得
/*!
    @return  設定されているテキスト
 */ 
const String& IGUITextBox::GetText() const
{
  return m_InputText;

}

//! ボックス内に入るテキスト文字数の設定
/*!
    @param  [i ]  文字数（半角は１、全角は２で数えます）
 */ 
void IGUITextBox::SetTextLengthByte( int Length )
{
  m_TextLengthByte = Length;
  ClipText();
}


//! 編集カーソルの位置
/*!
    @param  [i ]  カーソルの位置（半角も全角も１で数えます）
 */ 
int IGUITextBox::GetTextCursor() const
{
  return m_CursorPos; 
}


//! IMで現在変換中の文字の取得
/*!
    @return  変換中の文字
 */ 
const String& IGUITextBox::GetIMText() const
{
  return m_IMText; 
}

//! 編集カーソルの位置
/*!
    @return カーソルの位置（半角も全角も１で数えます）
 */ 
int IGUITextBox::GetIMCursor() const
{
  return m_IMCursorPos; 
}

//! IM編集状態か？（半角ボタンおしてるか？）
/*!
    @return  編集状態なら true
 */ 
bool IGUITextBox::IsIMOpen() const 
{
  return m_IsIMOpen; 
}

//! 入力カーソルの位置が更新されたときに呼ばれる
/*!
    @param pos [i ] 新しいカーソル位置（バイト単位)
 */ 
void IGUITextBox::OnUpdateCursorPosition( int pos )
{

}



/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 設定されている文字の長さにクリッピングする。
/*!
 */
void	IGUITextBox::ClipText()
{
	if( String::CountByte(m_InputText) <= (int)m_TextLengthByte ) { return ; }

  {
    int byte=0;
    for( int i=0; i<(int)m_InputText.length(); ++i )
    {
      if( String::IsHankaku(m_InputText[i]) )	{ byte += 1; }
      else                                    { byte += 2; }
      if( (int)m_TextLengthByte < byte )
      {
	      m_InputText.resize(i);
	      break;
      }
    }
  }

  m_CursorPos = std::min( m_CursorPos, (int)m_InputText.length() );
}

IGUITextBox::MESSAGERETURN IGUITextBox::MessageExecuting( SPGUIPARAM& pParam )
{
  switch( pParam->Message )
  {
  case IGUIParam::MESSAGE_SETKEYBORDFOCUS:
    {
      m_CursorPos = std::min(m_CursorPos,(int)m_InputText.length());
    }break;

  case IGUIParam::MESSAGE_KEYBORDDOWN:
    {
      if( !IsEnable() ) { break; }
      const GUIMESSAGE_KEYBORDDOWN& m = static_cast<const GUIMESSAGE_KEYBORDDOWN&>(*pParam);
      const int oldpos = m_CursorPos;

      switch( m.Key )
      {
      case Keybord::BUTTON_LEFT:
        {
          --m_CursorPos;
          if( m_CursorPos<0 ) { m_CursorPos=0; }
        }break;

      case Keybord::BUTTON_RIGHT:
        {
          ++m_CursorPos;
          if( (int)m_InputText.length()<=m_CursorPos ) { m_CursorPos=m_InputText.length(); }
        }break;
      case Keybord::BUTTON_DELETE:
        {
          if( m_CursorPos<(int)m_InputText.length() )
          {
		        m_InputText.erase( m_CursorPos, 1 );
          }
        }break;

      }
      if( oldpos!=m_CursorPos ) { OnUpdateCursorPosition( m_CursorPos ); }

    }break;

  case IGUIParam::MESSAGE_TEXTBOX_UPDATESTATE:
    {     
      const GUIMESSAGE_TEXTBOX_UPDATESTATE& m = static_cast<const GUIMESSAGE_TEXTBOX_UPDATESTATE&>(*pParam);
      const int oldpos = m_CursorPos;

      {
        if( m.CharaCode.length()==1 )
        {
          switch( m.CharaCode[0] )
          {
          case '\b':
            {
				      const int DelPos = m_CursorPos-1;

				      if( DelPos<0 ) { return IGUIParts::MessageExecuting( pParam ); }

				      m_InputText.erase( DelPos, 1 );
				      m_CursorPos -= 1;
            }break;
          case '\t':
          case '\n':
          case '\r':
          case 0x1b: { }break;
          default:
	          {
		          m_InputText.insert( m_CursorPos, m.CharaCode );
		          m_CursorPos += 1;
	          }break;
          }

        }else
        {
          m_InputText.insert( m_CursorPos, m.CharaCode );
          m_CursorPos += (int)m.CharaCode.length();
        }
      }
      {
        m_IMText = m.IMCompString;
        m_IMCursorPos = m.IMCompPosition;
        m_IsIMOpen = m.IsIMOpen;
      }

      ClipText();
      if( oldpos!=m_CursorPos ) { OnUpdateCursorPosition( m_CursorPos ); }

    }break;
  }

  return IGUIParts::MessageExecuting( pParam );
}

}

