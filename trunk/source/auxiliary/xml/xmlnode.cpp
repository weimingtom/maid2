#include"xmlnode.h"
#include"../debug/assert.h"



namespace Maid
{
	/*!
	 	@class	XMLText XMLText.h
	 	@brief	XMLの値を表すテキスト
	\n			テキスト→数値変換のラッパです
	 */

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! コンストラクタ
/*!
 */
XMLText::XMLText()
{
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! コンストラクタ
/*!
 	@param	Text	[i ]	テキスト。これが<a>XXX</a> の XXXの部分です
 */
XMLText::XMLText( const String& Text ) : m_Text(Text)
{
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 持っているテキストをint型として取得する
/*!
 	@return int整数。テキストがおかしかったら謎
 */
int XMLText::GetInt() const
{
	return String::AtoI(m_Text);
}


float XMLText::GetFloat() const
{
	return String::AtoF(m_Text);
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 持っているテキストを取得する
/*!
 	@return テキスト
 */
const String& XMLText::GetStr() const
{
	return m_Text;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! テキストを持っていないか？
/*!
 	@return ないなら true
 */
bool XMLText::IsEmpty() const
{
	return m_Text.empty();
}








	/*!
	 	@class	XMLNode XMLText.h
	 	@brief	XMLのノード
	\n			  XMLDocument にぶら下がっているノードです。
	 */

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! コンストラクタ
/*!
 */
XMLNode::XMLNode()
{
	m_Type   = TYPE_EMPTY;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ノードの名前の取得
/*!
 	@return ノード名
 */
String XMLNode::GetName() const
{
	return m_Name;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ノードの種類の取得
/*!
 	@return ノードタイプ
 */
XMLNode::TYPE XMLNode::GetType() const
{
	return m_Type;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 何個の属性を持っているか？
/*!
 	@return 属性の個数
 */
int XMLNode::GetAttributeLength()const
{
	return (int)m_AttributeList.size();
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 指定した属性の取得
/*!
 	@return 属性。指定したものがないなら空
 */
XMLText XMLNode::GetAttribute( const String& Name )const
{
	ATTRIBUTELIST::const_iterator ite = m_AttributeList.find(Name);

	if( ite==m_AttributeList.end() ) { return XMLText(); }
	return (ite->second);
}

const XMLNode::ATTRIBUTELIST& XMLNode::GetAttributeTable() const
{
	return m_AttributeList;
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! テキストの取得
/*!
 	@return テキスト
 */
XMLText	XMLNode::GetText() const
{
	MAID_ASSERT( m_Type!=TYPE_TEXT, "このノードはテキストタイプではありません" );
	return m_Text;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! タグ名の取得
/*!
 	@return タグ名
 */
String XMLNode::GetElementName() const
{
	MAID_ASSERT( m_Type!=TYPE_ELEMENT, "このノードはタグタイプではありません" );
	return m_Name;
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! コメントの取得
/*!
 	@return コメント
 */
String XMLNode::GetComment() const
{
	MAID_ASSERT( m_Type!=TYPE_COMMENT, "このノードはコメントタイプではありません" );
	return m_Text.GetStr();
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! このノードは空っぽか？
/*!
 	@return 空ならtrue
 */
bool XMLNode::IsEmpty() const
{
	return m_Type == TYPE_EMPTY;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 子ノードが何個あるか？
/*!
 *	\return 子ノードの個数
 */
int  XMLNode::GetChildLength() const
{
	MAID_ASSERT( m_Type!=TYPE_ELEMENT, "このノードは要素タイプではありません" );
	return (int)m_ChildList.size();
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 子ノードの取得
/*!
 	@return 子ノード。
 */
XMLNode&	XMLNode::GetChildNode( int index ) const
{
	MAID_ASSERT( m_Type!=TYPE_ELEMENT, "このノードは要素タイプではありません" );
	MAID_ASSERT( !(index<(int)m_ChildList.size()), "範囲外です " << index );
	return *(m_ChildList[index]);
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 要素として扱うようにする
/*!
 	  @param  Name  新しく設定する要素名 (<XXX>YYY</XXX> のXXX部分)
 */
void XMLNode::SetElementName( const String& Name )
{
	m_Name = Name;
	m_Type = TYPE_ELEMENT;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 要素の設定
/*!
 	  @param  Text  新しく設定する要素 (<XXX>YYY</XXX> のYYY部分)
 */
void XMLNode::SetText( const String& Text )
{
	m_ChildList.clear();
	m_Text = XMLText(Text);
	m_Type = TYPE_TEXT;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! コメントとして扱うようにする
/*!
 	  @param  Text  新しく設定する要素 ( <!-- YYY --> のYYY部分)
 */
void XMLNode::SetComment( const String& Text )
{
	m_ChildList.clear();
	m_Text = XMLText(Text);
	m_Type = TYPE_COMMENT;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 属性の作成
/*!
    すでに存在している場合は削除されます

 	  @param  Name    属性名
 	  @param  Value   属性値
 */
void XMLNode::SetAttribute( const String& Name, const XMLText& Value )
{
	m_AttributeList[Name] = Value;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 属性の削除
/*!
 	  @param  Name    削除する属性名
 */
void XMLNode::DeleteAttribute( const String& Name )
{
	m_AttributeList.erase( m_AttributeList.find(Name) );
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 子ノードを最後に追加する
/*!
 	  @param  pNode    追加するノード
 */
void XMLNode::AddChildNode( const SPXMLNODE& pNode )
{
	m_ChildList.push_back( pNode );
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 子ノードを指定した位置に追加する
/*!
 	  @param  Index    追加する位置
 	  @param  pNode    追加するノード
 */
void XMLNode::InsertChildNode( int Index, const SPXMLNODE& pNode )
{
	m_ChildList.insert( m_ChildList.begin()+Index, pNode );
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 指定した子ノードを削除する
/*!
 	  @param  Index    削除する位置
 */
void XMLNode::DeleteChildNode( int Index )
{
	m_ChildList.erase( m_ChildList.begin()+Index );
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 指定した子ノードを書き換える
/*!
 	  @param  Index   書き換える位置
 	  @param  pNode   書き換えるノード
 */
void XMLNode::SetChildNode( int Index, const SPXMLNODE& pNode )
{
	MAID_ASSERT( (int)m_ChildList.size()>=Index, "範囲外です" );
	m_ChildList[Index] = pNode;
}

}

