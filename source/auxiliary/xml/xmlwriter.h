/*!
 	@file 
 	@brief	ＸＭＬファイルを作成するクラス
 */

#ifndef maid2_auxiliary_xml_xmlwriter_h
#define maid2_auxiliary_xml_xmlwriter_h

#include"../config/define.h"
#include"xmldocument.h"
#include<stack>

namespace Maid
{
	class XMLWriter
	{
	public:
		XMLWriter();

		void AscendNode( const String& ElementName );
		void DescendNode();

		void NextNode();

		void SetElementName( const String& name );
		void SetValueText( const String& value );
		void SetValueFloat( float value );
		void SetValueInt( int value );

		void AddTag( const String& element, const String& value );
		void AddTag( const String& element, const float& value );
		void AddTag( const String& element, const int&   value );

		void AddAttribute( const String& name, const String& value );
		void AddAttribute( const String& name, const float& value );
		void AddAttribute( const String& name, const int& value );

    void Save( std::string& FileImage );

	private:

		void SetValue( const SPXMLNODE& pNode );
		void ConvertSaveText( const XMLNode& Node, String& text );

    String EncodeSpecialCharacter( const String& src )const;

	private:
		SPXMLNODE		m_pCurrentNode;

		std::stack<SPXMLNODE>	m_ParentStack;
	};

	class XMLWriterAutoDescend_base
	{
	public:
		XMLWriterAutoDescend_base( XMLWriter& w, const String& ElementName )
			:m_Writer(w)
		{
			m_Writer.AscendNode(ElementName);
		}

		~XMLWriterAutoDescend_base()
		{
			m_Writer.DescendNode();
		}
	private:
		XMLWriter& m_Writer;
	};

	#define XMLWriterAutoDescend2( a, writer, name ) XMLWriterAutoDescend_base p##a(writer,name)
	#define XMLWriterAutoDescend( writer, name )  XMLWriterAutoDescend2(__LINE__, writer, name)

}

#endif