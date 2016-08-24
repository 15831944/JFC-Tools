// admin.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "admin.h"

#include "MainFrm.h"
#include "DlgSelectChemin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdminApp

BEGIN_MESSAGE_MAP(CAdminApp, CWinApp)
	//{{AFX_MSG_MAP(CAdminApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdminApp construction

CAdminApp::CAdminApp()
{
}

CAdminApp::~CAdminApp()
{
	for (int i=0;i<m_Tarifs.GetSize();i++)
	{
		delete m_Tarifs[i];
	}
}
	

/////////////////////////////////////////////////////////////////////////////
// The one and only CAdminApp object

CAdminApp theApp;
CString GCheminAccesDonnee;

/////////////////////////////////////////////////////////////////////////////
// CAdminApp initialization

BOOL CAdminApp::InitInstance()
{

	//GCheminAccesDonnee="c:\\dev\\AtelierCinemaFrance";

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	//SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	SetRegistryKey(_T("JFC"));

	// dans le cas ou l'on ne trouve rien...
	char tampon[1000];
	CString DefaultPath,Opti;
	_getcwd(tampon,1000);
	DefaultPath=tampon;
	int pos = DefaultPath.Find("\\admin");
	DefaultPath=DefaultPath.Left(pos);
	m_Chemin=GetProfileString("Configuration", "Chemin", DefaultPath);
	WriteProfileString("Configuration", "Chemin",m_Chemin);
	GCheminAccesDonnee=m_Chemin;

	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object.


	m_FlagBacRegieCharge = FALSE;

	CMainFrame* pFrame = new CMainFrame;
	m_pMainWnd = pFrame;

	// create and load the frame with its resources

	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);




	// The one and only window has been initialized, so show and update it.
	pFrame->CenterWindow(); 
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();

	// initialisation du chemin de l'atelier cinema
	InitCheminAtelierCinema();


	// chargement des tables de données
	InitTable();

	// chargement des fichiers de tarifs
	ChercheFichiersTarif();
	LoadTarif();

	// chargement des saisonnalités
	LoadSaisonnalite();


	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CAdminApp message handlers





/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CStatic	m_Cadre1;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CAdminApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CAdminApp message handlers


int CAdminApp::ExisteComplexe(LPCSTR libelle, LPCSTR code)
{
	// Cette fonction parcours la table des complexes de l'application 
	// pour s'assurer que le code et le libellé du complexe ajouté n'existe déjà pas dans la table
	int size = base.GetTableComplexe()->GetSize();
	int retcode=0;
	for(int cmplx=0;cmplx<size;cmplx++)
	{
		LPCSTR m_libelle = base.GetTableComplexe()->GetAt(cmplx).GetLibelle();
		// Le code ou le libelle existe
		if(!strcmp(m_libelle,libelle))
		{
			retcode=1;
			break;
		}
	}
	for(cmplx=0;cmplx<size;cmplx++)
	{
		LPCSTR	m_code = base.GetTableComplexe()->GetAt(cmplx).GetCode();
		if(!strcmp(m_code,code))
		{
			retcode+=2;
			break;
		}

	}

	// sinon tout va bien
	// retcode: 0 bon, 1 libelle, 2 code, 3 les deux existent
	return(retcode);
}

BOOL CAdminApp::CodeBacExiste(LPCSTR codebac)
{
	int size = base.GetTableBac()->GetSize();
	for(int bac=0;bac<size;bac++)
	{
		CString m_codebac = base.GetTableBac()->GetCode(bac); 
		if(m_codebac==codebac) return (1);
	}
	return(0);

}

BOOL CAdminApp::InitTable()
{
	if(!base.LoadTable(1)) return(0);
	// if(!base.LoadTable(0)) return(0);
	return(1);
}

int CAdminApp::IndexComplexeDuLibelle(LPCSTR libelle)
{
	// retourne l'index dans la table des complexes correspondant au libellé
	int size = base.GetTableComplexe()->GetSize();
	for(int cmplx=0;cmplx<size;cmplx++)
	{	CString m_libelle = base.GetTableComplexe()->GetAt(cmplx).GetLibelle();
	
		if(m_libelle==libelle)
		{
			return (cmplx);
		}
	}
	// Le libellé n'a pas été trouvé
	return(-1);
}

LPCSTR CAdminApp::CodeBacDuLibelle(LPCSTR libelle)
{
	// retourne le code dans la table des bacs correspondant au libellé
	int size = base.GetTableBac()->GetSize();
	for(int bac=0;bac<size;bac++)
	{	CString m_libelle = base.GetTableBac()->GetAt(bac).GetLibelle();
	
		if(m_libelle==libelle)
		{
			return (base.GetTableBac()->GetAt(bac).GetCode());
		}
	}
	// Le libellé n'a pas été trouvé
	return "";
}


LPCSTR CAdminApp::CodeBacCompositeDuLibelle(LPCSTR libelle)
{
	// retourne le code dans la table des bacs correspondant au libellé
	int size = base.GetTableBac()->GetSize();

	for(int bac=0;bac<size;bac++)
	{	
		CString m_libelle = base.GetTableBac()->GetAt(bac).GetLibelle();
	
		if(m_libelle==libelle)
		{
			return (base.GetTableBac()->GetAt(bac).GetCodeComposite());
		}
	}
	// Le libellé n'a pas été trouvé
	return "";
}


LPCSTR CAdminApp::LibelleDuCodeBac(LPCSTR codebac)
{
	int size = base.GetTableBac()->GetSize(); 
	for(int bac=0;bac<size;bac++)
	{
		if(!strcmp(codebac,base.GetTableBac()->GetCode(bac)))
		{
			return base.GetTableBac()->GetLibelle(bac); 
		}
	}
	return "";

}

int CAdminApp::IndexBacDuLibelle(LPCSTR libelle)
{
	// retourne l'index dans la table des bacs correspondant au libellé
	int size = base.GetTableBac()->GetSize();
	for(int bac=0;bac<size;bac++)
	{	CString m_libelle = base.GetTableBac()->GetAt(bac).GetLibelle();
	
		if(m_libelle==libelle)
		{
			return (bac);
		}
	}
	// Le libellé n'a pas été trouvé
	return(-1);
}

int CAdminApp::ExisteBac(LPCSTR libelle, LPCSTR code)
{
	// Cette fonction parcours la table des bacs de l'application 
	// pour s'assurer que le code et le libellé du bac ajouté n'existent déjà pas dans la table
	int size = base.GetTableBac()->GetSize();
	int retcode=0;
	for(int bac=0;bac<size;bac++)
	{
		LPCSTR m_libelle = base.GetTableBac()->GetAt(bac).GetLibelle();
		// Le code ou le libelle existe
		if(!strcmp(m_libelle,libelle))
		{
			retcode=1;
			break;
		}
	}
	for(bac=0;bac<size;bac++)
	{
		LPCSTR	m_code = base.GetTableBac()->GetAt(bac).GetCode();
		if(!strcmp(m_code,code))
		{
			retcode+=2;
			break;
		}

	}

	// sinon tout va bien
	// retcode: 0 bon, 1 libelle, 2 code, 3 les deux existent
	return(retcode);
}

void CAdminApp::SaveTable()
{
	// Sauvegarde des bases (Complexes, Bacs et Produits)
	base.SaveTable();
	
	// Sauvegarde des tarifs
	SaveTarifs();

	// Sauvegarde des saisonnalités
	SaveSaisonnalite();
	
}

int CAdminApp::ComplexeAttacheAuBac(CString codeBac)
{
	// parcours de la table des complexes pour rechercher le code bac correspondant
	int size = base.GetTableComplexe()->GetSize();
	for(int cmplx=0;cmplx<size;cmplx++)
	{
		CString m_codeBac = base.GetTableComplexe()->GetCodeBac(cmplx);
		if(m_codeBac==codeBac)
		{
			return (1);
		}
	}
	return (0);

}

int CAdminApp::IndexProduitDuLibelle(LPCSTR libelle)
{
	// retourne l'index dans la table des bacs correspondant au libellé
	int size = base.GetTableProduit()->GetSize();
	for(int p=0;p<size;p++)
	{	CString m_libelle = base.GetTableProduit()->GetAt(p).GetLibelle();
	
		if(m_libelle==libelle)
		{
			return (p);
		}
	}
	// Le libellé n'a pas été trouvé
	return(-1);
}

int CAdminApp::IndexTempoDuLibelle(LPCSTR libelle)
{
	// retourne l'index dans la table des tempos correspondant au libellé
	int size = base.GetTableTempo()->GetSize();
	for(int p=0;p<size;p++)
	{	CString m_libelle = base.GetTableTempo()->GetAt(p).GetLibelle();
	
		if(m_libelle==libelle)
		{
			return (p);
		}
	}
	// Le libellé n'a pas été trouvé
	return(-1);
}

int CAdminApp::IndexFormatDuLibelle(LPCSTR libelle)
{
	// retourne l'index dans la table des tempos correspondant au libellé
	int size = base.GetTableFormat()->GetSize();
	for(int p=0;p<size;p++)
	{	
		CString m_libelle = base.GetTableFormat()->GetAt(p).GetLibelle();
	
		if(m_libelle==libelle)
		{
			return (p);
		}
	}
	// Le libellé n'a pas été trouvé
	return(-1);
}
// cette méthode recherche tous les fichiers de tarifs présent dans le répertoire Data
void CAdminApp::ChercheFichiersTarif()
{
	// finder fichier
	CFileFind finder;
	CString motif;
	
	motif = GCheminAccesDonnee + "\\data\\Tarif*.table";
	if(!finder.FindFile(motif))
	{
		AfxMessageBox("Le répertoire de données ne contient aucun fichiers de Tarif");
		ASSERT(0);
	}

	CString chaine;

	// Il existe d'autres fichiers
	while(finder.FindNextFile()>0)
	{
		chaine = finder.GetFileName();
		m_NomFichiersTarif.Add(chaine); 
		
	}
	chaine = finder.GetFileName();
	m_NomFichiersTarif.Add(chaine); 


}

void CAdminApp::LoadTarif()
{
	int size = m_NomFichiersTarif.GetSize(); 
	m_Tarifs.RemoveAll(); 
	for(int nomFich=0;nomFich<size;nomFich++)
	{
		CTarifAnnee * tarif=new CTarifAnnee ;
		tarif->m_NomFich = m_NomFichiersTarif.GetAt(nomFich); 
		tarif->LoadTarif();
		m_Tarifs.Add(tarif); 
	}

}

// A VOIR
/*
void CAdminApp::LoadTarif(int Annee,int TypeElmt)
{
	for(int nomFich=0;nomFich<size;nomFich++)
	{
		CTarifAnnee * tarif=new CTarifAnnee ;
		tarif->m_NomFich = m_NomFichiersTarif.GetAt(nomFich); 
		tarif->LoadTarif(TypeElmt);
	}
}
*/

int CAdminApp::SaveTarifs()
{
	CFichierCinema m_Fichier;

	// les tables de données
	CTableComplexe *  TableComplexe = theApp.base.GetTableComplexe();
	CTableProduit *  TableProduit = theApp.base.GetTableProduit();
	CTableBAC *  TableBac = theApp.base.GetTableBac();

	// Pour chacun des fichiers de tarif, on met à jour
	for(int idtarif=0;idtarif<m_Tarifs.GetSize();idtarif++)
	{
		m_Tarifs.GetAt(idtarif)->SaveTarif();
	}
	return (1);		
}

// Sauvegarde d'une année particulière
int CAdminApp::SaveTarifAnnee(CString StrAnnee)
{
	// Pour chacun des fichiers de tarif, on met à jour
	for(int idtarif=0;idtarif<m_Tarifs.GetSize();idtarif++)
	{
		// Recherche table année à sauvegarder
		if (m_Tarifs.GetAt(idtarif)->m_NomFich.Find(StrAnnee) >= 0)
			m_Tarifs.GetAt(idtarif)->SaveTarif();
	}
	return (1);			
}

int CAdminApp::ImportFile(CString NomFich,CString chemin,CString ImportType)
{
	CFichierCinema m_Fichier;
	CString type,codebac;

	// le séparateur du fichier d'import est un tab
	m_Fichier.SetSeparator("	"); 

	// le délimiteur de fin de fichier
	m_Fichier.SetDelimitor(" ");
	m_Fichier.SetChemin(chemin);

	if(!m_Fichier.OpenFileForRead(NomFich,1))
	{
		return (0);
	}
	// récupération de l'année
	m_Fichier.ReadLine();
	CString annee = m_Fichier.GetElement(0); 

	// On vérifie que le type d'import réalisé correpsond bien au type de fichier indiqué
	m_Fichier.ReadLine();
	type = m_Fichier.GetElement(0);
	if(type!=ImportType)
	{
		CString message;
		message.Format("Le fichier: %s n'est pas un fichier d'import compatible",NomFich); 
		AfxMessageBox(message);
		return(0);
	}

	// différents type d'import
	if((type=="TARIFP")||(type=="TARIFB")||(type=="TARIFC"))
	{

		CString MessPrevent;

		if (type == "TARIFP")
			MessPrevent = "PRODUITS >> de l'année " + annee;
		else if (type == "TARIFB")
			MessPrevent = "BACS >> de l'année " + annee;
		else
			MessPrevent = "COMPLEXES >> de l'année " + annee;

		// Message de prévention, l'import Tarif Ecrase tous les tarifs de l'année en cours
		if (AfxMessageBox("Attention cet import va mettre à jour tous les tarifs " + MessPrevent,
                          MB_OKCANCEL | MB_DEFBUTTON2,MB_ICONEXCLAMATION ) == IDOK)
		{						  


			if (AfxMessageBox("Confirmation Import " + MessPrevent, MB_OKCANCEL | MB_DEFBUTTON1,MB_ICONEXCLAMATION ) == IDOK)
			{						  
				CString lettre;
				int idannee = IndexTarifAnnee(annee);
				CTarifAnnee * tarif = m_Tarifs.GetAt(idannee); 
				
				if(type=="TARIFP")lettre="P";
				if(type=="TARIFB")lettre="B";
				if(type=="TARIFC")lettre="C";

				double * tarifsem = new double[53];


				// Compteur de lignes tarifs lues (les tarifs commencent à la ligne n° 3)
				int NbLgTarif = 3;

				while(m_Fichier.ReadLine())
				{
					CString code;
					// Le tarif est il bien défini sur 53 semaines
					CString dernier;

					// Nombre de tarifs trouvés		
					int Nb = m_Fichier.GetNbElement();

					if(m_Fichier.GetNbElement()!=54)
					{
						CString message;
						// message.Format("Le fichier d'import du tarif est invalide:\nIl manque des tarifs sur la ligne"); 
						message.Format("Le fichier d'import du tarif est invalide:\nIl manque des tarifs sur la ligne %d", NbLgTarif); 
						AfxMessageBox(message);
						return(0);
					}

					// Get le 53eme elmt (chaque ligne doit avoit 53 tarifs)
					dernier = m_Fichier.GetElement(53);
					if (dernier=="")
					{
						CString message;
						message.Format("Le fichier d'import du tarif est invalide:\nIl manque le dernier tarif semaine 53"); 
						AfxMessageBox(message);
						return(0);
					}

					code = m_Fichier.GetElement(0);

					// MODIF FRED : 16/05/2002 BAC REGIE
					if(type=="TARIFB")
					{
						int erreur=0;
						if(code.GetLength()!=6)
						{
							CString message;
							message.Format("Le fichier d'import du tarif est invalide:\nDepuis la version 3.1e les codes bacs sont définis sur 6 caractères dans le fichier d'import\nCode:"+ code); 
							AfxMessageBox(message);
							return(0);
						}
						if((code[5]!='A')&&(code[5]!='M')&&(code[5]!='P')&&(code[6]!='N'))erreur=1;
						if(erreur)
						{
							CString message;
							message.Format("Le fichier d'import du tarif est invalide:\nle code du bac %s n'est pas un code composite",code); 
							AfxMessageBox(message);
							return(0);
						}
					}
					code.Insert(0,lettre);
					// Mise a jour du 27/03/2002 FRED
					// On affiche un message d'erreur lorsque le code n'est pas trouvé dans les tables

					CString codebase;
					if(type=="TARIFB")codebase = code.Left(code.GetLength()-1);   
					else codebase=code;

					if(theApp.base.FindLibelleFromCode(codebase)=="")
					{
						CString message;
						if(type=="TARIFC")message.Format("Le complexe %s n'existe pas dans la table des complexes.\nVeuillez mettre à jour la table des complexes.",code); 
						if(type=="TARIFB")message.Format("Le bac %s n'existe pas dans la table des complexes.\nVeuillez mettre à jour la table des bacs.",code); 
						if(type=="TARIFP")message.Format("Le produit %s n'existe pas dans la table des complexes.\nVeuillez mettre à jour la table des produit.",code); 
						AfxMessageBox(message);
					}
					// On boucle sur les semaines
					for(int sem=0;sem<53;sem++)
					{
						tarifsem[sem] = atof(m_Fichier.GetElement(sem+1));
					}

					// le tarif existe il?
					int idTarif;
					if( (idTarif=tarif->GetIndexForCode(code)) >=0 )
					{
						tarif->RemoveTarif(code);
					}
					tarif->AddTarif(code,tarifsem);
					double test = tarif->GetTarif(1,0);

					// Incrément lignes lues
					NbLgTarif++;

				}
				delete [] tarifsem;
				double test = tarif->GetTarif(0,0);
			}
		}
		
	}
	// Import données CNC (nb entree par complexe)
	if(type=="CNC")
	{
		CTableNbEntreeComplexe * TableNbEntree=base.GetTableNbEntree();

		// On initialise la table des entrée
		TableNbEntree->RemoveAll();
		TableNbEntree->SetModified(1);  

		while(m_Fichier.ReadLine())
		{
			CString code;
			tagNbEntree enr;
			code = m_Fichier.GetElement(0);
			code.Insert(0,"C");

			enr.m_CodeComplexe = code;
			enr.m_NbEntree =  atoi(m_Fichier.GetElement(1));
			int indexCpx= base.GetTableComplexe()->FindIndexFromCode(code,0);

			TableNbEntree->Add(enr); 
			if(indexCpx<0)
			{
				CString txt;
				txt.Format("Le complexe %s n'existe pas dans la table des complexes.\nVeuillez mettre à jour la table des complexes.",code);
				AfxMessageBox(txt);
			}
		}
	}
	// Import saisonnalité
	else if(type=="SAISON")
	{
		// On cherche l'index dans le tableau m_Saison de la table correpondante à l'année
		for(int s=0;s<m_Saisons.GetSize();s++)
		{
			if(m_Saisons.GetAt(s).m_NomFich.Find(annee)>=0)break;
		}

		// correction bug sur les saisonnalités: 27/03/2002 FRED
		CSaisonnalite saison;
		int flagmessage=0;
		for(int sem=0;sem<53;sem++)
		{
			if(!m_Fichier.ReadLine())flagmessage=1;
			if(m_Fichier.GetNbElement()==0)flagmessage=1;
			if(flagmessage)
			{
				CString message;
				message.Format("Le fichier d'import de saisonnalité est invalide:\nIl manque des saisonnalités"); 
				AfxMessageBox(message);
				return(0);
			}
			saison.SetSaisonnalite(atoi(m_Fichier.GetElement(0)),sem); 				
		}

		if(s<m_Saisons.GetSize()) // les saisonnalités de cette année existent déjà		
		{		
			//saison = m_Saisons.GetAt(s); 
			saison.SetModified();
			CString nomfich;
			nomfich.Format("Saison%d.table",atoi(annee));
			saison.m_NomFich = nomfich;
			m_Saisons.SetAt(s,saison);		
		}		
		// sinon on en crée une nouvelle		
		else		
		{		
			CString nom;		
			nom.Format("Saison%s.table",annee); 		
			saison.m_NomFich = nom;		
			saison.SetModified();
			m_Saisons.Add(saison); 		
		}

	}
	m_Fichier.Close(); 
	return(1);
}
// retourne l'index de m_Tarifs pour le fichier correspondant à l'année
// Si le fichier de tarif correspondant à l'année n'existe pas, cette fonction crée un nouvel objet CTarifAnnee et l'ajoute à m_Tarifs.
int CAdminApp::IndexTarifAnnee(LPCSTR annee)
{
	// On boucle sur m_Tarifs
	for(int tarif=0;tarif<m_Tarifs.GetSize();tarif++)
	{
		CString nomfich = m_Tarifs.GetAt(tarif)->m_NomFich;
		if(nomfich.Find(annee)>=0)
		{
			return tarif;
		}
	}
	// l'année n'a pas été trouvé
	// On ajoute une nouvelle table de tarif
	CTarifAnnee * TableTarif= new CTarifAnnee;
	TableTarif->m_NomFich = "Tarif";
	TableTarif->m_NomFich+=annee;
	TableTarif->m_NomFich+=".table";  
	return m_Tarifs.Add(TableTarif); 
}

void CAdminApp::ImportTarif(CString type)
{
	// définition du filtre de fichier
	static char BASED_CODE szFilter[] = "Fichier d'import (*.txt)|*.txt||";
	CFileDialog * ChoixFichier = new CFileDialog(TRUE,"*.txt",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,szFilter,NULL);
	if(ChoixFichier->DoModal()==IDCANCEL)
	{
		delete ChoixFichier;
		return;
	}
	// récupération du nom du fichier sélectionné
	CString nomfich = ChoixFichier->GetFileName(); 

	// récupération du chemin du fichier sélectionné
	CString fullpath = ChoixFichier->GetPathName(); 
	int pos = fullpath.Find("\\"+nomfich);
	CString chemin = fullpath.Left(pos); 
	delete ChoixFichier;

	theApp.ImportFile(nomfich,chemin,type);

}

// Permet de sélectionner le chemin du répertoire des données administrées (Atelier cinéma)
void CAdminApp::InitCheminAtelierCinema()
{
	CDlgSelectChemin select;
	select.m_chemin =GCheminAccesDonnee;
	select.m_OldChemin = GCheminAccesDonnee;
	if(select.DoModal()!=IDOK)return;
	GCheminAccesDonnee = select.m_chemin;
	WriteProfileString("Configuration", "Chemin",select.m_chemin);

}

void CAdminApp::LoadSaisonnalite()
{
	// On cherche les fichiers de saisonnalités dans la répertoire DATA
	// finder fichier
	CFileFind finder;
	CString motif = GCheminAccesDonnee + "\\data\\Saison*.table";
	if(!finder.FindFile(motif))
	{
		AfxMessageBox("Le répertoire de données ne contient aucun fichiers de Saisonnalité");
		ASSERT(0);
	}

	CString chaine;

	// Il existe d'autres fichiers
	while(finder.FindNextFile()>0)
	{
		chaine = finder.GetFileName();
		CSaisonnalite saison;
		saison.m_NomFich = chaine;
		saison.LoadSaison(); 
		m_Saisons.Add(saison); 
		
	}
	chaine = finder.GetFileName();
	CSaisonnalite saison;
	saison.m_NomFich = chaine;
	saison.LoadSaison(); 
	m_Saisons.Add(saison); 
	

}

int CAdminApp::SaveSaisonnalite()
{
	// On boucle sur toutes les tables de saisonnalité (par année)
	for(int s=0;s<m_Saisons.GetSize();s++)
	{
		CSaisonnalite saison = m_Saisons.GetAt(s); 
		saison.SaveSaisonnalite(); 
		m_Saisons.SetAt(s,saison);
	}
	return(1);

}

int CAdminApp::ExisteProduit(LPCSTR libelle, LPCSTR code)
{
	// Cette fonction parcours la table des bacs de l'application 
	// pour s'assurer que le code et le libellé du bac ajouté n'existent déjà pas dans la table
	int size = base.GetTableProduit()->GetSize();
	int retcode=0;
	for(int p=0;p<size;p++)
	{
		LPCSTR m_libelle = base.GetTableProduit()->GetAt(p).GetLibelle();

		// Le code ou le libelle existe
		if(!strcmp(m_libelle,libelle))
		{
			retcode=1;
			break;
		}
	}
	for(p=0;p<size;p++)
	{
		LPCSTR	m_code = base.GetTableProduit()->GetAt(p).GetCode();
		if(!strcmp(m_code,code))
		{
			retcode+=2;
			break;
		}

	}

	// sinon tout va bien
	// retcode: 0 bon, 1 libelle, 2 code, 3 les deux existent
	return(retcode);

}

BOOL CAdminApp::IsModified()
{
	if(base.GetTableBac()->IsModif())return 1;
	if(base.GetTableComplexe()->IsModif())return 1;
	if(base.GetTableProduit()->IsModif())return 1;
	if(base.GetTableTempo()->IsModif())return 1;
	if(base.GetTableFormat()->IsModif())return 1;
	if(base.GetTableNbEntree()->IsModif())return 1;

	for(int an=0;an<m_Tarifs.GetSize();an++)
	{
		if(m_Tarifs[an]->IsModified())return 1;
	}
	for(an=0;an<m_Saisons.GetSize();an++)
	{
		if(m_Saisons[an].IsModif())
		{
			return 1;
		}
	}
	return 0;

}
// retourne l'index dans la table des entrées à partir du code complexe

int CAdminApp::IndexNbEntreeDuCode(LPCSTR code)
{
	// retourne l'index dans la table des bacs correspondant au libellé
	int size = base.GetTableNbEntree()->GetSize();
	for(int idxentree=0;idxentree<size;idxentree++)
	{	CString m_code = base.GetTableNbEntree()->GetAt(idxentree).GetCode();
		if(m_code==code)
		{
			return (idxentree);
		}
	}
	// Le code n'a pas été trouvé
	return(-1);
}

void CAboutDlg::OnPaint() 
{
	/*
	CPaintDC dc(this); // device context for painting
	
	CBrush fond(RGB_BleuJFC);

	CRect rect;
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);
	*/
}
