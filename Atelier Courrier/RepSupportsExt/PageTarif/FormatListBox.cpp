#include "stdAfx.h"
#include "FormatListBox.h"
#include "DlgTarif.h"


CFormatListBox::CFormatListBox(void)
{
m_dlgParent = NULL;
}

CFormatListBox::~CFormatListBox(void)
{
}

bool CFormatListBox::IsAttribue(int ID) const
{
	bool bret = false;
	if(m_dlgParent)
	{
		JDate date, dateF;
		COleDateTime oledate;
		oledate.GetCurrentTime();
		date.SetDate(oledate.GetDay(), oledate.GetMonth(), oledate.GetYear());
		dateF.SetDate(31, 12, oledate.GetYear());

		if (m_dlgParent->m_pTarifs->GetStatutTarif(m_dlgParent->GetSelectedSupport(), ID, date, dateF) >0)
			bret = true;
	}
	return bret;
}

