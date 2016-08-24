// DlgModifBac.cpp : implementation file
//

#include "stdafx.h"
#include "admin.h"
#include "DlgModifBac.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// L'Application
extern CAdminApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDlgModifBac dialog


CDlgModifBac::CDlgModifBac(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgModifBac::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgModifBac)
	//}}AFX_DATA_INIT
}


void CDlgModifBac::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgModifBac)
	DDX_Control(pDX, IDC_TARIFNONMODIFIABLE, m_TarifNonModifiable);
	DDX_Control(pDX, IDC_COMBOREGIE, m_ComboRegie);
	DDX_Control(pDX, IDC_TARIFBAC, m_TarifBAC);
	DDX_Control(pDX, IDC_ANNEETARIF, m_AnneeTarif);
	DDX_Control(pDX, IDC_CADRE1, m_Cadre1);
	DDX_Control(pDX, IDC_LIBBAC, m_LibelleBac);
	DDX_Control(pDX, IDC_CODEBAC, m_CodeBac);
	DDX_Control(pDX, IDC_LISTBAC, m_ListBac);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgModifBac, CDialog)
	//{{AFX_MSG_MAP(CDlgModifBac)
	ON_BN_CLICKED(IDC_OK, OnOk)
	ON_BN_CLICKED(IDC_ANNULER, OnAnnuler)
	ON_EN_CHANGE(IDC_CODEBAC, OnChangeCodebac)
	ON_EN_CHANGE(IDC_LIBBAC, OnChangeLibbac)
	ON_LBN_SELCHANGE(IDC_LISTBAC, OnSelchangeListbac)
	ON_WM_PAINT()
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPINANNEE, OnDeltaposSpinannee)
	ON_CBN_SELCHANGE(IDC_COMBOREGIE, OnSelchangeComboregie)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgModifBac message handlers

void CDlgModifBac::OnOk() 
{
	// r�cup�ration des valeurs
	UpdateData(1);	
	CString CodeBac,LibelleBac,numero,CodeRegie;
	m_LibelleBac.GetWindowText(LibelleBac);
	m_CodeBac.GetWindowText(CodeBac);
	m_ComboRegie.GetWindowText(CodeRegie);
	numero = CodeBac.Right(5);

	// Le tarif
	CString NewTarif;
	m_TarifBAC.GetWindowText(NewTarif);

	// Y a t-il quelque chose de modifi� ?
	if( (LibelleBac==m_OldLibelleBac) && (CodeBac==m_OldCodeBac) && (CodeRegie==m_OldRegieBac))
	{
		// Si en plus tarif inchang�, on ne fait rien
		if (m_OldTarif == NewTarif)
		{
			// Rien ne change, sauvegarde inutile
			CDialog::OnOK();
			return;
		}
	}

	// contr�le de saisie
	if((m_ListBac.GetCurSel()==CB_ERR) || (m_ComboRegie.GetCurSel()==CB_ERR))
	{
		MessageBox("          Saisie incorrecte\nVeuillez remplir les champs");
		return;
	}
	if(CodeBac[0]!='B')
	{
		MessageBox("Le code bac commence par un B");
		return;
	}
	if(CodeBac.GetLength()!=6)
	{
		MessageBox("Le code bac est d�finit sur 6 caract�res\n               Veuillez saisir un autre code");
		return;
	}


	if(LibelleBac!=m_OldLibelleBac) // Si le libell� a �t� modifi� ?
	{
		// Test pour savoir su le libell� existe d�j� dans cette table
		if(theApp.ExisteBac(LibelleBac,"")==1)
		{
				CString message;
				message.Format("              Le libell� est invalide:\nIl est d�j� utilis� dans la table des bacs"); 
				MessageBox(message);
				return;
		}

	
		// Test pour savoir su le libell� existe d�j� dans une autre table
		if((theApp.base.FindCodeFromLibelle(LibelleBac)!=""))
		{
				CString message;
				message.Format("Le libell� %s est d�ja utilis� dans les tables",LibelleBac);
				MessageBox(message);
				return;
		}
	}
	
	if(CodeBac!=m_OldCodeBac) // Si le code a �t� modifi�
	{
		if(theApp.ExisteBac("",CodeBac)==2)
		{
			CString message;
			message.Format("              Le code est invalide:\nIl est d�j� utilis� dans la table des bacs"); 
			MessageBox(message);
			return;
		}
	}

	// modification du BAC
	tagBAC bac;
	bac = m_TableBac.GetAt(m_IndexBac); 
	bac.m_Code = CodeBac;
	bac.m_Libelle = LibelleBac;
	m_TableBac.SetAt(m_IndexBac,bac);
	m_TableBac.SetModified(1); 

	// sauvegarde table bac
	m_TableBac.SaveTable();


	// Info pour sauvegarde tarif
	CString TxtAnnee;
	m_AnneeTarif.GetWindowText(TxtAnnee);
	CString CodeBACRegie = CodeBac + CodeRegie;
	double TarifUnique = atof(NewTarif);

	// Sauvegarde modif du tarif (uniquement si acces tarif autoris�)
	if (m_TarifBAC.IsWindowEnabled() == true) 
		ModifTarifBAC(CodeBACRegie,TxtAnnee,TarifUnique);


	CDialog::OnOK();
}

void CDlgModifBac::OnAnnuler() 
{
	EndDialog(IDCANCEL);
}

BOOL CDlgModifBac::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Ajout des libell�s bacs dans la liste
	int size = m_TableBac.GetSize(); 
	for(int bac=0;bac<size;bac++)
	{	CString libelle = m_TableBac.GetAt(bac).GetLibelle(); 
		m_ListBac.AddString(libelle); 		
	}
	m_CodeBac.SetLimitText(6);
	m_LibelleBac.SetLimitText(30);
	
	// Init Combo R�gie
	m_ComboRegie.ResetContent();
	m_ComboRegie.AddString("A");
	m_ComboRegie.AddString("M");
	m_ComboRegie.AddString("N");

	// Init Derni�re Ann�e Tarif�e
	InitAnneeTarif();
	
	UpdateData(0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgModifBac::OnChangeCodebac() 
{
	m_ListBac.EnableWindow(0);	
}

void CDlgModifBac::OnChangeLibbac() 
{
	m_ListBac.EnableWindow(0);	
	
}

void CDlgModifBac::OnSelchangeListbac() 
{
	// On met a jour le libell� et le code correspondant � la s�lection.
	CString libelle;
	int indexListBac = m_ListBac.GetCurSel();
	m_ListBac.GetText(indexListBac,libelle);
	m_OldLibelleBac = libelle;
	int indexBac = theApp.IndexBacDuLibelle(libelle);
	m_LibelleBac.SetWindowText(m_TableBac.GetAt(indexBac).GetLibelle());

	// R�cup les valeurs en cours du Code Bac, 
	m_CodeBac.SetWindowText(m_TableBac.GetAt(indexBac).GetCode());
	m_CodeBac.GetWindowText(m_OldCodeBac);
	m_ComboRegie.GetWindowText(m_OldRegieBac);

	UpdateData(0);	

	// On r�cup�re l'index du bac pour le rechercher dans la table (modification)
	m_IndexBac = indexBac; 

	// si le tarif existe d�j� pour ce BAC, on l'affiche / sinon retour ann�e courante
	bool Ok = AfficheTarifBAC();
	
	m_TarifBAC.GetWindowText(m_OldTarif);
	
}

void CDlgModifBac::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CBrush fond(RGB_BleuJFC);

	CRect rect;
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);
}

void CDlgModifBac::OnDeltaposSpinannee(NMHDR* pNMHDR, LRESULT* pResult) 
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

/////////////////////////////////////////////////////////////////////////////////////
// Affichage du tarif courant pour ce BAC
bool CDlgModifBac::AfficheTarifBAC()
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

		// R�cup R�gie
		CString Regie;
		m_ComboRegie.GetWindowText(Regie);

		// No ligne dans fichier tarif
		CString CodeBACRegie = CodeBAC + Regie;
		int NoLigne;
		NoLigne = theApp.m_Tarifs.GetAt(NoTarif)->GetIndexForCode(CodeBACRegie); 

		if (NoLigne >= 0)
		{

			// Tarif existant pour cette ann�e
			// R�cup tarif associ� 1ere semaine
			double TarifElmt = theApp.m_Tarifs.GetAt(NoTarif)->GetTarif(NoLigne,0); 	

			// affichage du tarif
			CString StrTarif;
			StrTarif.Format("%7.2f",TarifElmt);
			m_TarifBAC.SetWindowText(StrTarif);

			// interdiction de modifier le tarif si tarif diff�rent par semaine
			bool TarifUnique = true;
			for (int i=0;i<53;i++)
			{
				double TarifSem = theApp.m_Tarifs.GetAt(NoTarif)->GetTarif(NoLigne,i); 	
				if (TarifSem != TarifElmt)
				{
					// Ici tarif non uniforme sur toutes les semaines
					TarifUnique = false;
					break;
				}
			}
			// Valide ou d�valide la possibilit� de modifier le tarif
			if (TarifUnique == true)
			{
				m_TarifBAC.EnableWindow(true);
				m_TarifNonModifiable.ShowWindow(SW_HIDE);
			}
			else
			{	
				// non modifiable, on ajoute le message info
				m_TarifBAC.EnableWindow(false);
				m_TarifNonModifiable.ShowWindow(SW_SHOW);
				m_TarifNonModifiable.FlashWindow(true);
			}

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

//////////////////////////////////////////////////////////////////////////////////////
// Affichage dern�re ann�e tarif�e
void CDlgModifBac::InitAnneeTarif()
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

//////////////////////////////////////////////////////////////////////////////////////
// Sauve le nouveau tarif du BAC
void CDlgModifBac::ModifTarifBAC(CString CodeBAC,CString StrAnnee,double TarifUnique)
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

		// V�rifie si elemt BAC r�ellement nouveau
		int IndexCode = theApp.m_Tarifs.GetAt(tarif)->GetIndexForCode(CodeBAC);

		// Ajout ou Modif
		if (IndexCode != -1)
		{
			// Ici on va modifier les tarifs pour ce code BAC
			// ajoute nouveaux tarifs
			theApp.m_Tarifs.GetAt(tarif)->ModifyTarif(CodeBAC,tarifsem);
		}

		else
		{
			// Ici c'est un nouveau BAC >> Ajout ligne tarifs
			// ajoute nouveaux tarifs
			theApp.m_Tarifs.GetAt(tarif)->AddTarif(CodeBAC,tarifsem,true);
		}

		// sauvegarde tarif
		theApp.m_Tarifs.GetAt(tarif)->SaveTarif();

	}

}	

void CDlgModifBac::OnSelchangeComboregie() 
{
	// Changement de r�gie, on affiche le tarif correspondant
	bool Ok = AfficheTarifBAC();	
}
