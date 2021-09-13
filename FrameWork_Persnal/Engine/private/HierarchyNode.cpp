#include "..\public\HierarchyNode.h"

CHierarcyNode::CHierarcyNode()
{
}

HRESULT CHierarcyNode::NativeConstruct(char * pName, _fmatrix TransformationMatrix, CHierarcyNode * pParent, _uint iDepth)
{
	strcpy(m_szNodeName, pName);

	XMStoreFloat4x4(&m_TransformationMatrix, TransformationMatrix);

	m_pParent = pParent;
	Safe_AddRef(m_pParent);

	m_iDepth = iDepth;

	return S_OK;
}

void CHierarcyNode::Update_CombindTransformationMatrix()
{
	if (nullptr != m_pParent)
	{
		// 부모 노드의 행렬을 받아와 내 행렬에 값을 넣어주자
		XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMLoadFloat4x4(&m_TransformationMatrix) * XMLoadFloat4x4(&m_pParent->m_CombinedTransformationMatrix));
	}
	else
		m_CombinedTransformationMatrix = m_TransformationMatrix;
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
