
#include "Shader_Defines.hpp"


struct BONEMATRICES
{
	matrix		Matrices[128];
};

cbuffer BoneMatrixDesc
{
	BONEMATRICES		g_BoneMatrices;
};
//
//cbuffer LightDesc
//{
//	float3		vLightDirection;
//	float3		vLightPosition; /* 점광원의 위치. */
//	float		fRange;			/* 광원의 범위다. */
//	float4		vLightDiffuse;
//	float4		vLightAmbient;
//	float4		vLightSpecular;
//};
//
//cbuffer MtrlDesc
//{
//	float4		vMtrlDiffuse = vector(1.f, 1.f, 1.f, 1.f);
//	float4		vMtrlAmbient = vector(1.f, 1.f, 1.f, 1.f);
//	float4		vMtrlSpecular = vector(1.f, 1.f, 1.f, 1.f);
//};

texture2D		g_DissolveTexture;

texture2D		g_MaskingTexture;
texture2D		g_DiffuseTexture;
texture2D		g_AmbientTexture;
texture2D		g_SpecularTexture;
float3			g_RGBColor;
matrix			g_PivotMatrix;
float4			g_vColor;
float			g_fTime;

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
	float3 vTangent : TANGENT;
	float2 vTexUV : TEXCOORD0;
	uint4  vBlendIndex : BLENDINDEX;
	float4 vBlendWeight : BLENDWEIGHT;
};

struct VS_OUT
{
	float4 vPosition : SV_POSITION;
	float4 vNormal : NORMAL;
	float2 vTexUV : TEXCOORD0;
	float4 vProjPosition : TEXCOORD1;
};


/* 그리려하는 정점 하나에 대한 Shading을 한다. */
/* Shading : 정점의 스페이스 변환. */
/* 정점의 멤버를 필요에따라 변형한다. */
// 
// VS_OUT VS_MAIN(float3 vPosition : POSITION, float2 vTexUV : TEXCOORD0)
VS_OUT VS_MAIN_DIRECTIONAL(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(WorldMatrix, ViewMatrix);
	matWVP = mul(matWV, ProjMatrix);

	matrix		BoneMatrix = g_BoneMatrices.Matrices[In.vBlendIndex.x] * In.vBlendWeight.x +
		g_BoneMatrices.Matrices[In.vBlendIndex.y] * In.vBlendWeight.y +
		g_BoneMatrices.Matrices[In.vBlendIndex.z] * In.vBlendWeight.z +
		g_BoneMatrices.Matrices[In.vBlendIndex.w] * In.vBlendWeight.w;

	BoneMatrix = mul(BoneMatrix, g_PivotMatrix);

	vector		vPosition = mul(vector(In.vPosition, 1.f), BoneMatrix);
	vector		vNormal = mul(vector(In.vNormal, 0.f), BoneMatrix);

	Out.vPosition = mul(vPosition, matWVP);
	Out.vNormal = normalize(mul(vNormal, WorldMatrix));
	Out.vTexUV = In.vTexUV;
	Out.vProjPosition = Out.vPosition;

	return Out;
}

VS_OUT VS_MAIN_DIRECTIONAL_TERRAIN(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(WorldMatrix, ViewMatrix);
	matWVP = mul(matWV, ProjMatrix);

	//matrix		BoneMatrix = g_BoneMatrices.Matrices[In.vBlendIndex.x] * In.vBlendWeight.x +
	//	g_BoneMatrices.Matrices[In.vBlendIndex.y] * In.vBlendWeight.y +
	//	g_BoneMatrices.Matrices[In.vBlendIndex.z] * In.vBlendWeight.z +
	//	g_BoneMatrices.Matrices[In.vBlendIndex.w] * In.vBlendWeight.w;

	//BoneMatrix = mul(BoneMatrix, g_PivotMatrix);

	vector		vPosition = mul(vector(In.vPosition, 1.f), g_PivotMatrix);
	vector		vNormal = mul(vector(In.vNormal, 0.f), g_PivotMatrix);

	Out.vPosition = mul(vPosition, matWVP);
	Out.vNormal = normalize(mul(vNormal, WorldMatrix));
	Out.vTexUV = In.vTexUV;
	Out.vProjPosition = Out.vPosition;

	return Out;
}

VS_OUT VS_MAIN_EFFECT_NOLIGHT(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(WorldMatrix, ViewMatrix);
	matWVP = mul(matWV, ProjMatrix);
	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);

	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN
{
	float4 vPosition : SV_POSITION;
	float4 vNormal : NORMAL;
	float2 vTexUV : TEXCOORD0;
	float4 vProjPosition : TEXCOORD1;
};

struct PS_OUT
{
	// 몇번째 렌더 타겟에 출력을 할래?
	vector	vDiffuse : SV_TARGET0;
	vector	vNormal : SV_TARGET1;
	vector	vDepth : SV_TARGET2;
};

/* 픽셀의 최종적인 색을 결정하낟. */
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vDiffuse = g_DiffuseTexture.Sample(DiffuseSampler, In.vTexUV);

	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPosition.w / 430.0f, In.vProjPosition.z / In.vProjPosition.w, 0.f, 0.f);

	return Out;	return Out;
}

PS_OUT PS_MAIN_COLOR(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vDiffuse = g_DiffuseTexture.Sample(DiffuseSampler, In.vTexUV);
	Out.vDiffuse.r *= g_vColor.r;
	Out.vDiffuse.g *= g_vColor.g;
	Out.vDiffuse.b *= g_vColor.b;
	Out.vDiffuse.a *= g_vColor.a;

	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPosition.w / 430.0f, In.vProjPosition.z / In.vProjPosition.w, 0.f, 0.f);

	return Out;	return Out;
}

PS_OUT PS_MAIN_MASKING_COLOR(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vDiffuse = g_DiffuseTexture.Sample(DiffuseSampler, In.vTexUV);
	vector		vMtrlDiffuse_Double = g_MaskingTexture.Sample(DiffuseSampler, In.vTexUV);

	if (0 != vMtrlDiffuse_Double.r)
	{
		Out.vDiffuse.r *= g_vColor.r;
		Out.vDiffuse.g *= g_vColor.g;
		Out.vDiffuse.b *= g_vColor.b;
		Out.vDiffuse.a *= g_vColor.a;
	}

	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPosition.w / 430.0f, In.vProjPosition.z / In.vProjPosition.w, 0.f, 0.f);



	return Out;
}

PS_OUT PS_ONLY_RED(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vColor = g_DiffuseTexture.Sample(DiffuseSampler, In.vTexUV);

	if (0.f < vColor.r)
		Out.vDiffuse.a = vColor.g;

	Out.vDiffuse.r = 0.f;
	Out.vDiffuse.g = 0.75f;
	Out.vDiffuse.b = 1.f;


	return Out;
}

PS_OUT PS_ONLY_GREEN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vColor = g_DiffuseTexture.Sample(DiffuseSampler, In.vTexUV);

	if (0.f < vColor.g)
		Out.vDiffuse.a = vColor.g;

	Out.vDiffuse.r = 0.f;
	Out.vDiffuse.g = 0.75f;
	Out.vDiffuse.b = 1.f;

	return Out;
}

PS_OUT PS_ONLY_BLUE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vColor = g_DiffuseTexture.Sample(DiffuseSampler, In.vTexUV);

	if (0.f < vColor.b)
		Out.vDiffuse.a = vColor.g;

	Out.vDiffuse.r = 0.f;
	Out.vDiffuse.g = 0.75f;
	Out.vDiffuse.b = 1.f;


	return Out;
}

PS_OUT PS_ALPHA_COLOR(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vColor = g_DiffuseTexture.Sample(DiffuseSampler, In.vTexUV);

	vColor.r *= g_vColor.r;
	vColor.g *= g_vColor.g;
	vColor.b *= g_vColor.b;
	vColor.a *= g_vColor.a;

	Out.vDiffuse = vColor;
	Out.vNormal = vector(1.f, 1.f, 1.f, 0.f);//vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPosition.w / 430.0f, In.vProjPosition.z / In.vProjPosition.w, 0.f, 0.f);

	return Out;
}

PS_OUT PS_ALPHA_BLUE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vDiffuse = g_DiffuseTexture.Sample(DiffuseSampler, In.vTexUV);


	if (0.05f >= Out.vDiffuse.b)
	{
		Out.vDiffuse.a = 0.f;
		return Out;
	}

	Out.vDiffuse.a = g_vColor.a;

	Out.vDiffuse.r *= g_vColor.r;
	Out.vDiffuse.g *= g_vColor.g;
	Out.vDiffuse.b *= g_vColor.b;


	return Out;
}

PS_OUT PS_ALPHA_BLUE_2(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vDiffuse = g_DiffuseTexture.Sample(DiffuseSampler, In.vTexUV);


	if (0.05f >= Out.vDiffuse.b)
	{
		Out.vDiffuse.rga = 0.f;
		return Out;
	}
	Out.vDiffuse.rga = 1.f;
	Out.vDiffuse.a = g_vColor.a;

	Out.vDiffuse.r *= g_vColor.r;
	Out.vDiffuse.g *= g_vColor.g;
	Out.vDiffuse.b *= g_vColor.b;


	return Out;
}

PS_OUT PS_ALPHA_RED(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vDiffuse = g_DiffuseTexture.Sample(DiffuseSampler, In.vTexUV);

	if (0.01f >= Out.vDiffuse.r)
		Out.vDiffuse.a = 0.f;

	return Out;
}

PS_OUT PS_ALPHA_GREEN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vDiffuse = g_DiffuseTexture.Sample(DiffuseSampler, In.vTexUV);

	if (0.05f >= Out.vDiffuse.g)
		Out.vDiffuse.a = 0.f;

	return Out;
}

PS_OUT PS_DISSOLVE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float4 vDissolve = g_DissolveTexture.Sample(DiffuseSampler, In.vTexUV);

	Out.vDiffuse = g_DiffuseTexture.Sample(DiffuseSampler, In.vTexUV);

	// 0.7 >= 0 (최초 시작)
	if (vDissolve.x >= g_fTime)
	{
		Out.vDiffuse.a = 1.f;
		// 이건 그린다.
		float fDis = abs(vDissolve.x - g_fTime);

		if (fDis <= 0.08f)
		{
			//0.294117659f, 0.000000000f, 0.509803951f, 1.000000000f
			Out.vDiffuse.r = 1.f;
			Out.vDiffuse.g = 0.8f;
			Out.vDiffuse.b = 0.6f;
			Out.vDiffuse.a = 1.f;

		}

	}
	else
		discard;


	return Out;
}

technique11		DefaultTechnique
{
	pass BoneLight__Directional // 0
	{
		SetRasterizerState(Rasterizer_Solid);
		SetDepthStencilState(DepthStecil_Default, 0);
		SetBlendState(BlendState_None, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN_DIRECTIONAL();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Light_Directional__Terrain // 1
	{
		SetRasterizerState(Rasterizer_Solid);
		SetDepthStencilState(DepthStecil_Default, 0);
		SetBlendState(BlendState_None, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN_DIRECTIONAL_TERRAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Effect_NoLight__OnlyRed // 2
	{
		SetRasterizerState(Rasterizer_Solid_NotCurl);
		SetDepthStencilState(DepthStecil_Default, 0);
		SetBlendState(BlendState_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN_EFFECT_NOLIGHT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_ONLY_RED();
	}

	pass Effect_NoLight__OnlyGreen // 3
	{
		SetRasterizerState(Rasterizer_Solid_NotCurl);
		SetDepthStencilState(DepthStecil_Default, 0);
		SetBlendState(BlendState_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN_EFFECT_NOLIGHT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_ONLY_GREEN();
	}

	pass Effect_NoLight__OnlyBlue // 4
	{
		SetRasterizerState(Rasterizer_Solid_NotCurl);
		SetDepthStencilState(DepthStecil_Default, 0);
		SetBlendState(BlendState_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN_EFFECT_NOLIGHT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_ONLY_BLUE();
	}

	pass Light_Directional_Color // 5
	{
		SetRasterizerState(Rasterizer_Solid);
		SetDepthStencilState(DepthStecil_Default, 0);
		SetBlendState(BlendState_None, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN_DIRECTIONAL_TERRAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_COLOR();
	}

	pass Light_Directional__Masking_Color // 6
	{
		SetRasterizerState(Rasterizer_Solid);
		SetDepthStencilState(DepthStecil_Default, 0);
		SetBlendState(BlendState_None, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN_DIRECTIONAL();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_MASKING_COLOR();
	}

	pass Light_Directional__AlphaColor // 7
	{
		SetRasterizerState(Rasterizer_Solid);
		SetDepthStencilState(DepthStecil_Default, 0);
		SetBlendState(BlendState_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN_DIRECTIONAL();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_ALPHA_COLOR();
	}

	pass Light_Directional__AlphaBlue // 8
	{
		SetRasterizerState(Rasterizer_Solid_NotCurl);
		SetDepthStencilState(DepthStecil_NotZWrite, 0);
		SetBlendState(BlendState_Add, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN_DIRECTIONAL_TERRAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_ALPHA_BLUE();
	}

	pass Light_Directional__AlphaRed // 9
	{
		SetRasterizerState(Rasterizer_Solid_NotCurl);
		SetDepthStencilState(DepthStecil_NotZWrite, 0);
		SetBlendState(BlendState_Add, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN_DIRECTIONAL_TERRAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_ALPHA_RED();
	}

	pass Light_Directional__AlphaGreen // 10
	{
		SetRasterizerState(Rasterizer_Solid_NotCurl);
		SetDepthStencilState(DepthStecil_NotZWrite, 0);
		SetBlendState(BlendState_Add, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN_DIRECTIONAL_TERRAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_ALPHA_GREEN();
	}

	pass Light_Directional__AlphaBlue2 // 11
	{
		SetRasterizerState(Rasterizer_Solid_NotCurl);
		SetDepthStencilState(DepthStecil_NotZWrite, 0);
		SetBlendState(BlendState_Add, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN_DIRECTIONAL_TERRAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_ALPHA_BLUE_2();
	}

	pass Light_Directional__Dissolve // 12
	{
		SetRasterizerState(Rasterizer_Solid);
		SetDepthStencilState(DepthStecil_Default, 0);
		SetBlendState(BlendState_None, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN_DIRECTIONAL();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_DISSOLVE();
	}
};
//g_DissolveTexture