#pragma once

#ifndef __FADE_H__

#include "Client_Defines.h"
#include "GameObject.h"

// 클론 생성시 인자가 없다면 스테이지 진입 FadeOut이다
typedef struct tagFade_Desc
{
	ELevel eNextLevel = ELevel::End;
	ELevel eNowLevel = ELevel::End;

	_bool	IsFadeIn = false;
}FADE_DESC;

class CFade final : public CGameObject
{
private:
	explicit CFade(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CFade(const CFade& rhs);
	virtual ~CFade() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float TimeDelta) override;
	virtual _int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Component(void* pArg);

private:
	CRenderer*				m_pRendererCom = nullptr;
	CTextures*				m_pTexturesCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;
	CMovement*				m_pMovementCom = nullptr;

private:
	ELevel m_eNextLevel = ELevel::End;
	ELevel m_eNowLevel = ELevel::End;

	_float m_fTime = 1.25f;
	_bool m_IsFadeIn = false;

public:
	static CFade* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free() override;
};

#define __FADE_H__
#endif // !__FADE_H__
