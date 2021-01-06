#include "stdAfx.h"
#include ".\TitreInXListBox.h"
#include "FormatStr.h"


TitreInXListBox::TitreInXListBox(void)
{
	m_ColorAttribue = RGB(0, 0, 255);
}

TitreInXListBox::~TitreInXListBox(void)
{
}

bool TitreInXListBox::IsAttribue(int ID) const
{
	bool bret = false;
	m_mapIDRef.MoveTo(ID);
	if (m_mapIDRef.IsValid() &&	m_mapIDRef.GetItem() > 0 )
		bret = true;

	return bret;
}

int TitreInXListBox::CompareItem( LPCOMPAREITEMSTRUCT lpCompareItemStruct)
{
	return CCompareTitre::CompareTitre(
		((IElem *)lpCompareItemStruct->itemData1)->GetLabel().AsJCharPtr(),
		((IElem *)lpCompareItemStruct->itemData2)->GetLabel().AsJCharPtr());
}