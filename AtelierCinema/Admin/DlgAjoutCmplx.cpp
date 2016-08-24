// DlgAjoutCmplx.cpp : implementation file
//

#include "stdafx.h"
#include "admin.h"
#include "DlgAjoutCmplx.h"
#include "TableComplexe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
// L'Application
extern CAdminApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CDlgAjoutCmplx dialog


CDlgAjoutCmplx::CDlgAjoutCmplx(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAjoutCmplx::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAjoutCmplx)
	//}}AFX_DATA_INIT
}


void CDlgAjoutCmplx::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAjoutCmplx)
	DDX_Control(pDX, IDC_EDIT_CNC, m_EditCNC);
	DDX_Control(pDX, IDC_CADRE1, m_Cadre1);
	DDX_Control(pDX, IDC_SPINANNEE, m_ChangeAnnee);
	DDX_Control(pDX, IDC_ANNEETARIF, m_AnneeTarif);
	DDX_Control(pDX, IDC_TARIFCOMPLEXE, m_TarifComplexe);
	DDX_Control(pDX, IDC_COMBOREGIE, m_ComboRegie);
	DDX_Control(pDX, IDC_LIBCOMPLEXE, m_LibComplexe);
	DDX_Control(pDX, IDC_CODECOMPLEXE, m_CodeComplexe);
	DDX_Control(pDX, IDC_COMBOBAC, m_ComboBac);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAjoutCmplx, CDialog)
	//{{AFX_MSG_MAP(CDlgAjoutCmplx)
	ON_BN_CLICKED(IDC_OK, OnOk)
	ON_BN_CLICKED(IDC_ANNULER, OnAnnuler)
	ON_WM_PAINT()
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPINANNEE, OnDeltaposSpinannee)
	ON_EN_CHANGE(IDC_CODECOMPLEXE, OnChangeCodecomplexe)
	ON_EN_CHANGE(IDC_EDIT_CNC, OnChangeEditCnc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAjoutCmplx message handlers

void CDlgAjoutCmplx::OnOk() 
{
	// Mise à jour des données
	UpdateData(1);
	CString CodeComplexe,LibComplexe;

	// Récup libellé et code 
	m_CodeComplexe.GetWindowText(CodeComplexe);
	m_LibComplexe.GetWindowText(LibComplexe);

	CString numero = CodeComplexe.Right(5);

	// traitement de saisie incorrecte
	if((CodeComplexe=="")||(LibComplexe=="")||(m_ComboBac.GetCurSel()==CB_ERR)||(m_ComboRegie.GetCurSel()==CB_ERR))
	{
		MessageBox("          Saisie incorrecte\nVeuillez remplir tous les champs");
		return;
	}

	// Test validité du code complexe
	if(CodeComplexe[0]!='C')
	{
		MessageBox("Le code complexe commence par un C");
		return;
	}
	if(CodeComplexe.GetLength()!=6)
	{
		MessageBox("Le code complexe est définit sur 6 caractères\n               Veuillez saisir un autre code");
		return;
	}

	// le code peut être non numérique à présent!!
	/*if(!IsNumerique)
	{
		MessageBox("Le code commence par un C suivi de 5 chiffres");
		return;

	}*/

	if((theApp.base.FindCodeFromLibelle(LibComplexe)!=""))
	{
		CString message;
		message.Format("Le libellé %s est déja utilisé dans les tables",LibComplexe);
		MessageBox(message);
		return;

	}
	

	else
	{
		// Mise à jour de la table des complexes
		tagComplexe cmplx;
		int retcode;

		// Avant d'ajouter un complexe, on vérifie que le libellé et le code complexe n'existe pas
		if((retcode = theApp.ExisteComplexe(LibComplexe,CodeComplexe))==0)
		{
			CString libelle;
			CString codeR;
			// ajustement du libellé complexe à 30 caractères
			if(LibComplexe.GetLength()<30)
			{
				for(int pos=LibComplexe.GetLength();pos<30;pos++)
				{
					LibComplexe+=' ';
				}
			}
			m_ComboBac.GetLBText(m_ComboBac.GetCurSel(),libelle);
			m_ComboRegie.GetLBText(m_ComboRegie.GetCurSel(),codeR);
			CString codebac = theApp.CodeBacDuLibelle(libelle);
			char selection = codeR.GetAt(0);

			cmplx.m_Code = CodeComplexe;
			cmplx.m_Libelle = LibComplexe;
			cmplx.m_CodeBAC = codebac;
			cmplx.m_Regie = selection;

			m_TableComplexe.Add(cmplx);
			m_TableComplexe.SetModified(1); 

			// sauvegarde table complexe
			m_TableComplexe.SaveTable();

			// Récup année courante
			CString StrAnnee;
			m_AnneeTarif.GetWindowText(StrAnnee);

			// Récup Tarif courant
			CString StrTarifUnique;
			m_TarifComplexe.GetWindowText(StrTarifUnique);
			double TarifUnique = atof(StrTarifUnique);

			// Récup code complexe courant
			CString CodeCmplx;
			m_CodeComplexe.GetWindowText(CodeCmplx);

			// Ajout des tarif dans les tables correspondantes
			AjouteTarifCmplx(CodeCmplx,StrAnnee,TarifUnique);

			// Ajout du nombre d'entrées
			CString StrEntree;
			m_EditCNC.GetWindowText(StrEntree);
			int NbEntree = atoi(StrEntree);
			AjouteCNCCmplx(CodeCmplx,NbEntree);

			// message d'avertissement
			//MessageBox("Attention: Veuillez penser à importer la donnée CNC associée au complexe");
		
		}
		else  // message d'erreur: la saisie est incorrecte 
		{
			CString message;
			switch(retcode)
			{
				case 1:
					message.Format("                   Le libellé est invalide:\nIl est déjà utilisé dans la table des complexes"); 
					break;
				case 2:
					message.Format("                   Le code est invalide:\nIl est déjà utilisé dans la table des complexes"); 
					break;
				case 3:
					message.Format("             Le code et le libellé sont invalides:\nIls sont déjà utilisés dans la table des complexes"); 
					break;
			}

			MessageBox(message);
			return;
		}

		EndDialog(IDOK);
	}
}

BOOL CDlgAjoutCmplx::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_ComboBac.ResetContent(); 
	// Ajout des libellés bacs dans la liste
	int size = theApp.base.GetTableBac()->GetSize(); 
	for(int bac=0;bac<size;bac++)
	{	CString libelle = theApp.base.GetTableBac()->GetLibelle(bac);
		m_ComboBac.AddString(libelle); 		
	}

	// Init Combo-box Régie (A ou M)
	m_ComboRegie.ResetContent();
	m_ComboRegie.AddString("A");
	m_ComboRegie.AddString("M");
	m_ComboRegie.AddString("N");

	// limitation des libelles
	m_CodeComplexe.SetWindowText("C");
	m_CodeComplexe.SetLimitText(6);
	m_LibComplexe.SetLimitText(30);	

	// Init Dernière Année Tarifée
	InitAnneeTarif();

	// Mise à jour des contrôles
	UpdateData(0);
	return TRUE;  
}

void CDlgAjoutCmplx::OnAnnuler() 
{
	EndDialog(IDCANCEL);
}

void CDlgAjoutCmplx::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CBrush fond(RGB_BleuJFC); //(RGB(23,114,121));

	CRect rect;
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);
}

void CDlgAjoutCmplx::OnDeltaposSpinannee(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	CString TxtAnnee;
	int AnneeCur;
	
	// Récup dernière année courante
	m_AnneeTarif.GetWindowText(TxtAnnee);
	AnneeCur = atoi(TxtAnnee);

	// Incrémentation ou décrémentation de l'année courante
	if (pNMUpDown->iDelta < 0)
		AnneeCur +=1; 
	else
		AnneeCur -=1;

	TxtAnnee.Format("%d",AnneeCur);
	m_AnneeTarif.SetWindowText(TxtAnnee);

	// si le tarif existe déjà pour ce complexe, on l'affiche
	if (AfficheTarifCmplx() ==false)
	{
		// retour année précédemment chargée
		if (pNMUpDown->iDelta < 0)
			AnneeCur -=1; 
		else
			AnneeCur +=1;

		TxtAnnee.Format("%d",AnneeCur);
		m_AnneeTarif.SetWindowText(TxtAnnee);
	}

	*pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////////////
// Affichage du tarif courant pour cet élémt
bool CDlgAjoutCmplx::AfficheTarifCmplx()
{
	CString CodeCmplx;
	CString StrAnnee;
	int NoTarif = -1;

	// On boucle sur m_Tarifs
	m_AnneeTarif.GetWindowText(StrAnnee);
	int NbTarif = theApp.m_Tarifs.GetSize();
	NoTarif = theApp.m_Tarifs.GetSize();

	for(int tarif=0;tarif<theApp.m_Tarifs.GetSize();tarif++)
	{
		CString nomfich = theApp.m_Tarifs.GetAt(tarif)->m_NomFich;
		if(nomfich.Find(StrAnnee)>=0)
		{
			NoTarif = tarif;
			break;
		}
	}

	if (NoTarif < theApp.m_Tarifs.GetSize())
	{			

		// Récup texte code courant complexe
		m_CodeComplexe.GetWindowText(CodeCmplx);

		// No ligne dans fichier tarif
		int NoLigne;
		NoLigne = theApp.m_Tarifs.GetAt(NoTarif)->GetIndexForCode(CodeCmplx); 

		if (NoLigne >= 0)
		{

			// Tarif existant pour cette année
			// Récup tarif associé 1ere semaine
			double TarifElmt = theApp.m_Tarifs.GetAt(NoTarif)->GetTarif(NoLigne,0); 	

			// affichage du tarif
			CString StrTarif;
			StrTarif.Format("%7.2f",TarifElmt);
			m_TarifComplexe.SetWindowText(StrTarif);

			// interdiction de le modifié
			m_TarifComplexe.EnableWindow(false);

		}
		else
		{
			// rend la fenêtre édition active
			m_TarifComplexe.SetWindowText("");
			m_TarifComplexe.EnableWindow(true);
		}
	
	}
	else
	{
		// Création nouvelle table tarif pour cette année
		AfxMessageBox("Année tarifaire inexistante, non importée !!!!");
		return false;
	}

	return true;
}	


void CDlgAjoutCmplx::OnChangeCodecomplexe() 
{
	// Code complexe en cours
	CString StrCodeCmplx;
	m_CodeComplexe.GetWindowText(StrCodeCmplx);
	
	// affichage du CNC correspondant
	int indextablenbentree;
	if((indextablenbentree=theApp.IndexNbEntreeDuCode(StrCodeCmplx))>=0)
	{
		// le code a été trouvé, mise à jour du controle edit CNC
		tagNbEntree entree = theApp.base.GetTableNbEntree()->GetAt(indextablenbentree);
		int NbEntree = entree.GetNbEntree();
		CString StrEntree;

		// Reconstitue chaine nbombre d'entrée
		StrEntree.Format("%d",NbEntree);
		m_EditCNC.SetWindowText(StrEntree);
		
		
	}
	else
	{
		// affichage 0 entrée pour le moment
		CString StrEntree = "0";
		m_EditCNC.SetWindowText(StrEntree);
	}

	// Mise à jour du tarif courant
	if (StrCodeCmplx.GetLength() == 6) 

		// si le tarif existe déjà pour ce BAC, on l'affiche / sinon retour année courante
		bool Ok = AfficheTarifCmplx();
		
	else
	{
		// prix et CNC à 0
		m_TarifComplexe.SetWindowText("0");
		m_EditCNC.SetWindowText("0");
	}

	
}

/////////////////////////////////////////////////////////////////////////////////////
// MISE A JOUR BOITE EDITION NOMBRE D'ENTREE CNC
void CDlgAjoutCmplx::OnChangeEditCnc() 
{
	
	
}

/////////////////////////////////////////////////////////////////////////////////////
// Ajoute le tarif du complexe
void CDlgAjoutCmplx::AjouteTarifCmplx(CString CodeCmplx,CString StrAnnee,double TarifUnique)
{
	int NoTarif = -1;

	// On boucle sur m_Tarifs
	int NbTarif = theApp.m_Tarifs.GetSize();
	NoTarif = theApp.m_Tarifs.GetSize();

	// Recherche n° de la table tarif
	for(int tarif=0;tarif<theApp.m_Tarifs.GetSize();tarif++)
	{
		CString nomfich = theApp.m_Tarifs.GetAt(tarif)->m_NomFich;
		if(nomfich.Find(StrAnnee)>=0)
		{
			NoTarif = tarif;
			break;
		}
	}

	// Archivage tarif de l'élmt
	if (NoTarif < theApp.m_Tarifs.GetSize())
	{	
		// même tarif pour toutes les semaines
		double * tarifsem = new double[53];
		for (int i= 0;i<53;i++)
			tarifsem[i] = TarifUnique;


		// ajoute nouveaux tarifs
		theApp.m_Tarifs.GetAt(tarif)->AddTarif(CodeCmplx,tarifsem,true);

		// sauvegarde tarif
		theApp.m_Tarifs.GetAt(tarif)->SaveTarif();

	}

}

// Ajoute le nombre d'entrée du nouveau complexe
void CDlgAjoutCmplx::AjouteCNCCmplx(CString CodeComplexe,int NbEntree)
{

	// Recherche index dans table des entrées
	CTableNbEntreeComplexe TableEntree = theApp.base.GetNewInstanceTableNbEntree();
	int indexTableEntree = TableEntree.FindIndexFromCode(CodeComplexe,false);
	if(indexTableEntree>=0)
	{
		// impossible ici on doit avoir une nouvelle entrée >> modif entrée existante
		TableEntree[indexTableEntree].m_NbEntree  = NbEntree;
		TableEntree.SetModified(1);  
	}
	else
	{
		// ajout info entrée nouvelle élmt
		tagNbEntree NvElmt;
		NvElmt.m_CodeComplexe = CodeComplexe;
		NvElmt.m_NbEntree     = NbEntree;
		TableEntree.Add(NvElmt);
		TableEntree.SetModified(1);  
	}
	
	// Sauve nouvelle table
	TableEntree.SaveTable();

}

//////////////////////////////////////////////////////////////////////////////////////
// Affichage dernère année tarifée
void CDlgAjoutCmplx::InitAnneeTarif()
{
	// Info Annee Courante
	m_DateCourante = COleDateTime::GetCurrentTime();
	CString TxtAnnee;
	TxtAnnee.Format("%d",m_DateCourante.GetYear());

	// Affichage derniere année ayant un fichier tarif
	int NoTarif = -1;
	for (int i= 0;i<10;i++)
	{
		for(int tarif=0;tarif<theApp.m_Tarifs.GetSize();tarif++)
		{
			CString nomfich = theApp.m_Tarifs.GetAt(tarif)->m_NomFich;
			if(nomfich.Find(TxtAnnee)>=0)
			{
				NoTarif = tarif;
				break;
			}
		}
		if (NoTarif != -1) break;

		// passe à l'année d'avant
		TxtAnnee.Format("%d",m_DateCourante.GetYear()-i-1);
	}

	if (NoTarif >= theApp.m_Tarifs.GetSize() || NoTarif == -1)
		// sinon affiche année en cours
		TxtAnnee.Format("%d",m_DateCourante.GetYear());

	m_AnneeTarif.SetWindowText(TxtAnnee);
}
