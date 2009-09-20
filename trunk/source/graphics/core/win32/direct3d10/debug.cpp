#include"debug.h"
#include"../debug.h"

#include"../../../../auxiliary/macro.h"

static std::string _sprintf( const char* format, ...)
{
  va_list args;
  char buf[1024];

  va_start(args,format);
  vsprintf_s(buf, format, args);

  return std::string(buf);
}




std::string DebugStringD3D10( BOOL b )
{
  std::string ret;

  if( b==TRUE ) { ret = "TRUE"; }
  else          { ret = "FALSE"; }

  ret += _sprintf( "(%0x)", b );
  return ret;
}


std::string DebugStringD3D10( D3D10_BLEND type )
{
  std::string ret;

  switch( type )
  {
  case D3D10_BLEND_ZERO: { ret = "ZERO"; }break;
  case D3D10_BLEND_ONE:  { ret = "ONE"; }break;
  case D3D10_BLEND_SRC_COLOR:       { ret = "SRC_COLOR";      }break;
  case D3D10_BLEND_INV_SRC_COLOR:   { ret = "INV_SRC_COLOR";  }break;
  case D3D10_BLEND_SRC_ALPHA:       { ret = "SRC_ALPHA";      }break;
  case D3D10_BLEND_INV_SRC_ALPHA:   { ret = "INV_SRC_ALPHA";  }break;
  case D3D10_BLEND_DEST_ALPHA:      { ret = "DEST_ALPHA";     }break;
  case D3D10_BLEND_INV_DEST_ALPHA:  { ret = "INV_DEST_ALPHA"; }break;
  case D3D10_BLEND_DEST_COLOR:      { ret = "DEST_COLOR";     }break;
  case D3D10_BLEND_INV_DEST_COLOR:  { ret = "INV_DEST_COLOR"; }break;
  case D3D10_BLEND_SRC_ALPHA_SAT:   { ret = "SRC_ALPHA_SAT";  }break;
  case D3D10_BLEND_BLEND_FACTOR:    { ret = "BLEND_FACTOR";   }break;
  case D3D10_BLEND_INV_BLEND_FACTOR:{ ret = "INV_BLEND_FACTOR"; }break;
  case D3D10_BLEND_SRC1_COLOR:      { ret = "SRC1_COLOR";     }break;
  case D3D10_BLEND_INV_SRC1_COLOR:  { ret = "INV_SRC1_COLOR"; }break;
  case D3D10_BLEND_SRC1_ALPHA:      { ret = "SRC1_ALPHA";     }break;
  case D3D10_BLEND_INV_SRC1_ALPHA:  { ret = "INV_SRC1_ALPHA"; }break;
  }

  ret += _sprintf( "(%0x)", type );
  return ret;
}

std::string DebugStringD3D10( D3D10_BLEND_OP type )
{
  std::string ret;

  switch( type )
  {
  case D3D10_BLEND_OP_ADD:          { ret = "ADD"; }break;
  case D3D10_BLEND_OP_SUBTRACT:     { ret = "SUBTRACT"; }break;
  case D3D10_BLEND_OP_REV_SUBTRACT: { ret = "REV_SUBTRACT"; }break;
  case D3D10_BLEND_OP_MIN:          { ret = "MIN"; }break;
  case D3D10_BLEND_OP_MAX:          { ret = "MAX"; }break;
  }

  ret += _sprintf( "(%0x)", type );
  return ret;
}


std::string DebugStringD3D10( D3D10_USAGE usage )
{
  std::string ret;

  switch( usage )
  {
  case D3D10_USAGE_DEFAULT:   { ret = "DEFAULT"; }break;
  case D3D10_USAGE_IMMUTABLE: { ret = "IMMUTABLE"; }break;
  case D3D10_USAGE_DYNAMIC:   { ret = "DYNAMIC"; }break;
  case D3D10_USAGE_STAGING:   { ret = "STAGING"; }break;
  }
  ret += _sprintf( "(%0x)", usage );

  return ret;
}

std::string DebugStringD3D10( D3D10_BIND_FLAG flag )
{
  std::string ret;

  if( IsFlag(flag,D3D10_BIND_VERTEX_BUFFER   ) ) { ret+="+VERTEX_BUFFER"; }
  if( IsFlag(flag,D3D10_BIND_INDEX_BUFFER    ) ) { ret+="+INDEX_BUFFER"; }
  if( IsFlag(flag,D3D10_BIND_CONSTANT_BUFFER ) ) { ret+="+CONSTANT_BUFFER"; }
  if( IsFlag(flag,D3D10_BIND_SHADER_RESOURCE ) ) { ret+="+SHADER_RESOURCE"; }
  if( IsFlag(flag,D3D10_BIND_STREAM_OUTPUT   ) ) { ret+="+STREAM_OUTPUT"; }
  if( IsFlag(flag,D3D10_BIND_RENDER_TARGET   ) ) { ret+="+RENDER_TARGET"; }
  if( IsFlag(flag,D3D10_BIND_DEPTH_STENCIL   ) ) { ret+="+DEPTH_STENCIL"; }

  ret += _sprintf( "(%0x)", flag );

  return ret;
}

std::string DebugStringD3D10( D3D10_CPU_ACCESS_FLAG flag )
{
  std::string ret;

  if( IsFlag(flag,D3D10_CPU_ACCESS_WRITE ) ) { ret+="+WRITE"; }
  if( IsFlag(flag,D3D10_CPU_ACCESS_READ  ) ) { ret+="+READ";  }

  ret += _sprintf( "(%0x)", flag );

  return ret;
}

std::string DebugStringD3D10( D3D10_RESOURCE_MISC_FLAG flag )
{
  std::string ret;

  if( IsFlag(flag,D3D10_RESOURCE_MISC_GENERATE_MIPS     ) ) { ret+="+GENERATE_MIPS"; }
  if( IsFlag(flag,D3D10_RESOURCE_MISC_SHARED            ) ) { ret+="+SHARED";  }
  if( IsFlag(flag,D3D10_RESOURCE_MISC_TEXTURECUBE       ) ) { ret+="+TEXTURECUBE";  }
//  if( IsFlag(flag,D3D10_RESOURCE_MISC_SHARED_KEYEDMUTEX ) ) { ret+="+SHARED_KEYEDMUTEX";  }
//  if( IsFlag(flag,D3D10_RESOURCE_MISC_GDI_COMPATIBLE    ) ) { ret+="+GDI_COMPATIBLE";  }

  ret += _sprintf( "(%0x)", flag );

  return ret;
}

std::string DebugStringD3D10( D3D10_MAP type )
{
  std::string ret;

  switch( type )
  {
  case D3D10_MAP_READ:          { ret = "READ"; }break;
  case D3D10_MAP_WRITE:         { ret = "WRITE"; }break;
  case D3D10_MAP_READ_WRITE:    { ret = "READ_WRITE"; }break;
  case D3D10_MAP_WRITE_DISCARD: { ret = "WRITE_DISCARD"; }break;
  case D3D10_MAP_WRITE_NO_OVERWRITE:{ ret = "WRITE_NO_OVERWRITE"; }break;
  }
  ret += _sprintf( "(%0x)", type );

  return ret;
}

std::string DebugStringD3D10( D3D10_INPUT_CLASSIFICATION type )
{
  std::string ret;

  switch( type )
  {
  case D3D10_INPUT_PER_VERTEX_DATA :          { ret = "PER_VERTEX"; }break;
  case D3D10_INPUT_PER_INSTANCE_DATA:         { ret = "PER_INSTANCE"; }break;
  }
  ret += _sprintf( "(%0x)", type );

  return ret;
}



std::string DebugStringD3D10( const D3D10_TEXTURE2D_DESC& desc )
{
  std::string ret;

  ret += _sprintf( " Width:%0d", desc.Width );
  ret += _sprintf( " Height:%0d", desc.Height );
  ret += _sprintf( " MipLevels:%0d", desc.MipLevels );
  ret += _sprintf( " ArraySize:%0d", desc.ArraySize );
  ret += _sprintf( " Format:%s", DebugStringDXGI(desc.Format).c_str() );
  ret += _sprintf( " SampleCount:%0d", desc.SampleDesc.Count );
  ret += _sprintf( " SampleQuality:%0d", desc.SampleDesc.Quality );
  ret += _sprintf( " Usage:%s", DebugStringD3D10(desc.Usage).c_str() );
  ret += _sprintf( " BindFlags:%s", DebugStringD3D10((D3D10_BIND_FLAG)desc.BindFlags).c_str() );
  ret += _sprintf( " CPUAccessFlags:%s", DebugStringD3D10((D3D10_CPU_ACCESS_FLAG)desc.CPUAccessFlags).c_str() );
  ret += _sprintf( " MiscFlags:%s", DebugStringD3D10((D3D10_RESOURCE_MISC_FLAG)desc.MiscFlags).c_str() );

  return ret;
}

std::string DebugStringD3D10( const D3D10_SUBRESOURCE_DATA* desc )
{
  if( desc==NULL ) { return "NULL"; }
  std::string ret;

  ret += _sprintf( " pSysMem:%0x", desc->pSysMem );
  ret += _sprintf( " SysMemPitch:%0d", desc->SysMemPitch );
  ret += _sprintf( " SysMemSlicePitch:%0d", desc->SysMemSlicePitch );

  return ret;
}




std::string DebugStringD3D10( const D3D10_BLEND_DESC& desc )
{
  std::string ret;

  ret += _sprintf( " AlphaToCoverageEnable:%s", DebugStringD3D10(desc.AlphaToCoverageEnable).c_str() );
  ret += _sprintf( " BlendEnable[0]:%s", DebugStringD3D10(desc.BlendEnable[0]).c_str() );
  ret += _sprintf( " BlendEnable[1]:%s", DebugStringD3D10(desc.BlendEnable[1]).c_str() );
  ret += _sprintf( " BlendEnable[2]:%s", DebugStringD3D10(desc.BlendEnable[2]).c_str() );
  ret += _sprintf( " BlendEnable[3]:%s", DebugStringD3D10(desc.BlendEnable[3]).c_str() );
  ret += _sprintf( " BlendEnable[4]:%s", DebugStringD3D10(desc.BlendEnable[4]).c_str() );
  ret += _sprintf( " BlendEnable[5]:%s", DebugStringD3D10(desc.BlendEnable[5]).c_str() );
  ret += _sprintf( " BlendEnable[6]:%s", DebugStringD3D10(desc.BlendEnable[6]).c_str() );
  ret += _sprintf( " BlendEnable[7]:%s", DebugStringD3D10(desc.BlendEnable[7]).c_str() );

  ret += _sprintf( " SrcBlend:%s", DebugStringD3D10(desc.SrcBlend).c_str() );
  ret += _sprintf( " DestBlend:%s", DebugStringD3D10(desc.DestBlend).c_str() );
  ret += _sprintf( " BlendOp:%s", DebugStringD3D10(desc.BlendOp).c_str() );
  ret += _sprintf( " SrcBlendAlpha:%s", DebugStringD3D10(desc.SrcBlendAlpha).c_str() );
  ret += _sprintf( " DestBlendAlpha:%s", DebugStringD3D10(desc.DestBlendAlpha).c_str() );
  ret += _sprintf( " BlendOpAlpha:%s", DebugStringD3D10(desc.BlendOpAlpha).c_str() );
  ret += _sprintf( " RenderTargetWriteMask[0]:%x", desc.RenderTargetWriteMask[0] );
  ret += _sprintf( " RenderTargetWriteMask[1]:%x", desc.RenderTargetWriteMask[1] );
  ret += _sprintf( " RenderTargetWriteMask[2]:%x", desc.RenderTargetWriteMask[2] );
  ret += _sprintf( " RenderTargetWriteMask[3]:%x", desc.RenderTargetWriteMask[3] );
  ret += _sprintf( " RenderTargetWriteMask[4]:%x", desc.RenderTargetWriteMask[4] );
  ret += _sprintf( " RenderTargetWriteMask[5]:%x", desc.RenderTargetWriteMask[5] );
  ret += _sprintf( " RenderTargetWriteMask[6]:%x", desc.RenderTargetWriteMask[6] );
  ret += _sprintf( " RenderTargetWriteMask[7]:%x", desc.RenderTargetWriteMask[7] );

  return ret;
}


std::string DebugStringD3D10( const D3D10_BUFFER_DESC& desc )
{
  std::string ret;

  ret += _sprintf( " ByteWidth:%0d", desc.ByteWidth );
  ret += _sprintf( " Usage:%s", DebugStringD3D10(desc.Usage).c_str() );
  ret += _sprintf( " BindFlags:%s", DebugStringD3D10((D3D10_BIND_FLAG)desc.BindFlags).c_str() );
  ret += _sprintf( " CPUAccessFlags:%s", DebugStringD3D10((D3D10_CPU_ACCESS_FLAG)desc.CPUAccessFlags).c_str() );
  ret += _sprintf( " MiscFlags:%s", DebugStringD3D10((D3D10_RESOURCE_MISC_FLAG)desc.MiscFlags).c_str() );

  return ret;
}


std::string DebugStringD3D10( const D3D10_INPUT_ELEMENT_DESC& desc )
{
  std::string ret;
  ret += _sprintf( " SemanticName:%s", desc.SemanticName );
  ret += _sprintf( " SemanticIndex:%0d", desc.SemanticIndex );
  ret += _sprintf( " Format:%s", DebugStringDXGI(desc.Format).c_str() );
  ret += _sprintf( " InputSlot:%0d", desc.InputSlot );
  ret += _sprintf( " AlignedByteOffset:%0d", desc.AlignedByteOffset );
  ret += _sprintf( " InputSlotClass:%s", DebugStringD3D10(desc.InputSlotClass).c_str() );

  ret += _sprintf( " InstanceDataStepRate:%0d", desc.InstanceDataStepRate );
  return ret;
}
