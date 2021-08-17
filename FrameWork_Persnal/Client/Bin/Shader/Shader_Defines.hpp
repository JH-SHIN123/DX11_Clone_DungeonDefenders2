


cbuffer Matrices
{
	matrix			WorldMatrix;
matrix			ViewMatrix;
matrix			ProjMatrix;
}


/* RasterizerState

D3D11_FILL_MODE FillMode;
D3D11_CULL_MODE CullMode;
BOOL FrontCounterClockwise;
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
};

RasterizerState Rasterizer_Solid
{
	FillMode = solid;
CullMode = back;
FrontCounterClockwise = false;
};

/*
BOOL DepthEnable;
D3D11_DEPTH_WRITE_MASK DepthWriteMask;
D3D11_COMPARISON_FUNC DepthFunc;
BOOL StencilEnable;
UINT8 StencilReadMask;
UINT8 StencilWriteMask;
D3D11_DEPTH_STENCILOP_DESC FrontFace;
D3D11_DEPTH_STENCILOP_DESC BackFace;
*/

DepthStencilState DepthStecil_Default
{
	DepthEnable = true;
DepthWriteMask = all;
DepthFunc = less;
};

DepthStencilState DepthStecil_NotZWrite
{
	DepthEnable = true;
DepthWriteMask = zero;
DepthFunc = less;
};

DepthStencilState DepthStecil_GARA
{
	DepthEnable = true;
DepthWriteMask = zero;
DepthFunc = greater;
};


/*
BOOL BlendEnable;
D3D11_BLEND SrcBlend;
D3D11_BLEND DestBlend;
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


