#ifndef maid2_graphic_core_driver_h
#define maid2_graphic_core_driver_h


#include"../../../config/define.h"
#include"../../../config/typedef.h"

#include<boost/smart_ptr.hpp>
#include<vector>

#include"../../../auxiliary/exception.h"
#include"../../../auxiliary/mathematics.h"
#include"../../color.h"
#include"../../pixelformat.h"

#include"surfacebuffer.h"
#include"surfacebuffermemory.h"
#include"vertexbuffer.h"
#include"indexbuffer.h"
#include"texturebuffer.h"
#include"texturebuffermemory.h"
#include"vertexshaderbuffer.h"
#include"pixelshaderbuffer.h"
#include"vertexdeclarationbuffer.h"
#include"vertexdeclarationbuffermemory.h"




namespace Maid
{
  /*!	
      @brief	�f�o�C�X�����X�g�����Ƃ��ɔ��ł����O
  \n			������E������ VideoCard::Reset() �ŕ��A�����݂�
  */
  class DeviceLost : public Exception
  {

  };

  /*!	
      @class	VideoCard videocard.h
      @brief	�r�f�I�h���C�o
  \n    			�h���C�o�ɂ���Ă͂ł��邱�ƁA�ł��Ȃ����Ƃ�����܂���
  \n		    	�ł��Ȃ��ꍇ�̓G���[���o���̂ł͂Ȃ��A�ؗ�ɃX���[����悤�ɂ��Ă�������
  \n			    �����^�C���G���[���N�������ꍇ�� Exception �𓊂��邱��
  */

  class VideoCard
  {
  public:
    struct SCREENMODE
    {
      enum DISPLAY
      {
        DISPLAY_FULL16,
        DISPLAY_FULL32,
        DISPLAY_FULL32EX,  //!<  RGB10bit���[�h
        DISPLAY_WINDOW,
      };
      SIZE2DI  ScreenSize;   //!<  ��ʉ𑜓x
      DISPLAY  ScreenMode;   //!<  �X�N���[�����[�h
      int      RefreshRate;  //!<  ���t���b�V�����[�g
      bool     IsWaitVSync;  //!<  VSync�������邩�H
    };


    virtual ~VideoCard(){};

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! �h���C�o�̏�����
    /*!
        @param	mode	[i ]	���������ė~�����𑜓x

        @exception Exception �������Ɏ��s�����ꍇ
     */
    virtual void Initialize( const SCREENMODE& mode )=0;


    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! �X�N���[�����[�h�̕ύX
    /*!
        �f�o�C�X���X�g����̕��A�ɂ��Ă�ŉ�����

        @param	mode	[i ]	�����炵���𑜓x

        @exception Exception �������Ɏ��s�����ꍇ
     */
    virtual void Reset( const SCREENMODE& mode )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! ���X�g����̕��A
    /*!
        @exception Exception ���A�Ɏ��s�����ꍇ
     */
    virtual void Restore()=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! �h���C�o�̊J��
    /*!
        @exception Exception �J���Ɏ��s�����ꍇ
     */
    virtual void Finalize()=0;

    enum DEVICESTATE
    {
      DEVICESTATE_OK,       //!<  ���Ȃ������Ă���
      DEVICESTATE_NOTRESET, //!<  ���A�ł���悤�ɏ����ł���
      DEVICESTATE_LOST,     //!<  ���X�g��
    };
    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! �h���C�o�̏�Ԃ��擾����
    /*!
     */
    virtual DEVICESTATE TestCooperativeLevel()=0;


    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! �t�����g�o�b�t�@�ɍX�V����
    /*!
      @exception DeviceLost	�f�o�C�X�����X�g�����ꍇ
      @exception Exception	�����^�C���G���[�ɂ������ꍇ
     */
    virtual void Flip()=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! �T�[�t�F�X�o�b�t�@�̍쐬
    /*!
      @param	size	[i ]	�e�N�X�`���̑傫��
      @param	fmt		[i ]	�s�N�Z���t�H�[�}�b�g

      @return	�쐬���ꂽ�T�[�t�F�X�o�b�t�@
     */
    virtual SPSURFACEBUFFER CreateSurface( const SIZE2DI& size, PIXELFORMAT fmt )=0;


    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! �e�N�X�`���o�b�t�@�̍쐬
    /*!
      @param	buffer	[i ]	��������ɍ��e�N�X�`���o�b�t�@
						      �s�N�Z�����e���R�s�[����܂�
						      �~�b�v�}�b�v���ł��邩�͓�

      @return	�쐬���ꂽ�e�N�X�`���o�b�t�@
     */
    virtual SPTEXTUREBUFFER CreateTextureBuffer( const TextureBufferMemory& buffer )=0;


    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! RenderTarget�e�N�X�`���o�b�t�@�̍쐬
    /*!
      @param	size	[i ]	�e�N�X�`���̑傫��
      @param	fmt	[i ]	�s�N�Z���t�H�[�}�b�g

      @return	�쐬���ꂽ�e�N�X�`���o�b�t�@
     */
    virtual SPTEXTUREBUFFER CreateRTTextureBuffer( const SIZE2DI& size, PIXELFORMAT fmt )=0;


    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! �C���f�b�N�X�o�b�t�@�̍쐬
    /*!
      @param	data	[i ]	�C���f�b�N�X�f�[�^
      @param	Format	[i ]	�P�C���f�b�N�X������̃r�b�g��

      @return	�쐬���ꂽ�C���f�b�N�X�o�b�t�@
     */
    virtual SPINDEXBUFFER CreateIndexBuffer( const std::vector<unt08>& data, int Format )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! ���_�o�b�t�@�̍쐬
    /*!
      @param	data	[i ]	���_�f�[�^
      @param	Format	[i ]	���_�t�H�[�}�b�g

      @return	�쐬���ꂽ���_�o�b�t�@
     */
    virtual SPVERTEXBUFFER CreateVertexBuffer( const std::vector<unt08>& data, unt32 Format )=0;
    
    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! �o�[�e�b�N�X�V�F�[�_�[�̍쐬
    /*!
      @param	Code	[i ]	�R�[�h

      @return	�쐬���ꂽ�o�[�e�b�N�X�V�F�[�_�[
     */
    virtual SPVERTEXSHADERBUFFER CreateVertexShader( const std::vector<unt08>& Code )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! �Q�[�����Ǝ��̃s�N�Z���V�F�[�_�[�̍쐬
    /*!
      @param	Code	[i ]	�R�[�h

      @return	�쐬���ꂽ�s�N�Z���V�F�[�_�[
     */
    virtual SPPIXELSHADERBUFFER CreatePixelShader( const std::vector<unt08>& Code )=0;


    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! �h���C�o���ŗp�ӂ���Ă��钸�_��`�̍쐬
    /*!
      @param	Name	[i ]	�f�t�H���g��

      @return	�쐬���ꂽ���_��`
     */
    virtual SPVERTEXDECLARATIONBUFFER CreateVertexDeclarationBuffer( const VertexDeclarationBufferMemory& buffer )=0;


    virtual void SetRenderTarget( const SurfaceBuffer* pBuffer )=0;
    virtual void ResetRenderTarget()=0;

    virtual void SetDepthStencil( const SurfaceBuffer* pBuffer )=0;
    virtual void ResetDepthStencil()=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! �e�N�X�`���o�b�t�@�̊J��
    /*!
      @param	stage	[i ]	�ݒ肷��X�e�[�W
     */
    virtual void UnsetTextureBuffer( int stage )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! �e�N�X�`���o�b�t�@�̃Z�b�g
    /*!
      @param	pBuffer	[i ]	�ݒ肷��o�b�t�@
      @param	stage	[i ]	�ݒ肷��X�e�[�W
     */
    virtual void SetTextureBuffer( const SPTEXTUREBUFFER& pBuffer, int stage )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! �C���f�b�N�X�o�b�t�@�̃Z�b�g
    /*!
      @param	pBuffer	[i ]	�ݒ肷��C���f�b�N�X
     */
    virtual void SetIndexBuffer( const SPINDEXBUFFER& pBuffer )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! ���_�o�b�t�@�̃Z�b�g
    /*!
      @param	pBuffer	[i ]	�ݒ肷��o�b�t�@
     */
    virtual void SetVertexBuffer( const SPVERTEXBUFFER& pBuffer, int pos )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! �o�[�e�b�N�X�V�F�[�_�[�̃Z�b�g
    /*!
      @param	pShader	[i ]	�ݒ肷��V�F�[�_�[
     */
    virtual void SetVertexShader( const SPVERTEXSHADERBUFFER& pShader )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! �s�N�Z���V�F�[�_�[�̃Z�b�g
    /*!
      @param	pShader	[i ]	�ݒ肷��V�F�[�_�[
     */
    virtual void SetPixelShader( const SPPIXELSHADERBUFFER& pShader )=0;


    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! ���_��`�̃Z�b�g
    /*!
      @param	pDecl	[i ]	�ݒ肷���`
     */
    virtual void SetVertexDeclaration( const SPVERTEXDECLARATIONBUFFER& pDecl )=0;

    virtual void SetVertexShaderConstF( int pos, const VECTOR4DF& vec )=0;

    virtual void SetViewPort( const RECT2DI& screen, float MinZ, float MaxZ ) = 0;

    virtual void CopySurface( const SPSURFACEBUFFER& pSrc, SPSURFACEBUFFERMEMORY& pDst )=0;

    virtual String GetVideoInfo()=0;


    enum RENDERSTATE
    {
      RENDERSTATE_CULLINGMODE,
      RENDERSTATE_ZWRITEENABLE,
      RENDERSTATE_ZWRITEFUNC,
      RENDERSTATE_ZTESTENABLE,
    };

    enum CULLINGMODE
    {
      CULLINGMODE_NONE=0,
      CULLINGMODE_LEFT,	//!<	�����v���ɃJ�����O
      CULLINGMODE_RIGHT	//!<	���v���ɃJ�����O
    };

    enum STATEBOOL
    {
      STATE_TRUE,
      STATE_FALSE,
    };

    enum CMPTYPE
    {
      CMPTYPE_NEVER		= 1,	//!<	��ɒʂ�
      CMPTYPE_LESS		= 2,	//!<	o >  n
      CMPTYPE_EQUAL		= 3,	//!<	o == n
      CMPTYPE_LESSEQUAL	= 4,	//!<	o >= n
      CMPTYPE_GREATER		= 5,	//!<	o <  n
      CMPTYPE_NOTEQUAL	= 6,	//!<	o != n
      CMPTYPE_GREATEREQUAL= 7,	//!<	o <= n
      CMPTYPE_ALWAYS		= 8,	//!<	��Ƀ_��
    };

    virtual void SetRenderState( RENDERSTATE state, unt value )=0;


    enum SAMPLERSTATE
    {
      SAMPLERSTATE_MAGFILTER,
      SAMPLERSTATE_MINFILTER,
      SAMPLERSTATE_TADDRESSU,
      SAMPLERSTATE_TADDRESSV,
    };

    enum FILTERTYPE
    {
      FILTERTYPE_POINT,
      FILTERTYPE_LINEAR,
      FILTERTYPE_ANISOTROPIC,
    };

    enum TEXTUREADDRESS
    {
      TEXTUREADDRESS_WRAP,
      TEXTUREADDRESS_MIRROR,
      TEXTUREADDRESS_CLAMP,
    };


    virtual void SetSamplerState( int Stage, SAMPLERSTATE state, unt value )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! �����_�����O�̊J�n
    /*!
     */
    virtual void BeginScene()=0;
    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! �����_�����O�̏I��
    /*!
     */
    virtual void EndScene()=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! ���݂� RenderTarget ��h��Ԃ�
    /*!
      @param	Color	[i ]	�h��Ԃ��F
     */
    virtual void ClearColor( const COLOR_A32B32G32R32F& Color )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! ���݂� Z�o�b�t�@ ��h��Ԃ�
    /*!
      @param	val	[i ]	�h��Ԃ��l
     */
    virtual void ClearZ( float val )=0;

    enum PRIMITIVE
    {
      PRIMITIVE_POINTLIST=0,
      PRIMITIVE_LINELIST,
      PRIMITIVE_LINESTRIP,
      PRIMITIVE_TRIANGLELIST,
      PRIMITIVE_TRIANGLESTRIP,
      PRIMITIVE_TRIANGLEFAN,
    };

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! �|���S���̕`��
    /*!
      @param	prim			[i ]	�`��̌`
      @param	StartVertex		[i ]	�`����J�n���钸�_�ԍ�
      @param	PrimitiveCount	[i ]	�`�悷�鐔
     */
    virtual void DrawPrimitive( PRIMITIVE prim, unt StartVertex, unt PrimitiveCount )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! �|���S���̕`��
    /*!
      @param	prim			[i ]	�`��̌`
      @param	pVertex			[i ]	�`����J�n���钸�_
      @param	Stride			[i ]	�P���_������̑傫��
      @param	PrimitiveCount	[i ]	�`�悷�鐔
     */
    virtual void DrawPrimitiveUP( PRIMITIVE prim, const void* pVertex, unt Stride, unt PrimitiveCount )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! �|���S���̕`��
    /*!
      @param	prim			[i ]	�`��̌`
      @param	BaseVertexOffset[i ]	VertexBuffer��� MinIndex �̃I�t�Z�b�g
      @param	MinIndex		[i ]	�C���f�b�N�X�̍ŏ��̒l
      @param	NumVertices		[i ]	�Ăяo���Ŏg�p����钸�_�̐�
      @param	StartIndex		[i ]	���ڂ̃C���f�b�N�X����`����J�n���邩�H
      @param	PrimitiveCount	[i ]	�`�悷�鐔
     */
    virtual void DrawIndexedPrimitive( PRIMITIVE prim, int BaseVertexOffset, unt MinIndex, unt NumVertices, unt StartIndex, unt PrimitiveCount )=0;


    virtual void DrawIndexPrimitiveUP( PRIMITIVE prim, int MinIndex, int NumVertices, const void* pVertex, int VertexSize, int PrimitiveCount, const void* pIndex, int IndexSize  )=0;

  private:

  };

  typedef	boost::shared_ptr<VideoCard>	SPVIDEOCARD;

}


#endif