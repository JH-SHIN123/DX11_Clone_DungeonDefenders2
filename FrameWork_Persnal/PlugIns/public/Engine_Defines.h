#pragma once

#ifndef __ENGINE_DEFINES_H__
#define DIRECTINPUT_VERSION	0x0800

#pragma warning (disable : 4251)
//#pragma warning (disable : 4005) // DIRECTINPUT_VERSION 매크로 재정의
#pragma warning (disable : 4996) // 문자열 변환 워닝

#include "D3D11.h"
#include "DirectXMath.h"	// 수학
#include "d3dx11effect.h"	// 쉐이더 담는거
#include "DirectXTex.h"		// 텍스처
#include "d3dxGlobal.h"	
#include "d3dcompiler.h"	// 쉐이더
#include <dinput.h>			// 키입력

//#pragma comment(lib, "dxguid.lib") #pragma warning (disable : 4099) // 라이브러리PDB 파일 디버깅 정보 없음 워닝
#pragma comment(lib, "dinput8.lib")

// 메쉬 로드 라이브러리들
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// 디버깅용 충돌체 렌더러
#include <DirectXTK/Effects.h>
#include <DirectXTK/SpriteBatch.h>
#include <DirectXTK/SpriteFont.h>


using namespace DirectX;

#include <list>
#include <map>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <functional>
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
#include "Engine_Easy_Defines.h"

using namespace Engine;


#define UPDATE_ERROR -1
#define OBJECT_DEAD 1
#define SCENE_CHANGE 2


#define __ENGINE_DEFINES_H__
#endif // !__ENGINE_DEFINES_H__