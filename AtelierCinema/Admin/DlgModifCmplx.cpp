// DlgModifCmplx.cpp : implementation file
//

#include "stdafx.h"
#include "admin.h"
#include "DlgModifCmplx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// L'Application
extern CAdminApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDlgModifCmplx dialog


CDlgModifCmplx::CDlgModifCmplx(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgModifCmplx::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgModifCmplx)
	//}}AFX_DATA_INIT
}


void CDlgModifCmplx::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgModifCmplx)
	DDX_Control(pDX, IDC_TARIFNONMODIFIABLE, m_TarifNonModifiable);
	DDX_Control(pDX, IDC_ANNEETARIF, m_AnneeTarif);
	DDX_Control(pDX, IDC_TARIFCOMPLEXE, m_TarifComplexe);
	DDX_Control(pDX, IDC_CADRE1, m_Cadre1);
	DDX_Control(pDX, IDC_COMBOREGIE, m_ComboRegie);
	DDX_Control(pDX, IDC_CODECOMPLEXE, m_CodeComplexe);
	DDX_Control(pDX, IDC_LIBCOMPLEXE, m_LibelleComplexe);
	DDX_Control(pDX, IDC_COMBOBAC, m_ComboBac);
	DDX_Control(pDX, IDC_LISTCOMPLEXE, m_ListComplexe);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgModifCmplx, CDialog)
	//{{AFX_MSG_MAP(CDlgModifCmplx)
	ON_BN_CLICKED(IDC_OK, OnOk)
	ON_BN_CLICKED(IDC_ANNULER, OnAnnuler)
	ON_LBN_SELCHANGE(IDC_LISTCOMPLEXE, OnSelchangeListcomplexe)
	ON_EN_CHANGE(IDC_LIBCOMPLEXE, OnChangeLibcomplexe)
	ON_EN_CHANGE(IDC_CODECOMPLEXE, OnChangeCodecomplexe)
	ON_CBN_SELCHANGE(IDC_COMBOBAC, OnSelchangeCombobac)
	ON_WM_PAINT()
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPINANNEE, OnDeltaposSpinannee)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgModifCmplx message handlers

void CDlgModifCmplx::OnOk() 
{
	// r�cup�ration des valeurs
	UpdateData(1);	

	CString LibelleComplexe,CodeComplexe,numero,bacactuel,regactuel;

	m_LibelleComplexe.GetWindowText(LibelleComplexe);
	m_CodeComplexe.GetWindowText(CodeComplexe);
	m_ComboBac.GetWindowText(bacactuel);
	m_ComboRegie.GetWindowText(regactuel);
	numero = CodeComplexe.Right(5);

	// le code est il num�rique?
	/*int IsNumerique=1;
	for(int c=0;c<numero.GetLength();c++)
	{
		if((numero.GetAt(c)<=47)||(numero.GetAt(c)>=58))IsNumerique=0;
	}*/

	// contr�le de saisie
	if(m_ListComplexe.GetCurSel()==CB_ERR)
	{
		MessageBox("          Saisie incorrecte\nVeuillez saisir un complexe");
		return;
	}
	if(m_ComboRegie.GetCurSel()==CB_ERR)
	{
		MessageBox("          Saisie incorrecte\nVeuillez saisir un complexe");
		return;
	}

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


	// Si le libell� est modifi�
	if(m_OldLibelleComplexe!=LibelleComplexe)
	{
		// Le m�me dans la table des complexes ?
		if(theApp.ExisteComplexe(LibelleComplexe,"")==1)
		{
			CString message;
			message.Format("                   Le libell� est invalide:\nIl est d�j� utilis� dans la table des complexes"); 
			MessageBox(message);
			return;
		}

		// Le m�me libell� dans une autre table ?
		if((theApp.base.FindCodeFromLibelle(LibelleComplexe)!=""))
		{
			CString message;
			message.Format("Le libell� %s est d�ja utilis� dans les tables",LibelleComplexe);
			MessageBox(message);
			return;
		}
	}

	// Le code a �t� modifi� ?
	if(CodeComplexe!=m_OldCodeComplexe)
	{
		if(theApp.ExisteComplexe("",CodeComplexe)==2)
		{
			CString message;
			message.Format("                   Le code est invalide:\nIl est d�j� utilis� dans la table des complexes"); 
			MessageBox(message);
			return;
		}
		// Il faut penser � changer le code du complexe dans la table des entr�es
		int indextablenbentree;
		if((indextablenbentree=theApp.IndexNbEntreeDuCode(m_OldCodeComplexe))>=0)
		{
			// le code a �t� trouv�, mise � jour de la table des entr�es
			tagNbEntree entree = theApp.base.GetTableNbEntree()->GetAt(indextablenbentree);
			entree.m_CodeComplexe = CodeComplexe;
			theApp.base.GetTableNbEntree()->SetAt(indextablenbentree,entree);
		}
		// On cherche dans la composition de tous les produits pour savoir si celui-ci est utilis�
		//theApp.base.GetTableProduit()->RemplaceCodeComposition(m_OldCodeComplexe,CodeComplexe);

	}

	// Nouveau tarif complexe saisi
	CString NewTarif;
	m_TarifComplexe.GetWindowText(NewTarif);

	//Est-ce qu'il faut sauvegarder?
	if ((m_OldLibelleComplexe==LibelleComplexe)&&(CodeComplexe==m_OldCodeComplexe)&&(m_OldCodeBac==bacactuel)&&(m_OldRegie==regactuel))
	{
		// Si en plus tarif inchang�, on ne fait rien
	
		if (m_OldTarif == NewTarif)
			CDialog::OnOK();
	}

	// modification des champs du complexe
	CString libelle;
	CString codeR;

	m_ComboBac.GetLBText(m_ComboBac.GetCurSel(),libelle);	
	CString codebac = theApp.CodeBacDuLibelle(libelle);	
	m_ComboRegie.GetLBText(m_ComboRegie.GetCurSel(),codeR);
	char selection = codeR.GetAt(0);

	tagComplexe complexe;
	complexe = m_TableComplexe.GetAt(m_IndexComplexe); 
	complexe.m_Code = CodeComplexe;
	complexe.m_Libelle = LibelleComplexe;
	complexe.m_CodeBAC = codebac;	
	complexe.m_Regie = selection;
	m_TableComplexe.SetAt(m_IndexComplexe,complexe);
	m_TableComplexe.SetModified(1); //Enregistrer les modifications

	// sauvegarde table complexe
	m_TableComplexe.SaveTable();

	// Info pour sauvegarde tarif
	CString TxtAnnee;
	m_AnneeTarif.GetWindowText(TxtAnnee);
	double TarifUnique = atof(NewTarif);

	// Sauvegarde modif du tarif (uniquement si acces tarif autoris�)
	if (m_TarifComplexe.IsWindowEnabled() == true) 
		ModifTarifCmplx(CodeComplexe,TxtAnnee,TarifUnique);

	CDialog::OnOK();

}

void CDlgModifCmplx::OnAnnuler() 
{
	EndDialog(IDCANCEL);
}

BOOL CDlgModifCmplx::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Ajout des libell�s complexes dans la liste
	int size = m_TableComplexe.GetSize(); 
	for(int cmplx=0;cmplx<size;cmplx++)
	{	CString libelle = m_TableComplexe.GetLibelle(cmplx); 
		m_ListComplexe.AddString(libelle); 		
	}

	// Ajout des libell�s des bacs � la combobox
	m_ComboBac.ResetContent(); 
	size = theApp.base.GetTableBac()->GetSize(); 
	for(int bac=0;bac<size;bac++)
	{	CString libelle = theApp.base.GetTableBac()->GetLibelle(bac);
		m_ComboBac.AddString(libelle); 		
	}

	//Ajout des codes r�gies dans la liste
	m_ComboRegie.ResetContent();
	m_ComboRegie.AddString("A");
	m_ComboRegie.AddString("M");
	m_ComboRegie.AddString("N");

	m_CodeComplexe.SetLimitText(6);
	m_LibelleComplexe.SetLimitText(30);	

	// Init Derni�re Ann�e Tarif�e
	InitAnneeTarif();

	UpdateData(0);

	return TRUE;  
}

// proc�dure r�agissant au changement de s�lection de la listeBox
void CDlgModifCmplx::OnSelchangeListcomplexe() 
{
	// On met a jour le libell�, le code et le code bac correspondant � la s�lection.
	CString libelle;
	int indexListComplexe = m_ListComplexe.GetCurSel();
	m_ListComplexe.GetText(indexListComplexe,libelle);
	m_OldLibelleComplexe = libelle;
	int indexCmplx = theApp.IndexComplexeDuLibelle(libelle);
	m_LibelleComplexe.SetWindowText(m_TableComplexe.GetAt(indexCmplx).GetLibelle());
	m_CodeComplexe.SetWindowText(m_TableComplexe.GetAt(indexCmplx).GetCode());
	m_CodeComplexe.GetWindowText(m_OldCodeComplexe);
	CString codebac = m_TableComplexe.GetCodeBac(indexCmplx); 
	CString coderegie = m_TableComplexe.GetRegie(indexCmplx);
	libelle = theApp.LibelleDuCodeBac(codebac);
	//R�cup�ration des anciens codes bac et r�gies.
	m_OldCodeBac=libelle;
	m_OldRegie=coderegie;
	
	for(int linecombo=0;linecombo<m_ComboBac.GetCount();linecombo++)
	{
		CString liblist;
		m_ComboBac.GetLBText(linecombo,liblist); 
		if(libelle==liblist)break;
	}
	m_ComboBac.SetCurSel(linecombo); 
	
	for(int line=0;line<m_ComboRegie.GetCount();line++)
	{
		CString linelist;
		m_ComboRegie.GetLBText(line,linelist);
		if(coderegie==linelist)break;
	}
	m_ComboRegie.SetCurSel(line);

	UpdateData(0);	

	// On r�cup�re l'index du complexe pour le rechercher dans la table (modification)
	m_IndexComplexe = indexCmplx; 

	// Affiche tarif du complexe � modifier
	bool Ok = AfficheTarifCmplx();
}

void CDlgModifCmplx::OnChangeLibcomplexe() 
{
	m_ListComplexe.EnableWindow(0);
}

void CDlgModifCmplx::OnChangeCodecomplexe() 
{
	m_ListComplexe.EnableWindow(0);
}


void CDlgModifCmplx::OnSelchangeCombobac() 
{
	m_ListComplexe.EnableWindow(0);
}


void CDlgModifCmplx::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CBrush fond(RGB_BleuJFC);

	CRect rect;
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);	
	
}

void CDlgModifCmplx::OnDeltaposSpinannee(NMHDR* pNMHDR, LRESULT* pResult) 
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

	// si le tarif existe d�j� pour ce complexe, on l'affiche / sinon retour ann�e courante
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
bool CDlgModifCmplx::AfficheTarifCmplx()
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
				m_TarifComplexe.EnableWindow(true);
				m_TarifNonModifiable.ShowWindow(SW_HIDE);
			}
			else
			{	
				// non modifiable, on ajoute le message info
				m_TarifComplexe.EnableWindow(false);
				m_TarifNonModifiable.ShowWindow(SW_SHOW);
				m_TarifNonModifiable.FlashWindow(true);
			}

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

	// sauve tarif en cours....
	m_TarifComplexe.GetWindowText(m_OldTarif);

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////
// Sauve le nouveau tarif du complexe
void CDlgModifCmplx::ModifTarifCmplx(CString CodeCmplx,CString StrAnnee,double TarifUnique)
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
		int IndexCode = theApp.m_Tarifs.GetAt(tarif)->GetIndexForCode(CodeCmplx);

		// Ajout ou Modif
		if (IndexCode != -1)
		{
			// Ici on va modifier les tarifs pour ce code Complexe
			// ajoute nouveaux tarifs
			theApp.m_Tarifs.GetAt(tarif)->ModifyTarif(CodeCmplx,tarifsem);
		}

		else
		{
			// Ici c'est un nouveau complexe >> Ajout ligne tarifs
			// ajoute nouveaux tarifs
			theApp.m_Tarifs.GetAt(tarif)->AddTarif(CodeCmplx,tarifsem,true);
		}

		// sauvegarde tarif
		theApp.m_Tarifs.GetAt(tarif)->SaveTarif();

	}

}	

//////////////////////////////////////////////////////////////////////////////////////
// Affichage dern�re ann�e tarif�e
void CDlgModifCmplx::InitAnneeTarif()
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
