#include"xmlwriter.h"
#include"../debug/assert.h"

#include<stdio.h>

/*
	{
		XMLWriter write;

		write.AscendNode(L"top");
			write.SetElementName( L"child1" );
			write.SetValueText( L"value" );

			write.NextNode();

			write.AscendNode(L"tag");
				write.SetElementName( L"child3" );
				write.SetValueInt( 100 );

			write.DescendNode();

			write.SetElementName( L"child2" );
			write.SetValueFloat( 0.5f );

		write.DescendNode();

    std::string str;
    write.Save( str );
	}

	↑のコードで↓みたいになる

<?xml version="1.0" encoding="shift_jis" ?> 
<top>
	<child1>value</child1> 
	<tag>
		<child3>100</child3> 
	</tag>
	<child2>0.500000</child2> 
</top>
*/


namespace Maid
{
	XMLWriter::XMLWriter()
	{
		m_pCurrentNode.reset( new XMLNode );
	}

	/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
	//! ひとつ入る
	/*!
	*/
	void XMLWriter::AscendNode( const String& ElementName )
	{
		SetElementName(ElementName);

		SPXMLNODE pNew( new XMLNode );
		m_pCurrentNode->AddChildNode( pNew );

		m_ParentStack.push( m_pCurrentNode );

		m_pCurrentNode = pNew;
	}

	/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
	//! ひとつ出る
	/*!
	*/
	void XMLWriter::DescendNode()
	{
		MAID_ASSERT( m_ParentStack.empty(), "これ以上昇れません" );

		//	要素名を設定してないときにassertを出したいんだけど
		//	AscendNode()直後のDescendNode()に対応できないので
		//	チェックしない
		//	MAID_ASSERT( m_pCurrentNode->GetElementName().empty(), "要素名を決める必要があります" );

		m_pCurrentNode = m_ParentStack.top();
		m_ParentStack.pop();

		//	トップ階層でなければ次に進む
		if( !m_ParentStack.empty() ) { NextNode(); }
	}

	/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
	//! 同じ階層のノードを作成する
	/*!
	*/
	void XMLWriter::NextNode()
	{
		MAID_ASSERT( m_ParentStack.empty(), "トップ階層のノードは１つだけです" );
		MAID_ASSERT( m_pCurrentNode->GetElementName().empty(), "要素名を決める必要があります" );

		SPXMLNODE pNew( new XMLNode );
		m_ParentStack.top()->AddChildNode( pNew );
		m_pCurrentNode = pNew;
	}

	/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
	//! 現在のノード名の設定
	/*!
      @param  name  設定する名前
	*/
	void XMLWriter::SetElementName( const String& name )
	{
		m_pCurrentNode->SetElementName( name );
	}

	/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
	//! 現在のノードに値を設定する
	/*!
      @param  name  設定するテキスト
	*/
	void XMLWriter::SetValueText( const String& value )
	{
		SPXMLNODE pNew( new XMLNode );
		pNew->SetText( value );

		SetValue( pNew );
	}

	/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
	//! 現在のノードに値を設定する
	/*!
      @param  name  設定する値
	*/
	void XMLWriter::SetValueFloat( float value )
	{
		SPXMLNODE pNew( new XMLNode );
		pNew->SetText( String::FtoA(value) );
		SetValue( pNew );
	}

	/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
	//! 現在のノードに値を設定する
	/*!
      @param  name  設定する値
	*/
  void XMLWriter::SetValueInt( int value )
	{
		SPXMLNODE pNew( new XMLNode );
		pNew->SetText( String::ItoA(value) );
		SetValue( pNew );
	}

	void XMLWriter::SetValue( const SPXMLNODE& pNode )
	{
		if( m_pCurrentNode->GetChildLength()==0 )
		{
			m_pCurrentNode->AddChildNode( pNode );
		}else
		{
			m_pCurrentNode->SetChildNode(0,pNode);
		}
	}


	/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
	//! 現在のノードに属性値を設定する
	/*!
      @param  name   設定する名前
      @param  value  設定する値
	*/
	void XMLWriter::AddAttribute( const String& name, const String& value )
	{
		m_pCurrentNode->SetAttribute( name, value );
	}

	/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
	//! 現在のノードに属性値を設定する
	/*!
      @param  name   設定する名前
      @param  value  設定する値
	*/
	void XMLWriter::AddAttribute( const String& name, const float& value )
	{
		m_pCurrentNode->SetAttribute( name, String::FtoA(value) );
	}

	/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
	//! 現在のノードに属性値を設定する
	/*!
      @param  name   設定する名前
      @param  value  設定する値
	*/
	void XMLWriter::AddAttribute( const String& name, const int& value )
	{
		m_pCurrentNode->SetAttribute( name, String::ItoA(value) );
	}

	void XMLWriter::AddTag ( const String& element, const String& value )
	{
		SetElementName( element );
		SetValueText( value );
    NextNode();
	}

	void XMLWriter::AddTag( const String& element, const float& value )
	{
		SetElementName( element );
		SetValueFloat( value );
    NextNode();
	}

	void XMLWriter::AddTag  ( const String& element, const int&   value )
	{
		SetElementName( element );
		SetValueInt( value );
    NextNode();
	}

	void XMLWriter::Save( std::string& FileImage )
	{
		//	ルートノードまで上る
		while( !m_ParentStack.empty() )
		{
			DescendNode();
		}

		//	お決まりの文字
		String text = MAIDTEXT("<?xml version=\"1.0\" encoding=\"shift_jis\"?>\n");

		ConvertSaveText( *m_pCurrentNode, text );

		FileImage = String::ConvertMAIDtoSJIS(text);
	}


	void XMLWriter::ConvertSaveText( const XMLNode& Node, String& text )
	{
		const XMLNode::TYPE type = Node.GetType();

		switch( type )
		{
		case XMLNode::TYPE_EMPTY:	{ }break;
		case XMLNode::TYPE_TEXT:
			{
				const String name = Node.GetText().GetStr();
				text += EncodeSpecialCharacter(name);
			}break;

		case XMLNode::TYPE_ELEMENT:
			{
				const String ElementName = Node.GetElementName();
				
				{
					text += MAIDTEXT("<") + ElementName;
					XMLNode::ATTRIBUTELIST List = Node.GetAttributeTable();

					for( XMLNode::ATTRIBUTELIST::const_iterator ite = List.begin();
									ite!=List.end(); ++ite )
					{
						const String name = ite->first;
						const String vale = ite->second.GetStr();

						text += MAIDTEXT(" ")+ EncodeSpecialCharacter(name) + MAIDTEXT("=\"") + EncodeSpecialCharacter(vale) + MAIDTEXT("\"");
					}
					text += MAIDTEXT(">");
				}


				for( int i=0; i<Node.GetChildLength(); ++i )
				{
					XMLNode& node = Node.GetChildNode( i );
					ConvertSaveText( node, text );
				}

				text += MAIDTEXT("</") + ElementName + MAIDTEXT(">\n");
			}break;

		case XMLNode::TYPE_COMMENT:{ }break;
		}
	}

  String XMLWriter::EncodeSpecialCharacter( const String& src )const
  {
    String ret;
    for( int i=0; i<(int)src.length(); ++i )
    {
      switch( src[i] )
      {
      case '&': { ret += MAIDTEXT("&amp;"); }break;
      case '<': { ret += MAIDTEXT("&lt;"); }break;
      case '>': { ret += MAIDTEXT("&gt;"); }break;
      case '\"':{ ret += MAIDTEXT("&quot;"); }break;
      case '\'':{ ret += MAIDTEXT("&apos;"); }break;
      default:  { ret += src[i]; }break;
      }
    }
    return ret;
  }



}

