
#include "Shader_Defines.hpp"

texture2D		g_DiffuseTexture;

sampler DiffuseSampler = sampler_state
{
	AddressU = wrap;
	AddressV = wrap;

	/*Filter = MIN_MAG_MIP_LINEAR;
	*/
};

struct VS_IN
{
	float3 vPosition : POSITION;
	float3 vNormal : NORMAL;
	float2 vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4 vPosition : SV_POSITION;
	float2 vTexUV : TEXCOORD0;
};


/* 그리려하는 정점 하나에 대한 Shading을 한다. */
/* Shading : 정점의 스페이스 변환. */
/* 정점의 멤버를 필요에따라 변형한다. */
// 
// VS_OUT VS_MAIN(float3 vPosition : POSITION, float2 vTexUV : TEXCOORD0)
VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(WorldMatrix, ViewMatrix);
	matWVP = mul(matWV, ProjMatrix);

	// 이미지는 왼쪽에서 읽는건 동일하지만
	// 위에서부터 읽고
	// 아래에서 그리니
	// 위아래가 반전된다

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV * 10.f;
	Out.vTexUV.y *= -1.f;

	return Out;
}



struct PS_IN
{
	float4 vPosition : SV_POSITION;
	float2 vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector	vColor : SV_TARGET;
};

/* 픽셀의 최종적인 색을 결정하낟. */
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	//In.vTexUV.y = 1 - In.vTexUV.y;
	// 이미지는 왼쪽 상단부터 읽었지만 UV좌표는 왼쪽 하단부터 세팅이 되버리기 때문에 자연스럽게 이미지가 뒤집힌다
	Out.vColor = g_DiffuseTexture.Sample(DiffuseSampler, In.vTexUV);
	Out.vColor.rgb *= 2.f;
	return Out;
}

technique11		DefaultTechnique
{
	pass Default
	{
		SetRasterizerState(Rasterizer_Solid);
		SetDepthStencilState(DepthStecil_Default, 0);
		SetBlendState(BlendState_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		PixelShader = compile ps_5_0 PS_MAIN();
	}
};



