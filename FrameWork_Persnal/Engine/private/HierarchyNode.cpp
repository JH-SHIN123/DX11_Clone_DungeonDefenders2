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
