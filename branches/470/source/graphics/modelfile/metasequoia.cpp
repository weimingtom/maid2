#include"metasequoia.h"
#include"../../auxiliary/macro.h"
#include"../../auxiliary/debug/assert.h"

#include<string>

namespace Maid { namespace Metasequoia {

  // トークンを抜き出す
  static std::string ReadTorken( const char*& p )
  {
    // スペース,タブが続いている間飛ばす
    while( true )
    {
      const char c = *p;
      if( String::IsSJIS1(c) ) { break; }
      if( c!=' ' && c!='\t' ) { break; }
      ++p;
    }
    const char* end = p;
    while( true )
    {
      const char c = *end;
      if( String::IsSJIS1(c) ) { end += 2; continue; }
      if( c==' ' || c=='\t' ) { break; }
      else if( c=='\r'|| c=='\n' ) { break; }
      ++end;
    }

    std::string str( p, end-p );
  	
    p = end;

    return str;
  }


  // 次のトークンの先頭ポインタを返す
  // 行の末端まで進んでいたら　ＮＵＬＬ
  static const char* NextTorken( const char*& p )
  {
    // スペースがでてくるまで飛ばす
    while( true )
    {
      const char c = *p;
      if( String::IsSJIS1(c) ) { p += 2; continue; }
      if( c==' ' || c=='\t' ) { break; }
      else if( c=='\r'||c=='\n' ) { return NULL; }
      ++p;
    }

    // スペースが続いている間飛ばす
    while( true )
    {
      const char c = *p;
      if( String::IsSJIS1(c) ) { break;; }
      if( c!=' '&& c!='\t' ) { break; }
      ++p;
    }

    if( p[0]=='\r'|| p[1]=='\n' ) { return NULL; }

    return p;
  }

  // 次の行の先頭ポインタを返す
  // ファイルの末端まで進んでいたら　ＮＵＬＬ
  static const char* NextLine( const char*& p )
  {
    while( true )
    {
      const char c = *p;
      if( String::IsSJIS1(c) ) { p += 2; continue; }
      if( c=='\r' ) { p+=2; break; }
      if( c=='\n' ) { p+=1; break; }
      ++p;
    }

    if( *p=='\0' ) { return NULL; }

    return p;
  }

  // "XXX" を XXX の文字列に変換する
  static std::string DivFileName( const std::string& Name )
  {
    return Name.substr(1,Name.length()-2);
  }


struct ATTRIBUTE
{
	std::string	              Type;
	std::vector<std::string>  Data;
};

struct FACEDATA
{
	int                     IndexCount;
	std::vector<ATTRIBUTE>  Attribute;
};

static void ParseAttribute( const char*& p, std::vector<ATTRIBUTE>& AttributeList )
{
	while( true )
	{
		if( *p=='\r' ) { break; }
		while( *p==' ' ) { ++p; }

		ATTRIBUTE d;
		while( *p!='(' ) { d.Type += *p; ++p; }
		++p; // '(' のスキップ

		while( *p!=')' )
		{
			std::string str;
			while( *p!=' ' && *p!=')') { str += *p; ++p; }
			d.Data.push_back( str );
			while( *p==' ' ) { ++p; }
		}
		++p; // ')' のスキップ
		AttributeList.push_back(d);
	}
}


//! Object チャンクの解析
/*!
    @param	p   [io]	解析するデータ（関数が成功するとObjectチャンクは終了直後のアドレスになります)
    @param	obj [ o]	出力先

    @return	関数の成功で 0
 */
static int ParseObject( const char*& p, DATA::OBJECT& obj )
{
  obj.Name = String::ConvertSJIStoMAID( DivFileName(ReadTorken(p)) );
	NextLine(p);

	while( true )
	{
		const std::string chanck = ReadTorken(p);

		if( chanck=="}"       ) { break; }
		else if( chanck=="visible" ) { /* 不明な値 */ }
		else if( chanck=="locking" ) { /* 不明な値 */ }
		else if( chanck=="shading" ) { obj.Shading = atoi(NextTorken(p)); }
		else if( chanck=="depth" ) { obj.Depth = atoi(NextTorken(p)); }
		else if( chanck=="facet"   ) { /* 意味不明 */ }
		else if( chanck=="scale"   )
    {
      obj.Scale.w = (float)atof(NextTorken(p));
      obj.Scale.h = (float)atof(NextTorken(p));
      obj.Scale.d = (float)atof(NextTorken(p));
    }
		else if( chanck=="rotation"   )
    {
      obj.RotateHedding = (float)atof(NextTorken(p));
      obj.RotatePitch = (float)atof(NextTorken(p));
      obj.RotateBank = (float)atof(NextTorken(p));
    }
		else if( chanck=="translation"   )
    {
      obj.Trans.x = (float)atof(NextTorken(p));
      obj.Trans.y = (float)atof(NextTorken(p));
      obj.Trans.z = (float)atof(NextTorken(p));
    }
    else if( chanck=="color"   )
		{
			obj.Color.SetA( 1.0f );
			obj.Color.SetR( (float)atof(NextTorken(p)) );
			obj.Color.SetG( (float)atof(NextTorken(p)) );
			obj.Color.SetB( (float)atof(NextTorken(p)) );
		}
		else if( chanck=="color_type" )
		{
      obj.ColorType = (DATA::OBJECT::COLORTYPE)atoi(NextTorken(p));
		}
		else if( chanck=="vertex" ) 
		{
			const int count = atoi(NextTorken(p));
			NextLine(p);
			obj.Point.resize(count);
			for( int i=0; i<count; ++i )
			{
				POINT3DF& pos = obj.Point[i];
				pos.x = (float)atof(NextTorken(p));
				pos.y = (float)atof(NextTorken(p));
				pos.z = (float)atof(NextTorken(p));
				NextLine(p);
			}
		}
		else if( chanck=="face" )
		{
			const int count = atoi(NextTorken(p));
			NextLine(p);
			obj.Face.resize(count);
			for( int i=0; i<count; ++i )
			{
        DATA::OBJECT::FACE& face = obj.Face[i];

        const int IndexCount = atoi(ReadTorken(p).c_str());
        MAID_ASSERT( IndexCount!=3, "三角形以外は受け付けません" );

        std::vector<ATTRIBUTE> attr;
				ParseAttribute( p, attr );

				for( int j=0; j<(int)attr.size(); ++j )
				{
					const ATTRIBUTE& tmp = attr[j];
					const std::string type = tmp.Type;

					if( tmp.Type=="V" )
					{
						for( int k=0; k<IndexCount; ++k )
						{
							face.PointIndex[k] = atoi(tmp.Data[k].c_str());
						}
					}
					else if( tmp.Type=="M" )
					{
						face.MaterialNo = atoi(tmp.Data[0].c_str());
					}
					else if( tmp.Type=="UV" )
					{
						for( int k=0; k<IndexCount; ++k )
						{
              const float u = (float)atof(tmp.Data[k*2+0].c_str());
              const float v = (float)atof(tmp.Data[k*2+1].c_str());

							face.UV[k] = POINT2DF( u, v );
						}
					}
					else if( tmp.Type=="COL" )
					{
						for( int k=0; k<IndexCount; ++k )
						{
    #pragma COMPILERMSG("TODO:toi(\"4294901760\") が 0x7FFFFFFFになる悲劇があるので、ちょっと放置 ")	
            //  const unt32 col = (unt32)atoi(tmp.Data[k].c_str());
						//	face.Color[k] = XXXXXXX;
						}
					}				}
				NextLine(p);
			}
		
		}

		NextLine(p);
	}
	NextLine(p);
  
  return 0;
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! Material チャンクの解析
/*!
    @param	p	[io]	解析するデータ（関数が成功するとMaterialチャンクは終了直後のアドレスになります)

    @return	関数の成功で 0
 */
int ParseMaterial( const char*& p, std::vector<DATA::MATERIAL>& MaterialList )
{
	const int MaterialCount = atoi(NextTorken(p));
	NextLine(p);
	MaterialList.resize(MaterialCount);
	
	for( int i=0; i<MaterialCount; ++i )
	{
		DATA::MATERIAL& material = MaterialList[i];

		std::vector<ATTRIBUTE> Attribute;

    material.Name = String::ConvertSJIStoMAID(DivFileName(ReadTorken(p)));

		ParseAttribute( p, Attribute );

		for( std::vector<ATTRIBUTE>::size_type j=0; j<Attribute.size(); ++j )
		{
			const ATTRIBUTE& a = Attribute[j];

			if( a.Type=="col" )
			{
				material.Color.SetR( (float)atof(a.Data[0].c_str()) );
				material.Color.SetG( (float)atof(a.Data[1].c_str()) );
				material.Color.SetB( (float)atof(a.Data[2].c_str()) );
				material.Color.SetA( (float)atof(a.Data[3].c_str()) );
			}
			else if( a.Type=="dif"   ) { material.Diffuse  = (float)atof(a.Data[0].c_str()); }
			else if( a.Type=="amb"   ) { material.Ambient  = (float)atof(a.Data[0].c_str()); }
			else if( a.Type=="emi"   ) { material.Emissive = (float)atof(a.Data[0].c_str()); }
			else if( a.Type=="spc"   ) { material.Specular = (float)atof(a.Data[0].c_str()); }
			else if( a.Type=="power" ) { material.Power    = (float)atof(a.Data[0].c_str()); }
      else if( a.Type=="tex"   ) { material.Texture  = String::ConvertSJIStoMAID( DivFileName(a.Data[0]) );    }
		}

		NextLine(p);
	}
	NextLine(p);

  return 0;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! Scene チャンクの解析
/*!
    @param	p	[io]	解析するデータ（関数が成功するとSceneチャンクは終了直後のアドレスになります)
 
    @return	関数の成功で 0
 */
static int ParseScene( const char*& p, DATA::SCENE& Scene )
{
  //	めんどくさいのでいいかげんに処理
  //  使わないしね。
  while( true )
  {
    const std::string chanck( ReadTorken(p) );

    if( chanck=="}" ) { break; }
    NextLine(p);
  }
  NextLine(p);

  //  ちゃんとパースするほうがいいけど、使わないと思うので適当に
  Scene.Ambient = COLOR_R32G32B32A32F( 0.250f, 0.250f, 0.250f, 1.0f );

  return 0;
}



bool Check( const std::vector<unt08>& FileImage )
{
	const std::string HEAD( "Metasequoia Document" );

	if( FileImage.size() < HEAD.length() ) { return false; }

	if( ::memcmp( &(FileImage[0]), HEAD.c_str(), HEAD.size() )!=0 ) { return false; }

  return true;
}

int Load( const std::vector<unt08>& FileImage, DATA& data )
{
  #pragma COMPILERMSG("TODO:FileImageをMSTRINGに変換してからパースしたほうがいいかも？")	

	const char* p = (const char*)&(FileImage[0]);
	const char* end = (const char*)&(FileImage.back());

	while( true )
	{
		if( end <= p ) { break; }

		const std::string torken = ReadTorken(p);
		if( torken=="Metasequoia" ) { NextLine(p); }
		else if( torken=="Format"      ) { NextLine(p); }
		else if( torken=="Scene"       ) { ParseScene(p, data.Scene ); }
		else if( torken=="Eof"         ) { break;       }
		else if( torken=="Material"    )
		{
			if( ParseMaterial( p, data.Material )!=0 ) { return 1; }
		}
		else if( torken=="Object"      )
		{
      DATA::OBJECT	object;
			if( ParseObject( p, object )!=0 ) { return 2; }
			data.Object.push_back( object );
		}
		else
		{
			NextLine(p);
		}
	}

  return 0;
}


}}