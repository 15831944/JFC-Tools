// Centre.cpp: implementation of the CCentre class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Centre.h"

#include "..\Visiteurs\VisiteurInsert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCentre::CCentre()
{
}

CCentre::CCentre(JUnt32 ID, const JChar* lbl, JUnt32 parentID) : ILot(ID, lbl, parentID)
{
}

CCentre::~CCentre()
{
}

JVoid CCentre::Accept (IVisiteur* v)
{
	v->VisiteCentre(this);
}

JArchive& CCentre::Send(JArchive & lhs, JInt32 Version) const
{
	switch(Version)
	{
	case 1:
		INode::Send(lhs, Version);
		lhs.Send( m_ParentID );
		break;

	default:
//		TRACE("CCentre   Invalid Version");
		throw JInternalError::GetInstance(); //("File Version error");
	}
	return lhs;
}

JArchive& CCentre::Recv(JArchive & lhs, JInt32 Version)
{
	switch(Version)
	{
	case 1:
		INode::Recv(lhs, Version);
		lhs.Recv( m_ParentID );
		break;

	default:
//		TRACE("CCentre   Invalid Version");
		throw JInternalError::GetInstance(); //("File Version error");
	}
	return lhs;	
}

