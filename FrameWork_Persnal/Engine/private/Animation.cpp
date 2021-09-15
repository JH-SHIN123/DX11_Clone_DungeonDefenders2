#include "..\public\Animation.h"
#include "AnimChannel.h"


CAnimation::CAnimation()
{
}

HRESULT CAnimation::NativeConstruct(const char * pName, _float Duration, _float TickPerSecond)
{
	strcpy(m_szName, pName);
	m_fDuration = Duration;
	m_TickPerSecond = TickPerSecond;

	return S_OK;
}

HRESULT CAnimation::Add_Channel(CAnimChannel * pAnimChannel)
{
	if (nullptr == pAnimChannel)
		return E_FAIL;

	m_Channels.push_back(pAnimChannel);

	return S_OK;
}

HRESULT CAnimation::Update_Transform(_float TimeDelta)
{
	m_fCurrentTime += fmod(m_TickPerSecond * TimeDelta, m_fDuration);

	if (m_fCurrentTime >= m_fLastTime)
	{
		m_fCurrentTime = fmod(m_TickPerSecond * TimeDelta, m_fDuration);

		m_isEnd = true;
	}
	else
		m_isEnd = false;

	for (auto& pAnimChannel : m_Channels)
	{
		vector<KEYFRAME*>	KeyFrames = pAnimChannel->Get_KeyFrames();

		KEYFRAME*			pFirst = KeyFrames.front();
		KEYFRAME*			pLast = KeyFrames.back();

		_uint iCurrentKeyFrame = pAnimChannel->Get_CurrentKeyFrame();

		if (true == m_isEnd)
		{
			iCurrentKeyFrame = 0;
			m_fCurrentTime = 0;
			pAnimChannel->Set_CurrentKeyFrame(iCurrentKeyFrame);
		}

		_vector			vScale, vRotation, vPosition;

		vScale = XMVectorZero();
		vRotation = XMVectorZero();
		vPosition = XMVectorZero();

		if (m_fCurrentTime < (_float)pFirst->Time)
		{
			vScale = XMLoadFloat3(&pFirst->vScale);
			vRotation = XMLoadFloat4(&pFirst->vRotation);
			vPosition = XMLoadFloat3(&pFirst->vPosition);
			vPosition = XMVectorSetW(vPosition, 1.f);
		}

		else if (m_fCurrentTime >= (_float)pLast->Time)
		{
			vScale = XMLoadFloat3(&pLast->vScale);
			vRotation = XMLoadFloat4(&pLast->vRotation);
			vPosition = XMLoadFloat3(&pLast->vPosition);
			vPosition = XMVectorSetW(vPosition, 1.f);
		}
		else
		{
			if (m_fCurrentTime > KeyFrames[iCurrentKeyFrame + 1]->Time)
				pAnimChannel->Set_CurrentKeyFrame(++iCurrentKeyFrame);

			_float		fRatio = (m_fCurrentTime - (_float)KeyFrames[iCurrentKeyFrame]->Time) /
				((_float)KeyFrames[iCurrentKeyFrame + 1]->Time - (_float)KeyFrames[iCurrentKeyFrame]->Time);

			_vector		vSourScale, vDestScale;
			_vector		vSourRotataion, vDestRotation;
			_vector		vSourPosition, vDestPosition;

			vSourScale = XMLoadFloat3(&KeyFrames[iCurrentKeyFrame]->vScale);
			vSourRotataion = XMLoadFloat4(&KeyFrames[iCurrentKeyFrame]->vRotation);
			vSourPosition = XMLoadFloat3(&KeyFrames[iCurrentKeyFrame]->vPosition);
			vSourPosition = XMVectorSetW(vSourPosition, 1.f);

			vDestScale = XMLoadFloat3(&KeyFrames[iCurrentKeyFrame + 1]->vScale);
			vDestRotation = XMLoadFloat4(&KeyFrames[iCurrentKeyFrame + 1]->vRotation);
			vDestPosition = XMLoadFloat3(&KeyFrames[iCurrentKeyFrame + 1]->vPosition);
			vDestPosition = XMVectorSetW(vDestPosition, 1.f);

			vScale = XMVectorLerp(vSourScale, vDestScale, fRatio);
			vRotation = XMQuaternionSlerp(vSourRotataion, vDestRotation, fRatio);
			vPosition = XMVectorLerp(vSourPosition, vDestPosition, fRatio);
			vPosition = XMVectorSetW(vPosition, 1.f);
		}

		_matrix		TransformMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);

		pAnimChannel->Set_TransformationMatrix(TransformMatrix);
	}

	return S_OK;
}

CAnimation * CAnimation::Create(const char * pName, _float Duration, _float TickPerSecond)
{
	CAnimation*		pInstance = new CAnimation();

	if (FAILED(pInstance->NativeConstruct(pName, Duration, TickPerSecond)))
	{
		MSG_BOX("Failed to Creating Instance (CAnimation) ");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAnimation::Free()
{
	for (auto& pChannel : m_Channels)
		Safe_Release(pChannel);

	m_Channels.clear();
}
