// on inclut les définitions nécessaires
#include "stdafx.h"
#include "ATPFrameQuickPlan.h"
#include "ATPEtude.h"
#include "ATPLibelle.h"
#include "ATPPeriodeAction.h"
#include "ATPVersion.h"
#include "ATPPupitre.h"
#include ".\ATPFrameQuickPlan.h"
#include "JATPDocExport.h"
#include "ATPExpCsvQuickPlan.h"
#include "ATPExportEchelleQP.h"
#include "afxadv.h"
#include "ExcelPrinter.h"
#include "JATPDocExport.h"
#include "JMFCStream.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// on définit la taille des splits
static const LONG g_CXSplit = 3;
static const LONG g_CYSplit = 3;

const CRect CATPFrameQuickPlan::s_rectDefault(100, 100, 1010, 700);  // statique


//////////////////
// le constructeur

CATPFrameQuickPlan::CATPFrameQuickPlan(JATPDocCreuset *pDoc)
:CATPBaseFrame(pDoc)
{
    m_bFirstTime = TRUE;
}

//////////////////
// le destructeur

CATPFrameQuickPlan::~CATPFrameQuickPlan()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPFrameQuickPlan, CATPBaseFrame)
	//{{AFX_MSG_MAP(CATPFrameQuickPlan)
	ON_WM_CREATE()
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_COMMAND(IDM_CR_EXPORT_TOUT, OnExportTout)
	ON_COMMAND(IDM_CR_EXCEL_TOUT, OnExcelTout)
	ON_COMMAND(IDM_CR_CLIPBOARD_TOUT, OnClipboardTout)
	ON_COMMAND(IDM_CR_PRINT_TOUT, OnPrintTout)
	ON_COMMAND(IDM_CR_EXIT, OnClose)
END_MESSAGE_MAP()


//////////////////////////////////////////
// la fonction pour mettre à jour les vues

VOID CATPFrameQuickPlan::UpdateAllViews(const JFCUpdate & Message)
{
	if(m_pATPDocCreuset->m_AFFCreuset.GetAffQuickPlan())
		m_pSplitterHypo->UpdateAllViews(Message);

	//if(Message == UPDATE_AFF_CONTRIB)
	//{
	//	m_dlgContribution.Update();
	//	return;
	//}
	//m_dlgContribution.Update();

	if(m_pATPDocCreuset)
	{
		if (IsIconic())
		{
			ShowWindow( SW_RESTORE );
			m_pATPDocCreuset->m_AFFCreuset.SetAffQuickPlan(1);
			//m_pATPDocCreuset->m_AFFCreuset.SetAffHypo(1);
			//m_pATPDocCreuset->m_AFFCreuset.SetHypoActif(1);
		}
		else
			ShowWindow((m_pATPDocCreuset->m_AFFCreuset.GetAffQuickPlan()) ?  SW_SHOW : SW_HIDE);
	}
	
		
	// on met à jour les composants

	if(Message == UPDATE_COLOR)
	{
		m_pSplitterHypo->SetColors();
		m_dlgContribution.SetColors();
		m_dlgContribution.Invalidate();
		Invalidate();
		return;
	}

	// Construction du titre fenêtre
	CString   LabelEtude;
	CString   LabelFen;
	LabelEtude = m_pATPDocCreuset->m_KEYEtude.GetLabel();
	LabelFen = "QuickPlan - " + LabelEtude;
	SetWindowText(LabelFen);

}

////////////////////////////////
// CATPFrameQuickPlan message handlers

int CATPFrameQuickPlan::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	// on appelle le gestionnaire de base
	if (this->CATPBaseFrame::OnCreate(lpCreateStruct) == -1) return (-1);

	// on crée les composants
	m_pSplitterHypo = new CATPSplitterHypoQP();
	if (m_pSplitterHypo->Create(NULL, NULL, WS_CHILD | WS_VISIBLE |	WS_CLIPCHILDREN, CRect(0, 0, CATPSplitterHypoQP::GetCXMin(), CATPSplitterHypoQP::GetCYMin()), this, 0) == FALSE) return (-1);


	m_pSplitterHypo->SetDocument(m_pATPDocCreuset);

	SetMenu(NULL);
	::DestroyMenu(m_hMenuDefault);
	CMenu menu;
	menu.LoadMenu(IDR_CR_ATPQUICKPLAN);
	SetMenu(&menu);
   // Assign default menu
   m_hMenuDefault = menu.GetSafeHmenu();


	// on quitte	
	return (0);
}

void CATPFrameQuickPlan::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// on appelle le gestionnaire de base
	this->JFCFrame::OnGetMinMaxInfo(lpMMI);

	// on récupère les rectangles de la fenêtre
	RECT RectC; this->GetClientRect(&RectC);
	RECT RectW; this->GetWindowRect(&RectW); this->ScreenToClient(&RectW);

	// on corrige la taille minimum de la fenêtre
	lpMMI->ptMinTrackSize.x = CATPPupitre::GetCXMin() + RectC.left - RectW.left + RectW.right  - RectC.right;
	lpMMI->ptMinTrackSize.y = CATPSplitterHypoQP::GetCYMin() + RectC.top  - RectW.top  + RectW.bottom - RectC.bottom;
}

void CATPFrameQuickPlan::OnSize(UINT nType, int cx, int cy) 
{
	// on appelle le gestionnaire de base
	this->JFCFrame::OnSize(nType, cx, cy);

	// on repositionne les composants
	if (m_pSplitterHypo->GetSafeHwnd() != NULL) m_pSplitterHypo->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
}

void CATPFrameQuickPlan::OnClose() 
{
	if(CanClose())
	{
		// Hide Zoom and Distribution boxes
		m_pATPDocCreuset->m_AFFFiltreAudience = false;
		m_pATPDocCreuset->m_AFFOptionsMoteur = false;
		m_pATPDocCreuset->m_AFFCourbeReponse = false;
		m_pATPDocCreuset->m_AFFZoomPlan.SetVisible(false);
		m_pATPDocCreuset->m_AFFContribution.SetVisible(false);
		m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_PANEL);


		m_pATPDocCreuset->m_AFFVentilation.SetVisible(false);

		// Hide this window
		//m_pATPDocCreuset->m_AFFCreuset.SetAffHypo(false);
		//m_pATPDocCreuset->m_AFFCreuset.SetHypoActif(false);
		m_pATPDocCreuset->m_AFFCreuset.SetAffQuickPlan(false);

		m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_HYPO);
	}

}

void CATPFrameQuickPlan::OnDestroy()
{
    CString strText;
    BOOL bIconic, bMaximized;

    WINDOWPLACEMENT wndpl;
    wndpl.length = sizeof(WINDOWPLACEMENT);
    // lit la position courante de la fenêtre et
    // l'état (zoomé/réduit)
    BOOL bRet = GetWindowPlacement(&wndpl);
    if (wndpl.showCmd == SW_SHOWNORMAL)
	{
        bIconic = FALSE;
        bMaximized = FALSE;
    }
    else if (wndpl.showCmd == SW_SHOWMAXIMIZED)
	{
        bIconic = FALSE;
        bMaximized = TRUE;
    } 
    else if (wndpl.showCmd == SW_SHOWMINIMIZED)
	{
        bIconic = TRUE;
        if (wndpl.flags)
            bMaximized = TRUE;
        else
            bMaximized = FALSE;
    }
 
	strText.Format("%04d %04d %04d %04d",
                   wndpl.rcNormalPosition.left,
                   wndpl.rcNormalPosition.top,
                   wndpl.rcNormalPosition.right,
                   wndpl.rcNormalPosition.bottom);
 
	AfxGetApp()->WriteProfileString("Options\\QuickPlan", "Rect", strText);
    //AfxGetApp()->WriteProfileInt("Options\\QuickPlan", "Icon", bIconic);
    AfxGetApp()->WriteProfileInt("Options\\QuickPlan", "Max", bMaximized);

	CATPBaseFrame::OnDestroy();
}

void CATPFrameQuickPlan::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CString strText;
    BOOL bIconic, bMaximized;
    UINT flags;
    WINDOWPLACEMENT wndpl;
    CRect rect;

    if (m_bFirstTime)
	{
        m_bFirstTime = FALSE;
        strText = AfxGetApp()->GetProfileString("Options\\QuickPlan", "Rect");
        if (!strText.IsEmpty())
		{
            rect.left = atoi((const char*) strText);
            rect.top = atoi((const char*) strText + 5);
            rect.right = atoi((const char*) strText + 10);
            rect.bottom = atoi((const char*) strText + 15);
        }
        else 
            rect = s_rectDefault;

        bIconic = AfxGetApp()->GetProfileInt("Options\\QuickPlan", "Icon", 0);
        bMaximized = AfxGetApp()->GetProfileInt("Options\\QuickPlan", "Max", 0);   
        if (bIconic)
		{
            nStatus = SW_SHOWMINNOACTIVE;
            if (bMaximized)
                flags = WPF_RESTORETOMAXIMIZED;
            else
                flags = WPF_SETMINPOSITION;
        }
        else
		{
            if (bMaximized)
			{
                nStatus = SW_SHOWMAXIMIZED;
                flags = WPF_RESTORETOMAXIMIZED;
            }
            else
			{
                nStatus = SW_NORMAL;
                flags = WPF_SETMINPOSITION;
            }
        }
        wndpl.length = sizeof(WINDOWPLACEMENT);
        wndpl.showCmd = nStatus;
        wndpl.flags = flags;
        wndpl.ptMinPosition = CPoint(0, 0);
        wndpl.ptMaxPosition =  CPoint(-::GetSystemMetrics(SM_CXBORDER), -::GetSystemMetrics(SM_CYBORDER));
        wndpl.rcNormalPosition = rect;

		// définit la position de la fenêtre 
		// et l'état (zoomé/réduit)
        BOOL bRet = SetWindowPlacement(&wndpl);
    }

	//if(nStatus == SW_SHOWNORMAL)
	//{
	//	m_dlgFiltres.SetParent(this);
	//	m_dlgCourbeReponse.SetParent(this);
	//	m_dlgChoixMoteur.SetParent(this);
	//}

	CATPBaseFrame::OnShowWindow(bShow, nStatus);
}

bool CATPFrameQuickPlan::ExportResultPlan(CFile & file)
{
	// Flag validité de l'export
	bool ExportOk = false;
	// on teste la validité du document courant
	if (m_pATPDocCreuset->IsValid())
	{
		// on modifie le curseur
		CWaitCursor Wait;


		m_pATPDocCreuset->UpdateAllViews(UPDATE_FORCEVENTIL);
		//m_pATPDocCreuset->UpdateAllViews(UPDATE_FORCECONTRIB);

		// Test regroupement data export
		JATPDocExport DocExport(*m_pATPDocCreuset);

		// on teste les exceptions
		try
		{
			// Création de l'objet export test plans
			CATPExpCsvQuickPlan *pExportCsvTestPlan = new CATPExpCsvQuickPlan();
			//CATPExpCsvTestPlan *pExportCsvTestPlan = new CATPExpCsvTestPlan();

			// on exporte le document test plans
			ExportOk = DocExport.Accept(pExportCsvTestPlan); 
			delete pExportCsvTestPlan;

		}

		catch (...)
		{
			// on affiche le message
			ExportOk = false;
		}

		m_pATPDocCreuset->UpdateAllViews(UPDATE_VENTIL);
		if (ExportOk == true)
		{
			file.Write(DocExport.m_StrExport.GetBuffer(), DocExport.m_StrExport.GetLength());
			//::AfxMessageBox(_T("L'export des hypothèses s'est effectué avec succès"), MB_ICONINFORMATION | MB_OK);
		}
		else
		{
			::AfxMessageBox(_T("La création du fichier d'export a échoué."), MB_ICONERROR | MB_OK);
		}

		
	}
	return ExportOk;
}

bool CATPFrameQuickPlan::ExportEchelle(CFile & file)
{
	// Flag validité de l'export
	bool ExportOk = false;
	// on teste la validité du document courant
	if (m_pATPDocCreuset->IsValid())
	{
		// on modifie le curseur
		CWaitCursor Wait;

		// Test regroupement data export
		JATPDocExport DocExport(*m_pATPDocCreuset);

		// on teste les exceptions
		try
		{
			// Création de l'objet export échelle
			CATPExportEchelleQP *pExportEchelle = new CATPExportEchelleQP();

			// on exporte le document échelle
			ExportOk = DocExport.AcceptEch(pExportEchelle);
			delete pExportEchelle;
		}

		catch (...)
		{
			// on affiche le message
			::AfxMessageBox(_T("La création de l'export échelle a échoué."), MB_ICONERROR | MB_OK);
			ExportOk = false;
		}

		if (ExportOk == true)
		{
			file.Write(DocExport.m_StrExport.GetBuffer(), DocExport.m_StrExport.GetLength() );
			//::AfxMessageBox(_T("L'export de l'echelle s'est effectué avec succès"), MB_ICONINFORMATION | MB_OK);
		}

	}	
	return ExportOk;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Export Tout Creuset Excel (ouvrir)
void CATPFrameQuickPlan::OnExcelTout()
{
	CSharedFile clipb (GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);

	if(ExportEchelle(clipb) && ExportResultPlan(clipb))
	{
		VERIFY(OpenClipboard());
		BOOL bEmpty = EmptyClipboard();
		VERIFY(CloseClipboard());

		HGLOBAL hData = 0;
		HANDLE hResult = 0;
		hData = clipb.Detach();

		// Now, hand over memory block to clipboard
		if (bEmpty && hData)
		{
			VERIFY(OpenClipboard());
			hResult = SetClipboardData(CF_TEXT, hData);
			VERIFY(CloseClipboard());

			CExcelPrinter excelPrinter(CATPColors::COLORCREUSET, this);
			CString path = AfxGetApp()->GetProfileString("", "InstallPath", 0);
			path += "\\MacroPresse.xla";
			excelPrinter.Open(path, "DefinirTypeExportPresse");
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////////////
// Export Tout Creuset
void CATPFrameQuickPlan::OnExportTout()
{
	// pop-up file-open dlg to ask for location
	CFileDialog dlgFile(
		FALSE,
		_T(".xls"),
		NULL,
		OFN_NOCHANGEDIR|OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("Fichier Excel (*.xls)|*.xls|Fichier Texte(*.txt)|*.txt|"),this);

	if (dlgFile.DoModal( ) == IDCANCEL)
		return;

	if(dlgFile.GetFileExt().CompareNoCase("xls") == 0)
	{
		CSharedFile clipb (GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);

		if(ExportEchelle(clipb) && ExportResultPlan(clipb))
		{

			VERIFY(OpenClipboard());
			BOOL bEmpty = EmptyClipboard();
			VERIFY(CloseClipboard());

			HGLOBAL hData = 0;
			HANDLE hResult = 0;
			hData = clipb.Detach();

			// Now, hand over memory block to clipboard
			if (bEmpty && hData)
			{
				VERIFY(OpenClipboard());
				hResult = SetClipboardData(CF_TEXT, hData);
				VERIFY(CloseClipboard());

				CExcelPrinter excelPrinter(CATPColors::COLORCREUSET, this);
				CString path = AfxGetApp()->GetProfileString("", "InstallPath", 0);
				path += "\\MacroPresse.xla";
				excelPrinter.SaveAsFile(dlgFile.GetPathName( ), path, "DefinirTypeExportPresse");

			}
		}
	}
	else
	{

		CStdioFile txtFile;
		CFileException ex;
		if(txtFile.Open(dlgFile.GetPathName( ), CFile::modeCreate | CFile::modeWrite | CFile::typeText, &ex))
		{
			ExportEchelle(txtFile);
			ExportResultPlan(txtFile);
			txtFile.Close();
		}
		else
			ex.ReportError();



	}
}

void CATPFrameQuickPlan::OnClipboardTout()
{
	CSharedFile clipb (GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);

	if(ExportEchelle(clipb) && ExportResultPlan(clipb) )
	{

		VERIFY(OpenClipboard());
		BOOL bEmpty = EmptyClipboard();
		VERIFY(CloseClipboard());

		HGLOBAL hData = 0;
		HANDLE hResult = 0;
		hData = clipb.Detach();

		// Now, hand over memory block to clipboard
		if (bEmpty && hData)
		{
			VERIFY(OpenClipboard());
			hResult = SetClipboardData(CF_TEXT, hData);
			VERIFY(CloseClipboard());
		}
	}
}

void CATPFrameQuickPlan::OnPrintTout()
{
	CSharedFile clipb (GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);

	if(ExportEchelle(clipb) && ExportResultPlan(clipb) )
	{

		VERIFY(OpenClipboard());
		BOOL bEmpty = EmptyClipboard();
		VERIFY(CloseClipboard());

		HGLOBAL hData = 0;
		HANDLE hResult = 0;
		hData = clipb.Detach();

		// Now, hand over memory block to clipboard
		if (bEmpty && hData)
		{
			VERIFY(OpenClipboard());
			hResult = SetClipboardData(CF_TEXT, hData);
			VERIFY(CloseClipboard());

			CExcelPrinter excelPrinter(CATPColors::COLORCREUSET, this);
			CString path = AfxGetApp()->GetProfileString("", "InstallPath", 0);
			path += "\\MacroPresse.xla";
			#ifdef _DEBUG
				excelPrinter.PrintPreview( path, "DefinirTypeExportPresse");
			#else
				excelPrinter.PrintPreview( path, "DefinirTypeExportPresse");
			#endif

		}
	}
}
