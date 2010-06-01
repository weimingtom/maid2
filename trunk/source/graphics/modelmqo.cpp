#include"modelmqo.h"
#include"modelfile/metasequoia.h"
#include"../storage/filereadersync.h"
#include"../auxiliary/debug/warning.h"

namespace Maid
{
  namespace KEEPOUT
  {
    class MQOFunction : public IJobFunction
    {
    public:
      void Execute( const IJobInput& Input, IJobOutput& Output );

    private:
      void CreateScene( const Metasequoia::DATA::SCENE& src, MQOSCENE& dst );
      void CreateMaterial( const Metasequoia::DATA::MATERIAL& src, MQOMATERIAL& dst );
      void CreateObject( const Metasequoia::DATA::OBJECT& src, MQOOBJECT& dst );
      void CreateNormal( const Metasequoia::DATA::OBJECT& src, std::vector<VECTOR3DF>& normal );
    };

    void MQOFunction::Execute( const IJobInput& Input, IJobOutput& Output )
    {
      const MQOInput&  in  = static_cast<const MQOInput&>(Input);
      MQOOutput& out = static_cast<MQOOutput&>(Output);

      Metasequoia::DATA SrcData;
      {
        FileReaderSync  hFile;

        {
          const bool ret = hFile.Open( in.FileName );
          if( ret ) { MAID_WARNING( MAIDTEXT("オープンに失敗") << in.FileName ); return ; }
        }

        std::vector<unt08> image( hFile.GetSize() );

        {
          const size_t ret = hFile.Read( &(image[0]), image.size() );
          if( ret!=image.size() ) { MAID_WARNING( MAIDTEXT("読み込みに失敗") << in.FileName ); return ; }
        }

        {
          if( !Metasequoia::Check( image ) ) { MAID_WARNING( MAIDTEXT("ファイルがmqoではありません") << in.FileName ); return ; }
          Metasequoia::Load( image, SrcData );
        }
      }

      //  これで mqo が src に入った。
      //  out に出力していく

      {
        CreateScene( SrcData.Scene, out.Scene );
      }
      {
        out.Material.resize( SrcData.Material.size() );

        for( int i=0; i<(int)out.Material.size(); ++i )
        {
          CreateMaterial( SrcData.Material[i], out.Material[i] );
        }
      }

      {
        for( int i=0; i<(int)SrcData.Object.size(); ++i )
        {
          const Metasequoia::DATA::OBJECT& src = SrcData.Object[i];
          boost::shared_ptr<MQOOBJECT> pDst ( new MQOOBJECT );

          CreateObject( src, *pDst );

          if( src.Depth==0 ) { out.Object.push_back(pDst); }
          else
          {
            boost::shared_ptr<MQOOBJECT> pParent = out.Object.back();
            for( int i=1; i<src.Depth; ++i )
            {
              pParent = pParent->Child.back();
            }
            pParent->Child.push_back(pDst);
          }
        }
      }
    }

    void MQOFunction::CreateScene( const Metasequoia::DATA::SCENE& src, MQOSCENE& dst )
    {
      dst.Ambient = src.Ambient;
    }


    void MQOFunction::CreateMaterial( const Metasequoia::DATA::MATERIAL& src, MQOMATERIAL& dst )
    {
      dst.Color = src.Color;
      dst.Diffuse = src.Diffuse;
      dst.Ambient = src.Ambient;
      dst.Specular = src.Specular;
      dst.Emissive = src.Emissive;
      dst.Power = src.Power;

      
      //  ファイル名がフルパスで入ってしまっているので、mqoと同じディレクトリにあることを保障してもらう
      if( !src.Texture.empty() )
      {
        dst.Texture.LoadFile( String::GetFileName(src.Texture) );
        while( true )
        {
          if(  !dst.Texture.IsLoading() ) { break; }
          ThreadController::Sleep(1);
        }
      }

      if( !src.Bump.empty() )
      {
        dst.Bump.LoadFile( String::GetFileName(src.Bump) );
        while( true )
        {
          if(  !dst.Bump.IsLoading() ) { break; }
          ThreadController::Sleep(1);
        } 
      }
      #pragma COMPILERMSG("TODO:この後にやっている法線の計算も結構重いはずなので、最後にチェックするほうがパフォーマンス出るかもしれない")	
    }


    void MQOFunction::CreateObject( const Metasequoia::DATA::OBJECT& src, MQOOBJECT& dst )
    {
      std::vector<POINT3DF>             VertexPoint;  //  最終的に作成する座標
      std::vector<VECTOR3DF>            VertexNormal; //  最終的に作成する法線
      std::vector<POINT2DF>             VertexUV;     //  最終的に作成するUV
      std::vector<COLOR_R32G32B32A32F>  VertexColor;  //  最終的に作成する色
      std::vector<VECTOR3DF>            VertexTangent;//  最終的に作成する接ベクトル

      typedef std::map<int,std::vector<unt16> > INDEXMAP;
      INDEXMAP IndexMap; //  マテリアル番号を元にインデックスが並んでる

      VertexPoint  = src.Point;
      VertexNormal = std::vector<VECTOR3DF>( src.Point.size(), VECTOR3DF(0,0,0) );
      VertexUV     = std::vector<POINT2DF>( src.Point.size(), POINT2DF(0,0) );
      VertexColor  = std::vector<COLOR_R32G32B32A32F>( src.Point.size(), COLOR_R32G32B32A32F(1,1,1,1) );

      {
        //  メタセコイアの座標系は右手座標系(+x:右 +y:上 +z:手前)なんだけど
        //  ライブラリは左手座標系(+x:右 +y:上 +z:奥) なので、Ｚだけ反転させる
        for( int i=0; i<(int)VertexPoint.size(); ++i )
        {
          VertexPoint[i].z = -(VertexPoint[i].z);
        }
      }
      //  法線を作るときも座標系の変換を反映させないといけないけど
      //  設計としては元データから作るべきなので、内部で再度行っています
      CreateNormal( src, VertexNormal );

      //	インデックスのコピー
      //	このときにＵＶ,Colorもコピーする
      {
		    //	メタセコイアは同じインデックスでもUV が違うことがある。
		    //	なので、すでに設定している場所は別途コピーした頂点をあてがうようにする
		    //	false..まだ true..設定した
		    std::vector<bool>		WriteFlag( src.Point.size(), false );

        for( int i=0; i < (int)src.Face.size(); ++i )
        {
          const Metasequoia::DATA::OBJECT::FACE& face = src.Face[i];
          std::vector<unt16>& IndexList = IndexMap[face.MaterialNo];

          for( int j=0; j<NUMELEMENTS(face.PointIndex); ++j )
          {
            const unt16 index  = face.PointIndex[j];
            const POINT2DF& uv = face.UV[j];
            const COLOR_R32G32B32A32F& col = face.Color[j];
            const bool IsWrite = WriteFlag[index];
            const POINT2DF& olduv = VertexUV[index];

            if( IsWrite && (uv.x!=olduv.x || uv.y!=olduv.y) )
            {
              const unt32 new_index = VertexPoint.size();
              VertexPoint.push_back( VertexPoint[index] );
              VertexNormal.push_back( VertexNormal[index] );
              VertexUV.push_back( uv );
              VertexColor.push_back( col );
              WriteFlag.push_back( true );

              IndexList.push_back(new_index);
            }else
            {
              VertexUV[index]    = uv;
              VertexColor[index] = col;
              WriteFlag[index] = true;
              IndexList.push_back(index);
            }
          }
        }
      }

      {
        VertexTangent= std::vector<VECTOR3DF>( VertexPoint.size(), VECTOR3DF(0,0,0) );

        //  接ベクトルの計算
        //  DirectX9シェーダプログラミングブック ISBN4-8399-1247-5 (P277) を参照
        for( INDEXMAP::const_iterator ite=IndexMap.begin(); ite!=IndexMap.end(); ++ite )
        {
          const std::vector<unt16>& IndexList = ite->second;
          MAID_ASSERT( IndexList.size()%3!=0, "インデックスの数が合いません " << IndexList.size() );
          for( int i=0; i<(int)IndexList.size(); i+=3 )
          {
            const unt16 index0 = IndexList[i+0];
            const unt16 index1 = IndexList[i+1];
            const unt16 index2 = IndexList[i+2];

            const POINT3DF& p0 = VertexPoint[index0];
            const POINT3DF& p1 = VertexPoint[index1];
            const POINT3DF& p2 = VertexPoint[index2];

            const POINT2DF& uv0 = VertexUV[index0];
            const POINT2DF& uv1 = VertexUV[index1];
            const POINT2DF& uv2 = VertexUV[index2];

            const VECTOR3DF& norm = VertexNormal[index0];

            const float u20 = uv2.x - uv0.x;
            const float u10 = uv1.x - uv0.x;
            
            const VECTOR3DF Bdash   = (VECTOR3DF(p0,p1) * (-u20)) + (VECTOR3DF(p0,p2)*u10);
            const VECTOR3DF Tangent = VectorCross( Bdash, norm ).Normalize();

            VertexTangent[index0] += Tangent;
            VertexTangent[index1] += Tangent;
            VertexTangent[index2] += Tangent;
          }

          for( int i=0; i<(int)VertexTangent.size(); ++i )
          {
            VertexTangent[i].Normalize();
          }
        }
      }
      //  これで全部そろったので、頂点の作成

      dst.Point.Create( &(VertexPoint[0]), VertexPoint.size()*sizeof(POINT3DF) );
      dst.Normal.Create( &(VertexNormal[0]), VertexNormal.size()*sizeof(VECTOR3DF) );
      dst.UV.Create( &(VertexUV[0]), VertexUV.size()*sizeof(POINT2DF) );
      dst.Color.Create( &(VertexColor[0]), VertexColor.size()*sizeof(COLOR_R32G32B32A32F) );
      dst.Tangent.Create( &(VertexTangent[0]), VertexTangent.size()*sizeof(VECTOR3DF) );

      for( INDEXMAP::iterator ite=IndexMap.begin(); ite!=IndexMap.end(); ++ite )
      {
        const int no = ite->first;
        const std::vector<unt16>& index = ite->second;
        MQOOBJECT::PRIMITIVE prim;

        prim.MaterialNo = no;
        prim.Index.Create( &(index[0]), index.size()*sizeof(unt16) );
        dst.Primitive.push_back(prim);
      }

      dst.Scale = src.Scale;
      dst.RotateHedding = src.RotateHedding;
      dst.RotatePitch = src.RotatePitch;
      dst.RotateBank = src.RotateBank;
      dst.Trans = src.Trans;
    }

    void MQOFunction::CreateNormal( const Metasequoia::DATA::OBJECT& src, std::vector<VECTOR3DF>& normal )
    {
      //  法線を作る作業
      for( int facepos=0; facepos < (int)src.Face.size(); ++facepos )
      {
        const Metasequoia::DATA::OBJECT::FACE& face = src.Face[facepos];
        const unt32 index0 = face.PointIndex[0];
        const unt32 index1 = face.PointIndex[1];
        const unt32 index2 = face.PointIndex[2];
        POINT3DF p0 = src.Point[index0];
        POINT3DF p1 = src.Point[index1];
        POINT3DF p2 = src.Point[index2];

        //  座標系を取り替える作業
        p0.z = -p0.z; p1.z = -p1.z; p2.z = -p2.z;

        VECTOR3DF vec( VectorCross( VECTOR3DF(p0,p1), VECTOR3DF(p0,p2) ) );
        vec.Normalize();
        normal[index0] += vec;
        normal[index1] += vec;
        normal[index2] += vec;
      }
      for( int i=0; i<(int)normal.size(); ++i )
      {
        normal[i].Normalize();
      }
    }


  }




void ModelMQO::Load( const String& FileName )
{
  m_Cache.Start( KEEPOUT::MQOInput(FileName) );
}

bool ModelMQO::IsLoading()const
{
  //  処理が終わるまでは忙しい
  if( m_Cache.IsExecuting() ) { return true; }

  return false;
}

bool ModelMQO::IsEmpty()const
{
  return m_Cache.IsEmpty();
}


void ModelMQO::Delete()
{
  m_Cache.Reset();
}

String ModelMQO::GetFileName() const
{
  return m_Cache.GetInput().FileName;

}

const MQOSCENE& ModelMQO::GetScene() const
{
  return m_Cache.GetOutput().Scene;
}

const std::vector<MQOMATERIAL>& ModelMQO::GetMaterialList() const
{
  return m_Cache.GetOutput().Material;
}

const std::vector<boost::shared_ptr<MQOOBJECT> >& ModelMQO::GetObjectList() const
{
  return m_Cache.GetOutput().Object;
}


}

