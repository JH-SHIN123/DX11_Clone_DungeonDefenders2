#pragma once

static const unsigned int g_iWinCX = 1280;
static const unsigned int g_iWinCY = 720;

#pragma warning (disable : 4005)

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 일부 CString 생성자는 명시적으로 선언됩니다.

// MFC의 공통 부분과 무시 가능한 경고 메시지에 대한 숨기기를 해제합니다.
#define _AFX_ALL_WARNINGS

//#define SHADER_PATH(FileName)					\
//FileName = L"../../Client/Bin/Shader/" + FileName.c_str())	\
//const _tchar* szSahderPath = L"../../Client/Bin/Shader/"	*\

//static _tchar szFilePath[256] = L"../../Client/Bin/Shader/";
//static wstring wstrFilePath = L"../../Client/Bin/Shader/";

#define SHADER_PATH(FileName)						\
wstring wstrFilePath = L"../../Client/Bin/Shader/"	\
return (wstrFilePath += FileName).c_str()



#include "Tool_Enum.h"
#include "public\GameInstance.h"

#include <string>
#include "strsafe.h"

namespace Tool {

}

using namespace Tool;