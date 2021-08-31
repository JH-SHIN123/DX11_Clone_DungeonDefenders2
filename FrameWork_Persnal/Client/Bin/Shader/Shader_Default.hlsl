
#include "Shader_Defines.hpp"

// 텍스처의 픽셀정보를 가지고있음
texture2D		g_DiffuseTexture;
texture2D		g_MaskTexture;
float			g_AlphaTime;

// 샘플러 : 텍스처를 어떻게 필터링하고 샘플링 할것인지 세팅하는것
// RS보다 세밀한 설정느낌
sampler DiffuseSampler = sampler_state
{
	AddressU = wrap;
	AddressV = wrap;

	/*Filter = MIN_MAG_MIP_LINEAR;
	*/
	// DX11의 기본 샘플링 옵션은 Clamp (UV가 1이 넘어갈 경우 그냥 출력 안함)
};

// 구조체 정의
struct VS_IN
{
	// 콜론 뒤에 붙은 키워드는 시멘틱스
	// 시멘틱스를 통해 장치가 해당 변수를 어디에 써야하는지 알 수 있다.
	float3 vPosition : POSITION;
	float2 vTexUV : TEXCOORD0;
};


struct VS_OUT
{
	// VS의 Input과 Output의 vPosition이 다르다
	// 투영에서의 Z나누기 전 단계까지 정점쉐이더가 작동해야하기 때문에
	// w값이 있을 수 있도록 float4 를 사용
	float4 vPosition : SV_POSITION;
	float2 vTexUV : TEXCOORD0;
};


/* 그리려하는 정점 하나에 대한 Shading을 한다.	*/
/* Shading : 정점의 스페이스 변환.			*/
/* 정점의 멤버를 필요에따라 변형한다.			*/
// 
// VS_OUT VS_MAIN(float3 vPosition : POSITION, float2 vTexUV : TEXCOORD0)
VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	Out.vPosition = mul(vector(In.vPosition, 1.f), WorldMatrix);

	vector		vPosition = vector(In.vPosition, 1.f);
	// w가 크면 작아짐
	// Z 나누기가 이 정점쉐이더 후에 되고있다는 증거가 아닐까

	Out.vPosition = vPosition;
	Out.vTexUV = In.vTexUV; // 텍스처 UV

	return Out;

	// 정점쉐이더 단계에서 스페이스 변환을 거친다 (지금은 변환없이 그대로 출력 한다)
	// 그렇다면 렌더링 파이프 라인에서 정점 쉐이더 단계는 래스터라이즈를 하기 전은 맞는데 (픽셀의 색이 아직없음)
	// 조명이나 클리핑같은것은 여기에 넣기 나름인가? (DX9 때의 고정적인 렌더링 파이프 라인은 잊자)
	// 근데 조명도 픽셀의 색이니 결국 픽셀 쉐이더 단계에 가는게 맞나?
}

VS_OUT VS_MAIN_UI(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;
	matrix		matWV, matWVP;

	matWV = mul(WorldMatrix, ViewMatrix);
	matWVP = mul(matWV, ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);

	Out.vTexUV = In.vTexUV; // 텍스처 UV

	return Out;
}

VS_OUT VS_MAIN_UI_REVERCE(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;
	matrix		matWV, matWVP;

	matWV = mul(WorldMatrix, ViewMatrix);
	matWVP = mul(matWV, ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);

	Out.vTexUV = In.vTexUV; // 텍스처 UV
	Out.vTexUV.x *= -1.f;

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
// 정점을 선형 보간한 영역 안의 픽셀의 정보를 채우는 것이기 때문에
// 가장 간단해야한다(연산이 적어야 함) for문은 미친짓, 끽해야 if문 하나 정도 사용한다고 한다.
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	// 픽셀의 색 = Texture의 UV 좌표에서 g_DiffuseTexture의 색을 따라가되 DiffuseSampler라는 이름의 샘플러를 사용 할 것
	Out.vColor = g_DiffuseTexture.Sample(DiffuseSampler, In.vTexUV);

	//Out.vColor.a = 1.f;

	return Out;
}

PS_OUT PS_MAIN_RED(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	// 픽셀의 색 = Texture의 UV 좌표에서 g_DiffuseTexture의 색을 따라가되 DiffuseSampler라는 이름의 샘플러를 사용 할 것
	Out.vColor = g_DiffuseTexture.Sample(DiffuseSampler, In.vTexUV);

	Out.vColor.a = 0.95f;
	Out.vColor.r = 1.f;

	return Out;
}

PS_OUT PS_MAIN_BLUE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	// 픽셀의 색 = Texture의 UV 좌표에서 g_DiffuseTexture의 색을 따라가되 DiffuseSampler라는 이름의 샘플러를 사용 할 것
	Out.vColor = g_DiffuseTexture.Sample(DiffuseSampler, In.vTexUV);

	Out.vColor.a = 0.95f;
	Out.vColor.b = 1.f;

	return Out;
}

PS_OUT PS_REDMASKING(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	if (In.vPosition.y >= 640.f ||
		In.vPosition.y <= 500.f /*||
		In.vTexUV.y <= 0.2f*/)
	{
		Out.vColor.a = 0.f;
		return Out;
	}

	float4 vFX_tex = g_MaskTexture.Sample(DiffuseSampler, In.vTexUV);
	Out.vColor = g_MaskTexture.Sample(DiffuseSampler, In.vTexUV);

	Out.vColor.r = 0.9f;
//	Out.vColor.gb = 1.f;
	Out.vColor.a = vFX_tex.g;

	return Out;
}

PS_OUT PS_ALPHATIME(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DiffuseSampler, In.vTexUV);

	if(any(Out.vColor))
		Out.vColor.a = g_AlphaTime;

	return Out;
}

PS_OUT PS_TIME(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DiffuseSampler, In.vTexUV);
	if (Out.vColor.a != 0.f)
		Out.vColor.a = g_AlphaTime;

	return Out;
}

PS_OUT PS_TIMEDARK(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float4 vFX_tex = g_MaskTexture.Sample(DiffuseSampler, In.vTexUV); // 쿨타임이미지
	Out.vColor = g_DiffuseTexture.Sample(DiffuseSampler, In.vTexUV); // 원래 이미지

	// 정규화된 시간이 특정 알파값보다 작다면 어둡게

	if (vFX_tex.a >= g_AlphaTime)
		Out.vColor.rgb -= 0.7f;

	return Out;
}

// 네임스페이스 정도
technique11		DefaultTechnique
{
// 외부에서 사용할 수 있도록 pass의 이름이나 Index로 접근해서 해당 쉐이더를 호출하는 방식
	pass Default //0
	{
		// hpp 에서 선언한 그리기모드들이 여기에 들어감
		SetRasterizerState(Rasterizer_Solid);
		SetDepthStencilState(DepthStecil_Default, 0);
		SetBlendState(BlendState_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass UI_Main //1
	{
		// 깊이버퍼 비교 X
		SetRasterizerState(Rasterizer_Solid);
		SetDepthStencilState(DepthStecil_NotZWrite, 0);
		SetBlendState(BlendState_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN_UI();
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Reverce_Main//2
	{
		// 깊이버퍼 비교 X
		SetRasterizerState(Rasterizer_Solid);
		SetDepthStencilState(DepthStecil_NotZWrite, 0);
		SetBlendState(BlendState_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN_UI_REVERCE();
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Main_Blue//3
	{
		// 깊이버퍼 비교 X
		SetRasterizerState(Rasterizer_Solid);
		SetDepthStencilState(DepthStecil_NotZWrite, 0);
		SetBlendState(BlendState_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN_UI();
		PixelShader = compile ps_5_0 PS_MAIN_BLUE();
	}

	pass Reverce_RedMask//4
	{
		// 깊이버퍼 비교 X
		SetRasterizerState(Rasterizer_Solid);
		SetDepthStencilState(DepthStecil_NotZWrite, 0);
		SetBlendState(BlendState_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN_UI_REVERCE();
		PixelShader = compile ps_5_0 PS_REDMASKING();
	}

	pass UI_AlphaTime//5
	{
		// 깊이버퍼 비교 X
		SetRasterizerState(Rasterizer_Solid);
		SetDepthStencilState(DepthStecil_NotZWrite, 0);
		SetBlendState(BlendState_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN_UI();
		PixelShader = compile ps_5_0 PS_ALPHATIME();
	}

	pass UI_Time//6
	{
		// 깊이버퍼 비교 X
		SetRasterizerState(Rasterizer_Solid);
		SetDepthStencilState(DepthStecil_NotZWrite, 0);
		SetBlendState(BlendState_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN_UI();
		PixelShader = compile ps_5_0 PS_TIME();
	}

	pass UI_TimeDark//7
	{
		// 깊이버퍼 비교 X
		SetRasterizerState(Rasterizer_Solid);
		SetDepthStencilState(DepthStecil_NotZWrite, 0);
		SetBlendState(BlendState_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN_UI();
		PixelShader = compile ps_5_0 PS_TIMEDARK();
	}
};



