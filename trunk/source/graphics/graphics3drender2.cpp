#include"graphics3drender.h"

#include"customvertextemplate.h"

namespace Maid
{

void Graphics3DRender::Fill()
{
  if( m_TestInputLayout.IsCompiling() ) { return ; }

  CUSTOMVERTEX_COLOR v[] =
  {
    CUSTOMVERTEX_COLOR( POINT3DF(0,0,0), COLOR_R32G32B32A32F(1,0,0,1) ),
    CUSTOMVERTEX_COLOR( POINT3DF(0,1,0), COLOR_R32G32B32A32F(0,1,0,1) ),
    CUSTOMVERTEX_COLOR( POINT3DF(1,1,0), COLOR_R32G32B32A32F(0,0,1,1) ),
  };

  m_TestVertex.Create( v, sizeof(v) );


  const MATRIX4DF trance = MATRIX4DF().SetTranslate(0,0,0) * m_ViewMatrix * m_ProjectionMatrix;

  const IVertex& vertex = m_TestVertex;
  const IInputLayout&  layout = m_TestInputLayout;
  const IVertexShader& vs = m_MQOVertexShader;
  const IPixelShader&  ps = m_MQOPixelShader;

  const Graphics::SPDRAWCOMMAND& pCommand = m_pDrawCommand;

  {
    const IConstant& con = m_ShaderConstant;

    const int sub = 0;
    Graphics::MAPPEDRESOURCE map;
    pCommand->ResourceMap( con.Get(), sub, Graphics::IDrawCommand::MAPTYPE_WRITE, 0, map );
    memcpy( map.pData, &(trance.GetTranspose()), sizeof(trance) );
    pCommand->ResourceUnmap( con.Get(), sub );
    pCommand->VSSetConstant( 0, con.Get() );
  }

  pCommand->SetPrimitiveTopology( Graphics::IDrawCommand::PRIMITIVE_TOPOLOGY_TRIANGLELIST );

  pCommand->SetVertex( 0, vertex.Get(), 0, v[0].GetStructSize() );
  pCommand->SetInputLayout( layout.Get() );

  pCommand->VSSetShader( vs.Get() );
  pCommand->PSSetShader( ps.Get() );
  pCommand->Draw( 3, 0 );

}


void Graphics3DRender::Blt( const POINT3DF& Pos, const ModelMQO& model )
{
  const MATRIX4DF trance = MATRIX4DF().SetTranslate(Pos.x,Pos.y,Pos.z) * m_ViewMatrix * m_ProjectionMatrix;

  const IInputLayout&  layout = m_MQOLayout;
  const IVertexShader& vs = m_MQOVertexShader;
  const IPixelShader&  ps = m_MQOPixelShader;

  const Graphics::SPDRAWCOMMAND& pCommand = m_pDrawCommand;

  pCommand->SetPrimitiveTopology( Graphics::IDrawCommand::PRIMITIVE_TOPOLOGY_TRIANGLELIST );
  pCommand->SetInputLayout( layout.Get() );
  pCommand->VSSetShader( vs.Get() );
  pCommand->PSSetShader( ps.Get() );

  {
    const IConstant& con = m_ShaderConstant;

    const int sub = 0;
    Graphics::MAPPEDRESOURCE map;
    pCommand->ResourceMap( con.Get(), sub, Graphics::IDrawCommand::MAPTYPE_WRITE, 0, map );
    memcpy( map.pData, &trance, sizeof(trance) );
    pCommand->ResourceUnmap( con.Get(), sub );
    pCommand->VSSetConstant( 0, con.Get() );
  }

  {
    const std::vector<MQOMATERIAL>& mat = model.GetMaterialList();
    const std::vector<boost::shared_ptr<MQOOBJECT> >& objlist = model.GetObjectList();

    for( int i=0; i<(int)objlist.size(); ++i )
    {
      const MQOOBJECT& Obj = *(objlist[i]);
      const IVertex& point = Obj.Point;
      const IVertex& color = Obj.Color;

      pCommand->SetVertex( 0, point.Get(), 0, sizeof(POINT3DF) );
      pCommand->SetVertex( 1, color.Get(), 0, sizeof(COLOR_R32G32B32A32F) );

      for( int j=0; j<(int)Obj.Primitive.size(); ++j )
      {
        const MQOOBJECT::PRIMITIVE& prim = Obj.Primitive[j];
        const IIndex& index = prim.Index;
        const size_t size = prim.Index.GetSize();

        pCommand->SetIndex( index.Get(), 0 );
//        pCommand->DrawIndexed( size/4, 0, 0 );
        pCommand->DrawIndexed( 3*1, 0, 0 );

      }
    }
  }
}

}

