/*!	
    @file
    @brief	ゲームフレームワークが持つ描画コア
*/

#ifndef maid2_graphics_graphicscore_h
#define maid2_graphics_graphicscore_h

#include"../../config/define.h"
#include"../../auxiliary/globalpointer.h"

#include"../pixelformat.h"


#include"idevice.h"


namespace Maid {

  class GraphicsCore : public GlobalPointer<GraphicsCore>
	{
	public:
		GraphicsCore();
		virtual ~GraphicsCore();

		void Initialize( const Graphics::SPDEVICE& pDevice );
		void Finalize();

    const Graphics::SPDEVICE& GetDevice();

    PIXELFORMAT GetUseFormat( PIXELFORMAT& fmt )const;

	private:
    void UpdateMember();


	private:
    Graphics::SPDEVICE	          m_pDevice;
	};
}


#endif
