// DlgCourbeReponse.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "resource.h"
#include "DlgCourbeReponse.h"
#include "resource.h"
#include <math.h>
#include "DlgEditCourbe.h"
#include ".\dlgcourbereponse.h"



// Boîte de dialogue CDlgCourbeReponse

IMPLEMENT_DYNAMIC(CDlgCourbeReponse, CDialog)
CDlgCourbeReponse::CDlgCourbeReponse(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCourbeReponse::IDD, pParent)
{
	m_pATPDocCreuset = 0;
	m_courbes = CCourbes::GetInstance();
	m_beta = AfxGetApp()->GetProfileInt("Moteur", "CourbeReponse", 0);
}

CDlgCourbeReponse::~CDlgCourbeReponse()
{
}

void CDlgCourbeReponse::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on mémorise le document
	m_pATPDocCreuset = pATPDocCreuset;
	OnUpdate();
}

void CDlgCourbeReponse::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CR_BETA, m_beta);
	DDV_MinMaxInt(pDX, m_beta, 1, 100);
}


BEGIN_MESSAGE_MAP(CDlgCourbeReponse, CDialog)
	ON_WM_NCHITTEST()
	ON_BN_CLICKED(IDC_CR_X, OnBtnX)
	ON_BN_CLICKED(IDC_CR_BTNCREER, OnClickedBtnCreer)
	ON_BN_CLICKED(IDC_CR_BTNMODIF, OnClickedBtnModif)
	ON_BN_CLICKED(IDC_CR_BTNSUPPR, OnClickedBtnSuppr)
	ON_BN_CLICKED(IDC_CR_BTNAUTO, OnClickBtnAuto)
	ON_CBN_SELCHANGE(IDC_CR_COMBOCRB, OnSelChangeComboCrb)
	ON_EN_KILLFOCUS(IDC_CR_BETA, OnKillFocusBeta)
END_MESSAGE_MAP()


// Gestionnaires de messages CDlgCourbeReponse
BOOL CDlgCourbeReponse::OnInitDialog() 
{
	// on appelle le gestionnaire de base
	CDialog::OnInitDialog();

	SetupControls();

	//on rempli les 20 cases
	for (int i = 0; i<20; i++)
	{
		CString txt;
		txt.Format("%d", i+1);
		m_gridValues.SetItemText(0, i, txt);
		m_gridValues.SetItemState(0, i, m_gridValues.GetItemState(0, i) | GVIS_READONLY);
		m_gridValues.SetItemFormat(0, i, DT_CENTER);
	}

	//On initialise le combo
	FillCombo(m_beta);

	return TRUE;
}

void CDlgCourbeReponse::FillCombo(int idSel)
{
	//on vide le combo
	m_Combo.ResetContent();

	CString txt;

	//Pas de courbe
	txt.LoadString(IDS_CR_PASCOURBE);
	int idx = m_Combo.AddString(txt);
	m_Combo.SetItemData(idx, 0);

	//Courbe binomiale
	txt.LoadString(IDS_CR_COURBEBINOMIALE);
	idx = m_Combo.AddString(txt);
	m_Combo.SetItemData(idx, m_beta);

	//On selectionnne le bon elem
	if(idSel == 0)
		m_Combo.SetCurSel(0);
	else
		m_Combo.SetCurSel(1);
	
	//Courbes perso
	for(m_courbes->MoveFirst(); m_courbes->IsValid(); m_courbes->MoveNext())
	{
		CString gg = m_courbes->GetItem()->GetLabel().AsJCharPtr();
		idx = m_Combo.AddString(gg);
		m_Combo.SetItemData(idx, m_courbes->GetItem()->GetId());
		if(idSel == m_courbes->GetItem()->GetId())
			m_Combo.SetCurSel(idx);
	}
	
	//La selection a changé
	OnSelChangeComboCrb();
}

void CDlgCourbeReponse::InitGrid()
{
	//init de la grille
	CString txt;

	for (int i = 0; i<20; i++)
	{
		int val = (int)m_Combo.GetItemData(m_Combo.GetCurSel());
		//on recupere la valeur
		double res = 100.0 * m_courbes->GetProba(i+1, val);
		//on la met dans la grille
		txt.Format("%.0f", res);
		m_gridValues.SetItemText(1, i, txt);
		m_gridValues.SetItemState(1, i, m_gridValues.GetItemState(0, i) | GVIS_READONLY);
		m_gridValues.SetItemFormat(1, i, DT_CENTER);
	}


	//maj de l'affichage
	m_gridValues.Refresh();
}

void CDlgCourbeReponse::OnCancel() 
{
	if (m_pATPDocCreuset != 0)
	{
		// on met à jour le pupitre
		m_pATPDocCreuset->m_AFFCourbeReponse = false;

		// on met à jour les vues du document
		m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_PANEL);
	}
	else
	{
		// on met à jour le composant
		this->ShowWindow(SW_HIDE);
	}
}

void CDlgCourbeReponse::OnOK() 
{
	// on ne fait rien
}

void CDlgCourbeReponse::Update()
{
	// on met à jour le composant
	OnUpdate();
}

void CDlgCourbeReponse::OnUpdate()
{
	// on met à jour le composant
	if (m_pATPDocCreuset != 0 && m_pATPDocCreuset->IsValid())
		this->ShowWindow(m_pATPDocCreuset->m_AFFCourbeReponse ? SW_SHOW : SW_HIDE);
}

void CDlgCourbeReponse::SetupControls()
{
//subclass 

	m_bkg.SubclassDlgItem(IDC_CR_BK, this);
	m_ttl.SubclassDlgItem(IDC_CR_TTL, this);
	m_Txt.SubclassDlgItem(IDC_CR_TXT, this);
	m_btnX.SubclassDlgItem(IDC_CR_X, this);
	m_Combo.SubclassDlgItem(IDC_CR_COMBOCRB, this);
	m_btnCreer.SubclassDlgItem(IDC_CR_BTNCREER, this);
	m_btnModif.SubclassDlgItem(IDC_CR_BTNMODIF, this);
	m_btnSuppr.SubclassDlgItem(IDC_CR_BTNSUPPR, this);


	m_bkg.SetTransparent(false);
	m_bkg.SetSunken(true);
	m_bkg.AddTopWindow(m_ttl);
	m_bkg.AddTopWindow(m_Txt);
	m_bkg.AddTopWindow(GetDlgItem(IDC_CR_BETA)->GetSafeHwnd());
	m_bkg.AddTopWindow(m_btnCreer);
	m_bkg.AddTopWindow(m_btnModif);
	m_bkg.AddTopWindow(m_btnSuppr);
	m_bkg.AddTopWindow(m_btnX);
	m_bkg.AddTopWindow(m_Combo);

////
	m_ttl.SetFontSize(CATPColors::GetFontLabelSize());
	m_ttl.SetBorder(false);

	//On affiche la grille
	CRect rect;
	GetDlgItem(IDC_CR_STATICGRID)->GetWindowRect(&rect);
	POINT pt1, pt2;
	pt1.x = rect.left, pt1.y = rect.top;
	pt2.x = rect.right, pt2.y = rect.bottom;
	::ScreenToClient(m_hWnd, &pt1);
	::ScreenToClient(m_hWnd, &pt2);
	rect.left = pt1.x, rect.top = pt1.y, rect.right = pt2.x, rect.bottom = pt2.y;
	m_gridValues.Create(rect, this, IDC_CR_STATICGRID, WS_CHILD | WS_TABSTOP| WS_VISIBLE );

	m_gridValues.ModifyStyleEx(WS_EX_CLIENTEDGE, 0);

	m_bkg.AddTopWindow(m_gridValues);

	m_gridValues.GetDefaultCell(FALSE, FALSE)->SetSelClr(CATPColors::GetColorSelect(CATPColors::COLORCREUSET));
	m_gridValues.EnableSelection(FALSE);
	m_gridValues.SetDefCellWidth(m_gridValues.GetDefCellWidth()/2);
	m_gridValues.SetColumnCount(20);
	m_gridValues.SetRowCount(2);

	SetColors();

}

void CDlgCourbeReponse::SetColors()
{
	m_bkg.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET,CATPColors::MEDIUM_DEGRAD);

	m_ttl.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET,CATPColors::DARK_DEGRAD);
	m_ttl.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORCREUSET));

	for (int i = 0; i<20; i++)
		m_gridValues.SetItemBkColour(0, i, CATPColors::GetColorLight(CATPColors::COLORCREUSET));

	m_Txt.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET,CATPColors::MEDIUM_DEGRAD);
	m_Txt.SetTextColor(CATPColors::GetColorBlack());
	
	m_btnCreer.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_btnCreer.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_btnCreer.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );

	m_btnModif.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_btnModif.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_btnModif.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );

	m_btnSuppr.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_btnSuppr.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_btnSuppr.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );

	m_btnX.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_btnX.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_btnX.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );

	m_Combo.SetSelectionColor(CATPColors::GetColorSelect(CATPColors::COLORCREUSET));
}


LRESULT CDlgCourbeReponse::OnNcHitTest(CPoint point)
{
    UINT hit = CDialog::OnNcHitTest(point);
    if ( hit == HTCLIENT ) 
    {
        return HTCAPTION;
    }
    else
        return hit;
}

void CDlgCourbeReponse::OnBtnX()
{
	OnCancel();
}

void CDlgCourbeReponse::OnSelChangeComboCrb()
{
	//afficha courbe perso
	m_Txt.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CR_BETA)->ShowWindow(SW_HIDE);
	m_btnModif.EnableWindow();
	m_btnSuppr.EnableWindow();

	if(m_Combo.GetCurSel() == 0)
	{
		//pas de courbe
		m_Txt.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CR_BETA)->ShowWindow(SW_HIDE);
		m_btnModif.EnableWindow(FALSE);
		m_btnSuppr.EnableWindow(FALSE);
		m_Combo.SetItemData(0, 0);
	}
	if(m_Combo.GetCurSel() == 1)
	{
		//courbe binomiale
		m_Txt.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CR_BETA)->ShowWindow(SW_SHOW);
		m_btnModif.EnableWindow(FALSE);
		m_btnSuppr.EnableWindow(FALSE);
		if (m_beta == 0 || m_beta >= 101)
			m_beta = 16;
		UpdateData(false);
		m_Combo.SetItemData(1, m_beta);
	}

	if(m_pATPDocCreuset)
	{
		//on sauve la valeur du beta
		AfxGetApp()->WriteProfileInt("Moteur", "CourbeReponse", (int)m_Combo.GetItemData(m_Combo.GetCurSel()));
		m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_PUPITRE);
	}

	InitGrid();
}

void CDlgCourbeReponse::OnClickedBtnCreer()
{
	// nouvelle courbe perso
	CDlgEditCourbe dlg(this);
	if(dlg.DoModal() == IDOK)
		FillCombo(dlg.GetIdCourbe());
}

void CDlgCourbeReponse::OnClickedBtnModif()
{
	//edition de la courbe actuelle
	CDlgEditCourbe dlg(this);
	dlg.SetIdCourbe((int)m_Combo.GetItemData(m_Combo.GetCurSel()));
	if(dlg.DoModal() == IDOK)
		FillCombo(dlg.GetIdCourbe());
}

void CDlgCourbeReponse::OnClickedBtnSuppr()
{
	//Suppression de la courbe actuelle
	int idCrb = (int)m_Combo.GetItemData(m_Combo.GetCurSel());
	//Confirm?

	CString ttl, qst;
	ttl.LoadString(IDR_CR_ATPFRAME);
	qst.LoadString(IDS_CR_CONFSUPPCRB);

	if ( MessageBox(qst, ttl, MB_ICONQUESTION|MB_OKCANCEL) == IDOK)
	{
		m_courbes->MoveTo(idCrb);
		if (m_courbes->IsValid())
		{
			delete 	m_courbes->GetItem();
			m_courbes->Remove();
			m_courbes->Save();
		}
		//on selection la courbe precedente
		FillCombo((int)m_Combo.GetItemData(m_Combo.GetCurSel()-1));
	}
}

void CDlgCourbeReponse::OnClickBtnAuto()
{
	int oldBeta = m_beta;
    UpdateData();
	if (m_beta>100)
	{
		m_beta = oldBeta;
	    UpdateData(FALSE);
	}
	m_Combo.SetItemData(1, m_beta);
	InitGrid();
	m_gridValues.SetFocus();
	AfxGetApp()->WriteProfileInt("Moteur", "CourbeReponse", m_beta);
	m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_PUPITRE);
}


void CDlgCourbeReponse::OnKillFocusBeta()
{
	int oldBeta = m_beta;
    UpdateData();
	if (m_beta>100)
	{
		m_beta = oldBeta;
	    UpdateData(FALSE);
	}
	m_Combo.SetItemData(1, m_beta);
	InitGrid();
	m_gridValues.SetFocus();
	AfxGetApp()->WriteProfileInt("Moteur", "CourbeReponse", m_beta);
	if(m_pATPDocCreuset && m_pATPDocCreuset->IsValid())

	m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_PUPITRE);
}
