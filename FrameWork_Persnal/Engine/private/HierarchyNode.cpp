#include "..\public\HierarchyNode.h"
#include "AnimChannel.h"

CHierarcyNode::CHierarcyNode()
{
}

HRESULT CHierarcyNode::Set_AnimChannelPointer(CAnimChannel * pAnimChannel)
{
	if (nullptr == pAnimChannel)
		return E_FAIL;

	m_pAnimChannel = pAnimChannel;

	Safe_AddRef(m_pAnimChannel);

	return S_OK;
}

HRESULT CHierarcyNode::NativeConstruct(char * pName, _fmatrix TransformationMatrix, CHierarcyNode * pParent, _uint iDepth)
{
	strcpy(m_szNodeName, pName);

	XMStoreFloat4x4(&m_TransformationMatrix, XMMatrixTranspose(TransformationMatrix));

	m_pParent = pParent;
	Safe_AddRef(m_pParent);

	m_iDepth = iDepth;

	return S_OK;
}

void CHierarcyNode::Update_CombindTransformationMatrix()
{
	_matrix		TransformationMatrix;

	if (nullptr == m_pAnimChannel)
		TransformationMatrix = XMLoadFloat4x4(&m_TransformationMatrix);
	else
		TransformationMatrix = m_pAnimChannel->Get_TransformationMatrix();


	if (nullptr != m_pParent)
	{	
		// 부모노드의 행렬을 받아와 내 행렬의 값을 채워주자
		XMStoreFloat4x4(&m_CombinedTransformationMatrix, TransformationMatrix * XMLoadFloat4x4(&m_pParent->m_CombinedTransformationMatrix));
	}
	else
		XMStoreFloat4x4(&m_CombinedTransformationMatrix, TransformationMatrix);
}

CHierarcyNode * CHierarcyNode::Create(char * pName, _fmatrix TransformationMatrix, CHierarcyNode * pParent, _uint iDepth)
{
	CHierarcyNode*		pInstance = new CHierarcyNode();
	if (FAILED(pInstance->NativeConstruct(pName, TransformationMatrix, pParent, iDepth)))
	{
		MSG_BOX("Failed to Creating Instance (CHierarcyNode) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CHierarcyNode::Free()
{
	Safe_Release(m_pParent);
}
