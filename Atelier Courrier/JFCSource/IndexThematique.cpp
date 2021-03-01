// IndexThematique.cpp: implementation of the CIndexThematique class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IndexThematique.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIndexThematique::CIndexThematique()
{
}

CIndexThematique::~CIndexThematique()
{
}

JArchive& CIndexThematique::Recv(JArchive& lhs, JInt32 Version)
{
	switch(Version)
	{
		case 1:
		{
			// Get number of grandthemes
			JInt32 Count;
			lhs.Recv(Count);

			m_vGrandThemes.SetCount(Count);

			for( m_vGrandThemes.MoveFirst(); m_vGrandThemes.IsValid(); m_vGrandThemes.MoveNext())
			{
				PTR_GRANDTHEME pItem(new CGrandTheme);

				pItem->Recv(lhs, Version);

				m_vGrandThemes.GetItem() = pItem;

				// Add grand theme to the map
				m_mGrandThemes.Add(m_vGrandThemes.GetItem()->GetID()) = m_vGrandThemes.GetIndex();
			}
		}
		break;

		default:
			TRACE("CIndexThematique Version error");
			throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
	return lhs;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Remplissage arbre index thématique  
JBool	CIndexThematique::FillTree(CTerrain *pTerrain, CTreeCtrl &Tree, HTREEITEM hParent, JBool AllLeaves, JBool OnlyQuanti)
{
	for(m_vGrandThemes.MoveFirst(); m_vGrandThemes.IsValid(); m_vGrandThemes.MoveNext())
	{
		m_vGrandThemes.GetItem()->FillTree(pTerrain, Tree,hParent,AllLeaves, OnlyQuanti);
	}
	return true;
}
