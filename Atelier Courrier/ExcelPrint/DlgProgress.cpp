// DlgProgress.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "resource.h"
#include "DlgProgress.h"
#include "ExcelPrinter.h"
#include ".\dlgprogress.h"

// Boîte de dialogue CDlgXLProgress

IMPLEMENT_DYNAMIC(CDlgXLProgress, CDialog)
CDlgXLProgress::CDlgXLProgress(CXLPrintThread & XLPrintThread, CATPColors::APPCOLOR app, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgXLProgress::IDD, pParent), m_XLPrintThread(XLPrintThread), m_appColor(app)
{
}

CDlgXLProgress::~CDlgXLProgress()
{
}

void CDlgXLProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_XLP_LABEL, m_lbl);
	DDX_Control(pDX, IDC_XLP_PROGRESS2, m_progress2);
	DDX_Control(pDX, IDC_BTNOK, m_btnOK);
}

BEGIN_MESSAGE_MAP(CDlgXLProgress, CDialog)
	ON_BN_CLICKED(IDC_BTNOK, OnBnClickedBtnOK)
END_MESSAGE_MAP()

UINT CDlgXLProgress::Thread(VOID* pParam)
{
	CDlgXLProgress* pDlgXLProgress = (CDlgXLProgress*)pParam;
	pDlgXLProgress->m_XLPrintThread.m_pdlgProgress = pDlgXLProgress;
	if (pDlgXLProgress->m_XLPrintThread.ExecuteMacro())
	{
		pDlgXLProgress->m_progress2.End();
		pDlgXLProgress->PostMessage(WM_COMMAND, MAKEWPARAM(IDOK, 0), NULL);
	}
	return (0);
}

void CDlgXLProgress::Error(CString errorMsg)
{
	SetWindowText("Erreur !");
	m_lbl.SetText(errorMsg);
	m_progress2.ShowWindow(SW_HIDE);
	m_progress2.End();
	m_btnOK.EnableWindow(TRUE);
	m_btnOK.ShowWindow(SW_SHOW);
	Invalidate();
}

// Gestionnaires de messages CDlgXLProgress
BOOL CDlgXLProgress::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_progress2.SetMode(BPC_MODE_BUSY);
	m_progress2.SetSpeed(250);
	m_progress2.Start();
	
	if(m_XLPrintThread.m_params.typeAction==XLParams::SAVE)
		m_lbl.SetText("Enregistrement en cours");
	else if(m_XLPrintThread.m_params.typeAction==XLParams::OPEN)
		m_lbl.SetText("Ouverture en cours");
	else
		m_lbl.SetText("Impression en cours");

	SetColors();

	if (NULL == ::AfxBeginThread(CDlgXLProgress::Thread, (LPVOID)this))
		this->EndDialog(IDCANCEL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION : les pages de propriétés OCX devraient retourner FALSE
}



void CDlgXLProgress::SetColors()
{
	m_progress2.SetColBlockEdge(CATPColors::GetColorSelect(m_appColor));
	m_progress2.SetColBlockEdgeHi(CATPColors::GetColorSelect(m_appColor));

	m_progress2.SetColBlockFace(CATPColors::GetColorLight(m_appColor));
	m_progress2.SetColBlockFaceHi(CATPColors::GetColorDark(m_appColor));
	m_btnOK.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorWhite(), m_appColor);
}

void CDlgXLProgress::OnBnClickedBtnOK()
{
	CDialog::OnOK();
}



BOOL CDlgXLProgress::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		//On court-circuite le traitement des MFC pour ne pas fermer la boite avec la touche Entrée
		::TranslateMessage(pMsg);
		::DispatchMessage(pMsg);
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}
