// DialogTiersEDI.cpp : implementation file
//

#include "stdafx.h"
#include "nomenclature.h"
#include "DialogTiersEDI.h"
#include "FormulaireEDI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CNomenclatureApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDialogTiersEDI dialog


CDialogTiersEDI::CDialogTiersEDI(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogTiersEDI::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogTiersEDI)
	//}}AFX_DATA_INIT
}


void CDialogTiersEDI::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogTiersEDI)
	DDX_Control(pDX, IDC_SUPPRIMER, m_SuppTiers);
	DDX_Control(pDX, IDC_AJOUTER, m_AjoutTiers);
	DDX_Control(pDX, IDC_LIST_TIERSEDI, m_ListTiersEDI);
	DDX_Control(pDX, IDC_CADRE1, m_Cadre1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogTiersEDI, CDialog)
	//{{AFX_MSG_MAP(CDialogTiersEDI)
	ON_BN_CLICKED(IDC_AJOUTER, OnAjouter)
	ON_BN_CLICKED(IDC_MODIFIER, OnModifier)
	ON_BN_CLICKED(IDC_SUPPRIMER, OnSupprimer)
	ON_WM_PAINT()
	ON_LBN_DBLCLK(IDC_LIST_TIERSEDI, OnDblclkListTiersedi)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogTiersEDI message handlers

// Ajout d'une nouvelle r�gie ou centrale d'achat
void CDialogTiersEDI::OnAjouter() 
{
	bool Ok;
	int IndexTab;

	// Affichage boite de dialogue
	CFormulaireEDI FormulaireEDI;
	FormulaireEDI.m_TypeTiers   = m_TypeTiers;
	FormulaireEDI.m_TypeCodeTiers = "107";

	// Titre de la fenetre EDI
	if (m_TypeTiers == "DS")
		FormulaireEDI.m_TitreFenEDI = "Informations EDI / REGIE";
	else
		FormulaireEDI.m_TitreFenEDI = "Informations EDI / CENTRALE ACHAT";

	// en mode ajout aucun contact au d�part
	FormulaireEDI.m_NbContact = 0;
	FormulaireEDI.m_Mode = 0;

	// Affichage du formulaire saisie des donn�es EDI
	if (FormulaireEDI.DoModal() == IDOK)
	{
		// Mise � jour du fichier r�gie ou centrale
		// V�rifie Elmt EDI non existant
		if (ElmtEDIExist(FormulaireEDI.m_NomTiers,IndexTab,false) == false)
		{
			// R�cup�re toutes les infos dans nouvelles stuctures SInfoEDI
			// Les informations g�n�rales
			m_InfoEDI.m_InfoGeneralEDI.m_TypeTiers       = FormulaireEDI.m_TypeTiers;
			m_InfoEDI.m_InfoGeneralEDI.m_CodeTiers       = FormulaireEDI.m_CodeTiers;
			m_InfoEDI.m_InfoGeneralEDI.m_TypeCodeTiers   = FormulaireEDI.m_TypeCodeTiers;
			m_InfoEDI.m_InfoGeneralEDI.m_NomTiers        = FormulaireEDI.m_NomTiers;    
			m_InfoEDI.m_InfoGeneralEDI.m_AdresseTiers1   = FormulaireEDI.m_AdresseTiers1;
			m_InfoEDI.m_InfoGeneralEDI.m_AdresseTiers2   = FormulaireEDI.m_AdresseTiers2 ;
			m_InfoEDI.m_InfoGeneralEDI.m_CodePostalTiers = FormulaireEDI.m_CodePostalTiers;
			m_InfoEDI.m_InfoGeneralEDI.m_VilleTiers      = FormulaireEDI.m_VilleTiers;
			m_InfoEDI.m_InfoGeneralEDI.m_CodePaysTiers   = FormulaireEDI.m_CodePaysTiers;
			m_InfoEDI.m_InfoGeneralEDI.m_NoTelTiers      = FormulaireEDI.m_NoTelTiers;
			m_InfoEDI.m_InfoGeneralEDI.m_NoFaxTiers      = FormulaireEDI.m_NoFaxTiers;

			// Les informations contacts
			m_InfoEDI.m_TContact.RemoveAll();
			for (int i=0;i<FormulaireEDI.m_TabInfoContact.GetSize();i++)
			{
				// Ajout nouveau contact interlocuteur
				m_InfoContact.m_NomContactTiers     = FormulaireEDI.m_TabInfoContact[i].m_NomContactTiers; 
				m_InfoContact.m_PrenomContactTiers  = FormulaireEDI.m_TabInfoContact[i].m_PrenomContactTiers; 
				m_InfoContact.m_AdrMailContactTiers = FormulaireEDI.m_TabInfoContact[i].m_AdrMailContactTiers;  
				m_InfoEDI.m_TContact.Add(m_InfoContact); 
			}

			// Ajoute au tableau existant
			m_TabInfoEDI.Add(m_InfoEDI);

			// Recopie tableau dans fichier
			if (SaveElmtEDI()== false)
			{
				AfxMessageBox("Probl�me sauvegarde");
			}
			else
				// Recharge la liste des nouveaux �l�ments
				if (m_TypeTiers == "DS")
					Ok = ChargeTiersEDI_Regie(); 
				else
					Ok = ChargeTiersEDI_Centrale(); 
			}

		else
			AfxMessageBox("Elmt EDI " + FormulaireEDI.m_NomTiers + " d�j� existant");
	}
	
}

// Modification d'une nouvelle r�gie ou centrale d'achat
void CDialogTiersEDI::OnModifier() 
{
	int IndexTab = -1;

	// Affichage boite de dialogue
	CFormulaireEDI FormulaireEDI;
	FormulaireEDI.m_TypeTiers   = m_TypeTiers;
	FormulaireEDI.m_TypeCodeTiers = "107";

	// Titre de la fenetre EDI
	if (m_TypeTiers == "DS")
		FormulaireEDI.m_TitreFenEDI = "Informations EDI / REGIE";
	else
		FormulaireEDI.m_TitreFenEDI = "Informations EDI / CENTRALE ACHAT";
	
	// Index �lmt EDI s�lectionn� pour �tre modifi�
	int IndexSelEDI = m_ListTiersEDI.GetCurSel();

	if (IndexSelEDI >=0)
	{
		// R�cup�ration des infos de l'�lmt EDI s�lectionn�
		CString StrElmt;
		m_ListTiersEDI.GetText(IndexSelEDI,StrElmt);

		// R�cup Index Elmt dans tableau des �lmts EDI
		if (ElmtEDIExist(StrElmt,IndexTab,m_TypeTiers == "DS") == true)
		{
			if (IndexTab >= 0)
			{
				// Passage Code code tiers
				FormulaireEDI.m_CodeTiers = m_TabInfoEDI[IndexTab].m_InfoGeneralEDI.m_CodeTiers;

				// Passage Type code tiers
				FormulaireEDI.m_TypeCodeTiers = m_TabInfoEDI[IndexTab].m_InfoGeneralEDI.m_TypeCodeTiers;

				// Passage Nom du tiers EDI
				FormulaireEDI.m_NomTiers = StrElmt;
			
				// Passage Adresse 1
				FormulaireEDI.m_AdresseTiers1 = m_TabInfoEDI[IndexTab].m_InfoGeneralEDI.m_AdresseTiers1;
				if (FormulaireEDI.m_AdresseTiers1 == "xxx") FormulaireEDI.m_AdresseTiers1 = "";
			
				// Passage Adresse 2
				FormulaireEDI.m_AdresseTiers2 = m_TabInfoEDI[IndexTab].m_InfoGeneralEDI.m_AdresseTiers2;
				if (FormulaireEDI.m_AdresseTiers2 == "xxx") FormulaireEDI.m_AdresseTiers2 = "";
				
				// Passeage Code Postal
				FormulaireEDI.m_CodePostalTiers = m_TabInfoEDI[IndexTab].m_InfoGeneralEDI.m_CodePostalTiers;
				if (FormulaireEDI.m_CodePostalTiers == "xxx") FormulaireEDI.m_CodePostalTiers = "";

				// Passage Ville
				FormulaireEDI.m_VilleTiers =  m_TabInfoEDI[IndexTab].m_InfoGeneralEDI.m_VilleTiers;
				if (FormulaireEDI.m_VilleTiers == "xxx") FormulaireEDI.m_VilleTiers = "";

				// Passage Code pays
				FormulaireEDI.m_CodePaysTiers =  m_TabInfoEDI[IndexTab].m_InfoGeneralEDI.m_CodePaysTiers;
				if (FormulaireEDI.m_CodePaysTiers == "xxx") FormulaireEDI.m_CodePaysTiers = "";
				
				// Passage T�l�phone
				FormulaireEDI.m_NoTelTiers = m_TabInfoEDI[IndexTab].m_InfoGeneralEDI.m_NoTelTiers;
				if (FormulaireEDI.m_NoTelTiers == "xxx") FormulaireEDI.m_NoTelTiers = "";

				// Passage Fax
				FormulaireEDI.m_NoFaxTiers = m_TabInfoEDI[IndexTab].m_InfoGeneralEDI.m_NoFaxTiers;
				if (FormulaireEDI.m_NoFaxTiers == "xxx") FormulaireEDI.m_NoFaxTiers = "";
						
				// Passage nombre d'interlocuteurs
				FormulaireEDI.m_NbContact = m_TabInfoEDI[IndexTab].m_TContact.GetSize();

				// puis on passe les 3 infos pour chacun des contacts de ce tiers
				FormulaireEDI.m_TabInfoContact.RemoveAll(); 
				for (int k=0;k<m_TabInfoEDI[IndexTab].m_TContact.GetSize();k++)
				{
					// Recup ki�me nom contact 
					FormulaireEDI.m_InfoContact.m_NomContactTiers     = m_TabInfoEDI[IndexTab].m_TContact[k].m_NomContactTiers;
					if (FormulaireEDI.m_InfoContact.m_NomContactTiers == "xxx") FormulaireEDI.m_InfoContact.m_NomContactTiers = "";

					// Recup ki�me pr�nom contact 
					FormulaireEDI.m_InfoContact.m_PrenomContactTiers  = m_TabInfoEDI[IndexTab].m_TContact[k].m_PrenomContactTiers;
					if (FormulaireEDI.m_InfoContact.m_PrenomContactTiers == "xxx") FormulaireEDI.m_InfoContact.m_PrenomContactTiers = "";

					// Recup ki�me adresse mail contact 
					FormulaireEDI.m_InfoContact.m_AdrMailContactTiers = m_TabInfoEDI[IndexTab].m_TContact[k].m_AdrMailContactTiers;
					if (FormulaireEDI.m_InfoContact.m_AdrMailContactTiers == "xxx") FormulaireEDI.m_InfoContact.m_AdrMailContactTiers = "";

					// Ajoute bloc infos au tableau contacts
					FormulaireEDI.m_TabInfoContact.Add(FormulaireEDI.m_InfoContact); 

				}

				// Signal formulaire qu'on ait en mode modification (nom EDI fig�)
				FormulaireEDI.m_Mode = 1;

				if (FormulaireEDI.DoModal() == IDOK)
				{
					// R�cup�re toutes les infos dans nouvelles stuctures SInfoEDI
					// Les informations g�n�rales
					m_InfoEDI.m_InfoGeneralEDI.m_TypeTiers       = FormulaireEDI.m_TypeTiers;
					m_InfoEDI.m_InfoGeneralEDI.m_CodeTiers       = FormulaireEDI.m_CodeTiers;
					m_InfoEDI.m_InfoGeneralEDI.m_TypeCodeTiers   = FormulaireEDI.m_TypeCodeTiers;

					// Nom du tiers
					if (m_InfoEDI.m_InfoGeneralEDI.m_TypeTiers == "DS")
						// pour les r�gies on prend la correspondance abr�g� du nom r�gie
						m_InfoEDI.m_InfoGeneralEDI.m_NomTiers        = NomAbrRegie(FormulaireEDI.m_NomTiers);    
					else
						// pour les centrales d'achat on prend le nom complet
						m_InfoEDI.m_InfoGeneralEDI.m_NomTiers        = FormulaireEDI.m_NomTiers;    

					m_InfoEDI.m_InfoGeneralEDI.m_AdresseTiers1   = FormulaireEDI.m_AdresseTiers1;
					m_InfoEDI.m_InfoGeneralEDI.m_AdresseTiers2   = FormulaireEDI.m_AdresseTiers2 ;
					m_InfoEDI.m_InfoGeneralEDI.m_CodePostalTiers = FormulaireEDI.m_CodePostalTiers;
					m_InfoEDI.m_InfoGeneralEDI.m_VilleTiers      = FormulaireEDI.m_VilleTiers;
					m_InfoEDI.m_InfoGeneralEDI.m_CodePaysTiers   = FormulaireEDI.m_CodePaysTiers;
					m_InfoEDI.m_InfoGeneralEDI.m_NoTelTiers      = FormulaireEDI.m_NoTelTiers;
					m_InfoEDI.m_InfoGeneralEDI.m_NoFaxTiers      = FormulaireEDI.m_NoFaxTiers;

					// Remplace infos g�n�rales du tableau existant
					m_TabInfoEDI[IndexTab].m_InfoGeneralEDI = m_InfoEDI.m_InfoGeneralEDI;

					// Remplace les informations contacts
					m_TabInfoEDI[IndexTab].m_TContact.RemoveAll();
					for (int i=0;i<FormulaireEDI.m_TabInfoContact.GetSize();i++)
					{
						// Ajout nouveau contact interlocuteur
						m_InfoContact.m_NomContactTiers     = FormulaireEDI.m_TabInfoContact[i].m_NomContactTiers; 
						m_InfoContact.m_PrenomContactTiers  = FormulaireEDI.m_TabInfoContact[i].m_PrenomContactTiers; 
						m_InfoContact.m_AdrMailContactTiers = FormulaireEDI.m_TabInfoContact[i].m_AdrMailContactTiers;  
						m_TabInfoEDI[IndexTab].m_TContact.Add(m_InfoContact); 
					}

					// Recopie tableau dans fichier
					if (SaveElmtEDI()== false)
					{
						AfxMessageBox("Probl�me sauvegarde");
					}
					else
						// Recharge la liste des nouveaux �l�ments
						if (m_TypeTiers == "DS")
							bool Ok = ChargeTiersEDI_Regie(); 
						else
							bool Ok = ChargeTiersEDI_Centrale(); 


					
				}
			}
		}
	}
}

// Suppression r�gie ou centrale d'achat existante
void CDialogTiersEDI::OnSupprimer() 
{
	// Suppression �lmt s�lectionner
	if (m_ListTiersEDI.GetCurSel() != LB_ERR )
		SupprimeTiersEDI();
}

void CDialogTiersEDI::OnPaint() 
{
	// Coloriage du fond de ce formulaire
	CRect rect;

	CPaintDC dc(this); // device context for painting
	
	// Coloriage du fond de ce formulaire
	CBrush fond(RGB_BleuJFC);
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);	


}

BOOL CDialogTiersEDI::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// R�ecriture entete fenetre
	SetWindowText(m_TitreFenTiersEDI);

	// En mode R�gie, pas d'ajout et suppression possible (seulement chargement des r�gies Atelier Radio)
	if (m_TypeTiers == "DS")
	{
		// Suppression des touches ajout et suppression
		m_AjoutTiers.ShowWindow(SW_HIDE);
		m_SuppTiers.ShowWindow(SW_HIDE);


		// Chargement fichier r�gie Atelier Radio
		if (ChargeRegie() == false)
		{
			AfxMessageBox("Erreur chargement fichier R�gies Atelier Radio");
			return false;
		}

		// Chargement des r�gies nomenclature
		if (ChargeTiersEDI_Regie() == false)
		{
			AfxMessageBox("Erreur chargement fichier R�gies");
			return FALSE;
		}

	}

	else
	{
		// Chargement centrale d'achat
		if (ChargeTiersEDI_Centrale() == false)
		{
			AfxMessageBox("Erreur chargement fichier Centrales d'achat");
			return FALSE;
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// Chargement des r�gies 
bool CDialogTiersEDI::ChargeTiersEDI_Regie()
{
	CString FicTiersEDI;
	CString Ligne;
	CPWFile File;
	int NbTiers;
	int NbContact;

	// Chargement fichier r�gie
	//if (ChargeRegie() == false) return false;

	// les structures infos EDI
	SInfoContact InfoContact;
	SInfoEDI InfoEDI;

	// Nom du fichier tiers
	FicTiersEDI = theApp.m_PathNomencl + "Regie.arb"; 

	// Initialisation de la liste existante
	m_ListTiersEDI.ResetContent();
	
	// Lecture des tiers
	if (File.OpenFile(FicTiersEDI))
	{
		// Ligne commentaire
		File.ReadString(&Ligne);

		// Ligne #REGIE
		File.ReadString(&Ligne);

		// Ligne info nb tiers
		File.ReadString(&Ligne);
		NbTiers = atoi(Ligne.Mid(1));

		// Dimensionne tableau courant des �l�mts EDI
		m_TabInfoEDI.RemoveAll();
		
		// On charge tous les tiers dans la liste
		for (int i=0;i<NbTiers;i++)
		{

			InfoEDI.m_TContact.RemoveAll();
			
			// Lecture ligne type tiers
			File.ReadString(&Ligne);
			InfoEDI.m_InfoGeneralEDI.m_TypeTiers = Ligne;	

			// Lecture ligne code tiers
			File.ReadString(&Ligne);
			InfoEDI.m_InfoGeneralEDI.m_CodeTiers = Ligne;

			// Lecture ligne type code tiers
			File.ReadString(&Ligne);
			InfoEDI.m_InfoGeneralEDI.m_TypeCodeTiers = Ligne;

			// Lecture nom du tiers
			File.ReadString(&Ligne);
			InfoEDI.m_InfoGeneralEDI.m_NomTiers = Ligne;

			// Ajout de ce tiers � la liste des noms r�gies
			CString NomRegie = NomCompletRegie(Ligne);
			m_ListTiersEDI.AddString(NomRegie);

			// puis on passe les 7 informations suivantes
			// Adresse 1
			File.ReadString(&Ligne);
			InfoEDI.m_InfoGeneralEDI.m_AdresseTiers1  = Ligne;

			// Adresse 2
			File.ReadString(&Ligne);
			InfoEDI.m_InfoGeneralEDI.m_AdresseTiers2 = Ligne;

			// Code Postal
			File.ReadString(&Ligne);
			InfoEDI.m_InfoGeneralEDI.m_CodePostalTiers  = Ligne;

			// Ville
			File.ReadString(&Ligne);
			InfoEDI.m_InfoGeneralEDI.m_VilleTiers  = Ligne;

			// Code pays
			File.ReadString(&Ligne);
			InfoEDI.m_InfoGeneralEDI.m_CodePaysTiers  = Ligne;

			// T�l�phone
			File.ReadString(&Ligne);
			InfoEDI.m_InfoGeneralEDI.m_NoTelTiers  = Ligne;

			// Fax
			File.ReadString(&Ligne);
			InfoEDI.m_InfoGeneralEDI.m_NoFaxTiers  = Ligne;

			
			// lecture nombre d'interlocuteurs
			File.ReadString(&Ligne);
			NbContact = atoi(Ligne.Mid(1));

			// puis on passe les 3 infos pour chacun des contacts de ce tiers
			for (int k=0;k<NbContact;k++)
			{
				// info nom
				File.ReadString(&Ligne);
				InfoContact.m_NomContactTiers = Ligne;

				// info pr�nom
				File.ReadString(&Ligne);
				InfoContact.m_PrenomContactTiers = Ligne;

				// info adr mail
				File.ReadString(&Ligne);
				InfoContact.m_AdrMailContactTiers = Ligne;

				// ajoute cet �lmt dans tableau des contacts
				InfoEDI.m_TContact.Add(InfoContact);

				// info #0
				File.ReadString(&Ligne);
			}

			// Ajoute infos de cet �lmt au tableau global des EDI courants
			m_TabInfoEDI.Add(InfoEDI);
		}


		int NbElmt = m_TabInfoEDI.GetSize();

		// fermeture fichier
		File.CloseFile();	

		return true;
	}
	else
	{
		// Cr�ation du nouveau fichier via le fichier R�gie Atelier Radio
		if( File.Open(FicTiersEDI,CFile::modeCreate | CFile::modeWrite))
		{
			// 2 lignes commentaires R�gie
			File.WriteString("// Descriptif des r�gies\n");
			return true;
		}
		
		else 
			return false;
	}

}		

// Chargement du tableau des infos r�gies
bool CDialogTiersEDI::ChargeTabInfoRegie()
{
	// les structures infos EDI
	SInfoContact InfoContact;
	SInfoEDI InfoEDI;
	CString FicTiersEDI;
	CString Ligne;
	CPWFile File;

	// Nom du fichier tiers
	FicTiersEDI = theApp.m_PathNomencl + "Regie.arb"; 

	// Initialisation de la liste existante
	m_ListTiersEDI.ResetContent();
	
	// Lecture des tiers
	if (File.OpenFile(FicTiersEDI))
	{
		// Ligne commentaire
		File.ReadString(&Ligne);

		// Ligne #REGIE
		File.ReadString(&Ligne);

		// Ligne info nb tiers
		File.ReadString(&Ligne);
		int NbTiers = atoi(Ligne.Mid(1));

		// Dimensionne tableau courant des �l�mts EDI
		m_TabInfoEDI.RemoveAll();
		
		// On charge tous les tiers dans la liste
		for (int i=0;i<NbTiers;i++)
		{

			InfoEDI.m_TContact.RemoveAll();
			
			// Lecture ligne type tiers
			File.ReadString(&Ligne);
			InfoEDI.m_InfoGeneralEDI.m_TypeTiers = Ligne;	

			// Lecture ligne code tiers
			File.ReadString(&Ligne);
			InfoEDI.m_InfoGeneralEDI.m_CodeTiers = Ligne;

			// Lecture ligne type code tiers
			File.ReadString(&Ligne);
			InfoEDI.m_InfoGeneralEDI.m_TypeCodeTiers = Ligne;

			// Lecture nom du tiers
			File.ReadString(&Ligne);
			InfoEDI.m_InfoGeneralEDI.m_NomTiers = Ligne;

			// Ajout de ce tiers � la liste des noms r�gies
			CString NomRegie = NomCompletRegie(Ligne);
			m_ListTiersEDI.AddString(NomRegie);

			// puis on passe les 7 informations suivantes
			// Adresse 1
			File.ReadString(&Ligne);
			InfoEDI.m_InfoGeneralEDI.m_AdresseTiers1  = Ligne;

			// Adresse 2
			File.ReadString(&Ligne);
			InfoEDI.m_InfoGeneralEDI.m_AdresseTiers2 = Ligne;

			// Code Postal
			File.ReadString(&Ligne);
			InfoEDI.m_InfoGeneralEDI.m_CodePostalTiers  = Ligne;

			// Ville
			File.ReadString(&Ligne);
			InfoEDI.m_InfoGeneralEDI.m_VilleTiers  = Ligne;

			// Code pays
			File.ReadString(&Ligne);
			InfoEDI.m_InfoGeneralEDI.m_CodePaysTiers  = Ligne;

			// T�l�phone
			File.ReadString(&Ligne);
			InfoEDI.m_InfoGeneralEDI.m_NoTelTiers  = Ligne;

			// Fax
			File.ReadString(&Ligne);
			InfoEDI.m_InfoGeneralEDI.m_NoFaxTiers  = Ligne;

			
			// lecture nombre d'interlocuteurs
			File.ReadString(&Ligne);
			int NbContact = atoi(Ligne.Mid(1));

			// puis on passe les 3 infos pour chacun des contacts de ce tiers
			for (int k=0;k<NbContact;k++)
			{
				// info nom
				File.ReadString(&Ligne);
				InfoContact.m_NomContactTiers = Ligne;

				// info pr�nom
				File.ReadString(&Ligne);
				InfoContact.m_PrenomContactTiers = Ligne;

				// info adr mail
				File.ReadString(&Ligne);
				InfoContact.m_AdrMailContactTiers = Ligne;

				// ajoute cet �lmt dans tableau des contacts
				InfoEDI.m_TContact.Add(InfoContact);

				// info #0
				File.ReadString(&Ligne);
			}

			// Ajoute infos de cet �lmt au tableau global des EDI courants
			m_TabInfoEDI.Add(InfoEDI);
		}
		// fermeture fichier
		File.CloseFile();	

		return true;
	}

	// Probl�me ouverture fichier nomenclature R�gie
	return false;

}	

// Synchronistaion fichier R�gie Atelier RAdio et Fichier R�gie courant Nomenclature
void CDialogTiersEDI::SynchroniseFichiersRegieRadio()
{
	// Ici il faut v�rifier que le fichier r�gie Atelier Radio est bien en phase avec le fichier R�gie Nomenclature
	// Chargement du tableau des infos r�gies (m_TabInfoEDI)
	if (ChargeTabInfoRegie())
	{
		// Verif avec m_TInfoRegie (tableau des r�gies Atelier radio)
		
		// On vire les r�gies plus utilis�es
		for (int i=m_TabInfoEDI.GetSize()-1;i>=0;i--)
		{
			CString NomComplet = NomCompletRegie(m_TabInfoEDI[i].m_InfoGeneralEDI.m_NomTiers);
			if (NomComplet == "xxx")
			{
				// Cette r�gie ne fait plus partie des r�gies Atelier Radio
				m_TabInfoEDI.RemoveAt(i,1);
			}
		}

		// on ajoute les nouvelles r�gies apparues dans fichier r�gie Atelier Radio
		for (int j=0;j<m_TInfoRegie.GetSize();j++)
		{
			// Recherche r�gie dans fichier nomenclature courant
			bool RegieTrouvee = false;
			for (int k=0;k<m_TabInfoEDI.GetSize();k++)
			{
				if (m_TabInfoEDI[k].m_InfoGeneralEDI.m_NomTiers == m_TInfoRegie[j].AbrRegie)
				{
					RegieTrouvee = true;
					break;
				}
			}

			if (RegieTrouvee == false)
			{
				// si r�gie non trouv�e on l'ajoute au tableau
				SInfoContact InfoContact;
				SInfoEDI InfoEDI;

				// pas de contact au d�part
				InfoEDI.m_TContact.RemoveAll();
				
				// Type tiers
				InfoEDI.m_InfoGeneralEDI.m_TypeTiers = "DS";	

				// Code tiers
				InfoEDI.m_InfoGeneralEDI.m_CodeTiers = "xxx";

				// Type Code tiers
				InfoEDI.m_InfoGeneralEDI.m_TypeCodeTiers = "107";

				// Nom du tiers
				InfoEDI.m_InfoGeneralEDI.m_NomTiers = m_TInfoRegie[j].AbrRegie;

				// Adresse 1 + Adresse 2  + Code Postal + Ville + Pays + Tel + Fax
				InfoEDI.m_InfoGeneralEDI.m_AdresseTiers1    = "xxx";
				InfoEDI.m_InfoGeneralEDI.m_AdresseTiers2    = "xxx";
				InfoEDI.m_InfoGeneralEDI.m_CodePostalTiers  = "xxx";
				InfoEDI.m_InfoGeneralEDI.m_VilleTiers       = "xxx";
				InfoEDI.m_InfoGeneralEDI.m_CodePaysTiers    = "xxx";
				InfoEDI.m_InfoGeneralEDI.m_NoTelTiers       = "xxx";
				InfoEDI.m_InfoGeneralEDI.m_NoFaxTiers       = "xxx";

				// Ajoute infos de cet �lmt au tableau global des EDI courants
				m_TabInfoEDI.Add(InfoEDI);
			}		
		}

	}
	else
		AfxMessageBox("Probl�me synchronisation fichier R�gie Atelier Radio",MB_ICONEXCLAMATION);

}	

// Chargement fichier origine des r�gies
bool CDialogTiersEDI::ChargeRegie()
{
	CPWFile File;
	int NbRegies = 0;
	CString Ligne;
	SInfoRegie InfoRegie;

	// Nom du fichiuer table r�gie dans Atelier Radio
	CString FicOrgRegie = theApp.m_PathArtRadio + "\Data\\Fr_Regie.tbl";

	// Lecture des tiers
	if (File.OpenFile(FicOrgRegie))
	{

		// recherche Nb r�gies	
		File.ReadString(&Ligne);		
		while (Ligne.Left(1) != "*")
			File.ReadString(&Ligne);		

		// renvoi false si *Nb Regie non trouv�
		if (Ligne.Left(1) != "*")
		{
			File.CloseFile();
			return false;
		}
		else
			// nombre de r�gies
			NbRegies = atoi(Ligne.Mid(1));

		// puis on recherche # (d�but de la liste des r�gies)
		File.ReadString(&Ligne);		
		while (Ligne.Left(1) != "#")
			File.ReadString(&Ligne);		

		// renvoi false si # non trouv�
		if (Ligne.Left(1) != "#")
		{
			File.CloseFile();
			return false;
		}

		// remplissage du tableau des r�gies
		m_TInfoRegie.RemoveAll();
		for (int i=0;i< NbRegies;i++)
		{
			// lecture ligne r�gie
			File.ReadString(&Ligne);		

			// r�cup abr�viation r�gie
			InfoRegie.AbrRegie = Ligne.Mid(4,5);

			// r�cup liblell� r�gie
			InfoRegie.StrRegie = Ligne.Mid(10,30);

			// Ajout r�gie � la table
			m_TInfoRegie.Add(InfoRegie);
		}
		
		// fermeture fichier
		File.CloseFile();	


		// puis on cr�e le fichier Nomenclature R�gie s'il n'existe pas
		// Nom du fichier tiers
		CString FicTiersEDI = theApp.m_PathNomencl + "Regie.arb"; 

		// Lecture des tiers
		if (File.OpenFile(FicTiersEDI) == false)
		{

			if( File.Open(FicTiersEDI,CFile::modeCreate | CFile::modeWrite))
			{
				// 2 lignes commentaires R�gie
				File.WriteString("// Descriptif des r�gies\n");
				File.WriteString("#REGIE\n");

				// Nombre d'�lmts (r�gie ou ce,trale d'achat)
				Ligne.Format("#%d\n",m_TInfoRegie.GetSize());
				File.WriteString(Ligne);

				// On boucle sur tous les �lmts EDI
				for (int i=0;i<m_TInfoRegie.GetSize();i++)
				{
					// Eciture ligne type tiers
					Ligne.Format("%s\n","DS");
					File.WriteString(Ligne);
					
					// Ecriture ligne code tiers vide
					File.WriteString("xxx\n");
					
					// Ecriture ligne type code tiers toujours 107
					File.WriteString("107\n");
					
					// Ecriture nom du tiers
					Ligne.Format("%s\n",m_TInfoRegie[i].AbrRegie);
					File.WriteString(Ligne);
					
					// Ecriture Adresse 1 vide
					File.WriteString("xxx\n");
					
					// Ecriture Adresse 2 vide
					File.WriteString("xxx\n");

					// Ecriture Code Postal vide
					File.WriteString("xxx\n");
					
					// Ecriture Ville vide
					File.WriteString("xxx\n");				

					// Ecriture Code pays vide
					File.WriteString("xxx\n");
					 
					// Ecriture T�l�phone vide
					File.WriteString("xxx\n");
					
					// Ecriture Fax vide
					File.WriteString("xxx\n");
								
					// Ecriture nombre d'interlocuteurs
					Ligne.Format("#%d\n",0);
					File.WriteString(Ligne);
				}

				// fermeture du fichier Nomenclature R�gie
				File.CloseFile();
			}

		}
		else
		{
			// fermeture du fichier avant �ventuellement la mise � jour	
			File.CloseFile();

			// s'il existe on v�rifie qu'il correspond au fichier Atelier Radio
			SynchroniseFichiersRegieRadio();

			// on sauve le nouveau fichier nomenclature r�gie
			// Recopie tableau dans fichier
			if (SaveElmtEDI()== false)
			{
				AfxMessageBox("Probl�me sauvegarde");
				return false;
			}
			else
				// Recharge la liste des nouveaux �l�ments
				return ChargeTiersEDI_Regie(); 

		}

		return true;
	}

	return false;
	
}

// Recherche nom r�gie via abr r�gie
CString CDialogTiersEDI::NomCompletRegie(CString &AbrRegie)
{
	for (int i=0;i<m_TInfoRegie.GetSize();i++)
	{
		if (m_TInfoRegie[i].AbrRegie == AbrRegie)
			return m_TInfoRegie[i].StrRegie;
	}
	return "xxx";
}	

// Recherche nom abr�g� r�gie via libell� r�gie
CString CDialogTiersEDI::NomAbrRegie(CString &LibelleRegie)
{
	for (int i=0;i<m_TInfoRegie.GetSize();i++)
	{
		if (m_TInfoRegie[i].StrRegie == LibelleRegie)
			return m_TInfoRegie[i].AbrRegie;
	}
	return "Nom Abr�g� R�gie non d�fini";
}	


// Chargement des centrales d'achat
bool CDialogTiersEDI::ChargeTiersEDI_Centrale()
{
	CString FicTiersEDI;
	CString Ligne;
	CPWFile File;
	int NbTiers;
	int NbContact;

	// les structures infos EDI
	SInfoContact InfoContact;
	SInfoEDI InfoEDI;

	// Nom du fichier tiers centrale d'achat
	FicTiersEDI = theApp.m_PathNomencl + "CtAchat.arb"; 
	
	// Initialisation de la liste existante
	m_ListTiersEDI.ResetContent();
	
	// Lecture des tiers
	if (File.OpenFile(FicTiersEDI))
	{
		// Ligne commentaire
		File.ReadString(&Ligne);

		// Ligne #CENTRALE ACHAT
		File.ReadString(&Ligne);

		// Ligne info nb tiers
		File.ReadString(&Ligne);
		NbTiers = atoi(Ligne.Mid(1));

		// Dimensionne tableau courant des �l�mts EDI
		m_TabInfoEDI.RemoveAll();
		
		// On charge tous les tiers dans la liste
		for (int i=0;i<NbTiers;i++)
		{

			InfoEDI.m_TContact.RemoveAll();
			
			// Lecture ligne type tiers
			File.ReadString(&Ligne);
			InfoEDI.m_InfoGeneralEDI.m_TypeTiers = Ligne;	

			// Lecture ligne code tiers
			File.ReadString(&Ligne);
			InfoEDI.m_InfoGeneralEDI.m_CodeTiers = Ligne;

			// Lecture ligne type code tiers
			File.ReadString(&Ligne);
			InfoEDI.m_InfoGeneralEDI.m_TypeCodeTiers = Ligne;

			// Lecture nom du tiers
			File.ReadString(&Ligne);
			InfoEDI.m_InfoGeneralEDI.m_NomTiers = Ligne;

			// Ajout de ce tiers � la liste
			m_ListTiersEDI.AddString(Ligne);

			// puis on passe les 7 informations suivantes
			// Adresse 1
			File.ReadString(&Ligne);
			InfoEDI.m_InfoGeneralEDI.m_AdresseTiers1  = Ligne;

			// Adresse 2
			File.ReadString(&Ligne);
			InfoEDI.m_InfoGeneralEDI.m_AdresseTiers2 = Ligne;

			// Code Postal
			File.ReadString(&Ligne);
			InfoEDI.m_InfoGeneralEDI.m_CodePostalTiers  = Ligne;

			// Ville
			File.ReadString(&Ligne);
			InfoEDI.m_InfoGeneralEDI.m_VilleTiers  = Ligne;

			// Code pays
			File.ReadString(&Ligne);
			InfoEDI.m_InfoGeneralEDI.m_CodePaysTiers  = Ligne;

			// T�l�phone
			File.ReadString(&Ligne);
			InfoEDI.m_InfoGeneralEDI.m_NoTelTiers  = Ligne;

			// Fax
			File.ReadString(&Ligne);
			InfoEDI.m_InfoGeneralEDI.m_NoFaxTiers  = Ligne;

			
			// lecture nombre d'interlocuteurs
			File.ReadString(&Ligne);
			NbContact = atoi(Ligne.Mid(1));

			// puis on passe les 3 infos pour chacun des contacts de ce tiers
			for (int k=0;k<NbContact;k++)
			{
				// info nom
				File.ReadString(&Ligne);
				InfoContact.m_NomContactTiers = Ligne;

				// info pr�nom
				File.ReadString(&Ligne);
				InfoContact.m_PrenomContactTiers = Ligne;

				// info adr mail
				File.ReadString(&Ligne);
				InfoContact.m_AdrMailContactTiers = Ligne;

				// ajoute cet �lmt dans tableau des contacts
				InfoEDI.m_TContact.Add(InfoContact);

				// info #0
				File.ReadString(&Ligne);
			}

			// Ajoute infos de cet �lmt au tableau global des EDI courants
			m_TabInfoEDI.Add(InfoEDI);
		}


		int NbElmt = m_TabInfoEDI.GetSize();

		// 
		File.CloseFile();	

		return true;
	}
	else
		return false;
}	


// Modification d'une r�gie ou d'une centrale d'achat
bool CDialogTiersEDI::ModifieTiersEDI()
{
	

	
	return true;
}

// Suppression d'une r�gie ou d'une centrale d'achat
bool CDialogTiersEDI::SupprimeTiersEDI()
{
	CString StrElmt;
	CString FicTiersEDI;
	CString MsgSupp;

	// R�cup�re index de l'�lmt � supprimer
	int SelIndex = m_ListTiersEDI.GetCurSel();

	if (SelIndex < m_ListTiersEDI.GetCount())
	{
		// R�cup�re son nom
		m_ListTiersEDI.GetText(SelIndex,StrElmt);

		// Nom du fichier concern� par la suppression
		if (m_TypeTiers == "DS")
		{
			FicTiersEDI = theApp.m_PathNomencl + "Regie.arb"; 
			MsgSupp = "Etes-vous s�r de vouloir supprimer la r�gie ";
		}
		else
		{
			FicTiersEDI = theApp.m_PathNomencl + "CtAchat.arb"; 
			MsgSupp = "Etes-vous s�r de vouloir supprimer la centrale d'achat ";
		}

		if (AfxMessageBox(MsgSupp + StrElmt,MB_YESNO) == IDYES)
		{
			// Vire les �lmts du tableau des elmts courants
			for (int i=0;i<m_TabInfoEDI.GetSize();i++)
			{
				if (m_TabInfoEDI[i].m_InfoGeneralEDI.m_NomTiers == StrElmt)
				{
					// Elmt EDI trouv� , on le supprime
					m_TabInfoEDI.RemoveAt(i,1);

					// puis on quitte la boucle des �lmts
					break;
				}				
			}

			// Recopie tableau dans fichier
			if (SaveElmtEDI()== false)
			{
				AfxMessageBox("Probl�me sauvegarde");
				return false;
			}
			
			// Recharge nouvelle liste des �lmts Centrale Achat EDI
			ChargeTiersEDI_Centrale();
		}

		return true;
	}
	else
		return false;
}

// Sauvegarde elmts EDI courant dans fichier
bool CDialogTiersEDI::SaveElmtEDI()
{
	CPWFile File;
	CString Ligne;
	CString FicTiersEDI;

	// Nom du fichier tiers
	if (m_TypeTiers == "DS")
		FicTiersEDI = theApp.m_PathNomencl + "Regie.arb"; 
	else
		FicTiersEDI = theApp.m_PathNomencl + "CtAchat.arb"; 

	// Lecture des tiers
	if( File.Open(FicTiersEDI,CFile::modeCreate | CFile::modeWrite))
	{
		// info commentaire
		if (m_TypeTiers == "DS")
		{   
			// 2 lignes commentaires R�gie
			File.WriteString("// Descriptif des r�gies\n");
			File.WriteString("#REGIE\n");
		}
		else
		{
			// 2 lignes commentaires Centrale d'achat
			File.WriteString("// Descriptif des centrales d'achat\n");
			File.WriteString("#CENTRALE ACHAT\n");
		}

		// Nombre d'�lmts (r�gie ou ce,trale d'achat)
		Ligne.Format("#%d\n",m_TabInfoEDI.GetSize());
		File.WriteString(Ligne);

		// On boucle sur tous les �lmts EDI
		for (int i=0;i<m_TabInfoEDI.GetSize();i++)
		{
			// Eciture ligne type tiers
			Ligne.Format("%s\n",m_TabInfoEDI[i].m_InfoGeneralEDI.m_TypeTiers);
			if (Ligne == "\n") Ligne = "xxx\n";
			File.WriteString(Ligne);
			
			// Ecriture ligne code tiers
			Ligne.Format("%s\n",m_TabInfoEDI[i].m_InfoGeneralEDI.m_CodeTiers);
			if (Ligne == "\n") Ligne = "xxx\n";
			File.WriteString(Ligne);
			
			// Ecriture ligne type code tiers
			Ligne.Format("%s\n",m_TabInfoEDI[i].m_InfoGeneralEDI.m_TypeCodeTiers);
			if (Ligne == "\n") Ligne = "xxx\n";
			File.WriteString(Ligne);
			
			// Ecriture nom du tiers
			CString NomTiers;
			/*
			if (m_TypeTiers == "DS")
				// cas des r�gies on �crit le libell� abr�g�
				NomTiers = NomAbrRegie(m_TabInfoEDI[i].m_InfoGeneralEDI.m_NomTiers);
			else
				NomTiers = m_TabInfoEDI[i].m_InfoGeneralEDI.m_NomTiers; 
			*/
			NomTiers = m_TabInfoEDI[i].m_InfoGeneralEDI.m_NomTiers; 
			Ligne.Format("%s\n",NomTiers);
			if (Ligne == "\n") Ligne = "xxx\n";
			File.WriteString(Ligne);
			
			// Ecriture Adresse 1
			Ligne.Format("%s\n",m_TabInfoEDI[i].m_InfoGeneralEDI.m_AdresseTiers1);
			if (Ligne == "\n") Ligne = "xxx\n";
			File.WriteString(Ligne);
			
			// Ecriture Adresse 2
			Ligne.Format("%s\n",m_TabInfoEDI[i].m_InfoGeneralEDI.m_AdresseTiers2);
			if (Ligne == "\n") Ligne = "xxx\n";
			File.WriteString(Ligne);
			
			// Ecriture Code Postal
			Ligne.Format("%s\n",m_TabInfoEDI[i].m_InfoGeneralEDI.m_CodePostalTiers);
			if (Ligne == "\n") Ligne = "xxx\n";
			File.WriteString(Ligne);
			
			// Ecriture Ville
			Ligne.Format("%s\n",m_TabInfoEDI[i].m_InfoGeneralEDI.m_VilleTiers);
			if (Ligne == "\n") Ligne = "xxx\n";
			File.WriteString(Ligne);
			
			// Ecriture Code pays
			Ligne.Format("%s\n",m_TabInfoEDI[i].m_InfoGeneralEDI.m_CodePaysTiers);
			if (Ligne == "\n") Ligne = "xxx\n";
			File.WriteString(Ligne);
			 
			// Ecriture T�l�phone
			Ligne.Format("%s\n",m_TabInfoEDI[i].m_InfoGeneralEDI.m_NoTelTiers);
			if (Ligne == "\n") Ligne = "xxx\n";
			File.WriteString(Ligne);
			
			// Ecriture Fax
			Ligne.Format("%s\n",m_TabInfoEDI[i].m_InfoGeneralEDI.m_NoFaxTiers);
			if (Ligne == "\n") Ligne = "xxx\n";
			File.WriteString(Ligne);
						
			// Ecriture nombre d'interlocuteurs
			Ligne.Format("#%d\n",m_TabInfoEDI[i].m_TContact.GetSize());
			if (Ligne == "\n") Ligne = "xxx\n";
			File.WriteString(Ligne);

			// puis on passe les 3 infos pour chacun des contacts de ce tiers
			for (int k=0;k<m_TabInfoEDI[i].m_TContact.GetSize();k++)
			{
				// Ecriture nom contact
				Ligne.Format("%s\n",m_TabInfoEDI[i].m_TContact[k].m_NomContactTiers);
				if (Ligne == "\n") Ligne = "xxx\n";
				File.WriteString(Ligne);

				// Ecriture pr�nom contact
				Ligne.Format("%s\n",m_TabInfoEDI[i].m_TContact[k].m_PrenomContactTiers);
				if (Ligne == "\n") Ligne = "xxx\n";
				File.WriteString(Ligne);

				// Ecriture adr mail contact
				Ligne.Format("%s\n",m_TabInfoEDI[i].m_TContact[k].m_AdrMailContactTiers);
				if (Ligne == "\n") Ligne = "xxx\n";
				File.WriteString(Ligne);

				// Ecriture fin contact #0
				File.WriteString("#0\n");
			}
		}

		// Fermeture du fichier des EDI
		File.CloseFile();
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////
// V�rifie existence �lmt EDI dans liste des �lmts courants
bool CDialogTiersEDI::ElmtEDIExist(CString &ElmtEDI,int &IndexTab,bool TypeRegie)
{
	if (TypeRegie == true)
	{
		// pour les r�gies (nom fichier abr�g�) et nom dans liste complet
		for (int i=0;i<m_TabInfoEDI.GetSize();i++)
		{
			CString NomComplet;
			NomComplet = NomCompletRegie(m_TabInfoEDI[i].m_InfoGeneralEDI.m_NomTiers);
			if (NomComplet == ElmtEDI)
			{
				IndexTab = i;
				return true;
			}
		}
	}

	else
	{   // pour les centrales d'achat nom dans fichier = nom dans liste
		for (int i=0;i<m_TabInfoEDI.GetSize();i++)
		{
			if (m_TabInfoEDI[i].m_InfoGeneralEDI.m_NomTiers == ElmtEDI)
			{
				IndexTab = i;
				return true;
			}
		}
	}

	IndexTab = -1;
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////
// Dbl-click sur une �l�mt >> modification �l�ment
void CDialogTiersEDI::OnDblclkListTiersedi() 
{
	CDialogTiersEDI::OnModifier(); 	
}
