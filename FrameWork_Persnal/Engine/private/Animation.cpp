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

HRESULT CAnimation::Update_Transform(_float TimeDelta, _float fEndTime, _float fNextCurrentTime)
{
	// m_fLastTime
	// 끝났을때 CurrentTime다시세팅할 기준점
	m_fLastTime = fEndTime;

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
			//난 하나의 채널안에서 애니메이션을 전부 돌리는 상황
			//m_fCurrentTime = fNextCurrentTime; // 이거 ㄹ이렇게 때려박는것은 지금의 구조에선 위험하다 (특정범위 안에서만 반복하도록 만든 코드)
			// iCurrentKeyFrame을 적절히 조절 가능하다면 저대로 써도 될거같은데 ㅜ
			iCurrentKeyFrame = 0;
			m_fCurrentTime = 0.f;
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

			// 이전 프레임
			vSourScale = XMLoadFloat3(&KeyFrames[iCurrentKeyFrame]->vScale);
			vSourRotataion = XMLoadFloat4(&KeyFrames[iCurrentKeyFrame]->vRotation);
			vSourPosition = XMLoadFloat3(&KeyFrames[iCurrentKeyFrame]->vPosition);
			vSourPosition = XMVectorSetW(vSourPosition, 1.f);

			// 다음프레임
			vDestScale = XMLoadFloat3(&KeyFrames[iCurrentKeyFrame + 1]->vScale);
			vDestRotation = XMLoadFloat4(&KeyFrames[iCurrentKeyFrame + 1]->vRotation);
			vDestPosition = XMLoadFloat3(&KeyFrames[iCurrentKeyFrame + 1]->vPosition);
			vDestPosition = XMVectorSetW(vDestPosition, 1.f);

			// 선형보간
			vScale = XMVectorLerp(vSourScale, vDestScale, fRatio);
			vRotation = XMQuaternionSlerp(vSourRotataion, vDestRotation, fRatio); // +구면선형보간
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
