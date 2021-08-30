#pragma once

#ifndef __ENGINE_FUNCTION_H__

namespace Engine
{
	template<typename T>
	void Safe_Delete(T& Pointer)
	{
		if (nullptr != Pointer)
		{
			delete Pointer;
			Pointer = nullptr;
		}
	}

	template<typename T>
	void Safe_Delete_Array(T& Pointer)
	{
		if (nullptr != Pointer)
		{
			delete[] Pointer;
			Pointer = nullptr;
		}
	}

	/* 레퍼런스 카운트를 감소시킨다. or 삭제한다. */
	template<typename T>
	unsigned long Safe_Release(T& pInstance)
	{
		unsigned long dwRefCnt = 0;

		if (nullptr != pInstance)
		{
			dwRefCnt = pInstance->Release();

			if (0 == dwRefCnt)
				pInstance = nullptr;
		}
		return dwRefCnt;
	}

	template<typename T>
	unsigned long Safe_AddRef(T& pInstance)
	{
		if (nullptr == pInstance)
			return 0;

		unsigned long dwRefCnt = 0;
		
		dwRefCnt = pInstance->AddRef();
	
		return dwRefCnt;
	}	

	class CTagFinder
	{
	public:
		explicit CTagFinder(const wchar_t* pTag)
			: m_pTargetTag(pTag) {		}
		~CTagFinder() {}

	public:
		template<typename T>
		_bool		operator()(const T& Pair)
		{
			if (0 == lstrcmp(m_pTargetTag, Pair.first))
				return true;

			return false;
		}

	private:
		const wchar_t*	m_pTargetTag;

	};

	class CDeleteMap
	{
	public:
		explicit CDeleteMap(void) {}
		~CDeleteMap(void) {}
	public: // operator	
		template <typename T>
		void operator () (T& Pair)
		{
			_ulong dwRefCnt = 0;

			dwRefCnt = Pair.second->Release();

			if (0 == dwRefCnt)
				Pair.second = NULL;
		}
	};
}

#define __ENGINE_FUNCTION_H__
#endif // !__ENGINE_FUNCTION_H__