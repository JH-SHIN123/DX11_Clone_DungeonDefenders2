#pragma once

#ifndef __ENGINE_DEFINES_H__

#pragma warning (disable : 4251)

#include "D3D11.h"
#include "DirectXMath.h"	// 수학
#include "d3dx11effect.h"	// 쉐이더 담는거
#include "DirectXTex.h"		// 텍스처
#include "d3dxGlobal.h"	
#include "d3dcompiler.h"	// 쉐이더
#include <dinput.h>			// 키입력
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")

using namespace DirectX;

#define DIRECTINPUT_VERSION	0x0800
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



#define UPDATE_ERROR -1
#define OBJECT_DEAD 1
#define SCENE_CHANGE 2


#define __ENGINE_DEFINES_H__
#endif // !__ENGINE_DEFINES_H__