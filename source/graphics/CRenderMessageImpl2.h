#ifndef CRenderMessageImpl2_h
#define CRenderMessageImpl2_h

#include"../../Setup/CompileMode.h"
#include"CRenderMessage.h"

#include"Color.h"

#include<vector>

//	このヘッダには２Ｄ的なメッセージを定義する


namespace Maid
{
	namespace RenderMessage
	{
		class DrawPrimitive : public Base
		{
		public:
			DrawPrimitive() : Base(DRAWPRIMITIVE){}

			enum TYPE
			{
				TYPE_POINTLIST=0,
				TYPE_LINELIST,
				TYPE_LINESTRIP,
				TYPE_TRIANGLELIST,
				TYPE_TRIANGLESTRIP,
				TYPE_TRIANGLEFAN,
			};

			TYPE	type;
			unt		StartVertex;
			unt		PrimitiveCount;
		};

		class DrawPrimitiveUP : public Base
		{
		public:
			DrawPrimitiveUP() : Base(DRAWPRIMITIVEUP){}

			enum TYPE
			{
				TYPE_POINTLIST=0,
				TYPE_LINELIST,
				TYPE_LINESTRIP,
				TYPE_TRIANGLELIST,
				TYPE_TRIANGLESTRIP,
				TYPE_TRIANGLEFAN,
			};

			TYPE	type;
			MySTL::vector<unt08>	Vertex;
			unt		Stride;
			unt		PrimitiveCount;
		};

		class DrawIndexedPrimitive : public Base
		{
		public:
			DrawIndexedPrimitive() : Base(DRAWINDEXPRIMITIVE){}

			enum TYPE
			{
				TYPE_POINTLIST=0,
				TYPE_LINELIST,
				TYPE_LINESTRIP,
				TYPE_TRIANGLELIST,
				TYPE_TRIANGLESTRIP,
				TYPE_TRIANGLEFAN,
			};

			TYPE	type;
			int		VertexOffset;
			unt		MinIndex;
			unt		NumVertex;
			unt		StartIndex;
			unt		PrimitiveCount;
		};

		class DrawIndexedPrimitiveUP : public Base
		{
		public:
			DrawIndexedPrimitiveUP() : Base(DRAWINDEXPRIMITIVEUP){}

			enum TYPE
			{
				TYPE_POINTLIST=0,
				TYPE_LINELIST,
				TYPE_LINESTRIP,
				TYPE_TRIANGLELIST,
				TYPE_TRIANGLESTRIP,
				TYPE_TRIANGLEFAN,
			};

			TYPE type;
			int MinIndex;
			int NumVertices;
			MySTL::vector<unt08>	Vertex;
			int VertexSize;
			int PrimitiveCount;
			MySTL::vector<unt08>	Index;
			int IndexSize;
		};
	}

}


#endif
