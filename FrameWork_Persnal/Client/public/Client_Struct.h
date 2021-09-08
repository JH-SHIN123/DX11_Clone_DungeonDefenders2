#pragma once

#ifndef __CLIENT_STRUCT_H__


typedef struct tagRank_Desc
{
	wchar_t			szName[MAX_PATH] = L"";
	unsigned int	iClass;
	unsigned int	iLevel;
	unsigned int	iScore;
}RANK_DESC;












#define __CLIENT_STRUCT_H__
#endif // !__CLIENT_STRUCT_H__
