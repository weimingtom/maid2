#include"xmldocument.h"
#include"../debug/assert.h"

#include<string>
#include<stack>

namespace Maid
{
  String DecodeSpecialCharacter( const String& src )
  {
    String ret;

    for( int i=0; i<(int)src.length(); ++i )
    {
      if( src[i]=='&' )
      {
        String tmp;
        while( true )
        {
          tmp += src[i];
          if( src[i]==';' ) { break; }
          ++i;
        }
             if( tmp==MAIDTEXT("&amp;")  ) { ret += MAIDTEXT("&");  }
        else if( tmp==MAIDTEXT("&lt;")   ) { ret += MAIDTEXT("<");  }
        else if( tmp==MAIDTEXT("&gt;")   ) { ret += MAIDTEXT(">");  }
        else if( tmp==MAIDTEXT("&quot;") ) { ret += MAIDTEXT("\""); }
        else if( tmp==MAIDTEXT("&apos;") ) { ret += MAIDTEXT("\'"); }
      }else
      {
        ret += src[i];
      }
    }

    return ret;
  }

	String ReadToken( const String& Text, int& pos )
	{
		String ret;

		if( Text[pos]=='<' )
		{	//	タグ読み込み
      int stack = 0;  // '<' '>' のペア回数
			while( true )
			{
				if( (int)Text.length()<=pos ) { break; }
				
				const String::value_type c = Text[pos];
				if( c=='\t' ) { ++pos; continue; }
				ret += c;
				++pos;
				     
        if( c=='<' ){ ++stack; }
        else
        if( c=='>' ){ --stack; if( stack==0 ) { break; } }
			}

		}else
		{	//	テキスト読み込み
			while( true )
			{
				if( (int)Text.length()<=pos ) { break; }
				
				const String::value_type c = Text[pos];
			//	if( c==' '  ) { ++pos; continue; }
			//	if( c=='\r' ) { ++pos; continue; }
			//	if( c=='\n' ) { ++pos; continue; }
			//	if( c=='\t' ) { ++pos; continue; }
				if( c=='<' ) { break; }
				ret += c;
				++pos;
			}
		}
		return ret;
	}


	String ReadAttributeName( const String& Text, int& pos )
	{
		String ret;

		while( true )
		{
			if( (int)Text.length()<=pos ) { break; }

			const String::value_type c = Text[pos];

			if( c=='/' )
			{
				if( Text.length() - pos==2 ) { break; }
			}

			if( c=='>' || c==' ' || c=='=' ) { break; }
			ret += c;
			++pos;
		}

		return ret;
	}

	String ReadAttributeValue( const String& Text, int& pos )
	{
		while( true )
		{
			if( (int)Text.length()<=pos ) { break; }

			const String::value_type c = Text[pos];
			++pos;
			if( c=='\"' ) {  break; }
		}

		String ret;
		while( true )
		{
			if( (int)Text.length()<=pos ) { break; }

			const String::value_type c = Text[pos];
			++pos;
			if( c=='\"' ) { break; }
			ret += c;
		}

		return ret;
	}

	void SkipSpace( const String& Text, int& pos )
	{
		while( true )
		{
			if( (int)Text.length()<=pos ) { break; }

			const String::value_type c = Text[pos];
			if( c!=' ' ) { break; }

			++pos;
		}
	}
	void SkipEqual( const String& Text, int& pos )
	{
		while( true )
		{
			if( (int)Text.length()<=pos ) { break; }

			const String::value_type c = Text[pos];
			if( c!=' ' && c!='=' ) { break; }

			++pos;
		}
	}

	String ReadComment( const String& Text )
	{
		String ret;

		int pos = 2;

		//	<!-- をすっ飛ばす
		while( true )
		{
			if( (int)Text.length()<=pos ) { break; }
			if( Text[pos-2]=='-' && Text[pos-1]=='-' ) { break; }
			++pos;
		}

		//	コメント文読み取り
		while( true )
		{
			if( (int)Text.length()<=pos ) { break; }
			if( Text[pos]=='-' && Text[pos+1]=='-' ) { break; }
			ret += Text[pos];
			++pos;
		}

		//	--> をすっ飛ばす
		while( true )
		{
			if( (int)Text.length()<=pos ) { break; }
			if( Text[pos-2]=='-' && Text[pos-1]=='>' ) { break; }
			++pos;
		}
		return ret;
	}



	/*!
	 	@class	XMLDocument XMLDocument.h
	 	@brief	XMLでごにょごにょするときにベースとなるクラス
	 */

FUNCTIONRESULT XMLDocument::Parse( const std::string& TextImage )
{
  //  本当はちゃんと変換しないとまずい。
  const String Text = String::ConvertSJIStoMAID(TextImage);


	int pos=0;

	{
		//	最初のトークンは<?xml で決まっている
		String token = ReadToken( Text, pos );
	}
	{

		//	次のトークンはルートノードですが、
		//	そこまでの空白等は全部飛ばす

		while( true )
		{
			if( Text[pos]=='<' ) { break; }
			++pos;
		}
	}

	std::stack<SPXMLNODE>	NodeStack;

	m_pRootNode = ReadChildNode( ReadToken( Text, pos ) );

	NodeStack.push( m_pRootNode );

	while( true )
	{
		if( (int)Text.length()<=pos ) { break; }
		if( NodeStack.empty() ) { break; }
		const String token = ReadToken( Text, pos );

		if( token.empty() ) { continue; }

		if( token[0]=='<' )//	ノード発見
		{
			if( token[1]=='/' )//	みつけたのは終了ノード
			{
				NodeStack.pop();

			}else if( token[1]=='!' )//	みつけたのはコメントノード
			{
				SPXMLNODE pNode( new XMLNode );
				const String comment = ReadComment( token );

				pNode->SetComment( comment );
				NodeStack.top()->AddChildNode( pNode );
			}
			else
			{
				SPXMLNODE pNode = ReadChildNode( token );
				NodeStack.top()->AddChildNode( pNode );

				if( token.substr(token.length()-2)==MAIDTEXT("/>") )
				{

				}else
				{
					NodeStack.push( pNode );
				}
			}
		}else
		{
			SPXMLNODE pNode( new XMLNode );
			pNode->SetText( DecodeSpecialCharacter(token) );
			NodeStack.top()->AddChildNode( pNode );
		}
	}

	//	スタックが０でない＝＝閉じタグの個数が合ってない
  MAID_ASSERT( !NodeStack.empty(), MAIDTEXT("閉じタグの個数が合ってない") );

  return FUNCTIONRESULT_OK;
}

XMLNode& XMLDocument::GetRoot()
{
	return *m_pRootNode;
}


const XMLNode& XMLDocument::GetRoot()const
{
	return *m_pRootNode;
}

void XMLDocument::DivAttribute( const String& Element, String& Name, std::vector<ATTRIBUTE>& Attribute )
{
	MAID_ASSERT( Element[0]!='<', "タグ名ではありません" << Element );

	int pos = 1;

	Name = ReadAttributeName( Element, pos );
	SkipSpace( Element, pos );

	while( true )
	{
		if( (int)Element.length()-2<=pos ) { break; }
		//	↑ -2 なのは ">" か "/>" のケースを考えてのこと
		//	">" か "/>" で条件わけをしなくても問題ない

		ATTRIBUTE a;

		a.Name = ReadAttributeName( Element, pos );
		SkipEqual( Element, pos );
		a.Value= ReadAttributeValue( Element, pos );
		SkipSpace( Element, pos );

		Attribute.push_back( a );
	}
}

SPXMLNODE XMLDocument::ReadChildNode( const String& token )
{
	String name;
	std::vector<ATTRIBUTE> att;

	DivAttribute( token, name, att );

	SPXMLNODE pNode( new XMLNode );
	pNode->SetElementName( name );

	for( int i=0; i<(int)att.size(); ++i )
	{
		pNode->SetAttribute( att[i].Name, DecodeSpecialCharacter(att[i].Value) );
	}

	return pNode;
}



}

