#include "..\public\HierarchyNode.h"
#include "AnimChannel.h"

CHierarchyNode::CHierarchyNode()
{
}
CHierarchyNode::CHierarchyNode(const CHierarchyNode& rhs)
	: m_CombinedTransformationMatrix(rhs.m_CombinedTransformationMatrix)
	, m_TransformationMatrix(rhs.m_TransformationMatrix)
	, m_iDepth(rhs.m_iDepth)
{
	strcpy(m_szNodeName, rhs.m_szNodeName);
}

HRESULT CHierarchyNode::Set_AnimChannelPointer(_uint iAnimIndex, CAnimChannel * pAnimChannel)
{
	if (nullptr == pAnimChannel)
		return E_FAIL;

	m_ppAnimChannel[iAnimIndex] = pAnimChannel;

	Safe_AddRef(pAnimChannel);

	return S_OK;
}

HRESULT CHierarchyNode::NativeConstruct(char * pName, _fmatrix TransformationMatrix, CHierarchyNode * pParent, _uint iDepth)
{
	strcpy(m_szNodeName, pName);

	XMStoreFloat4x4(&m_TransformationMatrix, XMMatrixTranspose(TransformationMatrix));

	m_pParent = pParent;
	Safe_AddRef(m_pParent);

	m_iDepth = iDepth;

	return S_OK;
}

HRESULT CHierarchyNode::Reserve_AnimChannel(_uint iNumAnimation)
{
	m_ppAnimChannel = new CAnimChannel*[iNumAnimation];
	ZeroMemory(m_ppAnimChannel, sizeof(CAnimChannel*) * iNumAnimation);

	m_iNumAnimation = iNumAnimation;

	return S_OK;
}

void CHierarchyNode::Update_CombindTransformationMatrix(_uint iAnimationIndex)
{
	_matrix		TransformationMatrix;

	if (nullptr == m_ppAnimChannel[iAnimationIndex])
		TransformationMatrix = XMLoadFloat4x4(&m_TransformationMatrix);
	else
		TransformationMatrix = m_ppAnimChannel[iAnimationIndex]->Get_TransformationMatrix();


	if (nullptr != m_pParent)       // 부모노드의 행렬과 내 행렬을 곱하자
		XMStoreFloat4x4(&m_CombinedTransformationMatrix, TransformationMatrix * XMLoadFloat4x4(&m_pParent->m_CombinedTransformationMatrix));

	else                            // 루트노드
		XMStoreFloat4x4(&m_CombinedTransformationMatrix, TransformationMatrix); 
}

CHierarchyNode * CHierarchyNode::Create(char * pName, _fmatrix TransformationMatrix, CHierarchyNode * pParent, _uint iDepth)
{
	CHierarchyNode*		pInstance = new CHierarchyNode();
	if (FAILED(pInstance->NativeConstruct(pName, TransformationMatrix, pParent, iDepth)))
	{
		MSG_BOX("Failed to Creating Instance (CHierarchyNode) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CHierarchyNode::Free()
{
	for (_uint i = 0; i < m_iNumAnimation; ++i)
		Safe_Release(m_ppAnimChannel[i]);

	Safe_Delete_Array(m_ppAnimChannel);


	Safe_Release(m_pParent);
}
