
#include "Shader_Defines.hpp"
// 헤더 인클루드를 한 모습 확장자는 hpp



struct VS_IN
{
	float3 vPosition : POSITION;
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

	//Out.vPosition = mul(vector(In.vPosition, 1.f), WorldMatrix);

	vector		vPosition = vector(In.vPosition * 2.f, 1.f);

	Out.vPosition = vPosition;
	Out.vTexUV = In.vTexUV;

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

	Out.vColor = vector(1.f, 0.f, 0.f, In.vTexUV.x);

	if (Out.vColor.a < 0.3f)
		discard;

	return Out;
}

technique11		DefaultTechnique
{
	pass Default
	{
		// DX9 때 했었던 것 처럼 RenderState세팅을 쉐이더 내부에서 설정 할 수 있다.
		SetRasterizerState(Rasterizer_Solid);
		SetDepthStencilState(DepthStecil_Default, 0);
		SetBlendState(BlendState_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		PixelShader = compile ps_5_0 PS_MAIN();
	}
};



