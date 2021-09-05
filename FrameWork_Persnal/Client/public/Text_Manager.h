#pragma once

#ifndef __TEXT_MANAGER_H__
#include "Client_Defines.h"
#include "Base.h"
#include "MyFont.h"

typedef struct tagFontFileData
{
	_uint iFontID;
	_float2 vTextureUV_Left;
	_float2 vTextureUV_Right;
}FONTFILE_DATA;

class CText_Manager final : public CBase
{
	DECLARE_SINGLETON(CText_Manager)
public:
	CText_Manager();
	virtual ~CText_Manager() = default;

public:
	HRESULT Ready_Font();
	FONTFILE_DATA* Get_Font(_uint ID);


private:
	vector<FONTFILE_DATA*> m_Font = {nullptr};

public:
	virtual void Free();
};

#define __TEXT_MANAGER_H__
#endif // !1__TEXT_MANAGER_H__
