#include "stdafx.h"
#include "resource.h"
#include ".\TitreApparie.h"
#include "JSRCPresse.h"
#include "..\IDLimits.h"

CTitreApparie::CTitreApparie(void)
{
	m_ID = 0;
	m_IDCentre = 0;
	m_IDTitreTemoin = 0;
	m_MediaWeight = 0;
}

CTitreApparie::CTitreApparie( JUnt32 id, JUnt32 idTitre, JUnt32 idCentre, JUnt32 idTitreTemoin, JUnt32 mw)
{
	JSRCPresse*  srcpresse = (JSRCPresse*)&JSRCPresse::GetInstance();
	m_ID = id;
	//on fabrique le lbl
	CString label;
	label.FormatMessage(IDS_SU_LBLAPPARIE,
							srcpresse->m_TBLTitres.GetLabelByID(idTitre),
							srcpresse->m_TBLTitres.GetLabelByID(idTitreTemoin),
							mw);
	m_Label = label;

	m_IDTitre = idTitre;
	m_IDCentre = idCentre;
	m_IDTitreTemoin = idTitreTemoin;
	m_MediaWeight = mw;


	m_Periodicite =  srcpresse->m_TBLTitres.GetPeriodByID(m_IDTitre);
	m_IDAudience =  srcpresse->m_TBLTitres.GetIDAudienceByID(m_IDTitreTemoin);
}

CTitreApparie::~CTitreApparie(void)
{
}

//JVoid CTitreApparie::SetLabel(const JChar* lbl)
//{
//	JSRCPresse*  srcpresse = (JSRCPresse*)&JSRCPresse::GetInstance();
//	//on fabrique le lbl
//	CString label;
//	label.FormatMessage(IDS_SU_LBLAPPARIE,
//	srcpresse->m_TBLTitres.GetLabelByID(m_IDTitre),
//	srcpresse->m_TBLTitres.GetLabelByID(m_IDTitreTemoin),
//	m_MediaWeight);
//
//	m_Label = label;
//}

JUnt32 CTitreApparie::GetIDTitre() const
{
	return m_IDTitre;
}

JArchive& CTitreApparie::Recv( JArchive& lhs, JInt32 Version )
{
	JSRCPresse*  srcpresse = (JSRCPresse*)&JSRCPresse::GetInstance();
	switch(Version)
	{
		case 1:
		{
			lhs.Recv(m_ID);
			lhs.Recv(m_IDTitre);
			m_Label.Recv(lhs);
			lhs.Recv(m_IDTitreTemoin);
			JInt32 nvParent;
			lhs.Recv( nvParent );
			lhs.Recv(m_IDCentre);
			lhs.Recv(m_MediaWeight);
			m_Periodicite =  srcpresse->m_TBLTitres.GetPeriodByID(m_IDTitre);
			m_IDAudience =  srcpresse->m_TBLTitres.GetIDAudienceByID(m_IDTitreTemoin);
		}
		break;

		default:
			TRACE("CTitreApparie   Invalid Version");
			throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
	return lhs;
}

JArchive& CTitreApparie::Send( JArchive& lhs, JInt32 Version ) const
{
	switch(Version)
	{
		case 1:
		{
			lhs.Send(m_ID);
			lhs.Send(m_IDTitre);
			m_Label.Send(lhs);
			lhs.Send(m_IDTitreTemoin);
			//niveau du parent
			if (m_IDCentre == 0 || (IDFAMILLE_MIN < m_IDCentre && m_IDCentre < IDFAMILLE_MAX ))
				lhs.Send( (JInt32)1 );
			if (IDCENTRE_MIN < m_IDCentre && m_IDCentre < IDCENTRE_MAX)
				lhs.Send( (JInt32)2 );
			lhs.Send(m_IDCentre);
			lhs.Send(m_MediaWeight);
		}
		break;

		default:
			TRACE("CTitreApparie   Invalid Version");
			throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
	return lhs;
}