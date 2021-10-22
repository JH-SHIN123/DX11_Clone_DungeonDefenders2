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

#pragma region Mesh
	typedef struct tagMeshTexture
	{
		class CTextures*			pMaterialTexture[AI_TEXTURE_TYPE_MAX];
	}MESHTEXTURE;

	typedef struct tagBoneDesc
	{
		class CHierarchyNode*		pHierarchyNode;
		XMFLOAT4X4					OffsetMatrix;
	}BONEDESC;

	typedef struct tagVertexMesh
	{
		XMFLOAT3			vPosition;
		XMFLOAT3			vNormal;
		XMFLOAT3			vTangent;
		XMFLOAT2			vTexUV;
		XMUINT4				vBlendIndex;
		XMFLOAT4			vBlendWeight;
	}VTXMESH;

	typedef struct tagKeyFrame
	{
		XMFLOAT3		vScale;
		XMFLOAT4		vRotation;
		XMFLOAT3		vPosition;
		double			Time;		/* 동작이 표현되야할 시간. */
	}KEYFRAME;

#pragma endregion

#pragma region VIBuffer
	typedef struct tagVertexPoint
	{
		// For. 지오메트리 쉐이딩
		XMFLOAT3			vPosition;
	}VTXPOINT;

	typedef struct tagVertexMatrix
	{
		// 행렬의 모습을 띄고있다. For. 인스턴싱
		XMFLOAT2			vSize;
		XMFLOAT4			vRight;
		XMFLOAT4			vUp;
		XMFLOAT4			vLook;
		XMFLOAT4			vPosition;
	}VTXMATRIX;

	typedef struct tagVertexMatrix_Extend
	{
		// 행렬의 모습을 띄고있다. For. 인스턴싱
		XMFLOAT2			vSize;
		XMFLOAT4			vRight;
		XMFLOAT4			vUp;
		XMFLOAT4			vLook;
		XMFLOAT4			vPosition;
		float				fTime;
	}VTXMATRIX_EXTEND;

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

	/* vPosition + UVmap */
	typedef struct tagVertexTexture
	{
		XMFLOAT3			vPosition;
		XMFLOAT2			vTexUV;
	}VTXTEX;

	typedef struct tagVertexColor
	{
		XMFLOAT3			vPosition;
		XMFLOAT4			vColor;
	}VTXCOL;

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

#pragma region Shader
	/* For.Shader Layout */
	typedef struct tagInputLayoutDesc
	{
		ID3D11InputLayout*			pLayout = nullptr;
		ID3DX11EffectPass*			pPass = nullptr;
	} INPUTLAYOUTDESC;

#pragma  endregion
}

#endif 