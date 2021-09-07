#ifndef __Engine_Struct_h__
#define __Engine_Struct_h__

namespace Engine
{
#pragma region Light
	typedef struct tagLightDesc
	{
		// 방향성 광원
		XMFLOAT3		vDirection;

		// 점광원
		XMFLOAT3		vPosition;
		float			fRadius;
		float			fAtt;		//감쇠값

		// 스포트라이트 광원

		// 필수 색상
		XMFLOAT4		vDiffuse;
		XMFLOAT4		vAmbient;
		XMFLOAT4		vSpecular;

		// 추가 옵션
		bool		isActive;
	} LIGHT_DESC;
#pragma endregion

#pragma region VIBuffer
	typedef struct tagInputLayoutDesc
	{
		ID3D11InputLayout*			pLayout = nullptr;
		ID3DX11EffectPass*			pPass = nullptr;
	} INPUTLAYOUTDESC;

	typedef struct tagMeshTexture
	{
		class CTextures*			pMaterialTexture[AI_TEXTURE_TYPE_MAX];
	}MESHTEXTURE;

	typedef struct tagVertexMesh
	{
		XMFLOAT3			vPosition;
		XMFLOAT3			vNormal;
		XMFLOAT3			vTangent;
		XMFLOAT2			vTexUV;
		XMUINT4				vBlendIndex;
		XMFLOAT4			vBlendWeight;
	}VTXMESH;

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
#pragma endregion

}

#endif 