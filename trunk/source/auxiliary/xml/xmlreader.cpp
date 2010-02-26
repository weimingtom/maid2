#include"xmlreader.h"
#include"../debug/assert.h"
#include"../debug/trace.h"


namespace Maid
{
	/*!
	 	@class	XMLReader xmlreader.h
	 	@brief	XMLを順番に読んでいくクラス
	 */


XMLReader::XMLReader()
  :m_pDocment(NULL)
  ,m_IsEOF(false)
{

}

bool XMLReader::IsEmpty() const
{
  return m_pDocment==NULL;
}


void XMLReader::Reset( const XMLDocument& doc )
{
  m_pDocment = &doc;
  while( true )
  {
    if( m_ParentStack.empty() ) { break; }
    m_ParentStack.pop();
  }

  m_IsEOF = false;
}

void XMLReader::Reset()
{
  Reset( *m_pDocment );
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 現在の階層で次のノードに進む
/*!
 	  すでに末端にいた場合は進みません
 */
void XMLReader::NextNode()
{
  if( m_IsEOF ) { return ; }

  if( m_ParentStack.empty() )
  {
    m_IsEOF = true;
    return ;
  }

	NODESTACK& s = m_ParentStack.top();

	if( s.ChildPos < s.ParentNode.GetChildLength() )
	{
		++s.ChildPos;
	}
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 現在の階層で末端にいるか？
/*!
 	  @return いるなら true
 */
bool XMLReader::IsEndNode()
{
  if( m_IsEOF ) { return true; }
  if( m_ParentStack.empty() ) { return false; }

	NODESTACK& s = m_ParentStack.top();

	return s.ChildPos == s.ParentNode.GetChildLength();
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 現在のいる階層が一番上か？
/*!
 	  @return 上なら true
 */
bool XMLReader::IsTopNode()
{
	return m_ParentStack.empty();
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 階層をひとつ入る
/*!
 */
void XMLReader::AscendNode()
{
	NODETYPE t = GetNodeType();

	MAID_ASSERT( t!=NODETYPE_TAG&&t!=NODETYPE_EMPTY, "子持ちではありません" );

	const XMLNode& node = GetCurrentNode();

	m_ParentStack.push( NODESTACK(node) );
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 階層をひとつ出る
/*!
 */
void XMLReader::DescendNode()
{
	m_ParentStack.pop();
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 現在の位置にあるノードのタイプを取得
/*!
    @return 現在のタイプ
 */
XMLReader::NODETYPE XMLReader::GetNodeType()const
{
	const XMLNode& node = GetCurrentNode();

	if( node.GetType()!=XMLNode::TYPE_ELEMENT ) { return NODETYPE_EMPTY; }

	int text_count=0;
	int element_count=0;
	int comment_count=0;
	for( int i=0; i<node.GetChildLength(); ++i )
	{
		switch( node.GetChildNode(i).GetType() )
		{
		case XMLNode::TYPE_TEXT:	 { ++text_count;	}break;
		case XMLNode::TYPE_ELEMENT: { ++element_count; }break;
		case XMLNode::TYPE_COMMENT: { ++comment_count; }break;

		case XMLNode::TYPE_EMPTY:
		default:					{ MAID_ASSERT( true, "ここはありえない" ); }break;
		}
	}

	if( element_count==0 )
	{
		if( text_count==1 ) { return NODETYPE_ELEMENT; }
		return NODETYPE_EMPTY;
	}else
	{
		return NODETYPE_TAG;
	}

	//	そうでないならデータ
	return NODETYPE_ELEMENT;

}


const XMLNode& XMLReader::GetCurrentNode()const
{
  if( m_ParentStack.empty() ) { return m_pDocment->GetRoot(); }

	const NODESTACK& s = m_ParentStack.top();

	return s.ParentNode.GetChildNode(s.ChildPos);
}

String XMLReader::GetNodeName()const
{
	const XMLNode& node = GetCurrentNode();

	return String::ToLower(node.GetName());
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 現在の位置にあるノードの中身をテキストとして取得
/*!
		テキストとして読み込む場合のみ空データを認める

    @return テキスト
 */
String XMLReader::GetString()const
{
	//	テキストとして読み込む場合は空データを認める
	const NODETYPE type = GetNodeType();

	MAID_ASSERT( type!=NODETYPE_ELEMENT && type!=NODETYPE_EMPTY, "データではありません" );

	if( type==NODETYPE_EMPTY ) { return String(); }

	const XMLNode& n = GetTextNode( GetCurrentNode() );

	return n.GetText().GetStr();
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 現在の位置にあるノードの中身を整数値として取得
/*!
    @return 整数
 */
int XMLReader::GetInteger()const
{
	MAID_ASSERT( GetNodeType()!=NODETYPE_ELEMENT, "データではありません" );

	const XMLNode& n = GetTextNode( GetCurrentNode() );

	return n.GetText().GetInt();
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 現在の位置にあるノードの中身を浮動少数値として取得
/*!
    @return 浮動少数値
 */
float XMLReader::GetFloat()const
{
	MAID_ASSERT( GetNodeType()!=NODETYPE_ELEMENT, "データではありません" );

	const XMLNode& n = GetTextNode( GetCurrentNode() );

	return n.GetText().GetFloat();
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 現在の位置にある属性を取得
/*!
    @param Name 欲しい属性

    @return 値
 */
XMLText XMLReader::GetAttribute( const String& Name )const
{
	const XMLNode& n = GetCurrentNode();

	return n.GetAttribute( Name );
}


const XMLNode& XMLReader::GetTextNode( const XMLNode& parent )const
{
	for( int i=0; i<parent.GetChildLength(); ++i )
	{
		XMLNode& n = parent.GetChildNode(i);
		if( n.GetType()==XMLNode::TYPE_TEXT )
		{
			return n;
		}
	}

	MAID_ASSERT( true, "テキストノードが見つかりませんでした" );

	return parent;	//	←これコンパイラ対策なので、これ自体に意味はありません
}

}