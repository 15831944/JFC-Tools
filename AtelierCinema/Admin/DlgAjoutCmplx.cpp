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
	// Mise � jour des donn�es
	UpdateData(1);
	CString CodeComplexe,LibComplexe;

	// R�cup libell� et code 
	m_CodeComplexe.GetWindowText(CodeComplexe);
	m_LibComplexe.GetWindowText(LibComplexe);

	CString numero = CodeComplexe.Right(5);

	// traitement de saisie incorrecte
	if((CodeComplexe=="")||(LibComplexe=="")||(m_ComboBac.GetCurSel()==CB_ERR)||(m_ComboRegie.GetCurSel()==CB_ERR))
	{
		MessageBox("          Saisie incorrecte\nVeuillez remplir tous les champs");
		return;
	}

	// Test validit� du code complexe
	if(CodeComplexe[0]!='C')
	{
		MessageBox("Le code complexe commence par un C");
		return;
	}
	if(CodeComplexe.GetLength()!=6)
	{
		MessageBox("Le code complexe est d�finit sur 6 caract�res\n               Veuillez saisir un autre code");
		return;
	}

	// le code peut �tre non num�rique � pr�sent!!
	/*if(!IsNumerique)
	{
		MessageBox("Le code commence par un C suivi de 5 chiffres");
		return;

	}*/

	if((theApp.base.FindCodeFromLibelle(LibComplexe)!=""))
	{
		CString message;
		message.Format("Le libell� %s est d�ja utilis� dans les tables",LibComplexe);
		MessageBox(message);
		return;

	}
	

	else
	{
		// Mise � jour de la table des complexes
		tagComplexe cmplx;
		int retcode;

		// Avant d'ajouter un complexe, on v�rifie que le libell� et le code complexe n'existe pas
		if((retcode = theApp.ExisteComplexe(LibComplexe,CodeComplexe))==0)
		{
			CString libelle;
			CString codeR;
			// ajustement du libell� complexe � 30 caract�res
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

			// R�cup ann�e courante
			CString StrAnnee;
			m_AnneeTarif.GetWindowText(StrAnnee);

			// R�cup Tarif courant
			CString StrTarifUnique;
			m_TarifComplexe.GetWindowText(StrTarifUnique);
			double TarifUnique = atof(StrTarifUnique);

			// R�cup code complexe courant
			CString CodeCmplx;
			m_CodeComplexe.GetWindowText(CodeCmplx);

			// Ajout des tarif dans les tables correspondantes
			AjouteTarifCmplx(CodeCmplx,StrAnnee,TarifUnique);

			// Ajout du nombre d'entr�es
			CString StrEntree;
			m_EditCNC.GetWindowText(StrEntree);
			int NbEntree = atoi(StrEntree);
			AjouteCNCCmplx(CodeCmplx,NbEntree);

			// message d'avertissement
			//MessageBox("Attention: Veuillez penser � importer la donn�e CNC associ�e au complexe");
		
		}
		else  // message d'erreur: la saisie est incorrecte 
		{
			CString message;
			switch(retcode)
			{
				case 1:
					message.Format("                   Le libell� est invalide:\nIl est d�j� utilis� dans la table des complexes"); 
					break;
				case 2:
					message.Format("                   Le code est invalide:\nIl est d�j� utilis� dans la table des complexes"); 
					break;
				case 3:
					message.Format("             Le code et le libell� sont invalides:\nIls sont d�j� utilis�s dans la table des complexes"); 
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
	// Ajout des libell�s bacs dans la liste
	int size = theApp.base.GetTableBac()->GetSize(); 
	for(int bac=0;bac<size;bac++)
	{	CString libelle = theApp.base.GetTableBac()->GetLibelle(bac);
		m_ComboBac.AddString(libelle); 		
	}

	// Init Combo-box R�gie (A ou M)
	m_ComboRegie.ResetContent();
	m_ComboRegie.AddString("A");
	m_ComboRegie.AddString("M");
	m_ComboRegie.AddString("N");

	// limitation des libelles
	m_CodeComplexe.SetWindowText("C");
	m_CodeComplexe.SetLimitText(6);
	m_LibComplexe.SetLimitText(30);	

	// Init Derni�re Ann�e Tarif�e
	InitAnneeTarif();

	// Mise � jour des contr�les
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
	
	// R�cup derni�re ann�e courante
	m_AnneeTarif.GetWindowText(TxtAnnee);
	AnneeCur = atoi(TxtAnnee);

	// Incr�mentation ou d�cr�mentation de l'ann�e courante
	if (pNMUpDown->iDelta < 0)
		AnneeCur +=1; 
	else
		AnneeCur -=1;

	TxtAnnee.Format("%d",AnneeCur);
	m_AnneeTarif.SetWindowText(TxtAnnee);

	// si le tarif existe d�j� pour ce complexe, on l'affiche
	if (AfficheTarifCmplx() ==false)
	{
		// retour ann�e pr�c�demment charg�e
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
// Affichage du tarif courant pour cet �l�mt
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

		// R�cup texte code courant complexe
		m_CodeComplexe.GetWindowText(CodeCmplx);

		// No ligne dans fichier tarif
		int NoLigne;
		NoLigne = theApp.m_Tarifs.GetAt(NoTarif)->GetIndexForCode(CodeCmplx); 

		if (NoLigne >= 0)
		{

			// Tarif existant pour cette ann�e
			// R�cup tarif associ� 1ere semaine
			double TarifElmt = theApp.m_Tarifs.GetAt(NoTarif)->GetTarif(NoLigne,0); 	

			// affichage du tarif
			CString StrTarif;
			StrTarif.Format("%7.2f",TarifElmt);
			m_TarifComplexe.SetWindowText(StrTarif);

			// interdiction de le modifi�
			m_TarifComplexe.EnableWindow(false);

		}
		else
		{
			// rend la fen�tre �dition active
			m_TarifComplexe.SetWindowText("");
			m_TarifComplexe.EnableWindow(true);
		}
	
	}
	else
	{
		// Cr�ation nouvelle table tarif pour cette ann�e
		AfxMessageBox("Ann�e tarifaire inexistante, non import�e !!!!");
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
		// le code a �t� trouv�, mise � jour du controle edit CNC
		tagNbEntree entree = theApp.base.GetTableNbEntree()->GetAt(indextablenbentree);
		int NbEntree = entree.GetNbEntree();
		CString StrEntree;

		// Reconstitue chaine nbombre d'entr�e
		StrEntree.Format("%d",NbEntree);
		m_EditCNC.SetWindowText(StrEntree);
		
		
	}
	else
	{
		// affichage 0 entr�e pour le moment
		CString StrEntree = "0";
		m_EditCNC.SetWindowText(StrEntree);
	}

	// Mise � jour du tarif courant
	if (StrCodeCmplx.GetLength() == 6) 

		// si le tarif existe d�j� pour ce BAC, on l'affiche / sinon retour ann�e courante
		bool Ok = AfficheTarifCmplx();
		
	else
	{
		// prix et CNC � 0
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

	// Recherche n� de la table tarif
	for(int tarif=0;tarif<theApp.m_Tarifs.GetSize();tarif++)
	{
		CString nomfich = theApp.m_Tarifs.GetAt(tarif)->m_NomFich;
		if(nomfich.Find(StrAnnee)>=0)
		{
			NoTarif = tarif;
			break;
		}
	}

	// Archivage tarif de l'�lmt
	if (NoTarif < theApp.m_Tarifs.GetSize())
	{	
		// m�me tarif pour toutes les semaines
		double * tarifsem = new double[53];
		for (int i= 0;i<53;i++)
			tarifsem[i] = TarifUnique;


		// ajoute nouveaux tarifs
		theApp.m_Tarifs.GetAt(tarif)->AddTarif(CodeCmplx,tarifsem,true);

		// sauvegarde tarif
		theApp.m_Tarifs.GetAt(tarif)->SaveTarif();

	}

}

// Ajoute le nombre d'entr�e du nouveau complexe
void CDlgAjoutCmplx::AjouteCNCCmplx(CString CodeComplexe,int NbEntree)
{

	// Recherche index dans table des entr�es
	CTableNbEntreeComplexe TableEntree = theApp.base.GetNewInstanceTableNbEntree();
	int indexTableEntree = TableEntree.FindIndexFromCode(CodeComplexe,false);
	if(indexTableEntree>=0)
	{
		// impossible ici on doit avoir une nouvelle entr�e >> modif entr�e existante
		TableEntree[indexTableEntree].m_NbEntree  = NbEntree;
		TableEntree.SetModified(1);  
	}
	else
	{
		// ajout info entr�e nouvelle �lmt
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
// Affichage dern�re ann�e tarif�e
void CDlgAjoutCmplx::InitAnneeTarif()
{
	// Info Annee Courante
	m_DateCourante = COleDateTime::GetCurrentTime();
	CString TxtAnnee;
	TxtAnnee.Format("%d",m_DateCourante.GetYear());

	// Affichage derniere ann�e ayant un fichier tarif
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

		// passe � l'ann�e d'avant
		TxtAnnee.Format("%d",m_DateCourante.GetYear()-i-1);
	}

	if (NoTarif >= theApp.m_Tarifs.GetSize() || NoTarif == -1)
		// sinon affiche ann�e en cours
		TxtAnnee.Format("%d",m_DateCourante.GetYear());

	m_AnneeTarif.SetWindowText(TxtAnnee);
}
