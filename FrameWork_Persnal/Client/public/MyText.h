#pragma once

#ifndef __MYTEXT_H__
#include "Client_Defines.h"
#include "MyFont.h"

/* 폰트 하나씩을 모아둔 클래스
ABC라는 문자가 들어오면
문자열의 길이를 구함
문자열의 길이만큼 컨테이너에 리절브
그 후 문자열을 하나씩 읽어가며 정수화
그리고 알맞게 UV를 넘김


폰트
텍스처의 UV
출력할 좌표와 크기
*/


BEGIN(Client)

class CMyFont;

typedef struct tagTextDesc
{
	_float4			vColor;
	_uint			iScaleCount;
	_tchar			szText[MAX_PATH];
	MOVESTATE_DESC	MoveDesc;
}TEXT_DESC;

class CMyText final : public CGameObject
{
private:
	explicit CMyText(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CMyText(const CMyText& rhs);
	virtual ~CMyText() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float TimeDelta) override;
	virtual _int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

private:
	CRenderer*				m_pRendererCom = nullptr;
	CMovement*				m_pMovementCom = nullptr;

private:
	vector<CMyFont*>		m_pFonts = { nullptr };
	TEXT_DESC				m_Text_Desc;

private:
	HRESULT	Ready_Component(void* pArg);
	void Make_Font();


public:
	static CMyText* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context, void* pArg);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free() override;

};
END
#define __MYTEXT_H__
#endif // !__MYTEXT_H__
