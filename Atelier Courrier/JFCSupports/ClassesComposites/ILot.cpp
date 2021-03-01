// ILot.cpp: implementation of the ILot class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ILot.h"
#include "Titre.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ILot::ILot()
{
}

ILot::ILot(JUnt32 ID, const JChar* lbl, JUnt32 parentID) : IComposite(ID, lbl, parentID)
{
}

ILot::~ILot()
{
}


IElem* ILot::GetFilsByID(JUnt32 id)
{
	return m_LstNode.GetNodeByID(id);
}

IElem* ILot::GetFilsByIDAudience(JUnt32 id)
{
	for(m_LstNode.MoveFirst(); m_LstNode.IsValid(); m_LstNode.MoveNext() )
	{
		INode * nd = m_LstNode.GetItem();
		if (nd->IsTitre() && nd->AsITitre()->GetIDAudience() == id)
			return nd;
	}
	return NULL;
}

JVoid ILot::GetFils(JList<JUnt32> & lst) const
{
	for(m_LstNode.MoveFirst(); m_LstNode.IsValid(); m_LstNode.MoveNext() )
		lst.AddTail() = m_LstNode.GetItem()->GetID(); 
}

JVoid ILot::GetFils(JList<IElem*> & lst) const
{
	for(m_LstNode.MoveFirst(); m_LstNode.IsValid(); m_LstNode.MoveNext() )
		lst.AddTail() = m_LstNode.GetItem(); 
}
/*
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

*/