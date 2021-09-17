#include "..\public\Animation.h"
#include "AnimChannel.h"


CAnimation::CAnimation()
{
}

void CAnimation::Set_LastTime(_float fLastTime)
{
	m_fLastTime = fLastTime;

	// 여기서 보간한 내 뼈의 정보를 저장할 동적배열을 생성한다.
	// 이건 플립북이 아니다 그저 뼈의 순서대로 정보를 하나씩 만 가질것
	_int iSize = (_int)m_Channels.size();
	m_pFrameLerp = new FRAME_LERP[iSize]; 


	ZeroMemory(m_pFrameLerp, sizeof(FRAME_LERP) * iSize);
	
	_uint iChannelNum = 0;
	for (auto& pAnimChannel : m_Channels)
	{
		vector<KEYFRAME*>	KeyFrames = pAnimChannel->Get_KeyFrames();

		_vector vScale, vRotation, vPosition;

		vScale = XMLoadFloat3(&KeyFrames[0]->vScale);
		vRotation = XMLoadFloat4(&KeyFrames[0]->vRotation);
		vPosition = XMLoadFloat3(&KeyFrames[0]->vPosition);
		vPosition = XMVectorSetW(vPosition, 1.f);

		XMStoreFloat3(&m_pFrameLerp[iChannelNum].vSrcScale, vScale);
		XMStoreFloat4(&m_pFrameLerp[iChannelNum].vSrcRotation, vRotation);
		XMStoreFloat3(&m_pFrameLerp[iChannelNum++].vSrcPosition, vPosition);
	}
}

void CAnimation::Set_AnimationIndex_Start(_float fAnimationStart, _float fAnimationStart_Term)
{
	m_IsChange				= true;
	m_fStartTime			= fAnimationStart;
	m_fStartTime_Term		= fAnimationStart_Term;
	m_fAnimationLerpTime	= 0.f;
	//m_isEnd					= false;
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
	if (true == m_IsChange)
	{
		if(false == Change_Animation_Check(TimeDelta))
			return S_OK;
	}

	m_fCurrentTime += fmod(m_TickPerSecond * TimeDelta, m_fLastTime);

	if (m_fCurrentTime >= m_fLastTime)
	{
		m_fCurrentTime = m_fStartTime - 0.1f;

		m_isEnd = true;
		
	}
	else
		m_isEnd = false;

	_uint iChannelNum = 0;
	for (auto& pAnimChannel : m_Channels)
	{
		vector<KEYFRAME*>	KeyFrames = pAnimChannel->Get_KeyFrames();

		KEYFRAME*			pFirst = KeyFrames[(_uint)m_fStartTime];
		KEYFRAME*			pLast	= KeyFrames[(_uint)m_fLastTime];


		_uint iCurrentKeyFrame = (_uint)m_fCurrentTime;//pAnimChannel->Get_CurrentKeyFrame();


		if (true == m_isEnd)
		{		
			iCurrentKeyFrame = (_uint)m_fStartTime;
			pAnimChannel->Set_CurrentKeyFrame(iCurrentKeyFrame);
		}

		_vector			vScale, vRotation, vPosition;

		vScale = XMVectorZero();
		vRotation = XMVectorZero();
		vPosition = XMVectorZero();

		if (m_fCurrentTime < (_float)pFirst->Time /*|| true == m_IsChange*/)
		{
			vScale = XMLoadFloat3(&pFirst->vScale);
			vRotation = XMLoadFloat4(&pFirst->vRotation);
			vPosition = XMLoadFloat3(&pFirst->vPosition);
			vPosition = XMVectorSetW(vPosition, 1.f);

			//_float		fRatio = (m_fCurrentTime - (_float)KeyFrames[iCurrentKeyFrame]->Time) /
			//	((_float)KeyFrames[iCurrentKeyFrame + 1]->Time - (_float)KeyFrames[iCurrentKeyFrame]->Time);
			//
			//_vector		vSourScale, vDestScale;
			//_vector		vSourRotataion, vDestRotation;
			//_vector		vSourPosition, vDestPosition;
			//
			//// 이전 프레임
			//vSourScale = XMLoadFloat3(&KeyFrames[iCurrentKeyFrame]->vScale);
			//vSourRotataion = XMLoadFloat4(&KeyFrames[iCurrentKeyFrame]->vRotation);
			//vSourPosition = XMLoadFloat3(&KeyFrames[iCurrentKeyFrame]->vPosition);
			//vSourPosition = XMVectorSetW(vSourPosition, 1.f);
			//
			//// 다음프레임
			//vDestScale = XMLoadFloat3(&KeyFrames[iCurrentKeyFrame + 1]->vScale);
			//vDestRotation = XMLoadFloat4(&KeyFrames[iCurrentKeyFrame + 1]->vRotation);
			//vDestPosition = XMLoadFloat3(&KeyFrames[iCurrentKeyFrame + 1]->vPosition);
			//vDestPosition = XMVectorSetW(vDestPosition, 1.f);
			//
			//// 선형보간
			//vScale = XMVectorLerp(vSourScale, vDestScale, fRatio);
			//vRotation = XMQuaternionSlerp(vSourRotataion, vDestRotation, fRatio); // +구면선형보간
			//vPosition = XMVectorLerp(vSourPosition, vDestPosition, fRatio);
			//vPosition = XMVectorSetW(vPosition, 1.f);
 		}

		else if (/*true == m_isEnd ||*/ m_fCurrentTime >= (_float)pLast->Time)
		{
			// 끝의 애니메이션과 첫번째 애니메이션을 붙여주자

			vScale = XMLoadFloat3(&pLast->vScale);
			vRotation = XMLoadFloat4(&pLast->vRotation);
			vPosition = XMLoadFloat3(&pLast->vPosition);
			vPosition = XMVectorSetW(vPosition, 1.f);

			//m_fAnimationLerpTime += TimeDelta * 5.f;
			//
			//// 0 ~ 1
			//_float		fRatio = m_fAnimationLerpTime;
			//
			//if (1 <= fRatio)
			//{
			//	fRatio = 1.f;
			//	m_fAnimationLerpTime = 0.f;
			//	m_fCurrentTime = m_fStartTime - 0.5f;
			//	m_isEnd = false;
			//}
			//
			//_vector		vSourScale, vDestScale;
			//_vector		vSourRotataion, vDestRotation;
			//_vector		vSourPosition, vDestPosition;
			//
			//// 내 프레임
			//vSourScale = XMLoadFloat3(&KeyFrames[(_uint)m_fLastTime]->vScale);
			//vSourRotataion = XMLoadFloat4(&KeyFrames[(_uint)m_fLastTime]->vRotation);
			//vSourPosition = XMLoadFloat3(&KeyFrames[(_uint)m_fLastTime]->vPosition);
			//vSourPosition = XMVectorSetW(vSourPosition, 1.f);
			//
			//// 다음프레임
			//vDestScale = XMLoadFloat3(&KeyFrames[(_uint)m_fStartTime]->vScale);
			//vDestRotation = XMLoadFloat4(&KeyFrames[(_uint)m_fStartTime]->vRotation);
			//vDestPosition = XMLoadFloat3(&KeyFrames[(_uint)m_fStartTime]->vPosition);
			//vDestPosition = XMVectorSetW(vDestPosition, 1.f);
			//
			//// 선형보간
			//vScale = XMVectorLerp(vSourScale, vDestScale, fRatio);
			//vRotation = XMQuaternionSlerp(vSourRotataion, vDestRotation, fRatio); // +구면선형보간
			//vPosition = XMVectorLerp(vSourPosition, vDestPosition, fRatio);
			//vPosition = XMVectorSetW(vPosition, 1.f);

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

		XMStoreFloat3(&m_pFrameLerp[iChannelNum].vSrcScale, vScale);
		XMStoreFloat3(&m_pFrameLerp[iChannelNum].vSrcPosition, vPosition);
		XMStoreFloat4(&m_pFrameLerp[iChannelNum].vSrcRotation, vRotation);
		++iChannelNum;

		_matrix		TransformMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);

		pAnimChannel->Set_TransformationMatrix(TransformMatrix);
	}

	return S_OK;
}

_bool CAnimation::Change_Animation_Check(_float TimeDelta)
{
	// 린다...G
	m_fAnimationLerpTime += TimeDelta * 5.f;	

	_vector		vScale, vRotation, vPosition;
	_vector		vSourScale, vDestScale;
	_vector		vSourRotataion, vDestRotation;
	_vector		vSourPosition, vDestPosition;

	_uint iChannelNum = 0;
	_uint iNextKetFrame = (_uint)m_fStartTime;

	for (auto& pAnimChannel : m_Channels)
	{
		vector<KEYFRAME*>	KeyFrames = pAnimChannel->Get_KeyFrames();

		// 비율은 0 ~ 1
		_float		fRatio = m_fAnimationLerpTime;

		if (1.f <= fRatio)
		{
			fRatio = 1.f;
			m_IsChange = false;
			m_fCurrentTime = m_fStartTime - 0.1f;
			m_fLastTime = m_fStartTime + m_fStartTime_Term;
			m_fAnimationLerpTime = 0.f;
			return true;
		}

		vSourScale		= XMLoadFloat3(&m_pFrameLerp[iChannelNum].vSrcScale);
		vSourRotataion	= XMLoadFloat4(&m_pFrameLerp[iChannelNum].vSrcRotation);
		vSourPosition	= XMLoadFloat3(&m_pFrameLerp[iChannelNum++].vSrcPosition);
		vSourPosition	= XMVectorSetW(vSourPosition, 1.f);

		vDestScale		= XMLoadFloat3(&KeyFrames[iNextKetFrame]->vScale);
		vDestRotation	= XMLoadFloat4(&KeyFrames[iNextKetFrame]->vRotation);
		vDestPosition	= XMLoadFloat3(&KeyFrames[iNextKetFrame]->vPosition);
		vDestPosition	= XMVectorSetW(vDestPosition, 1.f);

		// 선형보간
		vScale			= XMVectorLerp(vSourScale, vDestScale, fRatio);
		vRotation		= XMQuaternionSlerp(vSourRotataion, vDestRotation, fRatio); // +구면선형보간
		vPosition		= XMVectorLerp(vSourPosition, vDestPosition, fRatio);
		vPosition		= XMVectorSetW(vPosition, 1.f);

		_matrix		TransformMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);

		pAnimChannel->Set_TransformationMatrix(TransformMatrix);
	}
	return false;
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

	Safe_Delete_Array(m_pFrameLerp);

	m_Channels.clear();
}
