


float2 rcpres = { 0.0025, 0.0033333333333333333333333333333333 };

//텍스처 의 임의의 위치의 픽셀에 접근하귀 위한 좌표 ( 가로 방향)
float2 PixelKernelH[13] =
{
	{ -6, 0 },{ -5, 0 },{ -4, 0 },{ -3, 0 },{ -2, 0 },{ -1, 0 },{ 0, 0 },{ 1, 0 },{ 2, 0 },{ 3, 0 },{ 4, 0 },
	{ 5, 0 },{ 6, 0 },
};

//텍스처의 임의의 위치의 픽셀에 접근하기 위한 좌표 ( 세로 방향)
float2 PixelKernelV[13] =
{
	{ 0, -6 },{ 0, -5 },{ 0, -4 },{ 0, -3 },{ 0, -2 },{ 0, -1 },{ 0,  0 },{ 0,  1 },{ 0,  2 },{ 0,  3 },{ 0,  4 },
	{ 0,  5 },{ 0,  6 },
};

//미리 계산해 둔 가우스 필터의 마스크 값
float BlurWeights[13] =
{
	0.002216,    0.008764,    0.026995,    0.064759,    0.120985,    0.176033,    0.199471,    0.176033,    0.120985,    0.064759,
	0.026995,    0.008764,    0.002216,
};

texture2D g_BloomTexture;
texture2D g_DepthTexture;


sampler DepthSampler = sampler_state
{
	AddressU = wrap;
	AddressV = wrap;
};

sampler BloomSampler = sampler_state
{
	AddressU = wrap;
	AddressV = wrap;
};

struct VS_IN
{
	float4 vPosition : POSITION;
	float2 vTexUV : TEXCOORD;

};

struct VS_OUT
{
	float4 vPosition : SV_POSITION;
	float2 vTexUV : TEXCOORD;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	Out.vPosition = In.vPosition;
	Out.vTexUV = In.vTexUV;

	return Out;
}


struct PS_IN
{
	float4 vPosition : SV_POSITION;
	float2 vTexUV : TEXCOORD;
};

struct PS_OUT
{
	vector	vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In) 
{
	PS_OUT		Out = (PS_OUT)0;

	vector vDepth = g_DepthTexture.Sample(DepthSampler, In.vTexUV);

	if (vDepth.b >= 1.f)
	{
		Out.vColor = (float4)0;
		return Out;
	}

	vector vColor = g_BloomTexture.Sample(BloomSampler, In.vTexUV);
	vColor = pow(vColor, 4);

	vector vColor2 = -0.84f;

	
	[unroll(13)]for (int index = 0; index < 13; ++index)
	{
		vColor2 += g_BloomTexture.Sample(DepthSampler, In.vTexUV + (PixelKernelH[index] * rcpres)) * BlurWeights[index];
		vColor2 += g_BloomTexture.Sample(DepthSampler, In.vTexUV + (PixelKernelV[index] * rcpres)) * BlurWeights[index];
	}
	vColor2 *= 0.48f;

	Out.vColor = vColor + vColor2;

	return Out;
}

PS_OUT PS_MAIN_BLUR(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector BrightColor = (vector)0;
	vector FragColor = g_DepthTexture.Sample(DepthSampler, In.vTexUV);
	vector DifColor = g_BloomTexture.Sample(DepthSampler, In.vTexUV);

	//float Brightness = dot(FragColor.rgb, float3(0.2126f, 0.7152f, 0.0722f));
	//if (Brightness > 0.99)
	//	BrightColor = vector(FragColor.rgb, 1.f);

	FragColor += DifColor;
	Out.vColor = FragColor;

	if (Out.vColor.a < 0.02f)
		discard;

	return Out;
}

texture2D g_BlurTex;
sampler BlurSampler = sampler_state
{
	AddressU = clamp;
	AddressV = clamp;

	Filter = MIN_MAG_MIP_LINEAR;

};
float m_TexW = 1280.f;
float m_TexH = 720.f;
static const float Weight[13] =
{
	0.0561, 0.1353, 0.278, 0.4864, 0.7261, 0.9231,
	1, 0.9231, 0.7261, 0.4864, 0.278, 0.1353, 0.0561
};

static const float Total = 6.2108;

PS_OUT PS_MAINX(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	float2 t = In.vTexUV;
	float2 uv = (float2)0;

	float tu = 1.f / (m_TexH * 0.125f);

	for (int i = -6; i < 6; ++i)
	{
		uv = t + float2(tu * i, 0);
		vector Tex = g_BlurTex.Sample(BlurSampler, uv);
		//if (Tex.r)
		//	Tex = (float4)1;
		Out.vColor += Weight[6 + i] * Tex;
	}
	Out.vColor /= Total;

	return Out;
}

texture2D g_BlurTex_X;

PS_OUT PS_MAINY(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	float2 t = In.vTexUV;
	float2 uv = (float2)0;

	float tv = 1.f / (m_TexH * 0.125f);

	for (int i = -6; i < 6; ++i)
	{
		uv = t + float2(0, tv * i);
		Out.vColor += Weight[6 + i] * g_BlurTex.Sample(BlurSampler, uv);
	}
	Out.vColor /= Total;

	return Out;
}

texture2D g_FinalTex;
sampler FinalSampler = sampler_state
{
	AddressU = clamp;
	AddressV = clamp;
};

texture2D g_BloomingTex;
sampler BloomingSampler = sampler_state
{
	AddressU = clamp;
	AddressV = clamp;
};

texture2D g_BloomOriTex;
sampler BloomOriSampler = sampler_state
{
	AddressU = clamp;
	AddressV = clamp;
};

PS_OUT PS_MAINBLOOM(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	vector vFinalTex = g_FinalTex.Sample(FinalSampler, In.vTexUV);
	vector vBloomColor = g_BloomingTex.Sample(BloomingSampler, In.vTexUV);
	vector vBloomOriTex = g_BloomOriTex.Sample(BloomOriSampler, In.vTexUV);

	vector vBloom = pow(pow(abs(vBloomColor), 2.2f) + pow(abs(vBloomOriTex), 2.2f), 1.f / 2.2f);

	Out.vColor = vFinalTex;
	Out.vColor = pow(abs(Out.vColor), 2.2f);
	vBloom = pow(abs(vBloom), 2.2f);

	Out.vColor += vBloom;
	Out.vColor = pow(abs(Out.vColor), 1 / 2.2f);
	return Out;
}



RasterizerState Rasterizer_Solid
{
	FillMode = solid;
	CullMode = back;
	FrontCounterClockwise = false;
};

DepthStencilState DepthStecil_ZEnable
{
	DepthEnable = false;
};

BlendState BlendState_None
{
	BlendEnable[0] = false;
};

technique11		DefaultTechnique
{
	pass Default
	{
		SetRasterizerState(Rasterizer_Solid);
		SetDepthStencilState(DepthStecil_ZEnable, 0);
		SetBlendState(BlendState_None, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass BLU
	{
		SetRasterizerState(Rasterizer_Solid);
		SetDepthStencilState(DepthStecil_ZEnable, 0);
		SetBlendState(BlendState_None, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_BLUR();
	}

	pass RealBlurX
	{
		SetRasterizerState(Rasterizer_Solid);
		SetDepthStencilState(DepthStecil_ZEnable, 0);
		SetBlendState(BlendState_None, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAINX();
	}

	pass RealBlurY
	{
		SetRasterizerState(Rasterizer_Solid);
		SetDepthStencilState(DepthStecil_ZEnable, 0);
		SetBlendState(BlendState_None, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAINY();
	}

	pass RealBloom
	{
		SetRasterizerState(Rasterizer_Solid);
		SetDepthStencilState(DepthStecil_ZEnable, 0);
		SetBlendState(BlendState_None, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAINBLOOM();
	}
};