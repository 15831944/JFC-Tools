// MemPositionWnd.cpp : fichier d'implémentation
//
#include "stdafx.h"

#include "MemPositionWnd.h"

#include "JFCConfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Boîte de dialogue CMemPositionWnd

CMemPositionWnd::CMemPositionWnd()
{
}

CMemPositionWnd::~CMemPositionWnd()
{
}

void CMemPositionWnd::LoadPos(CWnd * wnd)
{
	CString strText;

	CRuntimeClass* pClass = wnd->GetRuntimeClass();
	strText = GETPROFILESTRINGD(SECTION_OPTIONS, pClass->m_lpszClassName, 0);

	if (strText.IsEmpty())
	    return;

	SetPos(wnd, strText);
}

//void CMemPositionWnd::LoadPos(CWnd * wnd, CRect rectDefault)
//{
//	CString strText;
//	WINDOWPLACEMENT wndpl;
//	CRect rect;
//
//	CRuntimeClass* pClass = wnd->GetRuntimeClass();
//	strText = GETPROFILESTRING(SECTION_OPTIONS, pClass->m_lpszClassName, 0);
//	if (!strText.IsEmpty())
//	{
//		rect.left = atoi((const char*) strText);
//		rect.top = atoi((const char*) strText + 5);
//		rect.right = atoi((const char*) strText + 10);
//		rect.bottom = atoi((const char*) strText + 15);
//	}
//	else 
//	    rect = rectDefault;
//
//
//	wndpl.length = sizeof(WINDOWPLACEMENT);
//	wndpl.showCmd = SW_HIDE;
//	wndpl.ptMinPosition = CPoint(0, 0);
//	wndpl.ptMaxPosition =  CPoint(-::GetSystemMetrics(SM_CXBORDER), -::GetSystemMetrics(SM_CYBORDER));
//	wndpl.rcNormalPosition = rect;
//
//	wnd->SetWindowPlacement(&wndpl);
//}

void CMemPositionWnd::LoadPos(CWnd * wnd, CString strRectDefault)
{
	CString strText;

	CRuntimeClass* pClass = wnd->GetRuntimeClass();
	strText = GETPROFILESTRINGD(SECTION_OPTIONS, pClass->m_lpszClassName, 0);

	if (strText.IsEmpty())
	    strText = strRectDefault;

	SetPos(wnd, strText);
}

void CMemPositionWnd::SetPos(CWnd * wnd, CString strRect)
{
	WINDOWPLACEMENT wndpl;
	CRect rect;

	rect.left = atoi((const char*) strRect);
	rect.top = atoi((const char*) strRect + 5);
	rect.right = atoi((const char*) strRect + 10);
	rect.bottom = atoi((const char*) strRect + 15);

	wndpl.length = sizeof(WINDOWPLACEMENT);
	wndpl.showCmd = SW_HIDE;
	wndpl.ptMinPosition = CPoint(0, 0);
	wndpl.ptMaxPosition =  CPoint(-::GetSystemMetrics(SM_CXBORDER), -::GetSystemMetrics(SM_CYBORDER));
	wndpl.rcNormalPosition = rect;

	wnd->SetWindowPlacement(&wndpl);
}


void CMemPositionWnd::SavePos(CWnd * wnd)
{
	CString strText;

	WINDOWPLACEMENT wndpl;
	wndpl.length = sizeof(WINDOWPLACEMENT);
	// lit la position courante de la fenêtre et
	// l'état (zoomé/réduit)
	wnd->GetWindowPlacement(&wndpl);

	strText.Format("%04d %04d %04d %04d",
		wndpl.rcNormalPosition.left,
		wndpl.rcNormalPosition.top,
		wndpl.rcNormalPosition.right,
		wndpl.rcNormalPosition.bottom);

	CRuntimeClass* pClass = wnd->GetRuntimeClass();
	WRITEPROFILESTRING(SECTION_OPTIONS, pClass->m_lpszClassName, strText);
}
