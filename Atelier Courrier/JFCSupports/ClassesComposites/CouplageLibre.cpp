// CouplageLibre.cpp: implementation of the CCouplageLibre class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CouplageLibre.h"
#include "..\Visiteurs\VisiteurInsert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCouplageLibre::CCouplageLibre()
{
}

CCouplageLibre::CCouplageLibre(JUnt32 ID, const JChar* lbl) : IComposite(ID, lbl, 0)
{
}

CCouplageLibre::~CCouplageLibre()
{
}

JVoid CCouplageLibre::Accept (IVisiteur* v)
{
	v->VisiteCouplageLibre(this);
}

IElem* CCouplageLibre::GetFilsByID(JUnt32 id) const
{
	return m_LstNode.GetNodeByID(id);
}

JInt32 CCouplageLibre::GetNbElem() const
{
	return GetNbFils();
}


JVoid CCouplageLibre::GetFils(JList<JUnt32> & lst) const
{
	for(m_LstNode.MoveFirst(); m_LstNode.IsValid(); m_LstNode.MoveNext() )
		lst.AddTail() = m_LstNode.GetItem()->GetID(); 
}

JVoid CCouplageLibre::GetFils(JList<IElem*> & lst) const
{
	for(m_LstNode.MoveFirst(); m_LstNode.IsValid(); m_LstNode.MoveNext() )
		lst.AddTail() = m_LstNode.GetItem(); 
}

JArchive& CCouplageLibre::Send(JArchive & lhs, JInt32 Version) const
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
//		TRACE("CCouplageLibre   Invalid Version");
		throw JInternalError::GetInstance(); //("File Version error");
	}
	return lhs;
}

JArchive& CCouplageLibre::Recv(JArchive & lhs, JInt32 Version)
{
	CVisiteurInsert vInsert;
	switch(Version)
	{
	case 1:
		INode::Recv(lhs, Version);
		JInt32 nCount;
		lhs.Recv( nCount );
		m_LstNode.Reset();
		CTitreCouplageLibre* titrecouplagelibre;
		for (int i = 0; i< nCount; i++)
		{
			titrecouplagelibre = new CTitreCouplageLibre();
			titrecouplagelibre->Recv(lhs, Version);
			titrecouplagelibre->SetParentNode(this);
			titrecouplagelibre->Accept(&vInsert);
		}
		break;

	default:
//		TRACE("CCouplageLibre   Invalid Version");
		throw JInternalError::GetInstance(); //("File Version error");
	}
	return lhs;	
}

