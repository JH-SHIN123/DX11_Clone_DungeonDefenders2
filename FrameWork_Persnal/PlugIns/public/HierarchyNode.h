#pragma once

#ifndef __HIERARCHYNODE_H__

#include "Base.h"

BEGIN(Engine)
class CHierarcyNode final :	public CBase
{
public:
	CHierarcyNode();
	virtual ~CHierarcyNode() = default;

public:
	const char* Get_Name() const { return m_szNodeName; }
	const _uint Get_Depth() const { return m_iDepth; }
	_fmatrix Get_CombindTransformationMatrix() const { return XMLoadFloat4x4(&m_CombinedTransformationMatrix); }

public:
	HRESULT NativeConstruct(char* pName, _fmatrix TransformationMatrix, CHierarcyNode* pParent, _uint iDepth);
	void Update_CombindTransformationMatrix();

private:
	char			m_szNodeName[MAX_PATH] = "";	
	_float4x4		m_TransformationMatrix;
	_float4x4		m_CombinedTransformationMatrix;
	CHierarcyNode*	m_pParent = nullptr;
	_uint			m_iDepth = 0;				// 몇대째인지 (재귀호출을 할것이기 때문에 재귀호출 회수 = 깊이)

public:
	static CHierarcyNode* Create(char* pName, _fmatrix TransformationMatrix, CHierarcyNode* pParent = nullptr, _uint iDepth = 0);
	virtual void Free() override;
};

END
#define __HIERACHYNODE_H__
#endif // !__HIERACHYNODE_H__



/* 노드의 계층을 담아둘 클래스 */
/*

뼈
정점이 움직이기 위해서는 어떻게 해야할까? 그 정점의 로컬 좌표를 움직여주면 된다.
생각해보자 단순한 형태의 메쉬를 사용하는것이 아닌 상당히 복잡한 형태의 메쉬를 사용한다.
사람 형태의 메쉬가 있다고 가정하고 이 사람은 大자 형태로 동작을 취하고있다가(정점이 그렇게 세팅되어 있다가 옳다)
팔을 뻗어 만세를 한다고 했을때 어깨부터 손가락 끝까지의 범위안에 있는 모든 정점을 올려 줄 것인가?
그렇기엔 너무나 빈번하고 많은양의 정점을 하나하나 세팅해야한다는 불편함이 생긴다.
그렇다면 실제의 사람처럼 안에 뼈를 넣어 뼈를 기준으로 움직이게 한다면?

노드
씬이 가지는 최고참 노드 = Root Node
루트노드는 자식노드들을 가지고 있으며
자신노드의 자식노드와 메쉬의 인덱스(배열) 등등을 가지고 있다.
노드의 행렬과 뼈의 행렬을 이용해 정점의 위치를 변경 가능하다.

*/