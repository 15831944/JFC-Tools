// BaseCinema.cpp: implementation of the CBaseCinema class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BaseCinema.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CString GCheminAccesDonnee;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseCinema::CBaseCinema()
{
}

CBaseCinema::~CBaseCinema()
{

}

bool CBaseCinema::LoadTable(int type)
{
	CWaitCursor wait;
	CString chemin = GCheminAccesDonnee+"\\DATA";

	for(int t=0;t<3;t++)
	{

		// Ouverture du fichier LockData.txt avec droit en lecture seule
		if(LockData.Open(chemin+"\\LockData.txt",CFile::modeRead|CFile::shareDenyWrite))
		{
			break;
		}
		Sleep(3000);

	}
	if(t>=3)
	{
		AfxMessageBox("La lecture n'a pas pu être effectuée: la base est utilisée par le module d'administration de l'atelier cinéma.");
		return(0);
	}


	// chargement de la table des BACS
	m_TableBac.LoadTable();	

	// chargement de la table des complexes
	m_TableComplexe.LoadTable();


	// Chargement de la table des groupes bacs
	m_TableGroupeBac.LoadTable(GCheminAccesDonnee);

	// Chargement de la table des groupes complexes
	m_TableGroupeComplexe.LoadTable(GCheminAccesDonnee);

	// Chargement de la table des entrées (CNC)
	m_TableNbEntree.LoadTable();

	if(!(LinkTable()))return false;

	// Création des bacs régie
	if(type==0)
	{
		if(!CreateBacRegie())return false;
	}

	// chargement de la table des produits
	m_TableProduit.LoadTable();

	// chargement de la table des formats
	m_TableFormat.LoadTable();
	
	// chargement de la table des tempos
	m_TableTempo.LoadTable();


	// Fermeture du fichier LockData.txt
	LockData.Close();

	if(!(LinkProduit()))return false;

	return true;

}

bool CBaseCinema::LinkTable()
{
	int index;
	// définition de l'index bac de chacun des complexes
	for(int i=0;i<m_TableComplexe.GetSize();i++)
	{
		// index du BAC
		index = m_TableBac.FindIndexFromCode (m_TableComplexe.GetCodeBac(i));  
		if(index<0)
		{
			// pour que ca fonctionne, on "corrige"
			index=0; 
		}
		// définition du contenu en complexe de chacun des BACS;
		tagBAC bac = m_TableBac.GetAt(index);
		bac.m_CompoComplexe.Add(WORD(i));
		m_TableBac.SetAt(index,bac);
		
		tagComplexe c = m_TableComplexe.GetAt(i);
		// définition de l'index BAC du complexe
		c.m_indexBAC =WORD(index);
		m_TableComplexe.SetAt(i,c);
	
	}

	// Affiche les bacs non utilisés (en lot)
	CString StrBACNonUtil = "";
	for(i=0;i<m_TableBac.GetSize();i++)
	{
			tagBAC bac = m_TableBac.GetAt(i); 
			if(bac.m_CompoComplexe.GetSize()==0)
			{
				CString message;
				message.Format("Le bac %s n'est utilisé par aucun complexe",bac.m_Code);
				StrBACNonUtil = StrBACNonUtil + message + "\n";
				//AfxMessageBox(message);
			}

	}
	if (StrBACNonUtil != "") AfxMessageBox(StrBACNonUtil,MB_ICONEXCLAMATION);

	return CalculPartDeMarche();
}

// Cette méthode permet d'initialiser la variable part de marché de chacun des complexes
bool CBaseCinema::CalculPartDeMarche()
{
	CString code;
	int indexTableNbEntree,indexTableCmpl;
	

	// On boucle sur les BACS
	for(int Bac=0;Bac<m_TableBac.GetSize();Bac++)
	{
		double total=0;

		//ASSERT(Bac!=idxbactest);

		// calcul de la part totale de marché pour le bac parent du complexe

		int NbCpx = m_TableBac.GetAt(Bac).m_CompoComplexe.GetSize();
		for(int Cpx=0;Cpx<NbCpx ;Cpx++)
		{
			// Recherche du complexe
			indexTableCmpl = m_TableBac.GetAt(Bac).m_CompoComplexe.GetAt(Cpx);
			tagComplexe c = m_TableComplexe.GetAt(indexTableCmpl); 

			// Recherche du nb d'entrées
			code = m_TableComplexe.GetCode(indexTableCmpl);
			indexTableNbEntree = m_TableNbEntree.FindIndexFromCode(code); 
			double nb;
			if(indexTableNbEntree!=-1)
			{
				nb=(double)m_TableNbEntree.GetAt(indexTableNbEntree).m_NbEntree;  
			}
			else
			{
				// correction: le nombre d'entrées est à 0 lorsqu'il n'existe pas dans la table des entrées
				// ajout d'un nombre d'entrée égale à 0
				tagNbEntree entree;
				entree.m_CodeComplexe=code;
				entree.m_NbEntree =0;
				m_TableNbEntree.Add(entree);
				m_TableNbEntree.SetModified(1); 
				nb=0;
			}

			// calcul du total et stoquage temporaire du nb d'entre par cpx
			c.m_PartDeMarche=nb;
			m_TableComplexe.SetAt(indexTableCmpl ,c);
			total+=nb;
		}
		
		// On boucle sur les complexes du bac
		// Un petit test pour verifier que le résultat vaut EXACTEMENT 1
		// TEST
//		double tot=0;
		// TEST
		for(Cpx=0;Cpx<NbCpx ;Cpx++)
		{
			// index dans la table des complexes 
			indexTableCmpl = m_TableBac.GetAt(Bac).m_CompoComplexe.GetAt(Cpx);
//			ASSERT(indexTableCmpl!=idxcpxtest);
			tagComplexe c = m_TableComplexe.GetAt(indexTableCmpl); 

			// la part de marché : le rapport entre le nombre d'entrée
			// du complexe et le nombre d'entrée total
			if(total>0)c.m_PartDeMarche = c.m_PartDeMarche/total;  
			else c.m_PartDeMarche = +0.0;
			// Rangement du résultat
			m_TableComplexe.SetAt(indexTableCmpl ,c);
			// TEST
//			tot+=m_TableComplexe[indexTableCmpl].GetPartDeMarche();
			// TEST
		}	
//		ASSERT(tot>.999999999999999);
//		ASSERT(tot<1.000000000000001);


		
		// De plus on range le total pour dans bac
		tagBAC theBac = m_TableBac.GetAt(Bac);
		theBac.m_NbEntree=total;
		m_TableBac.SetAt(Bac,theBac);
	}

	return (1);
	
}

CTableComplexe * CBaseCinema::GetTableComplexe()
{
	return &m_TableComplexe;
}

// Récup table des groupes de complexe
CTableGroupeComplexe * CBaseCinema::GetTableGroupeComplexe()
{
	return &m_TableGroupeComplexe;
}


CTableFormat * CBaseCinema::GetTableFormat()
{
	return &m_TableFormat;
}

CTableBAC * CBaseCinema::GetTableBac()
{
	return &m_TableBac;
}

// Récup table des groupes de bacs
CTableGroupeBAC * CBaseCinema::GetTableGroupeBac()
{
	return &m_TableGroupeBac;
}

CTableRegie * CBaseCinema::GetTableRegie()
{
	return &m_TableRegie;
}

CTableTempo * CBaseCinema::GetTableTempo()
{
	return &m_TableTempo;
}

CTableProduit * CBaseCinema::GetTableProduit()
{
	return &m_TableProduit;
}

CTableNbEntreeComplexe * CBaseCinema::GetTableNbEntree()
{
	return &m_TableNbEntree;
}



bool CBaseCinema::SaveTable()
{

	// Ouverture du fichier LockData.txt avec aucun partage
	// gestion d'attente: on fait trois tentatives d'ouverture séparée de deux secondes
	CString chemin = GCheminAccesDonnee+"\\DATA";


	for(int t=0;t<3;t++)
	{
		if(LockData.Open(chemin+"\\LockData.txt",CFile::modeCreate|CFile::shareExclusive))
		{
			// ouverture effectuée
			break;
		}

		Sleep(3000);
	}
	if(t>=3)
	{
		AfxMessageBox("La sauvegarde n'a pas pu être effectuée: la base est utilisée par l'atelier cinéma.");
		return(0);
	}

	if(m_TableProduit.IsModif())
	{
		// tri
		//m_TableProduit.Tri(); 

		if(!m_TableProduit.SaveTable())return 0;
	}
	if(m_TableBac.IsModif())
	{
		// tri
		//m_TableBac.Tri(); 
		if(!m_TableBac.SaveTable())return 0;
	}
	if(m_TableComplexe.IsModif())
	{
		// tri
		//m_TableComplexe.Tri(); 

		if(!m_TableComplexe.SaveTable())return 0;
	}

	if(m_TableNbEntree.IsModif())
	{
		// tri
		//m_TableNbEntree.Tri(); 

		if(!m_TableNbEntree.SaveTable())return 0;
	}
	if(m_TableTempo.IsModif())
	{
		if(!m_TableTempo.SaveTable())return 0;
	}
	if(m_TableFormat.IsModif())
	{
		if(!m_TableFormat.SaveTable())return 0;
	}

	// Fermeture du fichier LockData.txt
	LockData.Close();

	return (1);
	
}

CTableComplexe CBaseCinema::GetNewInstanceTableComplexe()
{
	return m_TableComplexe;
}


CTableBAC CBaseCinema::GetNewInstanceTableBac()
{
	return m_TableBac;
}

CTableGroupeBAC CBaseCinema::GetNewInstanceTableGroupeBac()
{
	return m_TableGroupeBac;
}

CTableGroupeComplexe CBaseCinema::GetNewInstanceTableGroupeComplexe()
{
	return m_TableGroupeComplexe;
}

CTableTempo CBaseCinema::GetNewInstanceTableTempo()
{
	return m_TableTempo;
}

void CBaseCinema::SetTableComplexe(CTableComplexe tabcomplexe)
{
	m_TableComplexe = tabcomplexe;
}

void CBaseCinema::SetTableBac(CTableBAC tablebac)
{
	m_TableBac = tablebac;
}

CTableProduit CBaseCinema::GetNewInstanceTableProduit()
{
	return m_TableProduit;
}

CTableFormat CBaseCinema::GetNewInstanceTableFormat()
{
	return m_TableFormat;
}

CTableNbEntreeComplexe CBaseCinema::GetNewInstanceTableNbEntree()
{
	return m_TableNbEntree;
}


void CBaseCinema::SetTableProduit(CTableProduit tableproduit)
{
	m_TableProduit = tableproduit;
}

void CBaseCinema::SetTableTempo(CTableTempo tabletempo)
{
	m_TableTempo = tabletempo;
}

void CBaseCinema::SetTableFormat(CTableFormat tableformat)
{
	m_TableFormat = tableformat;
}
void CBaseCinema::SetTableNbEntree(CTableNbEntreeComplexe tablenbentree)
{
	m_TableNbEntree = tablenbentree;
}

bool CBaseCinema::IsCodeDefined(LPCSTR Code)
{
	if(m_TableBac.FindIndexFromCode(Code,0)>=0)return(1);
	if(m_TableComplexe.FindIndexFromCode(Code,0)>=0)return(1);
	if(m_TableProduit.FindIndexFromCode(Code,0)>=0)return(1);
	return(0);
}

LPCSTR CBaseCinema::FindCodeFromLibelle(CString Libelle)
{
	// On boucle sur la table des produits pour chercher le libelle
	int size = m_TableProduit.GetSize();
	for(int p=0;p<size;p++)
	{
		CString m_Libelle =  m_TableProduit.GetLibelle(p);
		if(m_Libelle==Libelle)return m_TableProduit.GetCode(p); 	

	}
	size = m_TableBac.GetSize();
	for(int bac=0;bac<size;bac++)
	{
		if(m_TableBac.GetLibelle(bac)==Libelle)
		{
			CString codecomposite = m_TableBac.GetCodeComposite(bac);
			if(codecomposite[6]=='P') return m_TableBac.GetCode(bac); 
			else return codecomposite; 	
		}
	}
	size = m_TableComplexe.GetSize();
	for(int c=0;c<size;c++)
	{
		if(m_TableComplexe.GetLibelle(c)==Libelle)return m_TableComplexe.GetCode(c); 	

	}
	return("");

}

LPCSTR CBaseCinema::FindLibelleFromCode(CString code)
{
	// On boucle sur la table des produits pour chercher le libelle
	int size = m_TableProduit.GetSize();
	for(int p=0;p<size;p++)
	{
		if(m_TableProduit.GetCode(p)==code)return m_TableProduit.GetLibelle(p); 	

	}
	size = m_TableBac.GetSize();
	for(int bac=0;bac<size;bac++)
	{
		if(m_TableBac.GetCode(bac)==code)return m_TableBac.GetLibelle(bac); 	

	}
	size = m_TableComplexe.GetSize();
	for(int c=0;c<size;c++)
	{
		if(m_TableComplexe.GetCode(c)==code)return m_TableComplexe.GetLibelle(c); 	

	}
	return("");
}

// Création des bacs régie
bool CBaseCinema::CreateBacRegie()
{
	// copie de la table des bacs
	CArray<tagBAC,tagBAC&> BacRegieA;
	CArray<tagBAC,tagBAC&> BacRegieM;
	CArray<tagBAC,tagBAC&> BacRegieN;

	for(int idxbac=0;idxbac<m_TableBac.GetSize();idxbac++)
	{
		tagBAC bac = m_TableBac.GetAt(idxbac);
		tagBAC bacRegieA;
		tagBAC bacRegieM;
        tagBAC bacRegieN; 

	
		// On calcule la part de marché pour régieA et M.
		double partA=0,partM=0,partN=0;

		// Vérification au moins un complexe dans le bac
		if (bac.m_CompoComplexe.GetSize()<1)
		{
			#ifdef _DEBUG
						// AfxMessageBox("Ce bac n'est utilisé par aucun complexe");
			#endif
			goto regie;
		}

{
		// On boucle sur les complexes du bac
		for(int idxcomplexe=0;idxcomplexe<bac.m_CompoComplexe.GetSize();idxcomplexe++)
		{
			char regie = m_TableComplexe.GetRegie(bac.m_CompoComplexe[idxcomplexe]);
			// MODIF FRED: 30/08/2002
			// On utilise l'index de la table des entrées et non pas l'index de la table complexe.
			LPCSTR code = m_TableComplexe.GetCode(bac.m_CompoComplexe[idxcomplexe]);
			int idxentree = m_TableNbEntree.FindIndexFromCode(code);

			tagNbEntree entree = m_TableNbEntree.GetAt(idxentree);
			if(regie=='A')
			{
				partA+=entree.GetNbEntree(); 
				bacRegieA.m_CompoComplexe.Add(bac.m_CompoComplexe[idxcomplexe]); 
			}
			else if (regie=='M')
			{
				partM+=entree.GetNbEntree(); 
				bacRegieM.m_CompoComplexe.Add(bac.m_CompoComplexe[idxcomplexe]); 
			}
			else
			{
				// Les indépendants
				partN+=entree.GetNbEntree(); 
				bacRegieN.m_CompoComplexe.Add(bac.m_CompoComplexe[idxcomplexe]);
			}
		}

		// BAC REGIE
		// même code de base, la composition en complexe change radicalement!!
}
regie:;		
		bacRegieA.m_Code			= bac.m_Code;
		bacRegieA.m_CodeComposite	= bac.m_Code+"A";
		bacRegieA.m_Libelle			= bac.m_Libelle+" (A)";
		bacRegieA.m_NbEntree		= bac.m_NbEntree;
		bacRegieA.m_Quota			= partA/bac.m_NbEntree; 

		bacRegieM.m_Code			= bac.m_Code;
		bacRegieM.m_CodeComposite	= bac.m_Code+"M";
		bacRegieM.m_Libelle			= bac.m_Libelle+" (M)";
		bacRegieM.m_NbEntree		= bac.m_NbEntree;
		bacRegieM.m_Quota			= partM/bac.m_NbEntree; 

		// Les indépendants
		bacRegieN.m_Code			= bac.m_Code;
		bacRegieN.m_CodeComposite	= bac.m_Code+"N";
		bacRegieN.m_Libelle			= bac.m_Libelle+" (N)";
		bacRegieN.m_NbEntree		= bac.m_NbEntree;
		bacRegieN.m_Quota			= partN/bac.m_NbEntree; 

		// Ajout d'un bac régie dans son tableau
		BacRegieA.Add(bacRegieA);
		BacRegieM.Add(bacRegieM);
		BacRegieN.Add(bacRegieN);

		// modif du code bac (il devient bac proba)
		bac.m_CodeComposite = bac.m_Code+"P";
		m_TableBac.SetAt(idxbac,bac);
		// Ajout des codes composites
		m_TableBac.MapCodeToIndex(bac.m_CodeComposite,idxbac);
		m_TableBac.MapCodeToIndex(BacRegieA.GetAt(idxbac).m_CodeComposite,m_TableBac.GetSize()+idxbac);
		m_TableBac.MapCodeToIndex(BacRegieM.GetAt(idxbac).m_CodeComposite,2*m_TableBac.GetSize()+idxbac);
		m_TableBac.MapCodeToIndex(BacRegieN.GetAt(idxbac).m_CodeComposite,3*m_TableBac.GetSize()+idxbac);


	}
	// Ajout des tableaux bacs régie
	m_TableBac.Append(BacRegieA);
	m_TableBac.Append(BacRegieM);

	// Les indépendants
	m_TableBac.Append(BacRegieN);

	BacRegieA.RemoveAll();
	BacRegieM.RemoveAll(); 
	BacRegieN.RemoveAll(); 

	return true;


}


// Si on appelle cette fction avec flagcodebase, on récupère le code base du code composite du produit.
bool CBaseCinema::LinkProduit()
{
	// définition de l'index des complexes formant la table des produits
	// pour chaque produit
	for(int i=0;i<m_TableProduit.GetSize();i++)
	{
		tagProduit p = m_TableProduit.GetAt(i);
		// pour chaque complexe de chaque produit
		// le produit est il figé ? On boucle sur la période de fixation
		for(int k=0;k<p.GetDuree();k++)
		{
			p.m_DescriptionIndexComplexe[k].RemoveAll();
			p.m_DescriptionIndexBac[k].RemoveAll();

			for(int j=0;j<p.m_DescriptionCode[k].GetSize();j++)
			{	
				// On cherche l'index du complexe ou du bac correspondant au code
				LPCSTR code=p.m_DescriptionCode[k][j];
				// index du complexe 
				if(code[0]=='C')
				{
					int index = m_TableComplexe.FindIndexFromCode(code);  
					if(index>=0)
						p.m_DescriptionIndexComplexe[k].Add(WORD(index)); 
				}

				// ce n'est pas un complexe donc c'est un bac
				else if(code[0]=='B')
				{
					CString codebac = code;
					// pour chacun des produits ajouté, on vérifie que le bac est utilisé par aucun complexe
					//if(codebac[6]=='\0')AfxMessageBox("Ce code bac n'est pas un code composite, tous les bacs d'un produit doivent être défini à l'aide de leur code composite");
					//if(codebac.GetLength()==7)codebac = codebac.Left(codebac.GetLength()-1);

					int index = m_TableBac.FindIndexFromCode(codebac);  

					if(index>=0)
						p.m_DescriptionIndexBac[k].Add(WORD(index)); 
				}

				else
				{
					CString msgerr;
					msgerr.Format("Le code %s n'est pas un code valide.",code);
					AfxMessageBox(msgerr);		
				}
			}
		}
		//  On le remet dans sa table après manipulation
		m_TableProduit.SetAt(i,p);
	}
	return(1);
}
