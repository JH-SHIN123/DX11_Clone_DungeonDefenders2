#pragma once

#include "Component.h"
#include "DebugDraw.h"

BEGIN(Engine)

enum class ECollideType { AABB, OBB, SPHERE, End };

class ENGINE_DLL CCollider final : public CComponent
{
public:
	typedef struct tagColliderDesc
	{
		_float3		vScale, vRotation, vPosition;
	}COLLIDERDESC;
private:
	explicit CCollider(ID3D11Device* pDevice, ID3D11DeviceContext*	pDevice_Context);
	explicit CCollider(const CCollider& rhs);
	virtual ~CCollider() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(ECollideType eColliderType);
	virtual HRESULT NativeConstruct(void* pArg)override;

public:
	_int Update_Collider(_fmatrix WorldMatrix);
	_int Update_Collider(_fvector vPosition);
	HRESULT Render_Collider();

private:
	ECollideType			m_eColliderType = ECollideType::End;
	COLLIDERDESC			m_ColliderDesc;
	BoundingBox*			m_pAABB = nullptr;
	BoundingOrientedBox*	m_pOBB = nullptr;
	BoundingSphere*			m_pSphere = nullptr;
	_bool					m_isCollision = false;
	_float4					m_vColor;


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
public:
	static CCollider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context, ECollideType eColliderType);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END