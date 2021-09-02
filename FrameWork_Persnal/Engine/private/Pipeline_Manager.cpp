#include "..\public\Pipeline_Manager.h"

IMPLEMENT_SINGLETON(CPipeline_Manager)

CPipeline_Manager::CPipeline_Manager()
{
}

_uint CPipeline_Manager::Tick()
{
	XMStoreFloat4x4(&m_CameraWorldMatrix, XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_StateMatrices[(_uint)ETransformState::View])));

	return 0;
}

void CPipeline_Manager::Free()
{
}
