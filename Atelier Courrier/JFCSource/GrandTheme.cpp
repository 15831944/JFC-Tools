// GrandTheme.cpp: implementation of the CGrandTheme class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GrandTheme.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGrandTheme::CGrandTheme()
{
}

CGrandTheme::~CGrandTheme()
{
}

JArchive& CGrandTheme::Recv( JArchive& lhs, JInt32 Version)
{
	switch(Version)
	{
		case 1:
		{
			lhs.Recv(m_ID);
			m_Label.Recv(lhs);

			// Get number of themes
			JInt32 Count;
			lhs.Recv(Count);

			m_vThemes.SetCount(Count);

			for(m_vThemes.MoveFirst(); m_vThemes.IsValid(); m_vThemes.MoveNext())
			{
				PTR_THEME pItem(new CTheme());

				pItem->Recv(lhs, Version);

				m_vThemes.GetItem() = pItem;

				m_mThemeID.Add(m_vThemes.GetItem()->GetID()) = m_vThemes.GetIndex();
			}
		}
		break;

		default:
			TRACE("CGrandTheme Version error");
			throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
	return lhs;
}

JBool	CGrandTheme::FillTree(CTerrain *pTerrain, CTreeCtrl &Tree, HTREEITEM hParent, JBool AllLeaves, JBool OnlyQuanti)
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

	HTREEITEM hPar			=	Tree.InsertItem(&Ins);

	for(m_vThemes.MoveFirst(); m_vThemes.IsValid(); m_vThemes.MoveNext())
	{
		m_vThemes.GetItem()->FillTree(pTerrain, Tree, hPar, AllLeaves,OnlyQuanti);
	}
	return true;
}
