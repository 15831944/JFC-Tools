#include "StdAfx.h"
#include ".\profiling.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CProfiling::CProfiling(void)
{
	if(! m_DlgProfiling.Create(CDlgProfiling::IDD))   //Create failed.
		AfxMessageBox("Error creating Profiling");
}

CProfiling::~CProfiling(void)
{
	m_DlgProfiling.DestroyWindow();
}

void CProfiling::Show(CWnd *pWnd)
{
	m_DlgProfiling.ShowWindow(SW_RESTORE);
	m_DlgProfiling.ShowWindow(SW_SHOW);
	m_DlgProfiling.BringWindowToTop();
}

void CProfiling::Hide()
{
	m_DlgProfiling.ShowWindow(SW_HIDE);
}

void CProfiling::Open(CString path)
{
	m_DlgProfiling.OpenFile(path);
}

BOOL CProfiling::GetState()
{
	return (m_DlgProfiling.IsWindowVisible() && !m_DlgProfiling.IsIconic());
}

BOOL CProfiling::CanClose()
{
	return (m_DlgProfiling.CanClose());
}

void CProfiling::Close()
{
	m_DlgProfiling.Close();
}

//Permet de transmettre un message provenant d'un module
BOOL CProfiling::ForwardMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	return m_DlgProfiling.PostMessage(message, wParam , lParam);
}