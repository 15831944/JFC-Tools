// TitreApparente.cpp: implementation of the CTitreApparente class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "TitreApparente.h"
#include "..\Visiteurs\VisiteurInsert.h"
#include "..\IDLimits.h"
#include "JSRCPresse.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTitreApparente::CTitreApparente()
{
}

CTitreApparente::CTitreApparente(JUnt32 ID, const JChar* lbl, JUnt32 idsupptuteur, JUnt32 mw, JUnt32 parentID)
{
	JSRCPresse*  srcpresse = (JSRCPresse*)&JSRCPresse::GetInstance();

	m_ID = ID;


	m_ParentID = parentID;


	m_IDsupportTuteur = idsupptuteur;
	m_MediaWeight = mw;

	m_Periodicite =  srcpresse->m_TBLTitres.GetPeriodByID(m_IDsupportTuteur);
	m_IDAudience =  srcpresse->m_TBLTitres.GetIDAudienceByID(m_IDsupportTuteur);

	//on fabrique le lbl
	SetLabel(lbl);
}

CTitreApparente::~CTitreApparente()
{
}

JVoid CTitreApparente::SetLabel(const JChar* lbl)
{
	CString tmp;
	tmp.Format("%s", lbl);

	JSRCPresse*  srcpresse = (JSRCPresse*)&JSRCPresse::GetInstance();
	//on fabrique le lbl
	CString label;
	
	if(tmp.IsEmpty())
	{
		label.FormatMessage(IDS_SU_LBLAPPARENTE,
		srcpresse->m_TBLTitres.GetLabelByID(m_IDsupportTuteur),
		m_MediaWeight);
	}
	else
	{
		label.FormatMessage(IDS_SU_LBLAPPARENTE_NOM,
		tmp,
		srcpresse->m_TBLTitres.GetLabelByID(m_IDsupportTuteur),
		m_MediaWeight);
	}

	m_Label = label;
}

CString CTitreApparente::GetLabelForEdit () const
{
	CString out = m_Label.AsJCharPtr();
	int cur = 0;
	CString tmp = out.Tokenize("(", cur);
	if (tmp == out)
		return "";
	tmp.Trim();
	return tmp;
}

JVoid CTitreApparente::Accept (IVisiteur* v)
{
	v->VisiteTitreApparente(this);
}

JUnt32 CTitreApparente::GetSupportTuteur() const
{
	return m_IDsupportTuteur;
}

JUnt32 CTitreApparente::GetMediaWeight() const
{
	return m_MediaWeight;
}

JVoid  CTitreApparente::SetMediaWeight(JUnt32 mw)
{
	m_MediaWeight = mw;
}

JArchive& CTitreApparente::Send(JArchive & lhs, JInt32 Version) const
{
	switch(Version)
	{
	case 1:
		INode::Send(lhs, Version);
		//niveau du parent
		if (m_ParentID == 0 || (IDFAMILLE_MIN < m_ParentID && m_ParentID < IDFAMILLE_MAX ))
			lhs.Send( (JInt32)1 );
		if (IDCENTRE_MIN < m_ParentID && m_ParentID < IDCENTRE_MAX)
			lhs.Send( (JInt32)2 );
		lhs.Send( m_ParentID );
		lhs.Send( m_IDsupportTuteur );
		lhs.Send( m_MediaWeight );
		break;

	default:
//		TRACE("CTitreApparente   Invalid Version");
		throw JInternalError::GetInstance(); //("File Version error");
	}
	return lhs;
}

JArchive& CTitreApparente::Recv(JArchive & lhs, JInt32 Version)
{
	JSRCPresse*  srcpresse = (JSRCPresse*)&JSRCPresse::GetInstance();
	switch(Version)
	{
	case 1:
		INode::Recv(lhs, Version);
		//niveau du parent
		JInt32 nvParent;
		lhs.Recv( nvParent );
		lhs.Recv( m_ParentID );
		lhs.Recv( m_IDsupportTuteur );
		lhs.Recv( m_MediaWeight );
		m_Periodicite =  srcpresse->m_TBLTitres.GetPeriodByID(m_IDsupportTuteur);
		m_IDAudience =  srcpresse->m_TBLTitres.GetIDAudienceByID(m_IDsupportTuteur);
		break;

	default:
//		TRACE("CTitreApparente   Invalid Version");
		throw JInternalError::GetInstance(); //("File Version error");
	}
	return lhs;
}