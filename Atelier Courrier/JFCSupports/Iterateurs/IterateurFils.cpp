// IterateurFils.cpp: implementation of the CIterateurFils class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IterateurFils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIterateurFils::CIterateurFils(JNodeList* lstNode)
{
	m_plstNode = lstNode;
}

CIterateurFils::~CIterateurFils()
{
}

INode* CIterateurFils::getNodeByID(JUnt32 id)
{
	return m_plstNode->GetNodeByID(id);
}


INode* CIterateurFils::getHead()
{
	m_plstNode->MoveFirst();

	if ( m_plstNode->IsValid() )
		return m_plstNode->GetItem();
	else
		return NULL;
}

INode* CIterateurFils::getNext()
{
	m_plstNode->MoveNext();
	if ( !m_plstNode->IsValid() )
		return NULL;
	return m_plstNode->GetItem();
}

JInt32 CIterateurFils::nbElem()
{
	return m_plstNode->GetCount();
}

