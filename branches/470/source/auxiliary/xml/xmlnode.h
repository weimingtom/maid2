/*!
 	@file 
 	@brief	ＸＭＬノード
 */

#ifndef maid2_auxiliary_xml_xmlnode_h
#define maid2_auxiliary_xml_xmlnode_h

#include"../config/define.h"
#include"../string.h"

#include<vector>
#include<map>
#include<boost/smart_ptr.hpp>

namespace Maid
{

	class XMLText
	{
	public:
		XMLText();
		XMLText( const String& Text );

		int GetInt() const;
		float GetFloat() const;
		const String& GetStr() const;

		bool IsEmpty() const;
	private:
		String	m_Text;
	};

	class XMLNode;
	typedef boost::shared_ptr<XMLNode>	SPXMLNODE;

	class XMLNode
	{
	public:
		typedef std::map<String,XMLText> ATTRIBUTELIST;
		typedef std::vector<SPXMLNODE>			NODELIST;

		XMLNode();


		String	GetName() const;

		enum TYPE {
			TYPE_EMPTY,		//	空ノード
			TYPE_TEXT,		//	テキストノード
			TYPE_ELEMENT,	//	要素ノード
			TYPE_COMMENT	//	コメントノード
		};

		TYPE GetType() const;

		int GetAttributeLength()const;
		XMLText GetAttribute( const String& Name )const;
		const ATTRIBUTELIST& GetAttributeTable() const;

		XMLText GetText() const;
		String  GetComment() const;
		String  GetElementName() const;

		bool IsEmpty() const;

		int  GetChildLength()const;
		XMLNode&	GetChildNode( int index )const;

		void SetElementName( const String& Name );
		void SetText( const String& Text );
		void SetComment( const String& Text );
		void SetAttribute( const String& Name, const XMLText& Value );
		void DeleteAttribute( const String& Name );


		void AddChildNode( const SPXMLNODE& pNode );
		void InsertChildNode( int Index, const SPXMLNODE& pNode );
		void DeleteChildNode( int Index );
		void SetChildNode( int Index, const SPXMLNODE& pNode );


	private:
		String			  m_Name;
		TYPE			    m_Type;
		NODELIST		  m_ChildList;
		ATTRIBUTELIST	m_AttributeList;
		XMLText   		m_Text;
	};

}
#endif
