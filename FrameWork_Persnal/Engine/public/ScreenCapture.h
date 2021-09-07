#pragma once

#ifndef __SCREENCAPTURE_H__

#include "Base.h"

BEGIN(Engine)
class ENGINE_DLL CScreenCapture final :	public CBase
{
	DECLARE_SINGLETON(CScreenCapture)

public:
	CScreenCapture();
	virtual ~CScreenCapture() = default;

	/*
	https://docs.microsoft.com/ko-kr/windows/win32/gdi/bitmaps
	https://docs.microsoft.com/ko-kr/windows/win32/gdi/storing-an-image
	https://m.blog.naver.com/PostView.naver?isHttpsRedirect=true&blogId=eominsuk55&logNo=220393413410
	*/
public:
	virtual void Free();
};

END
#define __SCREENCAPTURE_H__
#endif // !__SCREENCAPTURE_H__
