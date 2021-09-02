
#include "Shader_Defines.hpp"

cbuffer LightDesc
{
	float3		vLightDirection;
	float4		vLightDiffuse;
	float4		vLightAmbient;
	float4		vLightSpecular;
};

cbuffer MtrlDesc
{
	float4		vMtrlDiffuse = vector(1.f, 1.f, 1.f, 1.f);
	float4		vMtrlAmbient = vector(1.f, 1.f, 1.f, 1.f);
	float4		vMtrlSpecular = vector(1.f, 1.f, 1.f, 1.f);
};

texture2D		g_DiffuseTexture;

sampler DiffuseSampler = sampler_state
{
	Filter = MIN_MAG_MIP_LINEAR;

	AddressU = wrap;
	AddressV = wrap;
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
	float4 vShade : COLOR0;
	float4 vSpecular : COLOR1;
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

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV * 30.f;

	vector		vWorldPosition = mul(vector(In.vPosition, 1.f), WorldMatrix);
	vector		vWorldNormal = normalize(mul(vector(In.vNormal, 0.f), WorldMatrix));

	Out.vShade = max(dot(normalize(vector(vLightDirection, 0.f)) * -1.f, vWorldNormal), 0.f);
	Out.vShade.a = 1.f;

	vector		vReflect = reflect(normalize(vector(vLightDirection, 0.f)), normalize(vWorldNormal));
	vector		vLook = vWorldPosition - vCameraPosition;

	Out.vSpecular = pow(max(dot(normalize(vLook) * -1.f, normalize(vReflect)), 0.f), 30.f);

	return Out;
}



struct PS_IN
{
	float4 vPosition : SV_POSITION;
	float4 vShade : COLOR0;
	float4 vSpecular : COLOR1;
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

	vector		vMtrlDiffuse = g_DiffuseTexture.Sample(DiffuseSampler, In.vTexUV);

	Out.vColor = (vLightDiffuse * vMtrlDiffuse) * (In.vShade + (vLightAmbient * vMtrlAmbient))
		+ (vLightSpecular * vMtrlSpecular) * In.vSpecular;




	return Out;
}

technique11		DefaultTechnique
{
	pass Default
	{
		SetRasterizerState(Rasterizer_Solid);
		SetDepthStencilState(DepthStecil_Default, 0);
		SetBlendState(BlendState_None, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		PixelShader = compile ps_5_0 PS_MAIN();
	}
};



