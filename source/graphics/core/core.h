/*!	
    @file
    @brief	ゲームフレームワークが持つ描画コア
*/

#ifndef maid2_graphics_core_core_h
#define maid2_graphics_core_core_h

#include"../../config/define.h"
#include"../../auxiliary/globalpointer.h"

#include"../pixelformat.h"


#include"idevice.h"


namespace Maid { namespace Graphics {

  class Core : public GlobalPointer<Core>
	{
	public:
		Core();
		virtual ~Core();

		void Initialize( const SPDEVICE& pDevice );
		void Finalize();

    const SPDEVICE& GetDevice();

    PIXELFORMAT GetUseFormat( PIXELFORMAT& fmt )const;

	private:
    void UpdateMember();


	private:
    SPDEVICE	          m_pDevice;
	};
}}


#endif
