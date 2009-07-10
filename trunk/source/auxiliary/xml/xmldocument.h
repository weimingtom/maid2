/*!
 	@file 
 	@brief	ＸＭＬドキュメント
 */

#ifndef maid2_auxiliary_xml_xmldocument_h
#define maid2_auxiliary_xml_xmldocument_h

#include"../config/define.h"
#include"xmlnode.h"
#include"../functionresult.h"
#include<string>

namespace Maid
{
	class XMLDocument
	{
	public:
    FUNCTIONRESULT Parse( const std::string& TextImage );

		XMLNode& GetRoot();
		const XMLNode& GetRoot() const;

	private:

		struct ATTRIBUTE
		{
			String Name;
			String Value;
		};

		void DivAttribute( const String& Element, String& Name, std::vector<ATTRIBUTE>& Attribute );
		SPXMLNODE ReadChildNode( const String& token );


	private:
		SPXMLNODE	m_pRootNode;
	};

}
#endif
