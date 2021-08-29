#ifndef __Engine_Struct_h__
#define __Engine_Struct_h__

namespace Engine
{
	typedef struct tagVertexNormalTexture
	{
		XMFLOAT3			vPosition;
		XMFLOAT3			vNormal;
		XMFLOAT2			vTexUV;
	}VTXNORTEX;

	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	/* For.Shader Layout */
	typedef struct tagInputLayoutDesc
	{
		ID3D11InputLayout*			pLayout = nullptr;
		ID3DX11EffectPass*			pPass = nullptr;
	} INPUTLAYOUTDESC;


	/* vPosition + UVmap */
	typedef struct tagVertexTexture
	{
		XMFLOAT3			vPosition;
		XMFLOAT2			vTexUV;
	}VTXTEX;

	/* vPosition + UVmap */
	typedef struct tagVertexTexture_Cube
	{
		XMFLOAT3			vPosition;
		XMFLOAT3			vTexUV;
	}VTXTEX_CUBE;

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