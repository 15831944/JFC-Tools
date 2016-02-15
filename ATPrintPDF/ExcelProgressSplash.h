#if !defined(AFX_EXCELPROGRESSSPLASH_H__4970B8EA_5F41_45EF_9390_28850B2DDF0C__INCLUDED_)
#define AFX_EXCELPROGRESSSPLASH_H__4970B8EA_5F41_45EF_9390_28850B2DDF0C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExcelProgressSplash.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExcelProgressSplash dialog

class CExcelProgressSplash : public CDialog
{
// Construction
public:
	CExcelProgressSplash(CString sTitle, HANDLE hProcess, DWORD dwTimeOut, CWnd* pParent = NULL);   // standard constructor


// Dialog Data
	enum { IDD = IDD_EXCEL_SPLASH };
	CProgressCtrl	m_Progress;


// Overrides
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:

	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:

	// variables
	HANDLE					m_hProcess;
	DWORD					m_dwTimeOut;
	DWORD					m_dwTotalTime;

	// constantes
	const DWORD				m_dwTimerMiliseconds;
	const UINT_PTR			m_TimerID;
	const CString			m_sTitle;

};

#endif // !defined(AFX_EXCELPROGRESSSPLASH_H__4970B8EA_5F41_45EF_9390_28850B2DDF0C__INCLUDED_)
