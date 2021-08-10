#ifndef Engine_Struct_h__
#define Engine_Struct_h__

namespace Engine
{
	/* vPosition + UVmap */
	typedef struct tagVertexTexture
	{
		XMFLOAT3			vPosition;
		XMFLOAT2			vTexUV;
	}VTXTEX;

	typedef struct tagPolygonIndices16
	{
		unsigned short		_0, _1, _2;
	}POLYGONINDICES16;

	typedef struct tagPolygonIndices32
	{
		unsigned long		_0, _1, _2;
	}POLYGONINDICES32;



}

#endif 