// TitreLot.cpp: implementation of the CTitreLot class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TitreLot.h"

#include "..\Visiteurs\VisiteurInsert.h"
#include "JTBLTitres.h"
#include "JSRCPresse.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTitreLot::CTitreLot()
{
}

CTitreLot::CTitreLot( JUnt32 ID, JUnt32 idcentre, JUnt32 parentID, INode* hpar)
{
	m_IDCentre = idcentre;
	m_ID = ID;
	m_ParentID = parentID;
	m_hParent = hpar;
}

CTitreLot::~CTitreLot()
{
}

JVoid CTitreLot::Accept (IVisiteur* v)
{
	v->VisiteTitreLot(this);
}

JUnt32 CTitreLot::GetIDCentre() const
{
	return m_IDCentre;
}

JUnt32 CTitreLot::GetPeriodicite() const
{
	JSRCPresse*  srcpresse = (JSRCPresse*)&JSRCPresse::GetInstance();
	return srcpresse->m_TBLTitres.GetPeriodByID(m_ID);
}


JArchive& CTitreLot::Send(JArchive & lhs, JInt32 Version) const
{
	switch(Version)
	{
	case 1:
		lhs.Send( m_ID );
		lhs.Send( m_IDCentre );
		break;

	default:
//		TRACE("CTitreLot   Invalid Version");
		throw JInternalError::GetInstance(); //("File Version error");
	}
	return lhs;
}

JArchive& CTitreLot::Recv(JArchive & lhs, JInt32 Version)
{
	switch(Version)
	{
	case 1:
		lhs.Recv( m_ID );
		lhs.Recv( m_IDCentre );
		//recuperer le label
		break;

	default:
		throw JInternalError::GetInstance(); //("File Version error");
	}
	return lhs;
}