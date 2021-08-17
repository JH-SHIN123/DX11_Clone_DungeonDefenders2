#pragma once

#pragma warning (disable : 4251)

#include "D3D11.h"
#include "DirectXMath.h"
#include "DirectXTex.h"
#include "d3dx11effect.h"
#include "d3dxGlobal.h"	
#include "d3dcompiler.h"

using namespace DirectX;

#define DIRECTINPUT_VERSION	0x0800
#include <dinput.h>

#include <list>
#include <unordered_map>
#include <algorithm>
using namespace std;


#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif

#endif // _DEBUG

#include "Engine_Enum.h"
#include "Engine_Macro.h"
#include "Engine_Struct.h"
#include "Engine_Typedef.h"
#include "Engine_Function.h"

using namespace Engine;
