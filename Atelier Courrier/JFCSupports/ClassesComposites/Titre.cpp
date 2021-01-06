// Titre.cpp: implementation of the CTitre class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Titre.h"
#include "..\AllTypeOfVisitor.h"
#include "..\IDLimits.h"
#include "JSRCPresse.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTitre::CTitre()
{
}

CTitre::CTitre(JUnt32 ID, const JChar* lbl, JUnt32 parentID) : ILeaf(ID, lbl, parentID)
{
	JSRCPresse*  srcpresse = (JSRCPresse*)&JSRCPresse::GetInstance();
	m_Periodicite =  srcpresse->m_TBLTitres.GetPeriodByID(m_ID);
	m_IDAudience =  srcpresse->m_TBLTitres.GetIDAudienceByID(m_ID);
}

CTitre::~CTitre()
{
}

JVoid CTitre::Accept (IVisiteur* v)
{
	v->VisiteTitre(this);
}

JArchive& CTitre::Send(JArchive & lhs, JInt32 Version) const
{
	switch(Version)
	{
	case 1:
		lhs.Send(m_ID);
		//niveau du parent
		if (IDFAMILLE_MIN < m_ParentID && m_ParentID < IDFAMILLE_MAX)
			lhs.Send( (JInt32)1 );
		if (IDCENTRE_MIN < m_ParentID && m_ParentID < IDCENTRE_MAX)
			lhs.Send( (JInt32)2 );
		lhs.Send( m_ParentID );
		break;

	default:
//		TRACE("CTitre   Invalid Version");
		throw JInternalError::GetInstance(); //("File Version error");
	}
	return lhs;
}

JArchive& CTitre::Recv(JArchive & lhs, JInt32 Version)
{
	JSRCPresse*  srcpresse = (JSRCPresse*)&JSRCPresse::GetInstance();
	switch(Version)
	{
	case 1:
		lhs.Recv(m_ID);
		//label
		m_Label = srcpresse->m_TBLTitres.GetLabelByID(m_ID);
		m_Periodicite =  srcpresse->m_TBLTitres.GetPeriodByID(m_ID);
		m_IDAudience =  srcpresse->m_TBLTitres.GetIDAudienceByID(m_ID);
		//niveau du parent
		JInt32 nvParent;
		lhs.Recv( nvParent );
		lhs.Recv( m_ParentID );
		break;

	default:
//		TRACE("CTitre   Invalid Version");
		throw JInternalError::GetInstance(); //("File Version error");
	}
	return lhs;	
}

