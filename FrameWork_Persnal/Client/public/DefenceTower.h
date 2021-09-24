#pragma once

#ifndef __DEFENCETOWER_H__

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)

enum class ETowerState
{ Pick, Rotate, Spawn, Idle, End };

enum class ETowerRange
{ Pizza, Quarter, One_Third, Half, Pi, End };

typedef struct tagTowerDesc
{
	_tchar			szModelName[MAX_PATH] = L"";
	STATUS_DESC		Stat_Desc;
	MOVESTATE_DESC	MoveState_Desc;
	_bool			IsTurnable = false;
	_float			fTowerRangeDis = 15.f;

	_float			fSpawnTime_Max = 1.f;
	ETowerRange		eTowerRange = ETowerRange::Half;
}TOWER_DESC;


class CDefenceTower abstract : public CGameObject
{
protected:
	explicit CDefenceTower(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CDefenceTower(const CDefenceTower& rhs);
	virtual ~CDefenceTower() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float TimeDelta) override;
	virtual _int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

public: // Setter
	void Set_TowerState(ETowerState eState) { m_eTowerState_Next = eState; }

public: // Getter
	_fvector Get_Pos() const { return m_pMovementCom->Get_State(EState::Position); }
	ETowerState Get_TowerState() const { return m_eTowerState_Next; }

public:
	void Set_TowerPos(_fvector vPosition);
	void Set_TowerRotation(_fvector vRotation);
	void Set_TowerRangeAngle(_float fRangeAngle_Max, _float fRangeAngle_Min) { m_fTowerRangeMax = fRangeAngle_Max, m_fTowerRangeMin = fRangeAngle_Min; }

protected:
	HRESULT	Ready_Component(void* pArg);
	void	TowerState_Check();
	_bool	Enemy_Check(_float TimeDelta, _vector* vTargetPos);

protected:
	CModel*			m_pModelCom		= nullptr;
	CStatus*		m_pStatusCom	= nullptr;
	CMovement*		m_pMovementCom	= nullptr;
	CRenderer*		m_pRendererCom	= nullptr;
	CTextures*		m_pTexturesCom	= nullptr;	// 타워 반경 

private: 
	ETowerState		m_eTowerState_Cur	= ETowerState::End;
	ETowerState		m_eTowerState_Next	= ETowerState::Pick;

private:
	_float			m_fSpawnTime		= 0;
	_float			m_fSpawnTime_Max	= 0;
	_uint			m_iShaderPass		= 0;
	ETowerRange		m_eTowerRange		= ETowerRange::End;
	_bool			m_IsTurnable		= false;
	_float3			m_vFirstLook_Dir;
	_float			m_fTowerRangeDis	= 0.f;
	_float			m_fTowerRangeCenter = 0.f;
	_float			m_fTowerRangeMax = 45.f; // 원점 90
	_float			m_fTowerRangeMin = -45.f;


public:
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free() override;

};

END
#define __DEFENCETOWER_H__
#endif // !__DEFENCETOWER_H__
