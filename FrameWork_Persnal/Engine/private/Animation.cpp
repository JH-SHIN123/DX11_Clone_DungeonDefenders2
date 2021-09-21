#include "..\public\Animation.h"
#include "AnimChannel.h"
#include "HierarchyNode.h"


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
	m_pFrameLerp_Second = new FRAME_LERP[iSize];

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
		XMStoreFloat3(&m_pFrameLerp[iChannelNum].vSrcPosition, vPosition);

		XMStoreFloat3(&m_pFrameLerp_Second[iChannelNum].vSrcScale, vScale);
		XMStoreFloat4(&m_pFrameLerp_Second[iChannelNum].vSrcRotation, vRotation);
		XMStoreFloat3(&m_pFrameLerp_Second[iChannelNum++].vSrcPosition, vPosition);
	}
}

void CAnimation::Set_AnimationIndex_Start(_float fAnimationStart, _float fAnimationStart_Term)
{
	m_IsChange = true;
	m_fStartTime = fAnimationStart;
	m_fStartTime_Term = fAnimationStart_Term;
	m_fAnimationLerpTime = 0.f;
	//m_isEnd					= false;
}

void CAnimation::Set_AnimationIndex_Start_Second(_float fAnimationStart, _float fAnimationStart_Term)
{
	m_fStartTime_Second = fAnimationStart;
	m_fStartTime_Term_Second = fAnimationStart_Term;
	m_fCurrentTime_Second = fAnimationStart;
	m_fLastTime_Second = fAnimationStart + fAnimationStart_Term;
	m_fAnimationLerpTime_Second = 0.f;
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

HRESULT CAnimation::Update_Transform(_float TimeDelta, _float fFrameSpeed)
{
	if (true == m_IsChange)
	{
		if (false == Change_Animation_Check(TimeDelta))
			return S_OK;
	}

	m_fCurrentTime += fmod(m_TickPerSecond * TimeDelta * fFrameSpeed, m_fLastTime);

	if (m_fCurrentTime >= m_fLastTime)
	{
		m_fCurrentTime = m_fStartTime - 0.1f;

		m_IsEnd = true;
	}
	else
		m_IsEnd = false;

	_uint iChannelNum = 0;
	m_Channels[0]->Get_Name();
	for (auto& pAnimChannel : m_Channels)
	{
		vector<KEYFRAME*>	KeyFrames = pAnimChannel->Get_KeyFrames();

		KEYFRAME*			pFirst = KeyFrames[(_uint)m_fStartTime];
		KEYFRAME*			pLast = KeyFrames[(_uint)m_fLastTime];


		_uint iCurrentKeyFrame = (_uint)m_fCurrentTime;


		if (true == m_IsEnd)
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
		}

		else if (true == m_IsEnd || m_fCurrentTime >= (_float)pLast->Time)
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

		XMStoreFloat3(&m_pFrameLerp[iChannelNum].vSrcScale, vScale);
		XMStoreFloat3(&m_pFrameLerp[iChannelNum].vSrcPosition, vPosition);
		XMStoreFloat4(&m_pFrameLerp[iChannelNum++].vSrcRotation, vRotation);

		_matrix		TransformMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);

		pAnimChannel->Set_TransformationMatrix(TransformMatrix);
	}

	return S_OK;
}

HRESULT CAnimation::Update_Transform_Node(CHierarchyNode * pNode, _float TimeDelta, _float fFrameSpeed)
{
	if (nullptr == pNode)
		return E_FAIL;

	m_fCurrentTime_Second += fmod(m_TickPerSecond * TimeDelta * fFrameSpeed, m_fLastTime_Second);

	if (m_fCurrentTime_Second >= m_fLastTime_Second)
	{
		m_fCurrentTime_Second = m_fStartTime_Second - 0.1f;

		m_IsEnd_Second = true;
	}
	else
		m_IsEnd_Second = false;

	_uint iChannelNum = 0;

	for (auto& pAnimChannel : m_Channels)
	{
		if (!strcmp(pAnimChannel->Get_Name(), pNode->Get_Name()))
		{
			vector<KEYFRAME*>	KeyFrames = pAnimChannel->Get_KeyFrames();

			KEYFRAME*			pFirst = KeyFrames[(_uint)m_fStartTime_Second];
			KEYFRAME*			pLast = KeyFrames[(_uint)m_fLastTime_Second];

			_uint iCurrentKeyFrame = (_uint)m_fCurrentTime_Second;


			if (true == m_IsEnd_Second)
			{
				iCurrentKeyFrame = (_uint)m_fStartTime_Second;
				//(*pNode->Get_AnimChannel())->Set_CurrentKeyFrame(iCurrentKeyFrame);
			}

			_vector			vScale, vRotation, vPosition;

			vScale = XMVectorZero();
			vRotation = XMVectorZero();
			vPosition = XMVectorZero();

			if (m_fCurrentTime_Second < (_float)pFirst->Time /*|| true == m_IsChange*/)
			{
				vScale = XMLoadFloat3(&pFirst->vScale);
				vRotation = XMLoadFloat4(&pFirst->vRotation);
				vPosition = XMLoadFloat3(&pFirst->vPosition);
				vPosition = XMVectorSetW(vPosition, 1.f);
			}

			else if (true == m_IsEnd_Second || m_fCurrentTime_Second >= (_float)pLast->Time)
			{
				vScale = XMLoadFloat3(&pLast->vScale);
				vRotation = XMLoadFloat4(&pLast->vRotation);
				vPosition = XMLoadFloat3(&pLast->vPosition);
				vPosition = XMVectorSetW(vPosition, 1.f);
			}

			else
			{
				if (m_fCurrentTime_Second > KeyFrames[iCurrentKeyFrame + 1]->Time)
					(*pNode->Get_AnimChannel())->Set_CurrentKeyFrame(++iCurrentKeyFrame);

				_float		fRatio = (m_fCurrentTime_Second - (_float)KeyFrames[iCurrentKeyFrame]->Time) /
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
	_uint iNextKeyFrame = (_uint)m_fStartTime;

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
		//50
		vSourScale = XMLoadFloat3(&m_pFrameLerp[iChannelNum].vSrcScale);
		vSourRotataion = XMLoadFloat4(&m_pFrameLerp[iChannelNum].vSrcRotation);
		vSourPosition = XMLoadFloat3(&m_pFrameLerp[iChannelNum++].vSrcPosition);
		vSourPosition = XMVectorSetW(vSourPosition, 1.f);

		vDestScale = XMLoadFloat3(&KeyFrames[iNextKeyFrame]->vScale);
		vDestRotation = XMLoadFloat4(&KeyFrames[iNextKeyFrame]->vRotation);
		vDestPosition = XMLoadFloat3(&KeyFrames[iNextKeyFrame]->vPosition);
		vDestPosition = XMVectorSetW(vDestPosition, 1.f);

		// 선형보간
		vScale = XMVectorLerp(vSourScale, vDestScale, fRatio);
		vRotation = XMQuaternionSlerp(vSourRotataion, vDestRotation, fRatio); // +구면선형보간
		vPosition = XMVectorLerp(vSourPosition, vDestPosition, fRatio);
		vPosition = XMVectorSetW(vPosition, 1.f);

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
	Safe_Delete_Array(m_pFrameLerp_Second);

	m_Channels.clear();
}

void CAnimation::Start_Frame(vector<KEYFRAME*> vecKeyFrame, _uint iCurrentKeyFrame, _fvector vScale, _fvector vRotation, _fvector vPosition)
{
	//_float		fRatio = (m_fCurrentTime - (_float)vecKeyFrame[iCurrentKeyFrame]->Time) /
	//	((_float)vecKeyFrame[iCurrentKeyFrame + 1]->Time - (_float)vecKeyFrame[iCurrentKeyFrame]->Time);
	//
	//_vector		vSourScale, vDestScale;
	//_vector		vSourRotataion, vDestRotation;
	//_vector		vSourPosition, vDestPosition;
	//
	//// 이전 프레임
	//vSourScale = XMLoadFloat3(&vecKeyFrame[iCurrentKeyFrame]->vScale);
	//vSourRotataion = XMLoadFloat4(&vecKeyFrame[iCurrentKeyFrame]->vRotation);
	//vSourPosition = XMLoadFloat3(&vecKeyFrame[iCurrentKeyFrame]->vPosition);
	//vSourPosition = XMVectorSetW(vSourPosition, 1.f);
	//
	//// 다음프레임
	//vDestScale = XMLoadFloat3(&vecKeyFrame[iCurrentKeyFrame + 1]->vScale);
	//vDestRotation = XMLoadFloat4(&vecKeyFrame[iCurrentKeyFrame + 1]->vRotation);
	//vDestPosition = XMLoadFloat3(&vecKeyFrame[iCurrentKeyFrame + 1]->vPosition);
	//vDestPosition = XMVectorSetW(vDestPosition, 1.f);
	//
	//// 선형보간
	//vScale = XMVectorLerp(vSourScale, vDestScale, fRatio);
	//vRotation = XMQuaternionSlerp(vSourRotataion, vDestRotation, fRatio); // +구면선형보간
	//vPosition = XMVectorLerp(vSourPosition, vDestPosition, fRatio);
	//vPosition = XMVectorSetW(vPosition, 1.f);



	//// 끝의 애니메이션과 첫번째 애니메이션을 붙여주자
	//m_fAnimationLerpTime += TimeDelta * 5.f;

	// 0 ~ 1
	//_float		fRatio = m_fAnimationLerpTime;
	//
	//if (1 <= fRatio)
	//{
	//	fRatio = 1.f;
	//	m_fAnimationLerpTime = 0.f;
	//	m_fCurrentTime = m_fStartTime - 0.5f;
	//	m_IsEnd = false;
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
