// ExcelProgressSplash.cpp : implementation file
//

#include "stdafx.h"
#include "atprintpdf.h"
#include "ExcelProgressSplash.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CExcelProgressSplash dialog


CExcelProgressSplash::CExcelProgressSplash(CString sTitle, HANDLE hProcess, DWORD dwTimeOut, CWnd* pParent /*=NULL*/)
	:	CDialog(CExcelProgressSplash::IDD, pParent)
	,	m_dwTimerMiliseconds (20)
	,	m_TimerID (0x270679)
	,	m_sTitle(sTitle)
{
	m_hProcess = hProcess;
	m_dwTimeOut = dwTimeOut;
	m_dwTotalTime = 0;
}


void CExcelProgressSplash::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS, m_Progress);
}


BEGIN_MESSAGE_MAP(CExcelProgressSplash, CDialog)
	ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExcelProgressSplash message handlers

void CExcelProgressSplash::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == 0x270679)
	{
		bool bQuitDialog = false;
		m_dwTotalTime += m_dwTimerMiliseconds;
		
		// on a atteint le timeout
		if (m_dwTotalTime > m_dwTimeOut)
		{
			// on quitte
			bQuitDialog = true;
		}

		// on augmente la barre de progression
		int ProgValue = m_Progress.GetPos();
		if (ProgValue >= 100)	m_Progress.SetPos(0);
		else					m_Progress.SetPos(++ProgValue);
		
					
		// on attend que l'evenement envoyé en paramêtre soit envoyé
		DWORD res = ::WaitForSingleObject( m_hProcess, 10);

		// si l'evenement attendu a été recu (: fin du processus)
		if (res == WAIT_OBJECT_0)
		{
			bQuitDialog = true;	
		}
		// si la période d'attente est passée
		else if (WAIT_TIMEOUT)
		{
			// on ne fait rien
		}
			// attente abandonnée
		else if (WAIT_ABANDONED)
		{
			bQuitDialog = true;
		}

		// on doit quitter
		if (bQuitDialog)
		{			
			// on libère le timer
			this->KillTimer(0x270679);

			// on ferme automatiquement le splash
			this->EndDialog(IDOK);	
		}
	}

	// on appelle le gestionnaire de base
	CDialog::OnTimer(nIDEvent);
}

BOOL CExcelProgressSplash::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// on lance un timer
	this->SetTimer(0x270679, m_dwTimerMiliseconds, NULL);


	// on définit l'echelle de la progress bar	
	m_Progress.SetRange(0, 100);

	// on place le curseur de la progress bar à 0
	m_Progress.SetPos(0);
	
	// on change le titre
	SetDlgItemText(IDC_PROGRESS_TITLE, m_sTitle);

	return TRUE;
}

BOOL CExcelProgressSplash::PreTranslateMessage(MSG* pMsg) 
{
	// on bloque l'arrivée du message WM_KEYDOWN afin que l'utilisateur ne puisse fermer la fenêtre avant la fin du thread
	if (pMsg->message == WM_KEYDOWN)
	{
		// on renvoie TRUE pour indiquer que le message ne doit pas être traité	
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);;
}
