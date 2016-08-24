// Selection.cpp: implementation of the CSelection class.
//
//////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "Selection.h"
#include "cinema.h"
#include "TableProduit.h"
#include "TableBac.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


extern CCinemaApp theApp;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CSelection::CSelection()
{
	m_NrDirectParc=NULL;
	m_MapIntersection=NULL;
}

CSelection::~CSelection()
{
	FreeMemory();
}

int CSelection::GetSize()
{
	return m_Select.GetSize();
}

// Fc à appeler dès que quelque chose a été modifié dans la séléction
bool CSelection::BindAccelerateur()
{
	//CWaitCursor waitcursor;
	//FreeMemory();
	
	// des accès bien utile...
	m_NbParcTotal=0;
	m_NrDirectParc=new CWordArray[GetSize()];
	m_NrDirectBinome.RemoveAll();
	
	for(int b=0;b<GetSize();b++)
	{
		for(int p=0;p<2;p++)
		{
			m_NrDirectParc[b].Add(m_NbParcTotal++);
			m_NrDirectBinome.Add(b);
		}
	}

	for(b=0;b<m_NbParcTotal;b++)
	{
		int nr = m_NrDirectBinome[b];
	}
	m_TypeParLigne.SetSize(0,GetSize());
	for(int t=0;t<GetSize();t++)
		m_TypeParLigne.Add(GetType(t));

	return (1);
}

void CSelection::FreeMemory()
{
	if(m_NrDirectParc==NULL)return;
	delete[] 	m_NrDirectParc;
	m_NrDirectParc = NULL;

	// libération de MapIntersection
	if(m_MapIntersection==NULL)return;
	for(int ligne=0;ligne<GetSize();ligne++)
	{
		delete[] m_MapIntersection[ligne];
		m_MapIntersection[ligne] = NULL;
	}
	delete m_MapIntersection;
	m_MapIntersection = NULL;

}

LPCSTR CSelection::GetCode(int position)
{
	// retourne le code composite pour un bac sauf pour les bacs proba
	if(m_Select.GetAt(position).m_Type==1)
	{
		CTableBAC * tablebac = theApp.m_TableCinema.GetTableBac();
		CString codecomposite = tablebac->GetCodeComposite(m_Select.GetAt(position).m_Index);
		if(codecomposite[6]!='P')return codecomposite;
	}
	return m_Select.GetAt(position).m_Code;
}

// MODIF FRED: 17/05/2002 : BAC REGIE
// Méthode seulement utilisée pour récupérer le code, utilisé par la suite lors dec l'appel de GetTarif.
LPCSTR CSelection::GetCodeFromParc(int position)
{
	int index=m_NrDirectBinome[position];
	/*CString code;
	if(m_Select.GetAt(index).m_Type==1)
	{
		// On retourne le code de base pour un bac
		int idbac = m_Select.GetAt(index).m_Index;
		return theApp.m_TableCinema.GetTableBac()->GetCode(idbac);
	}
	else*/ 
	return m_Select.GetAt(index).m_Code;

}



int CSelection::GetType(int position)
{
	return m_Select.GetAt(position).m_Type;
}

// Cette fonction permet de récupérer le libellé du produit, bac, complexe en fonction de son index dans la table de sélection
LPCSTR CSelection::GetLibelle(int index)
{
	
	CString code = GetCode(index);
	LPCSTR txt;

	int type = GetType(index);
	bool erreur=0;
	// récupération des tables
	CTableProduit * TabProd = theApp.m_TableCinema.GetTableProduit();
	CTableBAC * TabBac = theApp.m_TableCinema.GetTableBac();		
	CTableComplexe * TabComplexe = theApp.m_TableCinema.GetTableComplexe();		

	int pos=0;	
	switch(type)		
	{			
		// produit		
		case tagSelection::PRODUITFIGE:		
		case tagSelection::PRODUIT:		
			if((pos = TabProd->FindIndexFromCode(code))==-1) erreur=1;		
			txt = TabProd->GetLibelle(pos);
			break;
		// bac			
		case tagSelection::BAC:	
			//if(code[6]=='P')code=code.Left(code.GetLength()-1); 
			if((pos = TabBac->FindIndexFromCode(code))==-1)  erreur=1; 		
			txt= TabBac->GetLibelle(pos);		
			break;
		// complexe		
		case tagSelection::COMPLEXE:		
			if((pos = TabComplexe->FindIndexFromCode(code))==-1) erreur=1;		
			txt= TabComplexe->GetLibelle(pos);		
			break;

	}
	if(erreur)AfxMessageBox("le code n'existe pas dans les tables");
	return txt;				
}


int CSelection::AddCode(LPCSTR code)
{

	// une sélection
	tagSelection s;
	bool erreur=false;
	int index;

	CTableComplexe *  tabComplexe = theApp.m_TableCinema.GetTableComplexe(); 
	CTableBAC *  tabBAC = theApp.m_TableCinema.GetTableBac(); 
	CTableProduit *  tabProduit = theApp.m_TableCinema.GetTableProduit(); 
	//s.InitialiseNbComplexe(tabComplexe->GetSize());
	s.m_Code = code;
	// On récupère le 1 er caractère du code pour savoir de quoi il s'agit

	if(code[0]=='C')
	{
		// c'est un complexe
		s.m_Type = tagSelection::COMPLEXE;
		if((index=tabComplexe->FindIndexFromCode(code))==-1)
		{
			erreur=1;
		}
		s.m_Index = index;
	
	}
	else if(code[0]=='B')
	{
		// ou c'est un BAC
		s.m_Type=tagSelection::BAC;
		if((index=tabBAC->FindIndexFromCode(code))==-1)
		{
			erreur=1;
			
		}
		s.m_Index = index;

	}
	else
	{
		// sinon c'est un produit
		s.m_Type=tagSelection::PRODUIT;
		if((index=tabProduit->FindIndexFromCode(code))==-1)
		{
			erreur=1;
			
		}
		s.m_Index = index;
		if(tabProduit->GetAt(index).m_flagProduitFige)s.m_Type=tagSelection::PRODUITFIGE;



	}
	if(erreur)
	{
		CString message;
		message.Format("Erreur dans la fonction CSelection::AddCode\nLe code: %s n'est pas un code valide, la sélection n'est pas ajoutée",code); 
		AfxMessageBox(message);
		return(-1);
	}
	else
	{
		// ajout de la nouvelle sélection
		m_Select.Add(s);
		return(0);
	}


}
// retourne le nb de lignes de la sélection avant chargement (sélection modifiéesi codes non trouvés)
void CSelection::Serialize(CArchive &ar)
{
	// sauvegarde
	if (ar.IsStoring())
	{
		// On mémorise d'abord le nombre de lignes de la sélection
		ar << m_Select.GetSize();
		// Puis les codes des lignes
		for(int i=0;i<m_Select.GetSize();i++)
		{
			ar << m_Select[i].m_Code;
		}
		// enregistrement des lignes sélectionnées
		SelectBinome.Serialize(ar);
	}
	// chargement
	else
	{
		// supression du contenu de m_select
		RemoveAll();
		// récupération du nombre de lignes
		int nblignes;
		ar >> nblignes;
		// bouclage sur les lignes
		for(int i=0;i<nblignes;i++)
		{
			CString code;
			ar >> code;
			// MODIF FRED : 03/06/2002 
			//	Pour un bac régie, il ne faut pas utiliser cette méthode. 
			if(code[0]=='B')
			{
				if(theApp.m_TableCinema.GetTableBac()->FindIndexFromCode(code)==-1)goto erreur;
				else goto valide;
			}
			// le code existe t'il toujours
			if(theApp.m_TableCinema.FindLibelleFromCode(code)=="")
			{
erreur:
				// message d'erreur
				CString message;
				CString type;
				if(code[0]=='B')
				{
					type = "bac";
				}
				if(code[0]=='C')
				{
					type = "complexe";
				}
				if(code[0]=='P')
				{
					type = "produit";
				}
				message.Format("Le %s %s n'existe plus dans la base de données\nIl est donc éliminé de la sélection\nLe plan est modifié",type,code);
				AfxMessageBox(message);
				// deux demi parcs
				TabPassageValide.Add(-1); 
				TabPassageValide.Add(-1); 
				continue;
			}
valide:
			// mise à jour de l'index et du type
			TabPassageValide.Add(2*i); 
			TabPassageValide.Add((2*i)+1); 
			AddCode(code);

		}
		// chargement des lignes sélectionnées
		SelectBinome.Serialize(ar);

		m_ListBacActif.SetSize(theApp.m_TableCinema.GetTableBac()->GetSize());
		m_ListComplexeActif.SetSize(theApp.m_TableCinema.GetTableComplexe()->GetSize());
		m_MasqueRegional.SetSize(theApp.NbIndiv);

		BindAccelerateur();
		BindMap();
		ConstruitListeBacComplexeActifs();

	}
}

int CSelection::GetIndex(int position)
{
	return m_Select.GetAt(position).m_Index;

}
// Controle l'intersection entre la ligne A et la ligne B
// attention passage par references
inline bool CSelection::IsIntersection(int & lignea, int & ligneb)
{

	ASSERT(AfxCheckMemory());
	ASSERT(lignea!=ligneb);


	// TODO REMI
	/*
	CStringArray * ListeCpxA = &(m_Select.GetAt(lignea).m_ComplexeLigne);
	CStringArray * ListeCpxB = &(m_Select.GetAt(ligneb).m_ComplexeLigne);
	// On cherche un élément commum aux tableaux de complexes des deux lignes
/*	for(int i=0;i<ListeCpxA->GetSize();i++)
	{
		for(int j=0;j<ListeCpxB->GetSize();j++)
		{
			CString txt1;
			txt1=ListeCpxA->GetAt(i);
			CString txt2;
			txt2=ListeCpxB->GetAt(j);

			if(txt1==txt2)
			//if((*ListeCpxA)[i] == (*ListeCpxB)[j])
			{
				return(true);
			}
		}

	}
	return(false);
*/
	// MODIF FRED: 31/05/2002
	// int size = theApp.m_TableCinema.GetTableComplexe()->GetSize(); 
	/*int sizeCompo1 = m_Select.GetAt(lignea).GetNbComplexe();
	int sizeCompo2 = m_Select.GetAt(ligneb).GetNbComplexe();
	
	// On cherche un élément commum aux tableaux de complexes des deux lignes
	for(int idxc1=0;idxc1<sizeCompo1;idxc1++)
	{
		for(int idxc2=0;idxc2<sizeCompo2;idxc2++)
		{
			if((m_Select.GetAt(lignea).IsComplexeActif(idxc1))&&(m_Select.GetAt(ligneb).IsComplexeActif(idxc2)))
			{
				return(1);
			}
		}
	}*/
	return(0);

}

bool CSelection::BindMap()
{
	// les tables de données
	CTableComplexe *  tabComplexe = theApp.m_TableCinema.GetTableComplexe(); 
	CTableBAC *  tabBAC = theApp.m_TableCinema.GetTableBac(); 
	CTableProduit *  tabProduit = theApp.m_TableCinema.GetTableProduit(); 

	for(int b=0;b<GetSize();b++)
	{
		// Remplissage du tableau de complexes pour chaque menbre de la sélection
		tagSelection s;
		s = m_Select.GetAt(b);
		int type = GetType(b);
		switch(type)
		{
			// produit
			case tagSelection::PRODUITFIGE : 
			case tagSelection::PRODUIT : 
			// On ajoute tous les complexes du produit
			// On boucle sur les semaines
			{
					tagProduit P;
					P = tabProduit->GetAt(s.m_Index);
					int size,index,taille;
					size = P.GetDuree(); 
					int sem;
					for(sem=0;sem<size;sem++)
					{
							// On boucle sur les complexes de la semaine
							for(int cpx=0;cpx<P.m_DescriptionIndexComplexe[sem].GetSize();cpx++)	
							{
								s.SetComplexeUsed(P.m_DescriptionIndexComplexe[sem][cpx]);
							}
							// On ajoute tous les complexes des bacs du produit
							for(int bac=0;bac<P.m_DescriptionIndexBac[sem].GetSize();bac++)	
							{
								index = P.m_DescriptionIndexBac[sem][bac];
								taille = tabBAC->GetAt(index).m_CompoComplexe.GetSize();
								for(cpx=0;cpx<taille;cpx++)	
								{
									// ajout de tous les complexes du bac
									s.SetComplexeUsed(tabBAC->GetAt(index).m_CompoComplexe[cpx]);
								}
								
							}
					
					}
					
				}
				break;
			
			//bac // ADD FRED
			case tagSelection::BAC : 
				{
					for(int cmplx=0;cmplx<tabBAC->GetAt(s.m_Index).m_CompoComplexe.GetSize();cmplx++)
					{
						s.SetComplexeUsed(tabBAC->GetAt(s.m_Index).m_CompoComplexe[cmplx]);
					}
				}
				break;

			// complexe
			case tagSelection::COMPLEXE :
				
				s.SetComplexeUsed(s.m_Index);
				break;
			default: ASSERT(0);

		}
		m_Select.SetAt(b,s);

	}

	// définition et calcul des valeurs de MapIntersection
	m_MapIntersection = new int *[GetSize()];
	for(int ligne=0;ligne<GetSize();ligne++)
	{
		m_MapIntersection[ligne] = new int[GetSize()];
	}
	int size = GetSize();
	int indexcomplexe1,indexcomplexe2;

	for(int ligneA=0;ligneA<size;ligneA++)
	{
		int sizeCompo1 = m_Select.GetAt(ligneA).GetNbComplexe();
		CWordArray CompoComplexeA;
		CompoComplexeA.Copy(m_Select.GetAt(ligneA).m_ComplexeLigne);

		for(int ligneB=ligneA+1;ligneB<size;ligneB++)
		{
			CWordArray CompoComplexeB;
			CompoComplexeB.Copy(m_Select.GetAt(ligneB).m_ComplexeLigne);

			int flag=0;
			// On cherche un élément commum aux tableaux de complexes des deux lignes
			int sizeCompo2 = m_Select.GetAt(ligneB).GetNbComplexe();

			// MODIF FRED: 02/07/2002 
			// Modification de l'élément complexe ligne de tagSelection de CByteAerray en CwordArray
			// + récupération d'un index complexe.

			for(int idxc1=0;idxc1<sizeCompo1;idxc1++)
			{
				indexcomplexe1 = CompoComplexeA.GetAt(idxc1);
		
				for(int idxc2=0;idxc2<sizeCompo2;idxc2++)
				{

					indexcomplexe2 = CompoComplexeB.GetAt(idxc2);
					if(indexcomplexe1==indexcomplexe2)
					{
						flag=1;
					}
					
					/*if((m_Select.GetAt(ligneA).IsComplexeActif(indexcomplexe1))&&(m_Select.GetAt(ligneB).IsComplexeActif(indexcomplexe2)))
					{
						flag=1;
					}*/
				}
			}
			if(flag)
			{
				m_MapIntersection[ligneA][ligneB] = 1;
				m_MapIntersection[ligneB][ligneA] = 1;
			}
			else
			{
				m_MapIntersection[ligneA][ligneB] = 0;
				m_MapIntersection[ligneB][ligneA] = 0;
			}
		}
	}

	return(1);
}

// CALCUL FRED
bool CSelection::ConstruitListeBacComplexeActifs()
{
	// la table des bacs
	CTableBAC * tabBac = theApp.m_TableCinema.GetTableBac();
	CTableComplexe * tabComplexe = theApp.m_TableCinema.GetTableComplexe();

	// Initialisation des tableaux
	for(int bac=0;bac<tabBac->GetSize();bac++)
	{
		m_ListBacActif.SetAt(bac,0);
	}
	for(int cmplx=0;cmplx<tabComplexe->GetSize();cmplx++)
	{
		m_ListComplexeActif.SetAt(cmplx,0);
	}

	// On boucle sur les bacs
	for(bac=0;bac<tabBac->GetSize();bac++)
	{
		// On boucle sur les complexes des bacs
		for(cmplx=0;cmplx<tabBac->GetAt(bac).m_CompoComplexe.GetSize();cmplx++)
		{
			// On boucle sur les lignes de la sélection
			for(int ligne=0;ligne<GetSize();ligne++)
			{
			
				// Si on complexe appartient à une ligne active alors le bac est actif
				if(SelectBinome[ligne])
				{
					tagSelection sel;
					sel = m_Select.GetAt(ligne);
					int indexcomplexe = tabBac->GetAt(bac).m_CompoComplexe.GetAt(cmplx);
					if(sel.IsComplexeActif(indexcomplexe))
					{
						// On ajoute le bac
						m_ListBacActif.SetAt(bac,1);

						/*
						CString message;
						message.Format("bac actif:%d\n",bac);
						TRACE(message);
						*/

						// On ajoute le complexe
						m_ListComplexeActif.SetAt(indexcomplexe,1);
						// passage au bac suivant
						goto BAC;
					}
				}
			}

		}
BAC:;

	}

	return(1);
}

// CALCUL FRED
bool CSelection::PrepareMasqueRegional()
{
	// Initialisation à 0 du masque (bouclage sur les individus)
	for(int ind=0;ind<theApp.NbIndiv;ind++)
	{
		m_MasqueRegional.SetAt(ind,0);		
	}
	// On boucle sur les bacs actifs
	for(int bac=0;bac<m_ListBacActif.GetSize();bac++)
	{
		// On récupère l'index bac probabilisé du bac
		int indexBacProba;
		// Si le bac est actif, alors on le prend en compte dans le masque
		if(m_ListBacActif[bac])
		{
			CString codeBac = theApp.m_TableCinema.GetTableBac()->GetCode(bac); 

			// ERREUR CODE BAC ALAIN !!!!!!
			// MODIF FRED: 15/05/2002 bac régie
			/*if(codeBac[6]!='P')
			{
				codeBac=codeBac.Left(codeBac.GetLength()-1);
				codeBac+="P";
			}*/
			if(!theApp.m_MapIndexBac.Lookup(codeBac,indexBacProba))
			{
				CString message;
				message.Format("Erreur lors de la construction de la cible régionale\nLe bac %s n'est pas trouvé",codeBac);
				AfxMessageBox(message);
				ASSERT(0);
			}
			// Pour chaque individu du bac proba
			for(ind=0;ind<theApp.m_NbIndivSupp[indexBacProba];ind++)
			{
				m_MasqueRegional.SetAt(theApp.m_NrIndivSupp[indexBacProba][ind],1); 		
			}
		}
		
	}
	return(1);
}

void CSelection::InitTab()
{
	SelectBinome.RemoveAll(); 
	// Initialisation des binomes sélectionnés (tous au départ)
	SelectBinome.SetSize(GetSize());
	for(int ligne=0;ligne<GetSize();ligne++)
	{
		SelectBinome.SetAt(ligne,1);
	}
	m_ListBacActif.SetSize(theApp.m_TableCinema.GetTableBac()->GetSize());
	m_ListComplexeActif.SetSize(theApp.m_TableCinema.GetTableComplexe()->GetSize());
	m_MasqueRegional.SetSize(theApp.NbIndiv);
}

void CSelection::PrepareMasqueNational()
{
	// Initialisation à 1 du masque (bouclage sur les individus)
	for(int ind=0;ind<theApp.NbIndiv;ind++)
	{
		m_MasqueRegional.SetAt(ind,1);		
	}

}

// Cette fonction permet de récupérer un tableau de codes de la sélection

void CSelection::GetAllCode(CStringArray &codes)
{
	// On boucle sur les membres de la sélection
	for(int ligne=0;ligne<GetSize();ligne++)
	{
		codes.Add(GetCode(ligne));
	}
}

