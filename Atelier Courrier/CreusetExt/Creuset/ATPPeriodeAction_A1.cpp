// on inclut les définitions nécessaires
#include "stdafx.h"
#include "ATPPeriodeAction_A1.h"
#include "ATPPeriodeAction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPPeriodeAction_A1::CATPPeriodeAction_A1()
{
	// on ne fait rien
}

//////////////////
// le destructeur

CATPPeriodeAction_A1::~CATPPeriodeAction_A1()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPPeriodeAction_A1, JFCCalendar)
	//{{AFX_MSG_MAP(CATPPeriodeAction_A1)
	ON_MESSAGE(MSG_CALENDAR_SELCHANGING, OnSelChanging)
	ON_MESSAGE(MSG_CALENDAR_SELCHANGED, OnSelChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////
// CATPPeriodeAction_A1 message handlers

LRESULT CATPPeriodeAction_A1::OnSelChanging(WPARAM, LPARAM)
{
	m_parent->m_A2.Invalidate();
	return 0;
}

LRESULT CATPPeriodeAction_A1::OnSelChanged(WPARAM, LPARAM)
{
//	JDate first, last;
//	GetSelRange(first, last);
//	first.IsValid() && last.IsValid() ?	m_parent->m_btnOk.EnableWindow() : m_parent->m_btnOk.EnableWindow(0);
	m_parent->m_A2.Invalidate();
	return 0;
}
