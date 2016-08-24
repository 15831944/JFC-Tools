// DlgAjoutProduit.cpp : implementation file
//

#include "stdafx.h"
#include "admin.h"
#include "DlgAjoutProduit.h"
#include "DlgSelectionBac.h"
#include "DlgSelectionComplexe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// L'Application
extern CAdminApp theApp;

// le chemin des données
extern CString GCheminAccesDonnee;

/////////////////////////////////////////////////////////////////////////////
// CDlgAjoutProduit dialog


CDlgAjoutProduit::CDlgAjoutProduit(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAjoutProduit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAjoutProduit)
	//}}AFX_DATA_INIT
	m_modif=0;
	m_IndexBacSemaine=NULL;
	m_IndexComplexeSemaine=NULL;
	//_TypeBacSemaine=NULL;
}


void CDlgAjoutProduit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAjoutProduit)
	DDX_Control(pDX, IDC_TARIFNONMODIFIABLE, m_TarifNonModifiable);
	DDX_Control(pDX, IDC_ANNEETARIF, m_AnneeTarif);
	DDX_Control(pDX, IDC_TARIFPRODUIT, m_TarifProduit);
	DDX_Control(pDX, IDC_CADRE1, m_Cadre1);
	DDX_Control(pDX, IDC_LISTBAC, m_ListBac);
	DDX_Control(pDX, IDC_LISTCOMPLEXE, m_ListComplexe);
	DDX_Control(pDX, IDC_CODEPRODUIT, m_CodeProduit);
	DDX_Control(pDX, IDC_COMBOCOMPO, m_Compo);
	DDX_Control(pDX, IDC_COMBODUREE, m_Duree);
	DDX_Control(pDX, IDC_LIBPRODUIT, m_LibProduit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAjoutProduit, CDialog)
	//{{AFX_MSG_MAP(CDlgAjoutProduit)
	ON_BN_CLICKED(IDC_OK, OnOk)
	ON_BN_CLICKED(IDC_ANNULER, OnAnnuler)
	ON_BN_CLICKED(IDC_AJOUTCOMPLEXE, OnAjoutcomplexe)
	ON_BN_CLICKED(IDC_AJOUTBAC, OnAjoutbac)
	ON_CBN_SELCHANGE(IDC_COMBODUREE, OnSelchangeComboduree)
	ON_BN_CLICKED(IDC_SUPP, OnSupp)
	ON_CBN_SELCHANGE(IDC_COMBOCOMPO, OnSelchangeCombocompo)
	ON_BN_CLICKED(IDC_SUPP2, OnSupp2)
	ON_WM_PAINT()
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPINANNEE, OnDeltaposSpinannee)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAjoutProduit message handlers

void CDlgAjoutProduit::OnOk() 
{
	CString LibelleProduit,CodeProduit,numero;

	// Mise à jour des données
	UpdateData(1);

	//Récupération du code produit et du libellé 
	m_CodeProduit.GetWindowText(CodeProduit);
	m_LibProduit.GetWindowText(LibelleProduit);
	numero = CodeProduit.Right(5);

	int retcode;
	int duree;

	if((CodeProduit=="")||(LibelleProduit==""))
	{
		MessageBox("          Saisie incorrecte\nVeuillez remplir tous les champs");
		return;
	}
	
	// controle du code
	if(CodeProduit[0]!='P')
	{
		MessageBox("Le code complexe commence par un P");
		return;
	}

	if(CodeProduit.GetLength()!=6)
	{
		MessageBox("Le code complexe doit compter 6 caractères");
		return;
	}



	if((theApp.base.FindCodeFromLibelle(LibelleProduit)!=""))
	{
		if(m_OldLibelleProduit!=LibelleProduit)
		{
			CString message;
			message.Format("Le libellé %s est déja utilisé dans les tables",LibelleProduit);
			MessageBox(message);
			return;
		}
	}
	
	if(LibelleProduit==m_OldLibelleProduit)
	{
		retcode = theApp.ExisteProduit("",CodeProduit);
	}
	else
	{
		if(CodeProduit==m_OldCodeProduit)
		{
			retcode = theApp.ExisteProduit(LibelleProduit,"");
		}
		else 
		{
			retcode = theApp.ExisteProduit(LibelleProduit,CodeProduit);
		}
	}

	if((LibelleProduit==m_OldLibelleProduit)&&(CodeProduit==m_OldCodeProduit))
	{
		retcode = 0;
	}

	// AJOUT ALAIN 2003
	// Gestion erreur produit dupliqué en utilisant Ajout depuis.....
	if (m_AjoutDepuis == 1)
	{
		if(LibelleProduit==m_OldLibelleProduit)
		{
			// même libllé produit imposible en mode ajout depuis
			retcode = 1;
		}
		else if (CodeProduit==m_OldCodeProduit)
		{
			// même libllé produit imposible en mode ajout depuis
			retcode = 2;
		}
	}

	if(!retcode)
	{
			tagProduit p;
			p.m_Libelle = LibelleProduit;
			p.m_Code = CodeProduit;
			if(m_Duree.GetCurSel()>0)
			{
				p.m_flagProduitFige = 1;
			}
			else
			{
				p.m_flagProduitFige = 0;

			}
			if(p.m_flagProduitFige) 
			{
				p.SetDuree(m_Duree.GetCurSel()); 
			}
			else
			{
				p.SetDuree(1);
			}
			duree = p.GetDuree();

			for(int sem=0;sem<duree;sem++)
			{
				// Ajout des bacs
				int size = m_IndexBacSemaine[sem].GetSize(); 
				for(int idbac=0;idbac<size;idbac++)
				{
					p.m_DescriptionIndexBac[sem].Add(m_IndexBacSemaine[sem].GetAt(idbac)); 
					CString CodeBacRegie = m_TableBac.GetCode(m_IndexBacSemaine[sem].GetAt(idbac));

					// On ajoute la régie au bac en fction du libellé sélectionné
					int type = m_TypeBacSemaine[sem].GetAt(idbac);

					if(type==0)CodeBacRegie+="P";
					if(type==1)CodeBacRegie+="A";
					if(type==2)CodeBacRegie+="M";
					if(type==3)CodeBacRegie+="N";

					p.m_DescriptionCode[sem].Add(CodeBacRegie);
				}

				// Ajout des complexes
				size = m_IndexComplexeSemaine[sem].GetSize(); 
				for(int idcmplx=0;idcmplx<size;idcmplx++)
				{
					if(!IsAjouteBacDuComplexe(m_IndexComplexeSemaine[sem].GetAt(idcmplx)))
					{
						p.m_DescriptionIndexComplexe[sem].Add(m_IndexComplexeSemaine[sem].GetAt(idcmplx)); 
						p.m_DescriptionCode[sem].Add(m_TableComplexe.GetCode(m_IndexComplexeSemaine[sem].GetAt(idcmplx)));   
					}
					else
					{
						// erreur
						return;
					}
				}
			}
			m_TableProduit.SetModified(1); 

			// Récup année courante ajout Produit
			CString StrAnnee;
			m_AnneeTarif.GetWindowText(StrAnnee);

			// Modification d'un produit
			if(m_modif)
			{
				// le code a t'il été changé?
				if(CodeProduit!=m_OldCodeProduit)
				{
					// suppression du fichier associé
					CString CheminFichier = GCheminAccesDonnee+"\\Data";
					CheminFichier += "\\Definition";
					CheminFichier += m_OldCodeProduit;
					CheminFichier += ".table";
					CFile::Remove(CheminFichier); 

					// le tarif doit etre sauvegardé!!
					int size = theApp.m_Tarifs.GetSize();
					for(int tarif=0;tarif<size;tarif++)
					{
						theApp.m_Tarifs.GetAt(tarif)->ModifyCode(m_OldCodeProduit,CodeProduit);
					}

				}

				// si tarif a changé
				CString StrTarifUnique;
				m_TarifProduit.GetWindowText(StrTarifUnique);
				double TarifUnique = atof(StrTarifUnique);
				if (m_OldTarif != StrTarifUnique)
					// Ajout des tarif dans les tables correspondantes
					ModifTarifProduit(CodeProduit,StrAnnee,TarifUnique);

				m_TableProduit.SetAt(m_indexModif,p); 
			}
			else
			{
				// Ajout du produit si celui ci est nouveau
				m_TableProduit.Add(p); 

				// Récup Tarif courant ajout Produit
				CString StrTarifUnique;
				m_TarifProduit.GetWindowText(StrTarifUnique);
				double TarifUnique = atof(StrTarifUnique);

				// Ajout des tarif dans les tables correspondantes
				AjouteTarifProduit(CodeProduit,StrAnnee,TarifUnique);


			}
	}
	// gestion d'erreur	
	else	
	{	
		CString message;	
		switch(retcode)	
		{	
			case 1:	
				message.Format("                  Le libellé est invalide:\nIl est déjà utilisé dans la table des produits"); 	
				break;	
			case 2:	
				message.Format("                  Le code est invalide:\nIl est déjà utilisé dans la table des produits"); 	
				break;	
			case 3:	
				message.Format("            Le code et le libellé sont invalides:\nIls sont déjà utilisés dans la table des produits"); 	
				break;	
		}	

		MessageBox(message);	
		return;	

	}
	
	// Libération mémoire
	RemoveComposition(duree);
	m_duree=duree;
	CDialog::OnOK();

}

void CDlgAjoutProduit::OnAnnuler() 
{
	RemoveComposition(m_duree);
	EndDialog(IDCANCEL);
}

BOOL CDlgAjoutProduit::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Initialisation Liste Complexe Multi-Colonnes
	m_ListComplexe.Initialise(UNISTATE,ETENDUE_JFC,ETENDUE_JFC,GetSysColor(COLOR_HIGHLIGHT),0xAA0000,INDEPENDANTE,0,NULL,0);
	m_ListBac.Initialise(UNISTATE,ETENDUE_JFC,ETENDUE_JFC,GetSysColor(COLOR_HIGHLIGHT),0xAA0000,INDEPENDANTE,0,NULL,0);

	// Définition 3 colonnes
	m_ListComplexe.InitNbColonne(3);

	// Définition taille des colonnes
	m_ListComplexe.InitSizeColonne(1,207);
	m_ListComplexe.InitSizeColonne(2,25);
	m_ListComplexe.InitSizeColonne(3,0);

	m_LibProduit.SetLimitText(30);
	m_CodeProduit.SetLimitText(6); 
	m_CodeProduit.SetWindowText("P");

	// constitution de la liste des durées
	m_Duree.AddString("Produit Libre");
	m_Duree.AddString("Produit Figé : 1 semaine");

	for(int duree=2;duree<10;duree++)	
	{		
		CString libduree;	
		libduree.Format("Produit Figé : %d semaines",duree);	
		m_Duree.AddString(libduree);	
	}	

	
	// Texte de la boite
	if(m_modif==1)
	{
		CString titre;
		titre.Format("Modifier le produit %s",m_TableProduit.GetAt(m_indexModif).GetLibelle()); 
		CDialog::SetWindowText(titre); 
		InitModif(m_indexModif);
	}
	else
	{
		// Texte de la boite : "Ajouter à partir de ...."
		if(m_AjoutDepuis==1)
		{
			CString titre2;
			titre2.Format("Ajouter produit à partir de %s",m_TableProduit.GetAt(m_indexModif).GetLibelle()); 
			CDialog::SetWindowText(titre2); 
			//InitAjoutDepuis(m_indexModif);
			InitModif(m_indexModif);
			
		}
		else
		{
			m_Duree.SetCurSel(0); 
			// mode par défaut: produit libre
			m_Compo.AddString("Composition produit libre"); 
			m_Compo.SetCurSel(0);
			m_Compo.EnableWindow(0);
			CreateComposition(1);
			m_duree=1;
			m_OldCodeProduit="";
			m_OldLibelleProduit="";
		}
	}

	// Init Dernière Année Tarifée
	InitAnneeTarif();

	// si le tarif existe déjà pour ce complexe, on l'affiche
	AfficheTarifProduit();
	
	// Mise à jour des contrôles
	UpdateData(0);
	return TRUE;  
}


void CDlgAjoutProduit::OnAjoutcomplexe() 
{
	CDlgSelectionComplexe selcmplx;
	
	CStringArray libelleAjoute;

	for(int idxlistbox=0;idxlistbox<m_ListComplexe.GetCount();idxlistbox++)
	{
		CString libelle;
		m_ListComplexe.GetText(idxlistbox,1,libelle);
		libelleAjoute.Add(libelle);
	}
	// copie de la table des complexes complète
	selcmplx.m_TableComplexe = theApp.base.GetNewInstanceTableComplexe();
	selcmplx.m_LibelleAjoute.Copy(libelleAjoute); 


	if(selcmplx.DoModal()!=IDOK)return;

	// affectation du contenu sélectionné dans la liste des complexes
	int semaine = m_Compo.GetCurSel();

	// On vérifie que l'on a pas déjà ajouté le bac associé au complexe
		int size = selcmplx.indexComplexe.GetSize();
		for(int id=0;id<size;id++)
		{
			// on vérifie que le complexe n'a pas déjà été ajouté
			if(!IsComplexeAjoute(selcmplx.indexComplexe.GetAt(id),semaine))
			{
				m_IndexComplexeSemaine[semaine].Add(selcmplx.indexComplexe.GetAt(id));  
			}
			else
			{
				CString message;
				message.Format("Le complexe %s a déjà été ajouté",m_TableComplexe.GetLibelle(selcmplx.indexComplexe.GetAt(id))); 
				MessageBox(message);
			}

		}

		// Mise a jour de la liste des complexes dans la boite Ajout Produit
		m_ListComplexe.ResetContent(); 
		size = m_IndexComplexeSemaine[semaine].GetSize();
		for(int idcmplx=0;idcmplx<size;idcmplx++)
		{	
			tagComplexe complexe = theApp.base.GetTableComplexe()->GetAt(m_IndexComplexeSemaine[semaine].GetAt(idcmplx));
			CString libelle = complexe.m_Libelle;   	
			char regie = complexe.m_Regie; //Récupère la régie
		
			//Récupération du complexe puis de son index et	deduction du bac 
			
			tagComplexe Cpx = m_TableComplexe.GetAt(m_IndexComplexeSemaine[semaine].GetAt(idcmplx));
			WORD indexBac = Cpx.m_indexBAC;	//Récupération de l'index Bac
			
			CString bac=theApp.base.GetTableBac()->GetLibelle(indexBac);
			CString Ligne;
			Ligne.Format("%s\t%c\t%s",libelle,regie,bac);	
			m_ListComplexe.AddString(Ligne); 				
	 
		}
	
	selcmplx.indexComplexe.RemoveAll(); 

	UpdateData(0);
}

void CDlgAjoutProduit::OnAjoutbac() 
{
	CDlgSelectionBac selbac;

	CStringArray bacAjoute;
	//Récupération des bac sélectionés dans m_ListBac 
	for(int idxlistbox=0;idxlistbox<m_ListBac.GetCount();idxlistbox++)
	{
		CString libelleBac;
		m_ListBac.GetText(idxlistbox,libelleBac);
		// MODIF FRED : 23/05/2002: il faut transmettre le libellé du bac de base.
		int type = m_TypeBacSemaine[m_Compo.GetCurSel()].GetAt(idxlistbox);
		if(type!=0)libelleBac=libelleBac.Left(libelleBac.GetLength()-4); 
		bacAjoute.Add(libelleBac);
	}



	// copie de la table des complexes
	selbac.m_TableBac = theApp.base.GetNewInstanceTableBac();
	selbac.m_BacAjoute.Copy(bacAjoute); 

	if(selbac.DoModal()!=IDOK)return;

	// affectation du contenu sélectionné dans la liste des complexes
	int semaine = m_Compo.GetCurSel();

	int size = selbac.m_indexBac.GetSize();
	for(int id=0;id<size;id++)
	{
		if(!IsBacAjoute(selbac.m_indexBac.GetAt(id),semaine))
		{
			// copie de la liste des bacs 
			m_IndexBacSemaine[semaine].Add(selbac.m_indexBac.GetAt(id));  
			// copie de la liste des types par bacs
			m_TypeBacSemaine[semaine].Add(selbac.GetBacType());
		}
		else
		{
			CString message;
			message.Format("Le bac %s a déjà été ajouté",m_TableBac.GetLibelle(selbac.m_indexBac.GetAt(id))); 
			MessageBox(message);
		}
	}

	// Mise à jour de la liste des bacs
	m_ListBac.ResetContent(); 
	size = m_IndexBacSemaine[semaine].GetSize();
	for(int idbac=0;idbac<size;idbac++)
	{	
		// MODIF FRED : 23/05/2002 : mise a jour des libellés incorrecte!!
		// Le libellé tient compte de la régie!!
		CString libelle = m_TableBac.GetAt(m_IndexBacSemaine[semaine].GetAt(idbac)).GetLibelle();  
		switch(m_TypeBacSemaine[semaine].GetAt(idbac))
		{
			case 1:
				libelle+=" (A)";
				break;
			case 2:
				libelle+=" (M)";
				break;
		}
		m_ListBac.AddString(libelle); 
	}
	selbac.m_indexBac.RemoveAll(); 

	UpdateData(0);	
}






void CDlgAjoutProduit::OnSelchangeComboduree() 
{
	// On ajoute autant de chaine que l'exige la durée
	int nbsemaine = m_Duree.GetCurSel(); 
	CString message;
	message.Format("Attention: la composition va être supprimée\n                 Voulez vous continuer"); 
	if(MessageBox(message,NULL,MB_OKCANCEL)==IDCANCEL)
	{
		m_Duree.SetCurSel(m_duree-1);
		return;
	}

	// suppression de la composition
	RemoveComposition(m_duree);

	// création de la composition
	if(nbsemaine==0)
	{
		CreateComposition(1);
		m_duree=1;
	}
	else
	{
		CreateComposition(nbsemaine);
		m_duree=nbsemaine;
	}
	m_Compo.ResetContent();

	if(nbsemaine>0)
	{
		// réactivation de la durée
		m_Duree.EnableWindow(1);
		// réactivation de la composition
		m_Compo.EnableWindow(1);
		m_Compo.DeleteString(0); 

		for(int duree=1;duree<nbsemaine+1;duree++)
		{	
			CString libsemaine;
			libsemaine.Format("Semaine %d",duree);
			m_Compo.AddString(libsemaine); 		
		}
		m_Compo.SetCurSel(0); 
	}
	else
	{
		m_Compo.AddString("Composition produit libre");
		m_Compo.SetCurSel(0); 

	}
	// effacement des listes
	m_ListBac.ResetContent();
	m_ListComplexe.ResetContent();


	
}

BOOL CDlgAjoutProduit::CreateComposition(int nbsemaine)
{
	// construction du tableau de composition bac par semaine
	m_IndexBacSemaine = new CWordArray [nbsemaine];
	// construction du tableau de composition complexe par semaine
	m_IndexComplexeSemaine = new CWordArray  [nbsemaine];
	// construction du tableau de type de bac par semaine
    m_TypeBacSemaine = new CWordArray  [nbsemaine];

	return(1);
}

void CDlgAjoutProduit::RemoveComposition(int nbsemaine)
{
	if((m_IndexBacSemaine!=NULL)&&(m_IndexComplexeSemaine!=NULL))
		{
			for(int sem=0;sem<nbsemaine;sem++)
			{	
				m_IndexBacSemaine[sem].RemoveAll(); 
				m_IndexComplexeSemaine[sem].RemoveAll(); 
				m_TypeBacSemaine[sem].RemoveAll();

			}
			delete[] m_IndexBacSemaine; 
			delete[] m_IndexComplexeSemaine; 
			delete[] m_TypeBacSemaine;
		}

}

// Supprime le ou les complexes sélectionnés
void CDlgAjoutProduit::OnSupp() 
{

	int nbselcomplexe =  m_ListComplexe.GetSelCount();
	int * IndexComplexeList = new int[nbselcomplexe];
	m_ListComplexe.GetSelItems(nbselcomplexe,IndexComplexeList);

	for(int idcomplx=nbselcomplexe-1;idcomplx>=0;idcomplx--)
	{	
		CString libelle;
		int indexCmplx; 
		m_ListComplexe.GetText(IndexComplexeList[idcomplx],1,libelle); 

		// effacement de la chaine
		m_ListComplexe.DeleteString(IndexComplexeList[idcomplx]);
		// effacement des complexes
		indexCmplx = theApp.IndexComplexeDuLibelle(libelle);
		m_IndexComplexeSemaine[m_Compo.GetCurSel()].RemoveAt(IdCmplxNewProduit(indexCmplx,m_Compo.GetCurSel()),1); 
	}
	// effacement des pointeurs
	delete IndexComplexeList;


}



// Supprime le ou les bacs sélectionnés
void CDlgAjoutProduit::OnSupp2() 
{
	int nbselbac =  m_ListBac.GetSelCount();
	int * IndexBacList = new int[nbselbac];
	m_ListBac.GetSelItems(nbselbac,IndexBacList);

	for(int idbac=nbselbac-1;idbac>=0;idbac--)
	{	
		CString libelle;
		int indexBac; 
		m_ListBac.GetText(IndexBacList[idbac],libelle); 

		// effacement de la chaine
		m_ListBac.DeleteString(IndexBacList[idbac]);
		int type = m_TypeBacSemaine[m_Compo.GetCurSel()].GetAt(IndexBacList[idbac]);
		if(type!=0)libelle=libelle.Left(libelle.GetLength()-4);
		// effacement des bacs
		indexBac = theApp.IndexBacDuLibelle(libelle);
		m_IndexBacSemaine[m_Compo.GetCurSel()].RemoveAt(IdBacNewProduit(indexBac,m_Compo.GetCurSel()),1); 
		// suppression du type du bac
		m_TypeBacSemaine[m_Compo.GetCurSel()].RemoveAt(IndexBacList[idbac],1);
		
	}
	delete IndexBacList;
	
}


int CDlgAjoutProduit::IdCmplxNewProduit(int indextabcomplexe,int semaine)
{
	int size = m_IndexComplexeSemaine[semaine].GetSize(); 
	for(int idcomplx=0;idcomplx<size;idcomplx++)
	{
		int index = m_IndexComplexeSemaine[semaine].GetAt(idcomplx);
		if(index==indextabcomplexe)
		{
			return(idcomplx);
		}
	}
	// index non trouvé
	return(-1);
}

int CDlgAjoutProduit::IdBacNewProduit(int indextablebac, int semaine)
{
	int size = m_IndexBacSemaine[semaine].GetSize(); 
	for(int idbac=0;idbac<size;idbac++)
	{
		int index = m_IndexBacSemaine[semaine].GetAt(idbac);
		if(index==indextablebac)
		{
			return(idbac);
		}
	}
	// index non trouvé
	return(-1);
}

// le bac correspondant au complexe a t-il été ajouté?

int CDlgAjoutProduit::IsAjouteBacDuComplexe(int index)
{

		CString codeBac = m_TableComplexe.GetCodeBac(index);
		for(int linebac=0;linebac<m_ListBac.GetCount();linebac++)
		{
			CString libelle;
			CString code;
			m_ListBac.GetText(linebac,libelle);
			code = theApp.CodeBacDuLibelle(libelle);
			if(code==codeBac)
			{
				CString message;
				message.Format("      Le complexe\n %s\n     appartient au bac\n %s\n                   Il ne peut être ajouté",m_TableComplexe.GetLibelle(index),libelle); 
				AfxMessageBox(message);
				return(1); 
			}
		}
	
	// le bac n'a pas été ajouté
	return(0);
}

// changement de semaine
void CDlgAjoutProduit::OnSelchangeCombocompo() 
{
	// effacement de la liste des bacs et des complexes
	m_ListBac.ResetContent();
	m_ListComplexe.ResetContent();

	// chargement de la composition associée à la semaine
	// des complexes
	int size = m_IndexComplexeSemaine[m_Compo.GetCurSel()].GetSize(); 
	for(int idcmplx=0;idcmplx<size;idcmplx++)
	{
		CString libelle;
		libelle = m_TableComplexe.GetAt(m_IndexComplexeSemaine[m_Compo.GetCurSel()].GetAt(idcmplx)).GetLibelle(); 
		m_ListComplexe.AddString(libelle); 
	}

	// des bacs
	size = m_IndexBacSemaine[m_Compo.GetCurSel()].GetSize(); 
	for(int idbac=0;idbac<size;idbac++)
	{
		CString libelle;
		libelle = m_TableBac.GetAt(m_IndexBacSemaine[m_Compo.GetCurSel()].GetAt(idbac)).GetLibelle(); 
		m_ListBac.AddString(libelle); 
	}
	UpdateData(0);

	
}

void CDlgAjoutProduit::InitModif(int IndexProduit)
{
	tagProduit p;
	int nbsemaine;
	p = m_TableProduit.GetAt(IndexProduit);
	m_CodeProduit.SetWindowText(p.GetCode());
	m_LibProduit.SetWindowText(p.GetLibelle());
	m_OldCodeProduit = p.GetCode();
	m_OldLibelleProduit = p.GetLibelle();

	if(p.m_flagProduitFige)
	{
		nbsemaine = p.GetDuree();
		m_Duree.SetCurSel(nbsemaine); 
		m_Compo.ResetContent(); 
		for(int duree=1;duree<nbsemaine+1;duree++)
		{	
			CString libsemaine;
			libsemaine.Format("Semaine %d",duree);
			m_Compo.AddString(libsemaine); 		
		}

		m_Compo.SetCurSel(0);
		
		// Ajout des composition 
		CreateComposition(nbsemaine);
		m_duree=nbsemaine;


	}
	else
	{
		m_Duree.SetCurSel(0);
		m_Compo.AddString("Composition produit libre"); 
		m_Compo.SetCurSel(0);
		m_Compo.EnableWindow(0);
		nbsemaine =1;
		// création de la composition
		CreateComposition(1);
		m_duree=1;
	}
		
	// Mise à jour des listes et des compositions
	for(int sem=0;sem<nbsemaine;sem++)	
	{	
		// la description en bac et en complexe: on utilise la liste des codes
		// Cela nous permet de récupérer l'information régie

		// On boucle sur les codes
		int size = p.m_DescriptionCode[sem].GetSize();	
		for(int idcode=0;idcode<size;idcode++)	
		{
			CString codecomposite = p.m_DescriptionCode[sem].GetAt(idcode);
			// S'agit il d'un bac?
			if(codecomposite[0]=='B')
			{
				// Pour assurer la compatibilité avec les anciens produit, on ajoute le P de probabilisé
				CString codebase; 
				if(codecomposite.GetLength()==6)
				{
					codebase=codecomposite;
					codecomposite+="P";
				}
				else codebase = codecomposite.Left(codecomposite.GetLength()-1);
				 
				int index = theApp.base.GetTableBac()->FindIndexFromCode(codebase,0); 
				m_IndexBacSemaine[sem].Add(index);	

				// mise à jour de la liste	
				CString libelle;	
				libelle = m_TableBac.GetAt(index).GetLibelle(); 	

				if(codecomposite[6]=='P')
				{
					m_TypeBacSemaine[sem].Add(0);
				}
				if(codecomposite[6]=='A')
				{
					libelle+=" (A)";
					m_TypeBacSemaine[sem].Add(1);
				}
				if(codecomposite[6]=='M')
				{
					libelle+=" (M)";
					m_TypeBacSemaine[sem].Add(2);
				}

				if(sem==0)m_ListBac.AddString(libelle); 
			}
			// ou d'un complexe
			else
			{
				int index = theApp.base.GetTableComplexe()->FindIndexFromCode(codecomposite,0);  	
				m_IndexComplexeSemaine[sem].Add(index);		
				// mise à jour de la liste		
				CString libelle;		
				libelle = m_TableComplexe.GetAt(index).GetLibelle(); 		
				libelle+="\t";
				// Ajout de la régie du complexe
				libelle+= m_TableComplexe.GetAt(index).GetRegie(); 
				libelle+="\t";
				// Ajout du bac du complexe
				libelle+=m_TableBac.GetLibelle(m_TableComplexe.GetAt(index).m_indexBAC);  
				if(sem==0)m_ListComplexe.AddString(libelle); 	

			}
		}
		/*int size = p.m_DescriptionIndexBac[sem].GetSize();	
		for(int idbac=0;idbac<size;idbac++)	
		{	
				int index = p.m_DescriptionIndexBac[sem].GetAt(idbac);	

				m_IndexBacSemaine[sem].Add(index);	
			
		}	
		size = p.m_DescriptionIndexComplexe[sem].GetSize();	
		for(int idcmplx=0;idcmplx<size;idcmplx++)	
		{		
			int index = p.m_DescriptionIndexComplexe[sem].GetAt(idcmplx);		
			m_IndexComplexeSemaine[sem].Add(index);		
			// mise à jour de la liste		
			CString libelle;		
			libelle = m_TableComplexe.GetAt(index).GetLibelle(); 		
			libelle+="\t";
			// Ajout de la régie du complexe
			libelle+= m_TableComplexe.GetAt(index).GetRegie(); 
			libelle+="\t";
			// Ajout du bac du complexe
			libelle+=m_TableBac.GetLibelle(m_TableComplexe.GetAt(index).m_indexBAC);  
			if(sem==0)m_ListComplexe.AddString(libelle); 	
			
		}*/		
	}	
	UpdateData(0);
}


int CDlgAjoutProduit::IsBacAjoute(int indexbac,int semaine)
{
	// parcours du tableau des bacs pour savoir si le bac a déjà été ajouté	
	int size = m_IndexBacSemaine[semaine].GetSize();	
	for(int idbac=0;idbac<size;idbac++)		
	{	
		if(indexbac==m_IndexBacSemaine[semaine].GetAt(idbac))return(1); 	
	}	
	return(0);	
}

int CDlgAjoutProduit::IsComplexeAjoute(int idcomplexe, int semaine)
{
	// parcours du tableau des complexes pour savoir si le complexe a déjà été ajouté	
	int size = m_IndexComplexeSemaine[semaine].GetSize();	
	for(int idcmplx=0;idcmplx<size;idcmplx++)		
	{	
		if(idcomplexe==m_IndexComplexeSemaine[semaine].GetAt(idcmplx))return(1); 	
	}	
	return(0);	

}

void CDlgAjoutProduit::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CBrush fond(RGB_BleuJFC);

	CRect rect;
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);
	
}


/////////////////////////////////////////////////////////////////////////////////////
// Ajoute le tarif du Produit
void CDlgAjoutProduit::AjouteTarifProduit(CString CodeProduit,CString StrAnnee,double TarifUnique)
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

	// Archivage tarif de l'élmt BAC
	if (NoTarif < theApp.m_Tarifs.GetSize())
	{	
		// même tarif pour toutes les semaines
		double * tarifsem = new double[53];
		for (int i= 0;i<53;i++)
			tarifsem[i] = TarifUnique;


		// ajoute nouveaux tarifs
		theApp.m_Tarifs.GetAt(tarif)->AddTarif(CodeProduit,tarifsem,true);

		// sauvegarde tarif
		theApp.m_Tarifs.GetAt(tarif)->SaveTarif();

	}

}

/////////////////////////////////////////////////////////////////////////////////////
// Modifie le tarif du Produit
void CDlgAjoutProduit::ModifTarifProduit(CString CodeProduit,CString StrAnnee,double TarifUnique)
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

	// Archivage tarif de l'élmt BAC
	if (NoTarif < theApp.m_Tarifs.GetSize())
	{	

		// même tarif pour toutes les semaines
		double * tarifsem = new double[53];
		for (int i= 0;i<53;i++)
			tarifsem[i] = TarifUnique;

		// Vérifie si elemt BAC réellement nouveau
		int IndexCode = theApp.m_Tarifs.GetAt(tarif)->GetIndexForCode(CodeProduit);

		// Ajout ou Modif
		if (IndexCode != -1)
		{
			// Ici on va modifier les tarifs pour ce code Produit
			// ajoute nouveaux tarifs
			theApp.m_Tarifs.GetAt(tarif)->ModifyTarif(CodeProduit,tarifsem);
		}

		else
		{
			// Ici c'est un nouveau BAC >> Ajout ligne tarifs
			// ajoute nouveaux tarifs
			theApp.m_Tarifs.GetAt(tarif)->AddTarif(CodeProduit,tarifsem,true);
		}

		// sauvegarde tarif
		theApp.m_Tarifs.GetAt(tarif)->SaveTarif();

	}
}


/////////////////////////////////////////////////////////////////////////////////////
// Affichage du tarif courant pour ce Produit
bool CDlgAjoutProduit::AfficheTarifProduit()
{
	CString CodeProduit;
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

		// Récup texte code courant Produit
		m_CodeProduit.GetWindowText(CodeProduit);

		// No ligne dans fichier tarif
		int NoLigne;
		NoLigne = theApp.m_Tarifs.GetAt(NoTarif)->GetIndexForCode(CodeProduit); 

		if (NoLigne >= 0)
		{

			// Tarif existant pour cette année
			// Récup tarif associé 1ere semaine
			double TarifElmt = theApp.m_Tarifs.GetAt(NoTarif)->GetTarif(NoLigne,0); 	

			// affichage du tarif
			CString StrTarif;
			StrTarif.Format("%7.2f",TarifElmt);
			m_TarifProduit.SetWindowText(StrTarif);

			// interdiction de modifier le tarif si tarif différent par semaine
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
			// Valide ou dévalide la possibilité de modifier le tarif
			if ((TarifUnique == true) && (m_modif == 1))
			{
				m_TarifProduit.EnableWindow(true);
				m_TarifNonModifiable.ShowWindow(SW_HIDE);
			}
			else
			{	
				// non modifiable, on ajoute le message info
				m_TarifProduit.EnableWindow(false);
				m_TarifNonModifiable.ShowWindow(SW_SHOW);
				m_TarifNonModifiable.FlashWindow(true);
			}


		}
		else
		{
			// rend la fenêtre édition active
			m_TarifProduit.SetWindowText("");
			m_TarifProduit.EnableWindow(true);
		}
	
	}
	else
	{
		// Création nouvelle table tarif pour cette année
		AfxMessageBox("Année tarifaire inexistante, non importée !!!!");
		return false;
	}

	// Init tarif en entrée
	m_TarifProduit.GetWindowText(m_OldTarif);

	return true;
}	


void CDlgAjoutProduit::OnDeltaposSpinannee(NMHDR* pNMHDR, LRESULT* pResult) 
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
	AfficheTarifProduit();
	
	*pResult = 0;
}

//////////////////////////////////////////////////////////////////////////////////////
// Affichage dernère année tarifée
void CDlgAjoutProduit::InitAnneeTarif()
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

	// Affiche année courante
	m_AnneeTarif.SetWindowText(TxtAnnee);
}
