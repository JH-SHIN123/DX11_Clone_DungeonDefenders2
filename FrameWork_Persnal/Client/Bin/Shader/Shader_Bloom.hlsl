


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

	vector vColor = g_BloomTexture.Sample(DepthSampler, In.vTexUV);
	vColor = pow(vColor, 4);

	vector vColor2 = -0.84f;

	for (int index = 0; index < 13; ++index)
	{
		vColor2 += g_BloomTexture.Sample(DepthSampler, In.vTexUV + (PixelKernelH[index] * rcpres)) * BlurWeights[index];
		vColor2 += g_BloomTexture.Sample(DepthSampler, In.vTexUV + (PixelKernelV[index] * rcpres)) * BlurWeights[index];
	}
	vColor2 *= 0.48f;

	Out.vColor = vColor + vColor2;

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
};