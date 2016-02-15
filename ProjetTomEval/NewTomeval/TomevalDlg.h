// TomevalDlg.h : header file
//

#if !defined(AFX_TOMEVALDLG_H__E79B55E7_DE32_4D88_841D_8A8CB8D5295F__INCLUDED_)
#define AFX_TOMEVALDLG_H__E79B55E7_DE32_4D88_841D_8A8CB8D5295F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTomevalDlg dialog
#ifdef OPTION_CARAT
#include "tomcarat.h"
#elif OPTION_JFC
#include "tom_msg.h"
#else
ERREUR: OPTION non définie
#endif
#include "Tomeval.h"
#include "TomevalCarat.h"
//##ModelId=3D9AB7D4012C
class CTomevalDlg : public CDialog
{
// Construction
public:
	//##ModelId=3D9AB7D40141
	CTomevalDlg(CWnd* pParent = NULL);	// standard constructor

	//##ModelId=3D9AB7D4014A
    afx_msg LRESULT OnMsgTom_TomjobReady(WPARAM wParam, LPARAM lParam);
	//##ModelId=3D9AB7D40154
    afx_msg LRESULT OnMsgTom_AuRevoir(WPARAM wParam, LPARAM lParam);
	//##ModelId=3D9AB7D4015E
    afx_msg LRESULT OnMsgTom_BreakCalcul(WPARAM wParam, LPARAM lParam);

	//##ModelId=3D9AB7D40168
    afx_msg LRESULT OnMsgTom_TOMCDZReady(WPARAM wParam, LPARAM lParam);
	//##ModelId=3D9AB7D4016C
    afx_msg LRESULT OnMsgTom_TOMCDZEnd(WPARAM wParam, LPARAM lParam);
	//##ModelId=3D9AB7D40175
    afx_msg LRESULT OnMsgTom_TOMCDZGetRestom(WPARAM wParam, LPARAM lParam);
	//##ModelId=3D9AB7D4017F
    afx_msg LRESULT OnMsgTom_AddSpot(WPARAM wParam, LPARAM lParam);
	//##ModelId=3D9AB7D40190
    afx_msg LRESULT OnMsgTom_AddZoom(WPARAM wParam, LPARAM lParam);
	//##ModelId=3D9AB7D4019A
    afx_msg LRESULT OnMsgTom_GetLien(WPARAM wParam, LPARAM lParam);

	//##ModelId=3D9AB7D401A4
    afx_msg LRESULT OnMsgTom_Info1(WPARAM wParam, LPARAM lParam);
	//##ModelId=3D9AB7D401AE
    afx_msg LRESULT OnMsgTom_Info2(WPARAM wParam, LPARAM lParam);
	//##ModelId=3D9AB7D401B8
    afx_msg LRESULT OnMsgTom_RestomReady(WPARAM wParam, LPARAM lParam);
	//##ModelId=3D9AB7D401C2
    afx_msg LRESULT OnMsgTom_HideTomeval(WPARAM wParam, LPARAM lParam);



// Dialog Data
	//{{AFX_DATA(CTomevalDlg)
	enum { IDD = IDD_TOMEVAL_DIALOG };
	//##ModelId=3D9AB7D401CC
	int		m_NrLien;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTomevalDlg)
	protected:
	//##ModelId=3D9AB7D401CD
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=3D9AB7D401D7
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTomevalDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtomTomjobReady();
	afx_msg void OnButtonAUREVOIR();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	afx_msg void OnButton6();
	afx_msg void OnButton7();
	afx_msg void OnButton8();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	//##ModelId=3D9AB7D4023B
#ifdef OPTION_CARAT
	CTomevalCarat m_TomEval; // Calcul Tomeval principal
#else
	CTomEval m_TomEval; // Calcul Tomeval principal
	CCalculMarginal m_TomMarginal; // Calcul Tomeval Marginal
#endif
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOMEVALDLG_H__E79B55E7_DE32_4D88_841D_8A8CB8D5295F__INCLUDED_)
