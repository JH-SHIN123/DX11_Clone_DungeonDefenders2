
/*
앵간한 시멘틱스
typedef enum _D3DDECLUSAGE
{
D3DDECLUSAGE_POSITION = 0,
D3DDECLUSAGE_BLENDWEIGHT,   // 1
D3DDECLUSAGE_BLENDINDICES,  // 2
D3DDECLUSAGE_NORMAL,        // 3
D3DDECLUSAGE_PSIZE,         // 4
D3DDECLUSAGE_TEXCOORD,      // 5
D3DDECLUSAGE_TANGENT,       // 6
D3DDECLUSAGE_BINORMAL,      // 7
D3DDECLUSAGE_TESSFACTOR,    // 8
D3DDECLUSAGE_POSITIONT,     // 9
D3DDECLUSAGE_COLOR,         // 10
D3DDECLUSAGE_FOG,           // 11
D3DDECLUSAGE_DEPTH,         // 12
D3DDECLUSAGE_SAMPLE,        // 13
} D3DDECLUSAGE;
*/


cbuffer Matrices
{
	matrix			WorldMatrix;
	matrix			ViewMatrix;
	matrix			ProjMatrix;
}

// 정점 하나의 정보
struct VS_IN
{
	float3			vPosition	: POSITION;		// 위치
	float3			vNormal		: NORMAL;		// 법선 벡터
	float2			vTexUV		: TEXCOORD0;	// 텍스처UV
};

// 온갖 연산을 통해서 투영이 될 정점 하나의 정보
struct VS_OUT	
{
	float4			vPosition	: SV_POSITION;	// Z나누기를 하기 이전의 정점 위치
	float3			vNormal		: NORMAL;		// 정점의 법선 벡터
	float2			vTexUV		: TEXCOORD0;	// 정점의 텍스처 UV
	float4			vWorldPos	: TEXCOORD1;	// 픽셀의 위치정보
};


/* 그리려하는 정점 하나에 대한 Shading을 한다. */
/* Shading : 정점의 스페이스 변환. */
/* 정점의 멤버를 필요에따라 변형한다. */
VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix		matWV = mul(WorldMatrix, ViewMatrix);
	matrix		matWVP = mul(matWV, ProjMatrix);
	Out.vPosition = mul((float4)(In.vPosition, 1.f), matWVP); 
	Out.vTexUV = In.vTexUV;		

	return Out;
}

struct PS_IN
{
	// 픽셀의 정보의 데이터 타입들은 정점의 정보의 데이터 타입과 같다.
	// 절대 값이 같은게 아님!
	float4			vPosition	: SV_POSITION;
	float3			vNormal		: NORMAL;
	float2			vTexUV		: TEXCOORD0;
	float4			vWorldPos	: TEXCOORD1;
};

vector PS_MAIN() : SV_TARGET
{
	vector			vTmp = (vector)0;

	return vTmp;
}

technique11		Default
{
	pass Default
	{
		VertexShader = compile vs_5_0 VS_MAIN();
		PixelShader = compile ps_5_0 PS_MAIN();
	}
};



