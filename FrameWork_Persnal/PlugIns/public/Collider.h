#pragma once

#include "Component.h"
#include "DebugDraw.h"

BEGIN(Engine)

enum class ECollideType { AABB, OBB, SPHERE, End };
enum class EDamageType { Direct, Dot, Dot_Slow, Shock, End };

typedef struct tagAttackDesc
{
	_int iDamage = 0;
	_int iDamage_2 = 0;
	_float fHitTime = 0.f;
	EDamageType eDamageType = EDamageType::Direct;
}ATTACK_DESC;


typedef struct tagColliderDesc
{
	_float3		vScale = { 0.f,0.f,0.f }, vRotation = { 0.f,0.f,0.f }, vPosition = { 1.f,1.f,1.f };
	_bool		IsCenter = false;
	ATTACK_DESC Attack_Desc;
}COLLIDER_DESC;

class ENGINE_DLL CCollider final : public CComponent
{
private:
	explicit CCollider(ID3D11Device* pDevice, ID3D11DeviceContext*	pDevice_Context);
	explicit CCollider(const CCollider& rhs);
	virtual ~CCollider() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(ECollideType eColliderType);
	virtual HRESULT NativeConstruct(void* pArg)override;

public:
	_bool	Get_IsCollide() { return m_IsCollision; }
	EDamageType Get_DamageType() const { return m_ColliderDesc.Attack_Desc.eDamageType; }
	_int Get_Damage() const { return m_ColliderDesc.Attack_Desc.iDamage; }
	_float Get_HitTime() const { return m_ColliderDesc.Attack_Desc.fHitTime; }

public:
	void Set_IsCollide(_bool IsCollide) { m_IsCollision = IsCollide; }
	void Set_Scale(_float3 vScale) { m_ColliderDesc.vScale = vScale; }
	void Set_Scale_Tick(_float3 vScale, _float TimeDelta);


public: //Collision On/Off
	_bool Get_NotCollide() { return m_IsNotCollide; }
	void  Set_NotCollide(_bool IsNotCollide) { m_IsNotCollide = IsNotCollide; }


public:
	_int Update_Collider(_fmatrix WorldMatrix);
	_int Update_Collider(_fvector vPosition);
	HRESULT Render_Collider();
	_bool	Intersect_Collider(CCollider* pCollide);

private:
	ECollideType			m_eColliderType = ECollideType::End;
	COLLIDER_DESC			m_ColliderDesc;
	BoundingBox*			m_pAABB = nullptr;
	BoundingOrientedBox*	m_pOBB = nullptr;
	BoundingSphere*			m_pSphere = nullptr;
	_bool					m_IsCollision = false;
	_float4					m_vColor;

private:
	_bool		m_IsNotCollide = false;


private:
	BasicEffect*			m_pEffect = nullptr;
	ID3D11InputLayout*		m_pInputLayout = nullptr;
	DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* m_pBatch = nullptr;


#ifdef FONT
	SpriteBatch*			m_pFontBT;
	SpriteFont*				m_pFont;
#endif

private:
	_matrix Remove_Scale(_matrix Transform);
	_matrix Remove_Rotation(_matrix Transform);
	_fvector Fix_Position(_fvector vPosition);
public:
	static CCollider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context, ECollideType eColliderType);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END