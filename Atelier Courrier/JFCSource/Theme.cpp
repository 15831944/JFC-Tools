// Theme.cpp: implementation of the CTheme class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Theme.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTheme::CTheme()
{
	m_ID = 0;
	m_Label = "";
}

CTheme::~CTheme()
{
}

JArchive& CTheme::Recv( JArchive& lhs, JInt32 Version)
{
	switch(Version)
	{
		case 1:
		{
			// Get ID & Libelle
			lhs.Recv(m_ID);
			m_Label.Recv(lhs);

			// Get number of blocs
			JInt32 Count;
			lhs.Recv(Count);

			m_vBlocs.SetCount(Count);

			for(m_vBlocs.MoveFirst(); m_vBlocs.IsValid(); m_vBlocs.MoveNext())
			{
				PTR_BLOCQUESTIONS pItem(new CBlocQuestions);

				pItem->Recv(lhs, Version);

				m_vBlocs.GetItem() = pItem;

				m_mBlocID.Add(m_vBlocs.GetItem()->GetID())= m_vBlocs.GetIndex();
			}
		}
		break;

		default:
			TRACE("CTheme Invalid Version");
			throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
	return lhs;
}

JBool CTheme::FillTree(CTerrain *pTerrain, CTreeCtrl &Tree, HTREEITEM hParent, JBool AllLeaves, JBool OnlyQuanti)
{
	TVINSERTSTRUCT Ins;
	Ins.hParent				=	hParent;
	Ins.hInsertAfter		=	NULL;
	Ins.item.mask			=	TVIF_CHILDREN|TVIF_PARAM|TVIF_STATE|TVIF_TEXT;
	Ins.item.state			=	(0x0001)|INDEXTOSTATEIMAGEMASK(1);
	Ins.item.stateMask		=	TVIS_STATEIMAGEMASK|0x000F ;
	Ins.item.pszText		=	(char *)m_Label.AsJCharPtr();
	Ins.item.cchTextMax		=	255;
	Ins.item.iImage			=	0;
	Ins.item.iSelectedImage	=	0;
	Ins.item.cChildren		=	1;
	Ins.item.lParam			=	(DWORD)AsITheme();	

	HTREEITEM hPar = Tree.InsertItem(&Ins);

	for(m_vBlocs.MoveFirst(); m_vBlocs.IsValid(); m_vBlocs.MoveNext())
	{
		m_vBlocs.GetItem()->FillTree(pTerrain, Tree, hPar, AllLeaves, OnlyQuanti);
	}
	return true;
}
