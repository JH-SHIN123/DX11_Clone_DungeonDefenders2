#include "..\public\Base.h"

CBase::CBase()
{

}

unsigned long CBase::AddRef()
{
	return ++m_dwRefCnt;
}

/* unsigned long : 삭제가 잘 됐니 안됐니?! */
unsigned long CBase::Release()
{
	if (0 == m_dwRefCnt)
	{
		Free();

		delete this;
		/* 삭제처리 .*/
		return 0;
	}
	else
		return m_dwRefCnt--;	
}



