// FormulaireTauxFO.cpp : implementation file
//

#include "stdafx.h"
#include "nomenclature.h"
#include "FormulaireTauxFO.h"
#include "FormulaireUtilTaux.h"
#include "Noeud.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CNomenclatureApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CFormulaireTauxFO dialog


CFormulaireTauxFO::CFormulaireTauxFO(CWnd* pParent /*=NULL*/)
	: CDialog(CFormulaireTauxFO::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFormulaireTauxFO)
	m_HistoriqueTaux = FALSE;
	m_ExNomDate = _T("");
	m_NvNomDate = _T("");
	m_ExTaux1 = _T("");
	m_ExTaux2 = _T("");
	m_ExTaux3 = _T("");
	m_ExTaux4 = _T("");
	m_ExTaux5 = _T("");
	m_ExTauxCM = _T("");
	m_ExTauxFO = _T("");
	m_Taux1 = _T("");
	m_Taux2 = _T("");
	m_Taux3 = _T("");
	m_Taux4 = _T("");
	m_Taux5 = _T("");
	m_TauxCM = _T("");
	m_TauxFO = _T("");
	m_SimulTaux1 = _T("");
	m_SimulTaux2 = _T("");
	m_SimulTaux3 = _T("");
	m_SimulTaux4 = _T("");
	m_SimulTaux5 = _T("");
	m_SimulTauxCM = _T("");
	m_SimulTauxFO = _T("");
	//}}AFX_DATA_INIT
}


void CFormulaireTauxFO::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormulaireTauxFO)
	DDX_Control(pDX, IDSuppTauxSimul, m_SuppTauxSimul);
	DDX_Control(pDX, IDC_SIMTAUXCM, m_EditSimulTauxCM);
	DDX_Control(pDX, IDC_SIMTAUX5, m_EditSimulTaux5);
	DDX_Control(pDX, IDC_SIMTAUX4, m_EditSimulTaux4);
	DDX_Control(pDX, IDC_SIMTAUX3, m_EditSimulTaux3);
	DDX_Control(pDX, IDC_SIMTAUX2, m_EditSimulTaux2);
	DDX_Control(pDX, IDC_SIMTAUX1, m_EditSimulTaux1);
	DDX_Control(pDX, IDC_TAUXSIMUL, m_TauxSimul);
	DDX_Control(pDX, IDC_SEPTAUXSIMUL, m_SepTauxSimul);
	DDX_Control(pDX, IDC_INFOTFO, m_InfoTFO);
	DDX_Control(pDX, IDC_SELRADIO, m_SelRadio);
	DDX_Radio(pDX, IDC_SELRADIO, m_SelRadioValue);
	DDX_Control(pDX, IDC_TAUXCM, m_EditTauxCM);
	DDX_Control(pDX, IDC_TAUX5, m_EditTaux5);
	DDX_Control(pDX, IDC_TAUX4, m_EditTaux4);
	DDX_Control(pDX, IDC_TAUX3, m_EditTaux3);
	DDX_Control(pDX, IDC_TAUX2, m_EditTaux2);
	DDX_Control(pDX, IDC_TAUX1, m_EditTaux1);
	DDX_Control(pDX, IDSuppTaux, m_SuppTaux);
	DDX_Control(pDX, IDC_CADRE9, m_Cadre9);
	DDX_Control(pDX, IDSauverTaux, m_SauverTaux);
	DDX_Control(pDX, IDC_LIST_STATIONS, m_ListStation);
	DDX_Text(pDX, IDC_EXNOMDATE2, m_ExNomDate);
	DDX_Text(pDX, IDC_NVNOMDATE, m_NvNomDate);
	DDX_Text(pDX, IDC_EXTAUX1, m_ExTaux1);
	DDX_Text(pDX, IDC_EXTAUX2, m_ExTaux2);
	DDX_Text(pDX, IDC_EXTAUX3, m_ExTaux3);
	DDX_Text(pDX, IDC_EXTAUX4, m_ExTaux4);
	DDX_Text(pDX, IDC_EXTAUX5, m_ExTaux5);
	DDX_Text(pDX, IDC_EXTAUXCM, m_ExTauxCM);
	DDX_Text(pDX, IDC_EXTAUXFO, m_ExTauxFO);
	DDX_Text(pDX, IDC_TAUX1, m_Taux1);
	DDX_Text(pDX, IDC_TAUX2, m_Taux2);
	DDX_Text(pDX, IDC_TAUX3, m_Taux3);
	DDX_Text(pDX, IDC_TAUX4, m_Taux4);
	DDX_Text(pDX, IDC_TAUX5, m_Taux5);
	DDX_Text(pDX, IDC_TAUXCM, m_TauxCM);
	DDX_Text(pDX, IDC_TAUXFO, m_TauxFO);
	DDX_Text(pDX, IDC_SIMTAUX1, m_SimulTaux1);
	DDX_Text(pDX, IDC_SIMTAUX2, m_SimulTaux2);
	DDX_Text(pDX, IDC_SIMTAUX3, m_SimulTaux3);
	DDX_Text(pDX, IDC_SIMTAUX4, m_SimulTaux4);
	DDX_Text(pDX, IDC_SIMTAUX5, m_SimulTaux5);
	DDX_Text(pDX, IDC_SIMTAUXCM, m_SimulTauxCM);
	DDX_Text(pDX, IDC_SIMTAUXFO, m_SimulTauxFO);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormulaireTauxFO, CDialog)
	//{{AFX_MSG_MAP(CFormulaireTauxFO)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDSauverTaux, OnSauverTaux)
	ON_BN_CLICKED(IDSuppTaux, OnSuppTaux)
	ON_LBN_SELCHANGE(IDC_LIST_STATIONS, OnSelchangeListStations)
	ON_EN_CHANGE(IDC_TAUX1, OnChangeTaux1)
	ON_EN_CHANGE(IDC_TAUX2, OnChangeTaux2)
	ON_EN_CHANGE(IDC_TAUX3, OnChangeTaux3)
	ON_EN_CHANGE(IDC_TAUX4, OnChangeTaux4)
	ON_EN_CHANGE(IDC_TAUX5, OnChangeTaux5)
	ON_EN_CHANGE(IDC_TAUXCM, OnChangeTauxcm)
	ON_BN_CLICKED(IDC_SELTV, OnSeltv)
	ON_BN_CLICKED(IDC_SELRADIO, OnSelradio)
	ON_EN_CHANGE(IDC_SIMTAUX1, OnChangeSimtaux1)
	ON_EN_CHANGE(IDC_SIMTAUX2, OnChangeSimtaux2)
	ON_EN_CHANGE(IDC_SIMTAUX3, OnChangeSimtaux3)
	ON_EN_CHANGE(IDC_SIMTAUX4, OnChangeSimtaux4)
	ON_EN_CHANGE(IDC_SIMTAUX5, OnChangeSimtaux5)
	ON_EN_CHANGE(IDC_SIMTAUXCM, OnChangeSimtauxcm)
	ON_BN_CLICKED(IDSuppTauxSimul, OnSuppTauxSimul)
	ON_BN_CLICKED(IDSauverTaux, OnSauverTaux)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormulaireTauxFO message handlers

BOOL CFormulaireTauxFO::OnInitDialog() 
{
	CRect Rect;

	CDialog::OnInitDialog();

	//this->GetClientRect(&Rect);
	//this->InvalidateRect(Rect,TRUE);
	m_SauverTaux.ShowWindow(SW_SHOW);
	SetWindowText("MISE A JOUR DES TAUX FIN D'ORDRE : " + m_LibelleNomencl);

	
	// Sélection Radio par défaut
	m_SelRadioValue = 0;
	UpdateData(false);

	// Chargement des chaines dans liste sélection
	if (ChargeChaine() == true)
	{
		// Initialise les éléments de la boite de dialogue
		InitTaux();
		InitSimulTaux();
	}
	else
		CDialog::OnCancel();

	// Initialisation chiane de base
	m_InxBaseStation = -1; 

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// Chargement des chaines régionales et locales
bool CFormulaireTauxFO::ChargeChaine()
{
	CPWFile File;
	CString FicChaine;
	CString StrReg;
	CString Ligne = "";
	CString ChaineStr;
	bool AuMoins1Fichier = false;
	int i;
	bool AppRadioOk = false;
	bool AppTeleOk = false;

	// Test si AppRadio ou AppTele installée
	FicChaine = theApp.m_PathArtRadio + "\\Zone\\zfr00\\frstatio.tbl";
	if (File.OpenFile(FicChaine))
	{
		AppRadioOk = true;
		File.CloseFile();
	}
	
	FicChaine = theApp.m_PathArtTele + "\\Zone\\zfr00\\frstatio.tbl";
	if (File.OpenFile(FicChaine))
	{
		AppTeleOk = true;
		File.CloseFile();
	}

	// Init Liste des chaines
	m_ListStation.ResetContent();

	// Chaines régionales / maxi 100 fichiers régionaux
	for (i=0;i<100;i++)
	{
		StrReg.Format("%02d",i);

		// Localisation des fichiers chaines à charger
		if (AppRadioOk == true && m_SelRadioValue == 0)
			FicChaine = theApp.m_PathArtRadio + "\\Zone\\zfr" + StrReg + "\\frstatio.tbl";
		else if (AppTeleOk == true && m_SelRadioValue != 0)
			FicChaine = theApp.m_PathArtTele + "\\Zone\\zfr" + StrReg + "\\frstatio.tbl";
		else
			FicChaine = theApp.m_PathNomencl + "\Frstatio" + StrReg + ".tbl";
		
		if (File.OpenFile(FicChaine))
		{
			// Recherche début info chaines
			while (Ligne != "#")
			{
				File.ReadString(&Ligne);
			}

			// lecture 1ere ligne chaine
			File.ReadString(&Ligne);
			while (Ligne != "")
			{
				// Sélection station radio ou station télévision
				if ((m_SelRadioValue == 0 && Ligne.Mid(12,1) == "R") || (m_SelRadioValue == 1 && Ligne.Mid(12,1) == "T"))
				{
					// Insertion des libelle chaine (avec entete Nat ou IDF)
					// Code 00 > National Code O1 > Regional
					if (StrReg == "00")
						// les stations nationales
						ChaineStr = "00-Nat - " + Ligne.Mid(19,30) + "          " + Ligne.Left(CNomenclatureApp::NBCODESTATION);
					else if (StrReg == "01")
						// les stations IDF
						ChaineStr = "01-Idf - " + Ligne.Mid(19,30) + "          " + Ligne.Left(CNomenclatureApp::NBCODESTATION);
					else
						// pas d'indication spécifique
						ChaineStr = "00-... - " + Ligne.Mid(19,30) + "          " + Ligne.Left(CNomenclatureApp::NBCODESTATION);

					// codage avant 24/10/2002
					//ChaineStr = StrReg + "-" + Ligne.Mid(19,30) + "          " + Ligne.Left(CNomenclatureApp::NBCODESTATION);

					
					ChaineStr.TrimRight(" ");
					if (ChaineStr != "")	m_ListStation.AddString(ChaineStr);
				}
				File.ReadString(&Ligne);
			}
			File.CloseFile();	

			AuMoins1Fichier = true;

		}
	}

	// Init Titre Taux Fin D'ordre
	m_InfoTFO.SetWindowText("Taux Fin Ordre");

	if (!AuMoins1Fichier)
	{
		// Aucun fichier des chaines régionales
		AfxMessageBox ("Manque les fichiers de chaines");
		return false;
	}
	else
		return true;
	
}

void CFormulaireTauxFO::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// Coloriage du fond de ce formulaire
	CRect rect;

	// Coloriage du fond de ce formulaire
	/*
	CBrush fond(RGB_VertEmeraude);
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);
	*/
	CBrush fond(RGB_BleuJFC);
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);

	// Colore séparateur Taux
	/*
	CPaintDC dc2(&m_SepTauxSimul);
	m_SepTauxSimul.GetClientRect(&rect);
	dc2.FillRect(rect,&fond);

	CPaintDC dc2(&m_ListStation);
	CBrush fond2(RGB_VertEmeraude);
	m_ListStation.GetClientRect(&rect);
	dc2.FillRect(rect,&fond2);
	*/

}

void CFormulaireTauxFO::OnCancel() 
{
	CDialog::OnCancel();
}

// Sauvevegarde des nouveaux taux fin d'ordre saisis
void CFormulaireTauxFO::OnSauverTaux() 
{
	CStringArray TabTaux;
	CString TxtStation;
	int CodeReg;
	int CodeStation;
	int NbTaux = 0;
	int NbTauxSimul = 0;
	int IndTaux;

	
	// Test si au moins 1 chaine sélectionnée
	if (m_ListStation.GetSelCount() == 0) return;

	
	// Si nouveaux taux saisis
	if (NouvelleValeurTaux() == true)
	{
		// Mise à jour des controles associés
		UpdateData(true);

		// Ecrire les nouveaux taux pour toutes les chaines sélectionnées
		if (m_TauxCM == "")
		{
			// Si au moins 1 taux saisie et TauxCM vide > force Taux CM à 0
			if (m_Taux1 != "" || m_Taux2 != "" || m_Taux3 != "" || m_Taux4 != "" || m_Taux5 != "")
			{
				m_TauxCM = "0";
				NbTaux++;
			}
		}

		if (m_TauxCM == "") return;
		NbTaux = 1;

		// Combien de taux saisis
		if (m_Taux1 != "") NbTaux++;
		if (m_Taux2 != "") NbTaux++;
		if (m_Taux3 != "") NbTaux++;
		if (m_Taux4 != "") NbTaux++;
		if (m_Taux5 != "") NbTaux++;
		
		// Dimensionne nouveau tableau des taux
		TabTaux.SetSize(NbTaux);
		IndTaux = 0;
		TabTaux[IndTaux] = m_TauxCM;IndTaux++;
		if (m_Taux1 !="")
		{
			TabTaux[IndTaux] = m_Taux1;
			IndTaux++;
		}
		if (m_Taux2 !="")
		{
			TabTaux[IndTaux] = m_Taux2;
			IndTaux++;
		}
		if (m_Taux3 !="")
		{
			TabTaux[IndTaux] = m_Taux3;
			IndTaux++;
		}
		if (m_Taux4 !="")
		{
			TabTaux[IndTaux] = m_Taux4;
			IndTaux++;
		}
		if (m_Taux5 !="")
		{
			TabTaux[IndTaux] = m_Taux5;
			IndTaux++;
		}	
		
		// Pour toutes les chaines on sauve les nouveaux taux
		for (int i=0;i<m_ListStation.GetCount();i++)
		{
			if (m_ListStation.GetSel(i) >0)
			{
				m_ListStation.GetText(i,TxtStation);
				
				// Récup code national ou régional
				CodeReg = atoi(TxtStation.Left(2));
				/*
				if      (TxtStation.Left(3) == "Nat")
					CodeReg = 0;
				else if (TxtStation.Left(3) == "Idf")
					CodeReg = 1;
				else
					CodeReg = 0;
				*/

				// Nouveau format code station (sur 5 caractéres)
				CString LibSta = TxtStation.Mid(49,CNomenclatureApp::NBCODESTATION);
				CodeStation = atoi(TxtStation.Mid(49,CNomenclatureApp::NBCODESTATION));
				EcrireTaux(m_LibelleNomencl,m_CodeNomencl,CodeReg,CodeStation,TabTaux);

			}
		}
		
	}


	// Si nouveaux taux simulés saisis
	// Ecrire les nouveaux taux pour toutes les chaines sélectionnées
	// Mise à jour des controles associés
	UpdateData(true);
	if (m_SimulTauxCM == "")
	{
		// Si au moins 1 taux saisie et TauxCM vide > force Taux CM à 0
		if (m_SimulTaux1 != "" || m_SimulTaux2 != "" || m_SimulTaux3 != "" || m_SimulTaux4 != "" || m_SimulTaux5 != "")
		{
			m_SimulTauxCM = "0";
			NbTauxSimul++;
		}
	}

	if (m_SimulTauxCM == "") return;
	NbTauxSimul = 1;

	// Combien de taux saisis simulés
	if (m_SimulTaux1 != "") NbTauxSimul++;
	if (m_SimulTaux2 != "") NbTauxSimul++;
	if (m_SimulTaux3 != "") NbTauxSimul++;
	if (m_SimulTaux4 != "") NbTauxSimul++;
	if (m_SimulTaux5 != "") NbTauxSimul++;
	
	// Dimensionne nouveau tableau des taux simulés
	TabTaux.SetSize(NbTauxSimul);
	IndTaux = 0;
	TabTaux[IndTaux] = m_SimulTauxCM;IndTaux++;
	if (m_SimulTaux1 !="")
	{
		TabTaux[IndTaux] = m_SimulTaux1;
		IndTaux++;
	}
	if (m_SimulTaux2 !="")
	{
		TabTaux[IndTaux] = m_SimulTaux2;
		IndTaux++;
	}
	if (m_SimulTaux3 !="")
	{
		TabTaux[IndTaux] = m_SimulTaux3;
		IndTaux++;
	}
	if (m_SimulTaux4 !="")
	{
		TabTaux[IndTaux] = m_SimulTaux4;
		IndTaux++;
	}
	if (m_SimulTaux5 !="")
	{
		TabTaux[IndTaux] = m_SimulTaux5;
		IndTaux++;
	}	
	
	// Pour toutes les chaines on sauve les nouveaux taux
	for (int i=0;i<m_ListStation.GetCount();i++)
	{
		if (m_ListStation.GetSel(i) >0)
		{
			m_ListStation.GetText(i,TxtStation);
			
			CodeReg = atoi(TxtStation.Left(2));

			/*
			// Récup code national ou régional
			if      (TxtStation.Left(3) == "Nat")
				CodeReg = 0;
			else if (TxtStation.Left(3) == "Idf")
				CodeReg = 1;
			else
				CodeReg = 0;
			*/


			// Nouveau format code station (sur 5 caractéres)
			CodeStation = atoi(TxtStation.Mid(49,CNomenclatureApp::NBCODESTATION));
			EcrireSimulTaux(m_LibelleNomencl,m_CodeNomencl,CodeReg,CodeStation,TabTaux);
		}
	}

	// Désélectionner les chaines
	m_ListStation.SelItemRange( FALSE, 0, m_ListStation.GetCount());

	// Initialiser les boites de saisie
	InitTaux();
	InitSimulTaux();

}

bool CFormulaireTauxFO::NouvelleValeurTaux()
{

	CString TauxOrg1;
	CString TauxOrg2;
	CString TauxOrg3;
	CString TauxOrg4;
	CString TauxOrg5;
	CString TauxOrgCM;

	// Récup les taux des boites Edit
	m_EditTaux1.GetWindowText(TauxOrg1);
	m_EditTaux2.GetWindowText(TauxOrg2);
	m_EditTaux3.GetWindowText(TauxOrg3);
	m_EditTaux4.GetWindowText(TauxOrg4);
	m_EditTaux5.GetWindowText(TauxOrg5);
	m_EditTauxCM.GetWindowText(TauxOrgCM);
	
	if (m_Taux1 != TauxOrg1 || m_Taux2 != TauxOrg2 || m_Taux3 != TauxOrg3 || m_Taux4 != TauxOrg4 || m_Taux5 != TauxOrg5 || m_TauxCM != TauxOrgCM)
		// au moins 1 taux de changé
		return true;
	else
		// Les valeurs taux n'ont pas changés
		return true;
	
}


void CFormulaireTauxFO::InitTaux()
{
	// Les taux courants
	m_Taux1 = "";
	m_Taux2 = "";
	m_Taux3 = "";
	m_Taux4 = "";
	m_Taux5 = "";
	m_TauxCM = "";
	m_TauxFO = "";

	// Les taux précédents
	m_ExTaux1 = "";
	m_ExTaux2 = "";
	m_ExTaux3 = "";
	m_ExTaux4 = "";
	m_ExTaux5 = "";
	m_ExTauxCM = "";
	m_ExTauxFO = "";

	// Les utilisateurs et dates
	m_NvNomDate = "---\n---";
	m_ExNomDate = "---\n---";

	// Mise à jour des controles associés
	UpdateData(false);

}


void CFormulaireTauxFO::InitSimulTaux()
{

	// Les taux simulés
	m_SimulTaux1 = "";
	m_SimulTaux2 = "";
	m_SimulTaux3 = "";
	m_SimulTaux4 = "";
	m_SimulTaux5 = "";
	m_SimulTauxCM = "";
	m_SimulTauxFO = "";

	m_TauxSimul.SetWindowText("---\n---");

	// Mise à jour des controles associés
	UpdateData(false);

}

// Suppression de tous les taux associés aux chaines
void CFormulaireTauxFO::OnSuppTaux() 
{
	CString TxtStation;
	int CodeReg;
	int CodeStation;
	
	// Initialiser les boites de saisie
	InitTaux();
	
	// Pour toutes les chaines sélectionnées on supprime les taux
	for (int i=0;i<m_ListStation.GetCount();i++)
	{
		if (m_ListStation.GetSel(i) >0)
		{
			// On vire les derniers taux de la station
			m_ListStation.GetText(i,TxtStation);

			// Récupére code national ou régional
			CodeReg = atoi(TxtStation.Left(2));

			// Récup code national ou régional
			/*
			if      (TxtStation.Left(3) == "Nat")
				CodeReg = 0;
			else if (TxtStation.Left(3) == "Idf")
				CodeReg = 1;
			else
				CodeReg = 0;
			*/

			CodeStation = atoi(TxtStation.Mid(49,CNomenclatureApp::NBCODESTATION));
			DetruireTaux(m_LibelleNomencl,m_CodeNomencl,CodeReg,CodeStation);
			
			// Déselectionne la station
			m_ListStation.SetSel( i, FALSE);
		}
	}

}

// Affichage des taux de la station
void CFormulaireTauxFO::AfficheTauxStation(CString StrStation)
{
	
	int CodeReg;
	int CodeStation;
	int i;

	// Les taux en cours
	CStringArray TabTaux;
	CString DateTaux;
	CString UtilTaux;
	int NbTaux=0;
	int ExNbTaux=0;
	int PosSep;

	// Les taux précédents
	CStringArray TabExTaux;
	CString ExDateTaux;
	CString ExUtilTaux;
	CString StrCodeNomencl;

	// Code du source Taux FO
	CString CodeSrcTFO;
	CNoeud *pNoeud = NULL;
		
	// Lecture Infos Taux station
	if (StrStation.GetLength() < 6) return;

	CodeReg = atoi(StrStation.Left(2));

	// // Nouveau format code station (sur 5 caractéres)
	CodeStation = atoi(StrStation.Mid(3,CNomenclatureApp::NBCODESTATION));

	// Recherche taux / éventuellement en remontant à la racine
	StrCodeNomencl = m_CodeNomencl;

	// Recup Noeud Courant
	if (m_pNoeudCur != NULL)
	{
		pNoeud = m_pNoeudCur;
	}
	
	while (StrCodeNomencl != "")
	{
		//LireTaux(m_CodeNomencl,CodeReg,CodeStation,TabTaux,NbTaux,DateTaux,UtilTaux,TabExTaux,ExNbTaux,ExDateTaux,ExUtilTaux);
		PosSep =  StrCodeNomencl.Find("_",0);
		LireTaux(StrCodeNomencl,CodeReg,CodeStation,TabTaux,NbTaux,DateTaux,UtilTaux,TabExTaux,ExNbTaux,ExDateTaux,ExUtilTaux);
		if (NbTaux != 0)
		{
			if (pNoeud != NULL)
				CodeSrcTFO = pNoeud->m_Libelle;
			else
				CodeSrcTFO = "";
			break;
		}

		// Passe à lecture taux FO parent
		if (PosSep > 0)
			StrCodeNomencl=StrCodeNomencl.Mid(PosSep+1);
		else
			StrCodeNomencl ="";

		// passe à noeud parent
		if (pNoeud != NULL)
			pNoeud = pNoeud->NoeudParent(pNoeud);

	}

	// Affichage provenance taux affichés
	if (CodeSrcTFO != "")
		m_InfoTFO.SetWindowText("Taux Fin Ordre (" + CodeSrcTFO + ")"); 
	else
		m_InfoTFO.SetWindowText("Taux Fin Ordre"); 

	// Affichage nom et date dernier utilisateur taux courants
	m_NvNomDate = "---\n---";
	m_TauxSimul.SetWindowText("---\n---");
	if (NbTaux >0 && UtilTaux != "") m_NvNomDate = UtilTaux + "\n" + DateTaux;
	
	// Affichage des taux courants
	if (NbTaux > 0)
	{
		for (i=0;i<NbTaux;i++)
		{
			switch (i)
			{
				case 0 : m_TauxCM = TabTaux[i];break;
				case 1 : m_Taux1 = TabTaux[i];break;
				case 2 : m_Taux2 = TabTaux[i];break;
				case 3 : m_Taux3 = TabTaux[i];break;
				case 4 : m_Taux4 = TabTaux[i];break;
				case 5 : m_Taux5 = TabTaux[i];break;
			}
		}
		
	}

	// Affichage nom et date précédent utilisateur taux courants
	m_ExNomDate = "---\n---";
	if (ExNbTaux > 0 && ExUtilTaux != "") m_ExNomDate = ExUtilTaux + "\n" + ExDateTaux;

	// Affichage des anciens taux
	if (ExNbTaux > 0)
	{
		for (i=0;i<ExNbTaux;i++)
		{
			switch (i)
			{
				case 0 : m_ExTauxCM = TabExTaux[i];break;
				case 1 : m_ExTaux1 = TabExTaux[i];break;
				case 2 : m_ExTaux2 = TabExTaux[i];break;
				case 3 : m_ExTaux3 = TabExTaux[i];break;
				case 4 : m_ExTaux4 = TabExTaux[i];break;
				case 5 : m_ExTaux5 = TabExTaux[i];break;
			}
		}
	
	}

	// Affichage des taux simulés
	// Recup Noeud Courant
	if (m_pNoeudCur != NULL)
	{
		pNoeud = m_pNoeudCur;
	}
	StrCodeNomencl = m_CodeNomencl;
	CodeSrcTFO = "";
	while (StrCodeNomencl != "")
	{
		PosSep =  StrCodeNomencl.Find("_",0);
		LireSimulTaux(StrCodeNomencl,CodeReg,CodeStation,TabTaux,NbTaux,DateTaux,UtilTaux);
		if (NbTaux != 0)
		{
			if (pNoeud != NULL)
				CodeSrcTFO = pNoeud->m_Libelle;
			else
				CodeSrcTFO = "";
			break;
		}

		// Passe à lecture taux FO parent
		if (PosSep > 0)
			StrCodeNomencl=StrCodeNomencl.Mid(PosSep+1);
		else
			StrCodeNomencl ="";

		// passe à noeud parent
		if (pNoeud != NULL)
			pNoeud = pNoeud->NoeudParent(pNoeud);

	}

	// Affichage des taux simulés
	if (NbTaux > 0)
	{
		for (i=0;i<NbTaux;i++)
		{
			switch (i)
			{
				case 0 : m_SimulTauxCM = TabTaux[i];break;
				case 1 : m_SimulTaux1 = TabTaux[i];break;
				case 2 : m_SimulTaux2 = TabTaux[i];break;
				case 3 : m_SimulTaux3 = TabTaux[i];break;
				case 4 : m_SimulTaux4 = TabTaux[i];break;
				case 5 : m_SimulTaux5 = TabTaux[i];break;
			}
		}
		
	}

	// Affichage provenance taux simulés
	if (CodeSrcTFO != "")
		m_TauxSimul.SetWindowText("Taux Simulés \n(" + CodeSrcTFO + ")"); 
	else
		m_TauxSimul.SetWindowText("Taux Simulés"); 


	// Mise à jour des contrôles taux	
	UpdateData(false);
	MajTauxFO(true);
	MajTauxFO(false);


}



void CFormulaireTauxFO::OnSelchangeListStations() 
{
	CString CodeStation;
	int IndexSel;
	int NbSel;
	int NbSelBloc;
	CString Str;
	int i;
	CString CodeReg;
	

	// Affectation Index Chaine de base Taux FO
	NbSel = m_ListStation.GetSelCount(); 
	

	// Nb sélections du bloc courant
	NbSelBloc = 0;
	IndexSel = m_ListStation.GetAnchorIndex();
	for (i = IndexSel;i<m_ListStation.GetCount();i++)
	{
		if (m_ListStation.GetSel(i) != 0)
			NbSelBloc++;
		else
			break;
	}

	// Init nouvelle chaine de base
	if (NbSel == 1 || NbSel == NbSelBloc)	m_InxBaseStation = -1; 

	if (NbSel > 0)
	{
		
		if (m_InxBaseStation == -1)
		{
			// Init Taux
			InitTaux();
			InitSimulTaux();

			if (NbSel == 1)
			{
				IndexSel = m_ListStation.GetCurSel();		
			}
			else
			{
				IndexSel = m_ListStation.GetAnchorIndex();		
			}
			m_InxBaseStation = IndexSel;
			//m_EditTaux1.SetFocus();

			// Affichage des taux de la station
			IndexSel = m_ListStation.GetCurSel();
			m_ListStation.GetText(IndexSel,CodeStation);
			
	
			/*
			if	(CodeStation.Left(3) == "Nat")
				// préfixe 00 pour Station Nationale
				CodeStation = "00_" + CodeStation.Mid(46,5);

			else if (CodeStation.Left(3) == "Idf")
				// préfixe 01 pour Station Régionale
				CodeStation = "01_" + CodeStation.Mid(46,5);

			else
				// par défaut préfixe 00 pour Station Nationale
				CodeStation = "00_" + CodeStation.Mid(46,5);
			*/	

			// Récupére Code Nat ou Reg (2 digits) et code station (5 digits)
			CodeStation = CodeStation.Left(2) + "_" + CodeStation.Mid(49,CNomenclatureApp::NBCODESTATION);
			AfficheTauxStation(CodeStation);

			//Maj des nouveaux taux
			MajTauxFO(true);

			//Maj des anciens taux
			MajTauxFO(false);
		}		
	}

	else
	{
		// Init Taux
		InitTaux();
		InitSimulTaux();
		m_InxBaseStation = -1;
	}

	
}

// Ecriture d'un nouveau taux pour une station
void CFormulaireTauxFO::EcrireTaux(CString LibNomencl,CString CodeNomencl,int CodeRegion,int CodeStation,CStringArray &TabTaux)
{

	// Nom utilisateur Util = variable défini en entrée
	CHAR Buffer[255];
	Buffer[254] = 0;
	CString StrBuffer;
	CString Str;
	CString KeyTaux;
	CString KeyExTaux;
	CString StrTaux;
	CString Date;
	CString ExUtil;
	float Taux;
	int NbTaux;
	int i;
	COleDateTime DateTime;
	
	// Test si l'enregistrement existe déjà
	KeyTaux = CodeNomencl;
	Str.Format("%s%02d%05d","Taux_Sta",CodeRegion,CodeStation);
	GetPrivateProfileString (KeyTaux, Str,"", Buffer, 255,theApp.m_PathNomenclAnnee + "TauxFO.ini");
	StrBuffer = Buffer;

	// Sauvegarde des précédents taux
	if (StrBuffer != "")
	{
		// Récupère ancienne date
		GetPrivateProfileString (KeyTaux, "Date","", Buffer, 255,theApp.m_PathNomenclAnnee + "TauxFO.ini");
		Date = Buffer;

		if (Date != "")
		{
			// Récupère ancien utilisateur
			GetPrivateProfileString (KeyTaux, "Util","", Buffer, 255,theApp.m_PathNomenclAnnee + "TauxFO.ini");
			ExUtil = Buffer;

			// Ecriture historique taux pour cette station
			
			KeyExTaux = CodeNomencl + "_sav";
			WritePrivateProfileString (KeyExTaux, "Libelle",LibNomencl, theApp.m_PathNomenclAnnee + "TauxFO.ini"); 
			WritePrivateProfileString (KeyExTaux, "Date",Date, theApp.m_PathNomenclAnnee + "TauxFO.ini"); 
			WritePrivateProfileString (KeyExTaux, "Util",ExUtil, theApp.m_PathNomenclAnnee + "TauxFO.ini"); 
			WritePrivateProfileString (KeyExTaux, Str,StrBuffer, theApp.m_PathNomenclAnnee + "TauxFO.ini"); 
		}

	}

	// Date courante format dd/mm/yyyy - hhHmm
	DateTime=COleDateTime::GetCurrentTime();
	Date.Format("%02d/%02d/%4d-%02dH%02d", DateTime.GetDay(), DateTime.GetMonth(), DateTime.GetYear(), DateTime.GetHour(), DateTime.GetMinute());
	

	// Fabrique nouvelle chaine des taux pour cette station
	NbTaux = TabTaux.GetSize();

	// Nombre total de taux
	StrTaux.Format("%d",NbTaux);

	// Les n taux, en premier toujours le Taux CM
	for (i=0;i<TabTaux.GetSize();i++)
	{
		// Ok mais on limite à 2 décimales
		Taux  = atof(TabTaux[i]);
		TabTaux[i].Format("%.2f",Taux);

		StrTaux = StrTaux + ","+ TabTaux[i];
	}

	// Stockage des infos taux station
	WritePrivateProfileString (KeyTaux, "Libelle",LibNomencl, theApp.m_PathNomenclAnnee + "TauxFO.ini"); 
	WritePrivateProfileString (KeyTaux, "Date",Date, theApp.m_PathNomenclAnnee + "TauxFO.ini"); 
	WritePrivateProfileString (KeyTaux, "Util",theApp.m_UtilTauxCur, theApp.m_PathNomenclAnnee + "TauxFO.ini"); 
	WritePrivateProfileString (KeyTaux, Str,StrTaux, theApp.m_PathNomenclAnnee + "TauxFO.ini"); 


}


// Enlever les derniers taux pour une station
void CFormulaireTauxFO::DetruireTaux(CString LibNomencl,CString CodeNomencl,int CodeRegion,int CodeStation)
{

	// Nom utilisateur Util = variable défini en entrée
	CHAR Buffer[255];
	Buffer[254] = 0;
	CString StrBuffer;
	CString Str;
	CString KeyTaux;
	CString StrTaux;
	CString Date;
		
	// Test si l'enregistrement existe déjà
	KeyTaux = CodeNomencl;
	Str.Format("%s%02d%05d","Taux_Sta",CodeRegion,CodeStation);
	GetPrivateProfileString (KeyTaux, Str,"", Buffer, 255,theApp.m_PathNomenclAnnee + "TauxFO.ini");
	StrBuffer = Buffer;

	// S'il existe on vire les taux
	if (StrBuffer != "")
	{
		// Aucun taux
		StrTaux = "0";

		// Stockage aucun taux
		WritePrivateProfileString (KeyTaux, Str,StrTaux, theApp.m_PathNomenclAnnee + "TauxFO.ini"); 
	}
	
}

// Enlever les derniers taux pour une station
void CFormulaireTauxFO::DetruireSimulTaux(CString LibNomencl,CString CodeNomencl,int CodeRegion,int CodeStation)
{

	// Nom utilisateur Util = variable défini en entrée
	CHAR Buffer[255];
	Buffer[254] = 0;
	CString StrBuffer;
	CString Str;
	CString KeyTaux;
	CString StrTaux;
	CString Date;
		
	// Test si l'enregistrement existe déjà
	KeyTaux = CodeNomencl;
	Str.Format("%s%02d%05d","Taux_Sta",CodeRegion,CodeStation);
	GetPrivateProfileString (KeyTaux, Str,"", Buffer, 255,theApp.m_PathNomenclAnnee + "TauxFO_" + theApp.m_UtilTauxCur + ".ini");
	StrBuffer = Buffer;

	// S'il existe on vire les taux
	if (StrBuffer != "")
	{
		// Aucun taux
		StrTaux = "0";

		// Stockage aucun taux
		WritePrivateProfileString (KeyTaux, Str,StrTaux, theApp.m_PathNomenclAnnee + "TauxFO_" + theApp.m_UtilTauxCur + ".ini"); 
	}
	
}

void CFormulaireTauxFO::LireTaux(CString CodeNomencl,int CodeRegion,int CodeStation,CStringArray &TabTaux,int &NbTaux,CString &DateTaux,CString &UtilTaux,CStringArray &TabExTaux,int &NbExTaux,CString &ExDateTaux,CString &ExUtilTaux)
{
	// Nom utilisateur Util = variable défini en entrée
	CHAR Buffer[255];
	CString KeyTaux;
	CString Str;
	CString StrTaux;
	int PosSep;
	int PosSepSuiv;
	int i;
	CString Txt;   // A VIRER
	
	
	// Recherche les derniers taux associés
	KeyTaux = CodeNomencl;

	GetPrivateProfileString (KeyTaux, "Date","", Buffer, 255,theApp.m_PathNomenclAnnee + "TauxFO.ini");
	DateTaux = Buffer;
	GetPrivateProfileString (KeyTaux, "Util","", Buffer, 255,theApp.m_PathNomenclAnnee + "TauxFO.ini");
	UtilTaux = Buffer;
	Str.Format("%s%02d%05d","Taux_Sta",CodeRegion,CodeStation);
	GetPrivateProfileString (KeyTaux, Str,"", Buffer, 255,theApp.m_PathNomenclAnnee + "TauxFO.ini");
	StrTaux = Buffer;

	// Remplissage tableau des derniers taux
	if (StrTaux != "")
	{
		PosSep = StrTaux.Find(",",0);
		if (PosSep != 0)
		{
			NbTaux = atoi(StrTaux.Left(PosSep));
			TabTaux.SetSize(NbTaux);
			for (i=0;i<NbTaux;i++)
			{
				PosSepSuiv = StrTaux.Find(",",PosSep+1);
				if (PosSepSuiv == -1)
				{
			
					Txt = StrTaux.Mid(PosSep+1,PosSepSuiv-PosSep-1);
					TabTaux[i]= StrTaux.Mid(PosSep+1);
					break;
				}
				else
				{
					Txt = StrTaux.Mid(PosSep+1,PosSepSuiv-PosSep-1);
					TabTaux[i]= StrTaux.Mid(PosSep+1,PosSepSuiv-PosSep-1);
					PosSep = PosSepSuiv;
				}
			}
		}
		else
			NbTaux = 0;
	}

	// S'ils existent remplissage tableau des derniers taux
	KeyTaux = CodeNomencl + "_sav";
	GetPrivateProfileString (KeyTaux, "Date","", Buffer, 255,theApp.m_PathNomenclAnnee + "TauxFO.ini");
	ExDateTaux = Buffer;
	GetPrivateProfileString (KeyTaux, "Util","", Buffer, 255,theApp.m_PathNomenclAnnee + "TauxFO.ini");
	ExUtilTaux = Buffer;
	Str.Format("%s%02d%05d","Taux_Sta",CodeRegion,CodeStation);
	GetPrivateProfileString (KeyTaux, Str,"", Buffer, 255,theApp.m_PathNomenclAnnee + "TauxFO.ini");
	StrTaux = Buffer;

	// Remplissage tableau des taux précédents
	if (StrTaux != "")
	{
		PosSep = StrTaux.Find(",",0);
		if (PosSep != 0)
		{
			NbExTaux = atoi(StrTaux.Left(PosSep));
			TabExTaux.SetSize(NbExTaux);
			for (i=0;i<NbExTaux;i++)
			{
				PosSepSuiv = StrTaux.Find(",",PosSep+1);
				if (PosSepSuiv == -1)
				{
					TabExTaux[i]= StrTaux.Mid(PosSep+1);
					break;
				}
				else
				{
					TabExTaux[i]= StrTaux.Mid(PosSep+1,PosSepSuiv-PosSep-1);
					PosSep = PosSepSuiv;
				}
			}
		}
		else
			NbExTaux = 0;
	}
}

void CFormulaireTauxFO::OnChangeTaux1() 
{
	// Récupère les valeurs
	UpdateData(true);

	// Test si valeur taux correct
	if (ValeurTauxOk(m_Taux1))
		// Maj et calcul taux FO
		MajTauxFO(true);
	else
	{
		// Init zone de saisie pour resaisie
		m_Taux1 = "";
		UpdateData(false);
		m_EditTaux1.SetFocus();
	}
}

		

void CFormulaireTauxFO::OnChangeTaux2() 
{
	// Récupère les valeurs
	UpdateData(true);

	// Test si valeur taux correct
	if (ValeurTauxOk(m_Taux2))
		// Maj et calcul taux FO
		MajTauxFO(true);	
	else
	{
		// Init zone de saisie pour resaisie
		m_Taux2 = "";
		UpdateData(false);
		m_EditTaux2.SetFocus();
	}
}

void CFormulaireTauxFO::OnChangeTaux3() 
{
	// Récupère les valeurs
	UpdateData(true);

	// Test si valeur taux correct
	if (ValeurTauxOk(m_Taux3))
		// Maj et calcul taux FO
		MajTauxFO(true);	
	else
	{
		// Init zone de saisie pour resaisie
		m_Taux3 = "";
		UpdateData(false);
		m_EditTaux3.SetFocus();
	}
}

void CFormulaireTauxFO::OnChangeTaux4() 
{
	// Récupère les valeurs
	UpdateData(true);

	// Test si valeur taux correct
	if (ValeurTauxOk(m_Taux4))
		// Maj et calcul taux FO
		MajTauxFO(true);	
	else
	{
		// Init zone de saisie pour resaisie
		m_Taux4 = "";
		UpdateData(false);
		m_EditTaux4.SetFocus();
	}
}

void CFormulaireTauxFO::OnChangeTaux5() 
{
	// Récupère les valeurs
	UpdateData(true);

	// Test si valeur taux correct
	if (ValeurTauxOk(m_Taux5))
		// Maj et calcul taux FO
		MajTauxFO(true);
	else
	{
		// Init zone de saisie pour resaisie
		m_Taux5 = "";
		UpdateData(false);
		m_EditTaux5.SetFocus();
	}
}

void CFormulaireTauxFO::OnChangeTauxcm() 
{
	// Récupère les valeurs
	UpdateData(true);

	// Test si valeur taux correct
	if (ValeurTauxOk(m_TauxCM))
		// Maj et calcul taux FO
		MajTauxFO(true);
	else
	{
		// Init zone de saisie pour resaisie
		m_TauxCM = "";
		UpdateData(false);
		m_EditTauxCM.SetFocus();
	}
	
}

// Test si taux saisie correct
bool CFormulaireTauxFO:: ValeurTauxOk(CString &StrTaux)
{
	CString StrValTaux;
	
	// Test si caractère préfixe - ou +
	if (StrTaux.Left(1) == "-" || StrTaux.Left(1) == "+")
		StrValTaux = StrTaux.Mid(1);
	else
		StrValTaux = StrTaux;

	// Test si caractères valides
	if (IsNumeric(StrValTaux) == false)
	{
		AfxMessageBox ("Chaine non numérique");
		StrTaux = "";
		return false;
	}

	// Test valeur taux / limite < 100%
	if (StrValTaux == "0") return true;

	// Test valeur taux / limite < 100%
	if (StrValTaux == "") return true;

	
	float Valeur = atof(StrValTaux);

	if (atof(StrValTaux) > 100.0)
	{
		// valeur trop grande
		AfxMessageBox("Valeur taux trop grande (limité à +/- 100%)");	
		return false;
	}
	else if (atof(StrValTaux) <= 0)
	{
		// Caractères non valides
		AfxMessageBox("Caractères non valides");	
		return false;
	}
	else
	{

		return true;
	}

}

// Test si chaine numèrique
bool CFormulaireTauxFO::IsNumeric(CString Txt)
{
	if (Txt != "")
	{
		for (int i = 0;i< Txt.GetLength();i++)
		{
			if (Txt[i] != '0' && Txt[i] != '1' && Txt[i] != '2' && Txt[i] != '3' && Txt[i] != '4' && Txt[i] != '5' && Txt[i] != '6' && Txt[i] != '7' && Txt[i] != '8' && Txt[i] != '9' && Txt[i] != '.')     
			{
				return false;
				break;
			}
		}		
		return true;
	}
	else
		return true;
}

void CFormulaireTauxFO::MajTauxFO(bool NvTaux)
{
	float TauxFO;
	
	if (NvTaux)
	{
		// Calcul du taux fin ordre nouveaux taux 
		TauxFO = (1+(atof(m_Taux1)/100)) * (1+(atof(m_Taux2)/100)) * (1+(atof(m_Taux3)/100)) * (1+(atof(m_Taux4)/100)) * (1+(atof(m_Taux5)/100));
		TauxFO = TauxFO * (1+(atof(m_TauxCM)/100));
		TauxFO = (TauxFO-1) * 100;
		m_TauxFO.Format("%.2f",TauxFO);

	}	
	else
	{	
		// Calcul du taux fin ordre ancien taux 
		TauxFO = (1+(atof(m_ExTaux1)/100)) * (1+(atof(m_ExTaux2)/100)) * (1+(atof(m_ExTaux3)/100)) * (1+(atof(m_ExTaux4)/100)) * (1+(atof(m_ExTaux5)/100));
		TauxFO = TauxFO * (1+(atof(m_ExTauxCM)/100));
		TauxFO = (TauxFO-1) * 100;
		m_ExTauxFO.Format("%.2f",TauxFO);
	}

	// Toujours Maj Taux FO Simulés
	// Calcul du taux fin ordre nouveaux taux 
	TauxFO = (1+(atof(m_SimulTaux1)/100)) * (1+(atof(m_SimulTaux2)/100)) * (1+(atof(m_SimulTaux3)/100)) * (1+(atof(m_SimulTaux4)/100)) * (1+(atof(m_SimulTaux5)/100));
	TauxFO = TauxFO * (1+(atof(m_SimulTauxCM)/100));
	TauxFO = (TauxFO-1) * 100;
	//if (TauxFO > 0)
		m_SimulTauxFO.Format("%.2f",TauxFO);
	//else
	//	m_SimulTauxFO = "";

	UpdateData(false);

}


// Gestion des taux simulés
void CFormulaireTauxFO::LireSimulTaux(CString CodeNomencl,int CodeRegion,int CodeStation,CStringArray &TabTaux,int &NbTaux,CString &DateTaux,CString &UtilTaux)
{
	// Nom utilisateur Util = variable défini en entrée
	CHAR Buffer[255];
	CString KeyTaux;
	CString Str;
	CString StrTaux;
	int PosSep;
	int PosSepSuiv;
	int i;
	CString Txt;   // A VIRER
	
	
	// Recherche les derniers taux associés
	KeyTaux = CodeNomencl;
	NbTaux = 0;

	GetPrivateProfileString (KeyTaux, "Date","", Buffer, 255,theApp.m_PathNomenclAnnee + "TauxFO_" + theApp.m_UtilTauxCur + ".ini");
	DateTaux = Buffer;
	GetPrivateProfileString (KeyTaux, "Util","", Buffer, 255,theApp.m_PathNomenclAnnee + "TauxFO_" + theApp.m_UtilTauxCur + ".ini");
	UtilTaux = Buffer;
	Str.Format("%s%02d%05d","Taux_Sta",CodeRegion,CodeStation);
	GetPrivateProfileString (KeyTaux, Str,"", Buffer, 255,theApp.m_PathNomenclAnnee + "TauxFO_" + theApp.m_UtilTauxCur + ".ini");
	StrTaux = Buffer;

	// Remplissage tableau des derniers taux
	if (StrTaux != "")
	{
		PosSep = StrTaux.Find(",",0);
		if (PosSep != 0)
		{
			NbTaux = atoi(StrTaux.Left(PosSep));
			TabTaux.SetSize(NbTaux);
			for (i=0;i<NbTaux;i++)
			{
				PosSepSuiv = StrTaux.Find(",",PosSep+1);
				if (PosSepSuiv == -1)
				{
			
					Txt = StrTaux.Mid(PosSep+1,PosSepSuiv-PosSep-1);
					TabTaux[i]= StrTaux.Mid(PosSep+1);
					break;
				}
				else
				{
					Txt = StrTaux.Mid(PosSep+1,PosSepSuiv-PosSep-1);
					TabTaux[i]= StrTaux.Mid(PosSep+1,PosSepSuiv-PosSep-1);
					PosSep = PosSepSuiv;
				}
			}
		}
		else
			NbTaux = 0;
	}
	
}

// Ecriture nouveau taux simulés pour une station	
void CFormulaireTauxFO::EcrireSimulTaux(CString LibNomencl,CString CodeNomencl,int CodeRegion,int CodeStation,CStringArray &TabTaux)
{

	// Nom utilisateur Util = variable défini en entrée
	CHAR Buffer[255];
	Buffer[254] = 0;
	CString StrBuffer;
	CString Str;
	CString KeyTaux;
	CString KeyExTaux;
	CString StrTaux;
	CString Date;
	CString ExUtil;
	float Taux;
	int NbTaux;
	int i;
	COleDateTime DateTime;
	
	// Test si l'enregistrement existe déjà
	KeyTaux = CodeNomencl;
	Str.Format("%s%02d%05d","Taux_Sta",CodeRegion,CodeStation);
	GetPrivateProfileString (KeyTaux, Str,"", Buffer, 255,theApp.m_PathNomenclAnnee + "TauxFO_" + theApp.m_UtilTauxCur +".ini");
	StrBuffer = Buffer;

	// Date courante format dd/mm/yyyy - hhHmm
	DateTime=COleDateTime::GetCurrentTime();
	Date.Format("%02d/%02d/%4d-%02dH%02d", DateTime.GetDay(), DateTime.GetMonth(), DateTime.GetYear(), DateTime.GetHour(), DateTime.GetMinute());
	

	// Fabrique nouvelle chaine des taux pour cette station
	NbTaux = TabTaux.GetSize();

	// Nombre total de taux
	StrTaux.Format("%d",NbTaux);

	// Les n taux, en premier toujours le Taux CM
	for (i=0;i<TabTaux.GetSize();i++)
	{
		// Ok mais on limite à 2 décimales
		Taux  = atof(TabTaux[i]);
		TabTaux[i].Format("%.2f",Taux);

		StrTaux = StrTaux + ","+ TabTaux[i];
	}

	// Stockage des infos taux station
	WritePrivateProfileString (KeyTaux, "Libelle",LibNomencl, theApp.m_PathNomenclAnnee + "TauxFO_" + theApp.m_UtilTauxCur +".ini"); 
	WritePrivateProfileString (KeyTaux, "Date",Date, theApp.m_PathNomenclAnnee + "TauxFO_" + theApp.m_UtilTauxCur +".ini"); 
	WritePrivateProfileString (KeyTaux, "Util",theApp.m_UtilTauxCur, theApp.m_PathNomenclAnnee + "TauxFO_" + theApp.m_UtilTauxCur +".ini"); 
	WritePrivateProfileString (KeyTaux, Str,StrTaux, theApp.m_PathNomenclAnnee + "TauxFO_" + theApp.m_UtilTauxCur +".ini"); 

}

void CFormulaireTauxFO::OnSeltv() 
{
	UpdateData(true);

	// Recharge les chaines télés
	m_ListStation.ResetContent();
	ChargeChaine();

	// Init et calage pour entrée nouveaux taux
	InitTaux();
	InitSimulTaux();
	m_EditTaux1.SetFocus();
}

void CFormulaireTauxFO::OnSelradio() 
{
	UpdateData(true);

	// Recharge les stations radios
	m_ListStation.ResetContent();
	ChargeChaine();	

	// Init et calage pour entrée nouveaux taux
	InitTaux();
	InitSimulTaux();
	m_EditTaux1.SetFocus();
}

void CFormulaireTauxFO::OnChangeSimtaux1() 
{
	// Récupère les valeurs
	UpdateData(true);

	// Test si valeur taux correct
	if (ValeurTauxOk(m_SimulTaux1))
		// Maj et calcul taux FO
		MajTauxFO(true);
	else
	{
		// Init zone de saisie pour resaisie
		m_SimulTaux1 = "";
		UpdateData(false);
		m_EditSimulTaux1.SetFocus();
	}
}

void CFormulaireTauxFO::OnChangeSimtaux2() 
{
	// Récupère les valeurs
	UpdateData(true);

	// Test si valeur taux correct
	if (ValeurTauxOk(m_SimulTaux2))
		// Maj et calcul taux FO
		MajTauxFO(true);
	else
	{
		// Init zone de saisie pour resaisie
		m_SimulTaux2 = "";
		UpdateData(false);
		m_EditSimulTaux2.SetFocus();
	}	
}

void CFormulaireTauxFO::OnChangeSimtaux3() 
{
	// Récupère les valeurs
	UpdateData(true);

	// Test si valeur taux correct
	if (ValeurTauxOk(m_SimulTaux3))
		// Maj et calcul taux FO
		MajTauxFO(true);
	else
	{
		// Init zone de saisie pour resaisie
		m_SimulTaux3 = "";
		UpdateData(false);
		m_EditSimulTaux3.SetFocus();
	}
}

void CFormulaireTauxFO::OnChangeSimtaux4() 
{
	// Récupère les valeurs
	UpdateData(true);

	// Test si valeur taux correct
	if (ValeurTauxOk(m_SimulTaux4))
		// Maj et calcul taux FO
		MajTauxFO(true);
	else
	{
		// Init zone de saisie pour resaisie
		m_SimulTaux4 = "";
		UpdateData(false);
		m_EditSimulTaux4.SetFocus();
	}
}

void CFormulaireTauxFO::OnChangeSimtaux5() 
{
	// Récupère les valeurs
	UpdateData(true);

	// Test si valeur taux correct
	if (ValeurTauxOk(m_SimulTaux5))
		// Maj et calcul taux FO
		MajTauxFO(true);
	else
	{
		// Init zone de saisie pour resaisie
		m_SimulTaux5 = "";
		UpdateData(false);
		m_EditSimulTaux5.SetFocus();
	}	
}

void CFormulaireTauxFO::OnChangeSimtauxcm() 
{
	// Récupère les valeurs
	UpdateData(true);

	// Test si valeur taux correct
	if (ValeurTauxOk(m_SimulTauxCM))
		// Maj et calcul taux FO
		MajTauxFO(true);
	else
	{
		// Init zone de saisie pour resaisie
		m_SimulTauxCM = "";
		UpdateData(false);
		m_EditSimulTauxCM.SetFocus();
	}
}

void CFormulaireTauxFO::OnSuppTauxSimul() 
{
	CString TxtStation;
	int CodeReg;
	int CodeStation;
	
	// Initialiser les boites de saisie
	InitSimulTaux();
	
	// Pour toutes les chaines sélectionnées on supprime les taux
	for (int i=0;i<m_ListStation.GetCount();i++)
	{
		if (m_ListStation.GetSel(i) >0)
		{
			// On vire les derniers taux de la station
			m_ListStation.GetText(i,TxtStation);
			
			// Récup code national ou régional
			CodeReg = atoi(TxtStation.Left(2));
			/*
			if      (TxtStation.Left(3) == "Nat")
				CodeReg = 0;
			else if (TxtStation.Left(3) == "Idf")
				CodeReg = 1;
			else
				CodeReg = 0;
			*/

			// Nouveau format code station (sur 5 caractéres)
			CodeStation = atoi(TxtStation.Mid(49,CNomenclatureApp::NBCODESTATION));
			DetruireSimulTaux(m_LibelleNomencl,m_CodeNomencl,CodeReg,CodeStation);
			
			// Déselectionne la station
			m_ListStation.SetSel( i, FALSE);
		}
	}
}

HBRUSH CFormulaireTauxFO::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Return a different brush if the default is not desired
	if (nCtlColor == IDC_LIST_STATIONS)
	{
		pDC->SetBkColor(RGB_VertEmeraude);
	}

	return hbr;
}
