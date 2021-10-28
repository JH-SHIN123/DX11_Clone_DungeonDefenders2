
cbuffer TransformDesc
{
	vector		g_vCamPosition;
	matrix		g_ProjMatrixInverse;
	matrix		g_ViewMatrixInverse;
}



texture2D		g_NormalTexture;

sampler NormalSampler = sampler_state
{
	AddressU = wrap;
	AddressV = wrap;
};

texture2D		g_DepthTexture;

sampler DepthSampler = sampler_state
{
	AddressU = wrap;
	AddressV = wrap;
};

cbuffer Directional
{
	vector		g_vLightDir;	
}

cbuffer Point
{
	vector		g_vLightPos;
	float		g_fRadius;
}

cbuffer LightColor
{
	vector		g_vLightDiffuse;
	vector		g_vLightAmbient;
	vector		g_vLightSpecular;
}

cbuffer MtrlDesc
{
	float		g_fPower = 20.f;
	vector		g_vMtrlDiffuse = (vector)1.f;
	vector		g_vMtrlAmbient = (vector)1.f;
	vector		g_vMtrlSpecular = (vector)1.f;
}

struct VS_IN
{
	float4 vPosition : POSITION;	
	float2 vTexUV : TEXCOORD;
	
};

struct VS_OUT
{
	float4 vPosition : SV_POSITION;	
	float2 vTexUV : TEXCOORD0;
	float4 vProjPosition : TEXCOORD1;
};


VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;
	
	Out.vPosition = In.vPosition;
	Out.vTexUV = In.vTexUV;
	Out.vProjPosition = Out.vPosition;

	return Out;
}



struct PS_IN
{
	float4 vPosition : SV_POSITION;
	float2 vTexUV : TEXCOORD;
	float4 vProjPosition : TEXCOORD1;
};

struct PS_OUT
{
	vector	vShade : SV_TARGET0;
	vector	vSpecular : SV_TARGET1;
};

PS_OUT PS_MAIN_DIRECTIONAL(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector	vNormalDesc = g_NormalTexture.Sample(NormalSampler, In.vTexUV);
	vector	vDepthDesc = g_DepthTexture.Sample(DepthSampler, In.vTexUV);
	float	fViewZ = vDepthDesc.x * 430.0f;

	vector	vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);	

	Out.vShade = max(dot(normalize(g_vLightDir) * -1.f, vNormal), 0.f) * (g_vLightDiffuse * g_vMtrlDiffuse)
		+ (g_vLightAmbient * g_vMtrlAmbient);

	/* 2차원 투영스페이스 상의 위치다. */
	vector	vWorldPos = vector(In.vProjPosition.x, In.vProjPosition.y, vDepthDesc.y, 1.f);

	/* 투영행렬을 곱해놓은 위치다. */
	vWorldPos = vWorldPos * fViewZ;

	/*  뷰스페이스 상의 위치를 구했다. */
	vWorldPos = mul(vWorldPos, g_ProjMatrixInverse);

	/*  월드스페이스 상의 위치를 구했다. */
	vWorldPos = mul(vWorldPos, g_ViewMatrixInverse);

	vector	vLook = vWorldPos - g_vCamPosition;
	vector	vReflect = reflect(normalize(g_vLightDir), vNormal);

	Out.vSpecular = pow(max(dot(normalize(vLook) * -1.f, vReflect), 0.f), g_fPower) * (g_vLightSpecular * g_vMtrlSpecular);
	Out.vSpecular.a = 0.f;

	return Out;
}

PS_OUT PS_MAIN_POINT(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector	vNormalDesc = g_NormalTexture.Sample(NormalSampler, In.vTexUV);
	vector	vDepthDesc = g_DepthTexture.Sample(DepthSampler, In.vTexUV);
	float	fViewZ = vDepthDesc.x * 430.f;

	vector	vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);

	/* 2차원 투영스페이스 상의 위치다. */
	vector	vWorldPos = vector(In.vProjPosition.x, In.vProjPosition.y, vDepthDesc.y, 1.f);

	/* 투영행렬을 곱해놓은 위치다. */
	vWorldPos = vWorldPos * fViewZ;

	/*  뷰스페이스 상의 위치를 구했다. */
	vWorldPos = mul(vWorldPos, g_ProjMatrixInverse);

	/*  월드스페이스 상의 위치를 구했다. */
	vWorldPos = mul(vWorldPos, g_ViewMatrixInverse);


	vector	vLightDir = vWorldPos - g_vLightPos;

	float	fDistance = length(vLightDir);

	float	fAtt = saturate((g_fRadius - fDistance) / g_fRadius);

	Out.vShade = (max(dot(normalize(vLightDir) * -1.f, vNormal), 0.f) * (g_vLightDiffuse * g_vMtrlDiffuse)
		+ (g_vLightAmbient * g_vMtrlAmbient)) * fAtt;

	
	vector	vLook = vWorldPos - g_vCamPosition;
	vector	vReflect = reflect(normalize(g_vLightDir), vNormal);

	Out.vSpecular = (pow(max(dot(normalize(vLook) * -1.f, vReflect), 0.f), g_fPower) * (g_vLightSpecular * g_vMtrlSpecular)) * fAtt;
	Out.vSpecular.a = 0.f;

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

BlendState BlendState_Add
{
	BlendEnable[0] = true;
	BlendEnable[1] = true;
	SrcBlend = ONE;
	DestBlend = ONE;
	BlendOp = Add;

	SrcBlendAlpha = ONE;
	DestBlendAlpha = ONE;
	BlendOpAlpha = Add;
};

technique11		DefaultTechnique
{
	pass Directional
	{		
		SetRasterizerState(Rasterizer_Solid);
		SetDepthStencilState(DepthStecil_ZEnable, 0);
		SetBlendState(BlendState_Add, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DIRECTIONAL();
	}	

	pass Point
	{
		SetRasterizerState(Rasterizer_Solid);
		SetDepthStencilState(DepthStecil_ZEnable, 0);
		SetBlendState(BlendState_Add, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_POINT();
	}
};



