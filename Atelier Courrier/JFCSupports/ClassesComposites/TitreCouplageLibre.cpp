// TitreCouplageLibre.cpp: implementation of the CTitreCouplageLibre class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TitreCouplageLibre.h"
#include "..\Visiteurs\VisiteurInsert.h"
#include "..\IDLimits.h"
#include "JSRCPresse.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTitreCouplageLibre::CTitreCouplageLibre()
{
}

CTitreCouplageLibre::CTitreCouplageLibre(JUnt32 ID, JUnt32 nbinsert, JUnt32 idcentre, JUnt32 parentID, INode* hpar )
{
	m_nbInsert = nbinsert;
	m_IDCentre = idcentre;

	m_ID = ID;
	m_ParentID = parentID;
	m_hParent = hpar;


}

CTitreCouplageLibre::~CTitreCouplageLibre()
{
}

JVoid CTitreCouplageLibre::Accept (IVisiteur* v)
{
	v->VisiteTitreCouplageLibre(this);
}

JUnt32 CTitreCouplageLibre::GetIDCentre() const
{
	return m_IDCentre;
}

JArchive& CTitreCouplageLibre::Send(JArchive & lhs, JInt32 Version) const
{
	switch(Version)
	{
	case 1:
		lhs.Send( m_ID );
		//niveau du parent
		if (m_IDCentre == 0 || (IDFAMILLE_MIN < m_IDCentre && m_IDCentre < IDFAMILLE_MAX ))
			lhs.Send( (JInt32)1 );
		if (IDCENTRE_MIN < m_IDCentre && m_IDCentre < IDCENTRE_MAX)
			lhs.Send( (JInt32)2 );
		lhs.Send( m_IDCentre );
		lhs.Send( m_nbInsert );
		break;

	default:
//		TRACE("CTitreCouplageLibre   Invalid Version");
		throw JInternalError::GetInstance(); //("File Version error");
	}
	return lhs;
}

JArchive& CTitreCouplageLibre::Recv(JArchive & lhs, JInt32 Version)
{
	JSRCPresse*  srcpresse = (JSRCPresse*)&JSRCPresse::GetInstance();
	switch(Version)
	{
	case 1:
		lhs.Recv( m_ID );
		JInt32 nvParent;
		lhs.Recv( nvParent );
		lhs.Recv( m_IDCentre );
		lhs.Recv( m_nbInsert );
		break;

	default:
//		TRACE("CTitreCouplageLibre   Invalid Version");
		throw JInternalError::GetInstance(); //("File Version error");
	}
	return lhs;
}