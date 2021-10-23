
#include "Shader_Defines.hpp"

cbuffer CameraDesc
{
	vector		g_vCamPosition;
}

texture2D		g_DiffuseTexture;
float2			g_vSize;
float4			g_vColor;
float			g_fTime;

sampler DiffuseSampler = sampler_state
{
	AddressU = wrap;
	AddressV = wrap;

	/*Filter = MIN_MAG_MIP_LINEAR;
	*/
};

struct VS_IN
{
	/* 본래 정점버퍼 */
	float3 vPosition : POSITION;

	/* 인스턴스 정점버퍼 */
	float2 vInstanceSize : PSIZE;
	row_major matrix WorldMatrix : WORLD;
};

struct VS_OUT
{
	float4 vPosition : POSITION;
	float2 vSize : PSIZE;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	Out.vPosition = mul(vector(In.vPosition, 1.f), In.WorldMatrix);
	Out.vSize = In.vInstanceSize;

	return Out;
}

struct GS_OUT
{
	float4 vPosition : SV_POSITION;
	float2 vTexUV : TEXCOORD0;
};

[maxvertexcount(6)]
void GS_MAIN(/*입력*/ point VS_OUT In[1], /*출력*/ inout TriangleStream<GS_OUT> TriStream)
{
	GS_OUT		Out[6];

	float3		vLook = normalize(g_vCamPosition - In[0].vPosition).xyz;
	float3		vAxisY = vector(0.f, 1.f, 0.f, 0.f).xyz;
	float3		vRight = normalize(cross(vAxisY, vLook));
	float3		vUp = normalize(cross(vLook, vRight));

	float2		vHalfSize = float2(In[0].vSize.x * 0.5f, In[0].vSize.y * 0.5f);

	matrix		matVP;
	matVP = mul(ViewMatrix, ProjMatrix);


	/* 좌상 */
	Out[0].vPosition = In[0].vPosition + vector(vRight, 0.f) * vHalfSize.x + vector(vUp, 0.f) * vHalfSize.y;
	Out[0].vPosition = mul(Out[0].vPosition, matVP);
	Out[0].vTexUV = float2(0.f, 0.f);
	TriStream.Append(Out[0]);


	/* 우상 */
	Out[1].vPosition = In[0].vPosition - vector(vRight, 0.f) * vHalfSize.x + vector(vUp, 0.f) * vHalfSize.y;
	Out[1].vPosition = mul(Out[1].vPosition, matVP);
	Out[1].vTexUV = float2(1.0f, 0.f);
	TriStream.Append(Out[1]);

	/* 우하 */
	Out[2].vPosition = In[0].vPosition - vector(vRight, 0.f) * vHalfSize.x - vector(vUp, 0.f) * vHalfSize.y;
	Out[2].vPosition = mul(Out[2].vPosition, matVP);
	Out[2].vTexUV = float2(1.0f, 1.f);
	TriStream.Append(Out[2]);

	TriStream.RestartStrip();

	TriStream.Append(Out[0]);

	TriStream.Append(Out[2]);

	/* 좌하 */
	Out[3].vPosition = In[0].vPosition + vector(vRight, 0.f) * vHalfSize.x - vector(vUp, 0.f) * vHalfSize.y;
	Out[3].vPosition = mul(Out[3].vPosition, matVP);
	Out[3].vTexUV = float2(0.0f, 1.f);
	TriStream.Append(Out[3]);
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

	Out.vColor = g_DiffuseTexture.Sample(DiffuseSampler, In.vTexUV);

	return Out;
}

PS_OUT PS_MAIN_ALPHA_TIME(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DiffuseSampler, In.vTexUV);

	float3 vColor = Out.vColor.rgb;

	if(all(vColor))
		Out.vColor.a = g_fTime;

	return Out;
}

PS_OUT PS_MAIN_ALPHA_RED(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DiffuseSampler, In.vTexUV);

	Out.vColor.a = Out.vColor.r;

	return Out;
}

PS_OUT PS_MAIN_COLOR(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DiffuseSampler, In.vTexUV);

	Out.vColor.a = (Out.vColor.r + Out.vColor.b + Out.vColor.g) * 0.3f;


	Out.vColor.r *= g_vColor.r;
	Out.vColor.g *= g_vColor.g;
	Out.vColor.b *= g_vColor.b;

	return Out;
}

technique11		DefaultTechnique
{
	pass PointInstance // 0
	{
		SetRasterizerState(Rasterizer_Solid);
		SetDepthStencilState(DepthStecil_NotZWrite, 0);
		SetBlendState(BlendState_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = compile gs_5_0 GS_MAIN();
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass PointInstance_ALPHA_RED // 1
	{
		SetRasterizerState(Rasterizer_Solid);
		SetDepthStencilState(DepthStecil_NotZWrite, 0);
		SetBlendState(BlendState_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = compile gs_5_0 GS_MAIN();
		PixelShader = compile ps_5_0 PS_MAIN_ALPHA_RED();
	}

	pass PointInstance_Color // 2
	{
		SetRasterizerState(Rasterizer_Solid);
		SetDepthStencilState(DepthStecil_NotZWrite, 0);
		SetBlendState(BlendState_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = compile gs_5_0 GS_MAIN();
		PixelShader = compile ps_5_0 PS_MAIN_COLOR();
	}

	pass PointInstance_AlphaTime // 3
	{
		SetRasterizerState(Rasterizer_Solid);
		SetDepthStencilState(DepthStecil_NotZWrite, 0);
		SetBlendState(BlendState_Add_Extern, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = compile gs_5_0 GS_MAIN();
		PixelShader = compile ps_5_0 PS_MAIN_ALPHA_TIME();
	}

	pass PointInstance_AlphaAdd // 4
	{
		SetRasterizerState(Rasterizer_Solid);
		SetDepthStencilState(DepthStecil_NotZWrite, 0);
		SetBlendState(BlendState_Add, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = compile gs_5_0 GS_MAIN();
		PixelShader = compile ps_5_0 PS_MAIN();
	}
};


struct EX_VS_IN
{
	/* 본래 정점버퍼 */
	float3 vPosition : POSITION;

	/* 인스턴스 정점버퍼 */
	float2 vInstanceSize : PSIZE;
	row_major matrix WorldMatrix : WORLD;
	float fTime : TEXCOORD0;
};

struct EX_VS_OUT
{
	float4 vPosition : POSITION;
	float2 vSize : PSIZE;
	float fTime : TEXCOORD0;
};

EX_VS_OUT EX_VS_MAIN(EX_VS_IN In)
{
	EX_VS_OUT			Out = (EX_VS_OUT)0;

	matrix		matWV, matWVP;

	Out.vPosition = mul(vector(In.vPosition, 1.f), In.WorldMatrix);
	Out.vSize = In.vInstanceSize;
	Out.fTime = In.fTime;

	return Out;
}

struct EX_GS_OUT
{
	float4 vPosition : SV_POSITION;
	float2 vTexUV : TEXCOORD0;
	float fTime : TEXCOORD1;
};

[maxvertexcount(6)]
void EX_GS_MAIN(/*입력*/ point EX_VS_OUT In[1], /*출력*/ inout TriangleStream<EX_GS_OUT> TriStream)
{
	EX_GS_OUT		Out[6];

	float3		vLook = normalize(g_vCamPosition - In[0].vPosition).xyz;
	float3		vAxisY = vector(0.f, 1.f, 0.f, 0.f).xyz;
	float3		vRight = normalize(cross(vAxisY, vLook));
	float3		vUp = normalize(cross(vLook, vRight));

	float2		vHalfSize = float2(In[0].vSize.x * 0.5f, In[0].vSize.y * 0.5f);

	matrix		matVP;
	matVP = mul(ViewMatrix, ProjMatrix);


	/* 좌상 */
	Out[0].vPosition = In[0].vPosition + vector(vRight, 0.f) * vHalfSize.x + vector(vUp, 0.f) * vHalfSize.y;
	Out[0].vPosition = mul(Out[0].vPosition, matVP);
	Out[0].vTexUV = float2(0.f, 0.f);
	Out[0].fTime = In[0].fTime;
	TriStream.Append(Out[0]);


	/* 우상 */
	Out[1].vPosition = In[0].vPosition - vector(vRight, 0.f) * vHalfSize.x + vector(vUp, 0.f) * vHalfSize.y;
	Out[1].vPosition = mul(Out[1].vPosition, matVP);
	Out[1].vTexUV = float2(1.0f, 0.f);
	Out[1].fTime = In[0].fTime;
	TriStream.Append(Out[1]);

	/* 우하 */
	Out[2].vPosition = In[0].vPosition - vector(vRight, 0.f) * vHalfSize.x - vector(vUp, 0.f) * vHalfSize.y;
	Out[2].vPosition = mul(Out[2].vPosition, matVP);
	Out[2].vTexUV = float2(1.0f, 1.f);
	Out[2].fTime = In[0].fTime;
	TriStream.Append(Out[2]);

	TriStream.RestartStrip();

	TriStream.Append(Out[0]);

	TriStream.Append(Out[2]);

	/* 좌하 */
	Out[3].vPosition = In[0].vPosition + vector(vRight, 0.f) * vHalfSize.x - vector(vUp, 0.f) * vHalfSize.y;
	Out[3].vPosition = mul(Out[3].vPosition, matVP);
	Out[3].vTexUV = float2(0.0f, 1.f);
	Out[3].fTime = In[0].fTime;
	TriStream.Append(Out[3]);
}

struct EX_PS_IN
{
	float4 vPosition : SV_POSITION;
	float2 vTexUV : TEXCOORD0;
	float fTime : TEXCOORD1;
};

struct EX_PS_OUT
{
	vector	vColor : SV_TARGET;
};

/* 픽셀의 최종적인 색을 결정하낟. */
EX_PS_OUT EX_PS_MAIN(EX_PS_IN In)
{
	EX_PS_OUT		Out = (EX_PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DiffuseSampler, In.vTexUV);

	float3 vRGB = Out.vColor.rgb;


	Out.vColor.a *= In.fTime;

	return Out;
}

EX_PS_OUT EX_PS_MAIN_TIME(EX_PS_IN In)
{
	EX_PS_OUT		Out = (EX_PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DiffuseSampler, In.vTexUV);

	//Out.vColor.a *= g_fTime;

	return Out;
}

EX_PS_OUT EX_PS_MAIN_ALPHATIME(EX_PS_IN In)
{
	EX_PS_OUT		Out = (EX_PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DiffuseSampler, In.vTexUV);

	if(0 < Out.vColor.a)
		Out.vColor.a = g_fTime;

	return Out;
}

EX_PS_OUT EX_PS_MAIN_ALPHATIME_BLACK(EX_PS_IN In)
{
	EX_PS_OUT		Out = (EX_PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DiffuseSampler, In.vTexUV);

	if (0 < Out.vColor.a)
		Out.vColor.a = g_fTime;

	float fColor = (Out.vColor.r + Out.vColor.g + Out.vColor.b);

	if(0.5 > fColor)
		Out.vColor.a = 0;

	return Out;
}

technique11		ExtendTechnique
{
	pass PointInstance_AlphaBlendTime // 0
	{
		SetRasterizerState(Rasterizer_Solid);
		SetDepthStencilState(DepthStecil_NotZWrite, 0);
		SetBlendState(BlendState_Test, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		VertexShader = compile vs_5_0 EX_VS_MAIN();
		GeometryShader = compile gs_5_0 EX_GS_MAIN();
		PixelShader = compile ps_5_0 EX_PS_MAIN();
	}

	pass PointInstance_AlphaBlend_gTime // 1
	{
		SetRasterizerState(Rasterizer_Solid);
		SetDepthStencilState(DepthStecil_NotZWrite, 0);
		SetBlendState(BlendState_Test, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		VertexShader = compile vs_5_0 EX_VS_MAIN();
		GeometryShader = compile gs_5_0 EX_GS_MAIN();
		PixelShader = compile ps_5_0 EX_PS_MAIN_TIME();
	}

	pass PointInstance_AlphaTime // 2
	{
		SetRasterizerState(Rasterizer_Solid);
		SetDepthStencilState(DepthStecil_NotZWrite, 0);
		SetBlendState(BlendState_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		VertexShader = compile vs_5_0 EX_VS_MAIN();
		GeometryShader = compile gs_5_0 EX_GS_MAIN();
		PixelShader = compile ps_5_0 EX_PS_MAIN_ALPHATIME();
	}

	pass PointInstance_AlphaTimeBlack // 3
	{
		SetRasterizerState(Rasterizer_Solid);
		SetDepthStencilState(DepthStecil_NotZWrite, 0);
		SetBlendState(BlendState_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		VertexShader = compile vs_5_0 EX_VS_MAIN();
		GeometryShader = compile gs_5_0 EX_GS_MAIN();
		PixelShader = compile ps_5_0 EX_PS_MAIN_ALPHATIME_BLACK();
	}
};

