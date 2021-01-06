// Lot.cpp: implementation of the CLot class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Lot.h"
#include "..\Visiteurs\VisiteurInsert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLot::CLot()
{
}

CLot::CLot(JUnt32 ID, const JChar* lbl) : ILot(ID, lbl, 0)
{
}

CLot::~CLot()
{
}

JVoid CLot::Accept (IVisiteur* v)
{
	v->VisiteLot(this);
}
/*
IElem* CLot::GetFilsByID(JUnt32 id)
{
	return m_LstNode.GetNodeByID(id);
}

IElem* CLot::GetFilsByIDAudience(JUnt32 id)
{
	for(m_LstNode.MoveFirst(); m_LstNode.IsValid(); m_LstNode.MoveNext() )
	{
		if (m_LstNode.GetItem()->IsTitre()
			&&
			m_LstNode.GetItem()->AsITitre()->GetIDAudience() == id)
			return m_LstNode.GetItem();
	}
	return NULL;
}

JVoid CLot::GetFils(JList<JUnt32> & lst) const
{
	for(m_LstNode.MoveFirst(); m_LstNode.IsValid(); m_LstNode.MoveNext() )
		lst.AddTail() = m_LstNode.GetItem()->GetID(); 
}

JVoid CLot::GetFils(JList<IElem*> & lst) const
{
	for(m_LstNode.MoveFirst(); m_LstNode.IsValid(); m_LstNode.MoveNext() )
		lst.AddTail() = m_LstNode.GetItem(); 
}
*/
JArchive& CLot::Send(JArchive & lhs, JInt32 Version) const
{
	switch(Version)
	{
	case 1:
		INode::Send(lhs, Version);
		lhs.Send( m_LstNode.GetCount() );
		for ( m_LstNode.MoveFirst(); m_LstNode.IsValid(); m_LstNode.MoveNext() )
			m_LstNode.GetItem()->Send(lhs, Version);
		break;

	default:
//		TRACE("CLot   Invalid Version");
		throw JInternalError::GetInstance(); //("File Version error");
	}
	return lhs;
}

JArchive& CLot::Recv(JArchive & lhs, JInt32 Version)
{
	CVisiteurInsert vInsert;
	switch(Version)
	{
	case 1:
		INode::Recv(lhs, Version);
		JInt32 nCount;
		lhs.Recv( nCount );
		m_LstNode.Reset();
		CTitreLot* titrelot;
		for (int i = 0; i< nCount; i++)
		{
			titrelot = new CTitreLot();
			titrelot->Recv(lhs, Version);
			titrelot->SetParentNode(this);
			titrelot->Accept(&vInsert);
		}
		break;

	default:
//		TRACE("CLot   Invalid Version");
		throw JInternalError::GetInstance(); //("File Version error");
	}
	return lhs;	
}

