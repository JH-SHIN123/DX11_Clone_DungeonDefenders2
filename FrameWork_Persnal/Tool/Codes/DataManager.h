#pragma once

#ifndef __DATAMANAGER_H__

#include "Base.h"

class CDataManager final : public CBase
{
	DECLARE_SINGLETON(CDataManager)
private:
	CDataManager();
	virtual ~CDataManager() = default;










private: // 맵배치 (게임오브젝트 관련)
	/*
	기본적으로 Engine의 데이터를 읽거나 쓰거나 탐색하면 그걸 복사
	Engine의 게임 오브젝트 매니저는 Map의 value로 list를가지고 있음
	상호작용이 없는 건물이나 구조물 같은것은 3ds에서 세팅을 하는편이 나을지도
	*/

private: // 내비 메쉬 관련
	/*
	직접 버텍스와 인덱스를 엮어야 할것임
	*/

private: // 터레인 관련
	/*
	텍스처나 다양한 정보에 따라 터레인을 입맛대로 바꾸는
	*/
	
private: // 충돌관련
	/*
	충돌
	*/


public:
	virtual void Free() override;
};

#define __DATAMANAGER_H__
#endif // !__DATAMANAGER_H__
