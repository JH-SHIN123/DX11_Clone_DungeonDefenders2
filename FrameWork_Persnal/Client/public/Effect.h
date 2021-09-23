#pragma once

#ifndef __EFFECT_H__

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)

enum class EEffectType { Mesh, Texture, End };

typedef struct tagEffectDesc
{
	EEffectType		eEffectType = EEffectType::End;
	_tchar			szResourceName[MAX_PATH] = L"";
	ELevel			eResourceLevel = ELevel::End;
	MOVESTATE_DESC	Move_Desc;
	_uint			iShaderPass = 0;
}EFFECT_DESC;


class CEffect abstract : public CGameObject
{
protected:
	explicit CEffect(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context);
	explicit CEffect(const CEffect& rhs);
	virtual ~CEffect() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float TimeDelta) override;
	virtual _int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_Rotate(_fmatrix Matrix);
	void Set_Position(_fvector vPosition);
	void Set_Rotate_Axis(_fvector vRatate, _float fRadian);

public:
	HRESULT Ready_Component(void* pArg);
	HRESULT Render_Mesh();
	HRESULT Render_Texture();

protected:
	CRenderer*		m_pRendererCom	= nullptr;
	CModel*			m_pModelCom		= nullptr;
	CMovement*		m_pMovementCom	= nullptr;
	CTextures*		m_pTexturesCom	= nullptr;
	CVIBuffer_Rect*	m_pVIBufferCom	= nullptr;

protected:
	_uint			m_iShaderPass = 0;
	 

public:
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
#define __EFFECT_H__
#endif // !__EFFECT_H__
