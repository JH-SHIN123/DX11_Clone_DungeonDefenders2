#pragma once

#ifndef __CLIENT_STRUCT_H__


typedef struct tagRank_Desc
{
	wchar_t			szName[MAX_PATH] = L"";
	unsigned int	iClass;
	unsigned int	iLevel;
	unsigned int	iScore;
}RANK_DESC;

typedef struct tagGameObject_Desc
{
	STATUS_DESC		Status_Desc;
	MOVESTATE_DESC	Movement_Desc;

	_tchar			szModelName[MAX_PATH] = L"";
	ELevel			eModelLevel = ELevel::End;
}GAMEOBJ_DESC;










#define __CLIENT_STRUCT_H__
#endif // !__CLIENT_STRUCT_H__
