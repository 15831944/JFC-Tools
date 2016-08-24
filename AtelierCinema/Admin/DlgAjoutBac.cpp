// DlgAjoutBac.cpp : implementation file
//

#include "stdafx.h"
#include "admin.h"
#include "DlgAjoutBac.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// L'Application
extern CAdminApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDlgAjoutBac dialog


CDlgAjoutBac::CDlgAjoutBac(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAjoutBac::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAjoutBac)
	//}}AFX_DATA_INIT
}


void CDlgAjoutBac::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAjoutBac)
	DDX_Control(pDX, IDC_COMBOREGIE, m_ComboRegie);
	DDX_Control(pDX, IDC_TARIFBAC, m_TarifBAC);
	DDX_Control(pDX, IDC_ANNEETARIF, m_AnneeTarif);
	DDX_Control(pDX, IDC_CADRE1, m_Cadre1);
	DDX_Control(pDX, IDC_CODEBAC, m_CodeBac);
	DDX_Control(pDX, IDC_LIBBAC, m_LibelleBac);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAjoutBac, CDialog)
	//{{AFX_MSG_MAP(CDlgAjoutBac)
	ON_BN_CLICKED(IDC_OK, OnOk)
	ON_BN_CLICKED(IDC_ANNULER, OnAnnuler)
	ON_WM_PAINT()
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPINANNEE, OnDeltaposSpinannee)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAjoutBac message handlers

void CDlgAjoutBac::OnOk() 
{
	// Mise � jour des donn�es
	UpdateData(1);

	CString LibelleBac,CodeBac,numero;
	m_CodeBac.GetWindowText(CodeBac);
	m_LibelleBac.GetWindowText(LibelleBac);
	numero = CodeBac.Right(5);
	
	if((CodeBac=="") || (LibelleBac=="") || (m_ComboRegie.GetCurSel()==CB_ERR))
	{
		MessageBox("          Saisie incorrecte\nVeuillez remplir tous les champs");
		return;
	}

	// On v�rifie que le code du bac est de 6 caract�res.
	if(m_CodeBac.GetWindowTextLength()!=6)
	{
		MessageBox("Le code bac est d�finit sur 6 caract�res\n               Veuillez saisir un autre code");
		return;
	}
	// On v�rifie que le code bac commence par un B
	if(CodeBac[0]!='B' )
	{
		MessageBox("Le code complexe commence par un B");
		return;
	}

	if((theApp.base.FindCodeFromLibelle(LibelleBac)!=""))
	{
		CString message;
		message.Format("Le libell� %s est d�ja utilis� dans les tables",LibelleBac);
		MessageBox(message);
		return;

	}
	else
	{
		// Mise � jour de la table des bacs
		tagBAC bac;
		int retcode;

		// Avant d'ajouter un bac, on v�rifie que le libell� et le code bac n'existent pas
		if((retcode = theApp.ExisteBac(LibelleBac,CodeBac))==0)
		{
			bac.m_Code = CodeBac;
			bac.m_Libelle = LibelleBac;
			m_TableBac.Add(bac); 
			m_TableBac.SetModified(1); 

			// sauvegarde table bac
			m_TableBac.SaveTable();

			// R�cup ann�e courante ajout BAC
			CString StrAnnee;
			m_AnneeTarif.GetWindowText(StrAnnee);

			// R�cup Tarif courant ajout BAC
			CString StrTarifUnique;
			m_TarifBAC.GetWindowText(StrTarifUnique);
			double TarifUnique = atof(StrTarifUnique);

			// R�cup Code R�gie
			CString CodeRegie;
			m_ComboRegie.GetWindowText(CodeRegie); 

			// Ajout des tarif dans les tables correspondantes
			CString CodeBacRegie = CodeBac + CodeRegie;
			AjouteTarifBAC(CodeBacRegie,StrAnnee,TarifUnique);

		}
		else  // message d'erreur: la saisie est incorrecte 
		{
			CString message;
			switch(retcode)
			{
				case 1:
					message.Format("              Le libell� est invalide:\nIl est d�j� utilis� dans la table des bacs"); 
					break;
				case 2:
					message.Format("              Le code est invalide:\nIl est d�j� utilis� dans la table des bacs"); 
					break;
				case 3:
					message.Format("        Le code et le libell� sont invalides:\nIls sont d�j� utilis�s dans la table des bacs"); 
					break;
			}

			MessageBox(message);
			return;
		}
		EndDialog(IDOK);
	}

	
	CDialog::OnOK();
}

void CDlgAjoutBac::OnAnnuler() 
{
	EndDialog(IDCANCEL);
}

BOOL CDlgAjoutBac::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Limitation taille saisie
	m_CodeBac.SetWindowText("B");
	m_CodeBac.SetLimitText(6);
	m_LibelleBac.SetLimitText(30);

	// Init Combo R�gie
	m_ComboRegie.ResetContent();
	m_ComboRegie.AddString("A");
	m_ComboRegie.AddString("M");
	m_ComboRegie.AddString("N");
	
	// Init Derni�re Ann�e Tarif�e
	InitAnneeTarif();

	// Mise � jour des contr�les
	UpdateData(0);
	return TRUE;  
}

void CDlgAjoutBac::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CBrush fond(RGB_BleuJFC);

	CRect rect;
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);
}

/////////////////////////////////////////////////////////////////////////////////////
// Ajoute le tarif du BAC
void CDlgAjoutBac::AjouteTarifBAC(CString CodeBAC,CString StrAnnee,double TarifUnique)
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

	// Archivage tarif de l'�lmt BAC
	if (NoTarif < theApp.m_Tarifs.GetSize())
	{	
		// m�me tarif pour toutes les semaines
		double * tarifsem = new double[53];
		for (int i= 0;i<53;i++)
			tarifsem[i] = TarifUnique;


		// ajoute nouveaux tarifs
		theApp.m_Tarifs.GetAt(tarif)->AddTarif(CodeBAC,tarifsem,true);

		// sauvegarde tarif
		theApp.m_Tarifs.GetAt(tarif)->SaveTarif();

	}

}

/////////////////////////////////////////////////////////////////////////////////////
// Affichage du tarif courant pour ce BAC
bool CDlgAjoutBac::AfficheTarifBAC()
{

	CString CodeBAC;
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

		// R�cup texte code courant BAC
		m_CodeBac.GetWindowText(CodeBAC);

		// No ligne dans fichier tarif
		int NoLigne;
		NoLigne = theApp.m_Tarifs.GetAt(NoTarif)->GetIndexForCode(CodeBAC); 

		if (NoLigne >= 0)
		{

			// Tarif existant pour cette ann�e
			// R�cup tarif associ� 1ere semaine
			double TarifElmt = theApp.m_Tarifs.GetAt(NoTarif)->GetTarif(NoLigne,0); 	

			// affichage du tarif
			CString StrTarif;
			StrTarif.Format("%7.2f",TarifElmt);
			m_TarifBAC.SetWindowText(StrTarif);

			// interdiction de le modifi�
			m_TarifBAC.EnableWindow(false);

		}
		else
		{
			// rend la fen�tre �dition active
			m_TarifBAC.SetWindowText("");
			m_TarifBAC.EnableWindow(true);

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

void CDlgAjoutBac::OnDeltaposSpinannee(NMHDR* pNMHDR, LRESULT* pResult) 
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

	// si le tarif existe d�j� pour ce BAC, on l'affiche / sinon retour ann�e courante
	if (AfficheTarifBAC() ==false)
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

//////////////////////////////////////////////////////////////////////////////////////
// Affichage dern�re ann�e tarif�e
void CDlgAjoutBac::InitAnneeTarif()
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
