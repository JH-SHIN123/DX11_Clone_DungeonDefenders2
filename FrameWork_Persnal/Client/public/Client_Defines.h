#pragma once

#ifndef __CLIENT_DEFINES_H__

#include <process.h>

static const unsigned int g_iWinCX = 1280;
static const unsigned int g_iWinCY = 720;

#include "d3dcompiler.h"
#pragma comment(lib,"d3dcompiler.lib")

extern HWND g_hWnd;

namespace Client {

}

using namespace Client;

#include "Loading.h"

#define __CLIENT_DEFINES_H__
#endif // !__CLIENT_DEFINES_H__
