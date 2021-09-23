/*
	어느 쉐이더에서나 공통적으로 사용할 부분들을 따로 모아 include 하자
	전역변수, 렌더스테이트
*/


cbuffer Matrices
{
	matrix			WorldMatrix;
	matrix			ViewMatrix;
	matrix			ProjMatrix;
}

cbuffer Camera
{
	float4		vCameraPosition;
}



/*		D3D11_RASTERIZER_DESC
D3D11_FILL_MODE FillMode;			=	정점 사이를 어떻게 채울건지
D3D11_CULL_MODE CullMode;			=	컬링할 기준	
BOOL FrontCounterClockwise;			=	어떤 방향으로 그릴지? 시계방향 = false
INT DepthBias;
FLOAT DepthBiasClamp;
FLOAT SlopeScaledDepthBias;
BOOL DepthClipEnable;
BOOL ScissorEnable;
BOOL MultisampleEnable;
BOOL AntialiasedLineEnable;
*/

RasterizerState Rasterizer_Wireframe
{
	FillMode = wireframe;
	CullMode = back;
	FrontCounterClockwise = false;

	// wireframe 으로 그려 (선)
	// 뒤통수 컬링 = 반시계 컬링
	// 앞부분을 반시계로 그릴까 = false = 시계방향으로 그려줘
};

RasterizerState Rasterizer_Solid
{
	FillMode = solid;
	CullMode = back;
	FrontCounterClockwise = false;
	
	// 채워서 그려줘 (픽셀쉐이더가 많이 호출 될것임)
};

RasterizerState Rasterizer_Solid_NotCurl
{
	FillMode = solid;
	CullMode = none;
	FrontCounterClockwise = false;

// 채워서 그려줘 (픽셀쉐이더가 많이 호출 될것임)
};

/*		D3D11_DEPTH_STENCIL_DESC
BOOL DepthEnable;							=	Z 비교 유무
D3D11_DEPTH_WRITE_MASK DepthWriteMask;		=	Z Write 관련 Zero(안함), All(함)
D3D11_COMPARISON_FUNC DepthFunc;			=	Z 비교를 어떻게 
BOOL StencilEnable;							=	스텐실 테스트 유무
UINT8 StencilReadMask;						=	스텐실은 나중에...
UINT8 StencilWriteMask;
D3D11_DEPTH_STENCILOP_DESC FrontFace;
D3D11_DEPTH_STENCILOP_DESC BackFace;
*/

DepthStencilState DepthStecil_Default
{
	DepthEnable = true;
	DepthWriteMask = all;
	DepthFunc = less;

	// DepthEnable : 깊이 비교 유무

};

DepthStencilState DepthStecil_NotZWrite
{
	DepthEnable = true;
	DepthWriteMask = zero;
	DepthFunc = less;
};


/*		D3D11_RENDER_TARGET_BLEND_DESC
BOOL BlendEnable;							=	블렌드 유무
D3D11_BLEND SrcBlend;						=	출력 할 픽셀 색상
D3D11_BLEND DestBlend;						=	비교 할 픽셀 색상 (이미 그려진 것)
D3D11_BLEND_OP BlendOp;
D3D11_BLEND SrcBlendAlpha;
D3D11_BLEND DestBlendAlpha;
D3D11_BLEND_OP BlendOpAlpha;
UINT8 RenderTargetWriteMask;
*/

BlendState BlendState_Alpha
{
	BlendEnable[0] = true;
	SrcBlend = SRC_ALPHA;
	DestBlend = INV_SRC_ALPHA;
	BlendOp = Add;
};

BlendState BlendState_Add
{
	BlendEnable[0] = true;
	BlendEnable[1] = true;
	SrcBlend = ONE;
	DestBlend = ONE;
	BlendOp = Add;
};

BlendState BlendState_None
{
	BlendEnable[0] = false;
};

