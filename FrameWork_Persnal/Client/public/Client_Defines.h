#pragma once

#ifndef __CLIENT_DEFINES_H__

#include <process.h>

static const unsigned int g_iWinCX = 1280;
static const unsigned int g_iWinCY = 720;

#include "d3dcompiler.h"
#pragma comment(lib,"d3dcompiler.lib")

extern HWND g_hWnd;
extern HINSTANCE g_hInst;

namespace Client {

}

using namespace Client;

#include "Loading.h"

#include "Client_Macro.h"
#include "Client_Struct.h"
#include "Client_Typedef.h"
#include <fstream>

#define __CLIENT_DEFINES_H__
#endif // !__CLIENT_DEFINES_H__
