#include "StdAfx.h"
#include "PeriodTree.h"
#include "DlgFamille.h"

#include <AllTypeOfNode.h>
#include <AllTypeOfIterator.h>
#include <IDLimits.h>

CPeriodTree::CPeriodTree(void)
{
}

CPeriodTree::~CPeriodTree(void)
{
}

bool CPeriodTree::IsAttribue(int ID) const
{
	bool bRet = false;

	IElem* elem = m_pDlgParent->m_pRegroupements->GetIElemByID(ID);
	if (elem && elem->IsTitre())
	{
		if (m_pDlgParent->m_lstPeriodicite.GetSel(
			m_pDlgParent->m_lstPeriodicite.FindItemIndex(
			elem->AsITitre()->GetPeriodicite())))
			bRet = true;
	}
	return bRet;
}