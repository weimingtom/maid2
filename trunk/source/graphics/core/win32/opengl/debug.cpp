#include"debug.h"

#include"../../../../auxiliary/macro.h"
#include"../debug.h"

static std::string _sprintf( const char* format, ...)
{
  va_list args;
  char buf[1024];

  va_start(args,format);
  vsprintf_s(buf, format, args);

  return std::string(buf);
}




std::string DebugStringOpenGL( bool b )
{
  std::string ret;

  if( b ) { ret = "TRUE"; }
  else    { ret = "FALSE"; }

  ret += _sprintf( "(%0x)", b );
  return ret;
}

#if 0

std::string DebugStringD3D11( D3D11_BLEND type )
{
  std::string ret;

  switch( type )
  {
  case D3D11_BLEND_ZERO: { ret = "ZERO"; }break;
  case D3D11_BLEND_ONE:  { ret = "ONE"; }break;
  case D3D11_BLEND_SRC_COLOR:       { ret = "SRC_COLOR";      }break;
  case D3D11_BLEND_INV_SRC_COLOR:   { ret = "INV_SRC_COLOR";  }break;
  case D3D11_BLEND_SRC_ALPHA:       { ret = "SRC_ALPHA";      }break;
  case D3D11_BLEND_INV_SRC_ALPHA:   { ret = "INV_SRC_ALPHA";  }break;
  case D3D11_BLEND_DEST_ALPHA:      { ret = "DEST_ALPHA";     }break;
  case D3D11_BLEND_INV_DEST_ALPHA:  { ret = "INV_DEST_ALPHA"; }break;
  case D3D11_BLEND_DEST_COLOR:      { ret = "DEST_COLOR";     }break;
  case D3D11_BLEND_INV_DEST_COLOR:  { ret = "INV_DEST_COLOR"; }break;
  case D3D11_BLEND_SRC_ALPHA_SAT:   { ret = "SRC_ALPHA_SAT";  }break;
  case D3D11_BLEND_BLEND_FACTOR:    { ret = "BLEND_FACTOR";   }break;
  case D3D11_BLEND_INV_BLEND_FACTOR:{ ret = "INV_BLEND_FACTOR"; }break;
  case D3D11_BLEND_SRC1_COLOR:      { ret = "SRC1_COLOR";     }break;
  case D3D11_BLEND_INV_SRC1_COLOR:  { ret = "INV_SRC1_COLOR"; }break;
  case D3D11_BLEND_SRC1_ALPHA:      { ret = "SRC1_ALPHA";     }break;
  case D3D11_BLEND_INV_SRC1_ALPHA:  { ret = "INV_SRC1_ALPHA"; }break;
  }

  ret += _sprintf( "(%0x)", type );
  return ret;
}

std::string DebugStringD3D11( D3D11_BLEND_OP type )
{
  std::string ret;

  switch( type )
  {
  case D3D11_BLEND_OP_ADD:          { ret = "ADD"; }break;
  case D3D11_BLEND_OP_SUBTRACT:     { ret = "SUBTRACT"; }break;
  case D3D11_BLEND_OP_REV_SUBTRACT: { ret = "REV_SUBTRACT"; }break;
  case D3D11_BLEND_OP_MIN:          { ret = "MIN"; }break;
  case D3D11_BLEND_OP_MAX:          { ret = "MAX"; }break;
  }

  ret += _sprintf( "(%0x)", type );
  return ret;
}


std::string DebugStringD3D11( D3D11_USAGE usage )
{
  std::string ret;

  switch( usage )
  {
  case D3D11_USAGE_DEFAULT:   { ret = "DEFAULT"; }break;
  case D3D11_USAGE_IMMUTABLE: { ret = "IMMUTABLE"; }break;
  case D3D11_USAGE_DYNAMIC:   { ret = "DYNAMIC"; }break;
  case D3D11_USAGE_STAGING:   { ret = "STAGING"; }break;
  }
  ret += _sprintf( "(%0x)", usage );

  return ret;
}

std::string DebugStringD3D11( D3D11_BIND_FLAG flag )
{
  std::string ret;

  if( IsFlag(flag,D3D11_BIND_VERTEX_BUFFER   ) ) { ret+="+VERTEX_BUFFER"; }
  if( IsFlag(flag,D3D11_BIND_INDEX_BUFFER    ) ) { ret+="+INDEX_BUFFER"; }
  if( IsFlag(flag,D3D11_BIND_CONSTANT_BUFFER ) ) { ret+="+CONSTANT_BUFFER"; }
  if( IsFlag(flag,D3D11_BIND_SHADER_RESOURCE ) ) { ret+="+SHADER_RESOURCE"; }
  if( IsFlag(flag,D3D11_BIND_STREAM_OUTPUT   ) ) { ret+="+STREAM_OUTPUT"; }
  if( IsFlag(flag,D3D11_BIND_RENDER_TARGET   ) ) { ret+="+RENDER_TARGET"; }
  if( IsFlag(flag,D3D11_BIND_DEPTH_STENCIL   ) ) { ret+="+DEPTH_STENCIL"; }

  ret += _sprintf( "(%0x)", flag );

  return ret;
}

std::string DebugStringD3D11( D3D11_CPU_ACCESS_FLAG flag )
{
  std::string ret;

  if( IsFlag(flag,D3D11_CPU_ACCESS_WRITE ) ) { ret+="+WRITE"; }
  if( IsFlag(flag,D3D11_CPU_ACCESS_READ  ) ) { ret+="+READ";  }

  ret += _sprintf( "(%0x)", flag );

  return ret;
}

std::string DebugStringD3D11( D3D11_RESOURCE_MISC_FLAG flag )
{
  std::string ret;

  if( IsFlag(flag,D3D11_RESOURCE_MISC_GENERATE_MIPS     ) ) { ret+="+GENERATE_MIPS"; }
  if( IsFlag(flag,D3D11_RESOURCE_MISC_SHARED            ) ) { ret+="+SHARED";  }
  if( IsFlag(flag,D3D11_RESOURCE_MISC_TEXTURECUBE       ) ) { ret+="+TEXTURECUBE";  }
//  if( IsFlag(flag,D3D11_RESOURCE_MISC_SHARED_KEYEDMUTEX ) ) { ret+="+SHARED_KEYEDMUTEX";  }
//  if( IsFlag(flag,D3D11_RESOURCE_MISC_GDI_COMPATIBLE    ) ) { ret+="+GDI_COMPATIBLE";  }

  ret += _sprintf( "(%0x)", flag );

  return ret;
}

std::string DebugStringD3D11( D3D11_MAP type )
{
  std::string ret;

  switch( type )
  {
  case D3D11_MAP_READ:          { ret = "READ"; }break;
  case D3D11_MAP_WRITE:         { ret = "WRITE"; }break;
  case D3D11_MAP_READ_WRITE:    { ret = "READ_WRITE"; }break;
  case D3D11_MAP_WRITE_DISCARD: { ret = "WRITE_DISCARD"; }break;
  case D3D11_MAP_WRITE_NO_OVERWRITE:{ ret = "WRITE_NO_OVERWRITE"; }break;
  }
  ret += _sprintf( "(%0x)", type );

  return ret;
}

std::string DebugStringD3D11( D3D11_INPUT_CLASSIFICATION type )
{
  std::string ret;

  switch( type )
  {
  case D3D11_INPUT_PER_VERTEX_DATA :          { ret = "PER_VERTEX"; }break;
  case D3D11_INPUT_PER_INSTANCE_DATA:         { ret = "PER_INSTANCE"; }break;
  }
  ret += _sprintf( "(%0x)", type );

  return ret;
}



std::string DebugStringD3D11( const D3D11_TEXTURE2D_DESC& desc )
{
  std::string ret;

  ret += _sprintf( " Width:%0d", desc.Width );
  ret += _sprintf( " Height:%0d", desc.Height );
  ret += _sprintf( " MipLevels:%0d", desc.MipLevels );
  ret += _sprintf( " ArraySize:%0d", desc.ArraySize );
  ret += _sprintf( " Format:%s", DebugStringDXGI(desc.Format).c_str() );
  ret += _sprintf( " SampleCount:%0d", desc.SampleDesc.Count );
  ret += _sprintf( " SampleQuality:%0d", desc.SampleDesc.Quality );
  ret += _sprintf( " Usage:%s", DebugStringD3D11(desc.Usage).c_str() );
  ret += _sprintf( " BindFlags:%s", DebugStringD3D11((D3D11_BIND_FLAG)desc.BindFlags).c_str() );
  ret += _sprintf( " CPUAccessFlags:%s", DebugStringD3D11((D3D11_CPU_ACCESS_FLAG)desc.CPUAccessFlags).c_str() );
  ret += _sprintf( " MiscFlags:%s", DebugStringD3D11((D3D11_RESOURCE_MISC_FLAG)desc.MiscFlags).c_str() );

  return ret;
}

std::string DebugStringD3D11( const D3D11_SUBRESOURCE_DATA* desc )
{
  if( desc==NULL ) { return "NULL"; }
  std::string ret;

  ret += _sprintf( " pSysMem:%0x", desc->pSysMem );
  ret += _sprintf( " SysMemPitch:%0d", desc->SysMemPitch );
  ret += _sprintf( " SysMemSlicePitch:%0d", desc->SysMemSlicePitch );

  return ret;
}




std::string DebugStringD3D11( const D3D11_BLEND_DESC& desc )
{
  std::string ret;

  ret += _sprintf( " AlphaToCoverageEnable:%s", DebugStringD3D11(desc.AlphaToCoverageEnable).c_str() );

  for( int i=0; i<NUMELEMENTS(desc.RenderTarget); ++i )
  {
    const D3D11_RENDER_TARGET_BLEND_DESC& rt = desc.RenderTarget[i];

    ret += _sprintf( " rt[%0d].BlendEnable:%s",   i, DebugStringD3D11(rt.BlendEnable).c_str() );
    ret += _sprintf( " rt[%0d].SrcBlend:%s",      i, DebugStringD3D11(rt.SrcBlend).c_str() );
    ret += _sprintf( " rt[%0d].DestBlend:%s",     i, DebugStringD3D11(rt.DestBlend).c_str() );
    ret += _sprintf( " rt[%0d].BlendOp:%s",       i, DebugStringD3D11(rt.BlendOp).c_str() );
    ret += _sprintf( " rt[%0d].SrcBlendAlpha:%s", i, DebugStringD3D11(rt.SrcBlendAlpha).c_str() );
    ret += _sprintf( " rt[%0d].DestBlendAlpha:%s",i, DebugStringD3D11(rt.DestBlendAlpha).c_str() );
    ret += _sprintf( " rt[%0d].BlendOpAlpha:%s",  i, DebugStringD3D11(rt.BlendOpAlpha).c_str() );
    ret += _sprintf( " rt[%0d].RenderTargetWriteMask:%x", i, rt.RenderTargetWriteMask );
  }


  return ret;
}


std::string DebugStringD3D11( const D3D11_BUFFER_DESC& desc )
{
  std::string ret;

  ret += _sprintf( " ByteWidth:%0d", desc.ByteWidth );
  ret += _sprintf( " Usage:%s", DebugStringD3D11(desc.Usage).c_str() );
  ret += _sprintf( " BindFlags:%s", DebugStringD3D11((D3D11_BIND_FLAG)desc.BindFlags).c_str() );
  ret += _sprintf( " CPUAccessFlags:%s", DebugStringD3D11((D3D11_CPU_ACCESS_FLAG)desc.CPUAccessFlags).c_str() );
  ret += _sprintf( " MiscFlags:%s", DebugStringD3D11((D3D11_RESOURCE_MISC_FLAG)desc.MiscFlags).c_str() );

  return ret;
}


std::string DebugStringD3D11( const D3D11_INPUT_ELEMENT_DESC& desc )
{
  std::string ret;
  ret += _sprintf( " SemanticName:%s", desc.SemanticName );
  ret += _sprintf( " SemanticIndex:%0d", desc.SemanticIndex );
  ret += _sprintf( " Format:%s", DebugStringDXGI(desc.Format).c_str() );
  ret += _sprintf( " InputSlot:%0d", desc.InputSlot );
  ret += _sprintf( " AlignedByteOffset:%0d", desc.AlignedByteOffset );
  ret += _sprintf( " InputSlotClass:%s", DebugStringD3D11(desc.InputSlotClass).c_str() );

  ret += _sprintf( " InstanceDataStepRate:%0d", desc.InstanceDataStepRate );
  return ret;
}
#endif
