// Noeud.cpp: implementation of the CNoeud class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Noeud.h"
 


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNoeud::CNoeud()
{
	m_Libelle = "";					//libelle de la branche
	m_Alias = "";					//alias de la branche
	m_Codif ="" ;					//code unique pour cette elmt
	m_DateExercice="xxx";
	m_Famille ="xxx";
	m_Secteur ="xxx";
	m_Marque ="xxx";
	m_pRacine = NULL;				//on ne connait pas la racine
}

CNoeud::CNoeud(CNoeud *Mere)
{
	m_Libelle = "";					//libelle de la branche
	m_Alias = "";					//alias de la branche
	m_Codif ="" ;					//code unique pour cette elmt
	m_DateExercice="xxx";
	m_Famille ="xxx";
	m_Secteur ="xxx";
	m_Marque ="xxx";
	m_pRacine = Mere;				//pointe sur la racine mere
}

CNoeud::~CNoeud()
{

	CNoeud *pNoeud;

	for(int x=0;x<m_pNoeud.GetSize();x++)
	{
		if(m_pNoeud[x]!=NULL) 
		{
			pNoeud = m_pNoeud[x]; 
			delete m_pNoeud[x];
		}
	}
	m_pNoeud.RemoveAll();
}

// Ajout en mémoire de la branche associé aux marques
bool CNoeud::AjoutArborescenceMarque(CPWFile &File,CNoeud *Arbre,CString &Ligne,int &Niveau)

{
	int NbBranche,NbBrancheSuiv;	
	CString Text;
	CString LigneSuiv;

	
	// Nombre de branches associées à ce nouveau noeud
	if (File.GetNbItems(&Ligne, &NbBranche, '#') != 0)
	{
				
		// Creation du ss tableau de feuille
		Arbre->m_pNoeud.SetSize(NbBranche);

		for (int i = 0;i<NbBranche;i++)
		{
			CNoeud *pNoeud=new CNoeud;

			// lecture allias marque
			if (m_Buffer == "" )
			{
				File.ReadString(&Text);
				pNoeud->m_Alias= Text;
			}
			else
			{
				pNoeud->m_Alias= m_Buffer;
				m_Buffer = "";
			}

			// pas de répertoire pour les marques
			pNoeud->m_Libelle= "Nothing";

			// niveau
			pNoeud->m_Niveau = Niveau + 1; 

			// lien racine mère
			pNoeud->m_pRacine = Arbre;

    		// path complet de la branche courante
			pNoeud->m_PathFichier = Arbre->m_PathFichier + "\\" + pNoeud->m_Libelle; 

			Arbre->m_pNoeud[i] =pNoeud;
			
			// lecture ligne suivante
			File.ReadString(&m_Buffer);
			LigneSuiv = m_Buffer;

			// S'agit-il d'une nouvelle branche fille
			if (File.GetNbItems(&LigneSuiv, &NbBrancheSuiv, '#') != 0)
            {
				m_Buffer = "";
				AjoutArborescenceMarque(File,pNoeud,LigneSuiv,pNoeud->m_Niveau);
			}
			
		}

	}

	return true;

}

// Ajout en mémoire de la branche associé aux annonceurs
bool CNoeud::AjoutArborescenceProduit(CPWFile &File,CNoeud *Arbre,CString &Ligne,int &Niveau,CNoeud *pArbreMarque)

{
	int NbBranche,NbBrancheSuiv;	
	CString Text;
	CString LigneSuiv;
	CNoeud *pNoeudMarque = NULL;

	
	// Nombre de branches associées à ce nouveau noeud
	if (File.GetNbItems(&Ligne, &NbBranche, '#') != 0)
	{
				
		// Creation du ss tableau de feuille
		Arbre->m_pNoeud.SetSize(NbBranche);

		for (int i = 0;i<NbBranche;i++)
		{
			CNoeud *pNoeud=new CNoeud;

			// lecture libellé feuille (correspond aux répertoires)
			if (m_Buffer == "")
			{
				File.ReadString(&Text);
				pNoeud->m_Libelle= Text;

				// Test si répertoire existe
				CString RepCur = Arbre->m_PathFichier + "\\" + Text;
				CFileFind FileFind;
				if (FileFind.FindFile(RepCur,0) == FALSE)
				{
					// Dans ce cas il faut le créer
					// AfxMessageBox(RepCur);
					CreateDirectory(RepCur,NULL); 
				}

			}
			else
			{
				pNoeud->m_Libelle= m_Buffer;
				m_Buffer = "";
			}

			// lecture alias feuille
			File.ReadString(&Text);
			pNoeud->m_Alias = Text;

			// lecture code feuille
			File.ReadString(&Text);		
			pNoeud->m_Codif = Text;

			if (Niveau == 2)
			{
				// Niveau Produit lecture marque associée
				File.ReadString(&Text);
				pNoeud->m_Marque = Text;

				// lecture famille et secteur associée
				File.ReadString(&Text);
				pNoeud->m_Famille = Text;
				File.ReadString(&Text);
				pNoeud->m_Secteur = Text;

				// Type Branche Produit
				File.ReadString(&Text);

				// 2 lignes suppl. à lire
				File.ReadString(&Text);
				File.ReadString(&Text);

			}

			if (Niveau == 0)
			{
				// Niveau Groupe lecture date exercice
				File.ReadString(&Text);
				pNoeud->m_DateExercice = Text;

				// Type Branche Groupe
				File.ReadString(&Text);

				// 2 lignes suppl. à lire
				File.ReadString(&Text);
				File.ReadString(&Text);
			}

			if (Niveau == 1)
			{
				// Niveau Annonceur (lecture Date exercice 31/06/2000)
				File.ReadString(&Text);
				pNoeud->m_DateExercice = Text;

				// Niveau Annonceur (lecture Famille + Secteur 31/06/2000)
				File.ReadString(&Text);
				pNoeud->m_Famille = Text;
				File.ReadString(&Text);
				pNoeud->m_Secteur = Text;

				// Type Branche Annonceur
				File.ReadString(&Text);

				// 2 lignes suppl. à lire
				File.ReadString(&Text);
				File.ReadString(&Text);

				// NEW FORMAT EDI !!!! Lecture des 14 lignes supplémentaires

				// Type tiers toujours BY pour annonceur
				File.ReadString(&Text);
				pNoeud->m_TypeTiers = Text;

				// Code Tiers
				File.ReadString(&Text);
				pNoeud->m_CodeTiers = Text; 

				// Type code tiers
				File.ReadString(&Text);
				pNoeud->m_TypeCodeTiers = Text;

				// Nom annoceur (redondant !!! mais bon)
				File.ReadString(&Text);
				pNoeud->m_NomTiers = Text;

				// Adresse 1
				File.ReadString(&Text);
				pNoeud->m_AdresseTiers1 = Text;
	
				// Adresse 2
				File.ReadString(&Text);
				pNoeud->m_AdresseTiers2 = Text;

				// Code Postal
				File.ReadString(&Text);
				pNoeud->m_CodePostalTiers = Text;

				// Ville
				File.ReadString(&Text);
				pNoeud->m_VilleTiers = Text;

				// Code pays
				File.ReadString(&Text);
				pNoeud->m_CodePaysTiers = Text;

				// No tel
				File.ReadString(&Text);
				pNoeud->m_NoTelTiers = Text;

				// No fax
				File.ReadString(&Text);
				pNoeud->m_NoFaxTiers = Text;

				// Nom Interlocuteur tiers
				File.ReadString(&Text);
				pNoeud->m_NomContactTiers = Text;

				// Prénom Interlocuteur tiers
				File.ReadString(&Text);
				pNoeud->m_PrenomContactTiers = Text;

				// Adr Mail Interlocuteur tiers
				File.ReadString(&Text);
				pNoeud->m_AdrMailContactTiers = Text;

			}

			// niveau
			pNoeud->m_Niveau = Niveau + 1; 

   		    // lien avec racine marque
			if (RechercheNoeudViaAlias(pArbreMarque,pNoeud->m_Marque,pNoeudMarque) == true)
				pNoeud->m_pRacineMarque = pNoeudMarque;
			else
				pNoeud->m_pRacineMarque = NULL;

			// lien racine mère
			pNoeud->m_pRacine = Arbre;

    		// path complet de la branche courante
			pNoeud->m_PathFichier = Arbre->m_PathFichier + "\\" + pNoeud->m_Libelle; 

			Arbre->m_pNoeud[i] =pNoeud;
			
			// lecture ligne suivante
			File.ReadString(&m_Buffer);
			LigneSuiv = m_Buffer;

			// S'agit-il d'une nouvelle branche fille
			if (File.GetNbItems(&LigneSuiv, &NbBrancheSuiv, '#') != 0)
            {
				m_Buffer = "";
				AjoutArborescenceProduit(File,pNoeud,LigneSuiv,pNoeud->m_Niveau,pArbreMarque);
			}
		}
	}

	return true;

}

// Ajout en mémoire de la branche associé aux secteurs
bool CNoeud::AjoutArborescenceSecteur(CPWFile &File,CNoeud *Arbre,CString &Ligne,int &Niveau)
{
	int NbBranche,NbBrancheSuiv;	
	CString Text;
	CString LigneSuiv;
	CNoeud *pNoeudMarque = NULL;
	
	// Nombre de branches associées à ce nouveau noeud
	if (File.GetNbItems(&Ligne, &NbBranche, '#') != 0)
	{
				
		// Creation du ss tableau de feuille
		Arbre->m_pNoeud.SetSize(NbBranche);

		for (int i = 0;i<NbBranche;i++)
		{
			CNoeud *pNoeud=new CNoeud;

			// lecture libellé feuille
			if (m_Buffer == "" )
			{
				File.ReadString(&Text);
				pNoeud->m_Alias= Text;
			}
			else
			{
				pNoeud->m_Alias= m_Buffer;
				m_Buffer = "";
			}

			// lecture alias feuille
			pNoeud->m_Libelle = "Nothing";

			// niveau
			pNoeud->m_Niveau = Niveau + 1; 

   		    // aucun lien avec racine marque
			pNoeud->m_pRacineMarque = NULL;

			// lien racine mère
			pNoeud->m_pRacine = Arbre;

    		// path complet de la branche courante
			pNoeud->m_PathFichier = Arbre->m_PathFichier + "\\" + pNoeud->m_Libelle; 

			Arbre->m_pNoeud[i] =pNoeud;
			
			// lecture ligne suivante
			File.ReadString(&m_Buffer);
			LigneSuiv = m_Buffer;

			// S'agit-il d'une nouvelle branche fille
			if (File.GetNbItems(&LigneSuiv, &NbBrancheSuiv, '#') != 0)
            {
				m_Buffer = "";
				AjoutArborescenceSecteur(File,pNoeud,LigneSuiv,pNoeud->m_Niveau);
			}
		}
	}

	return true;

}

// Ajout d'un elmt noeud sur un noeud existant
bool CNoeud::LierNoeud(CNoeud *Mere,CNoeud *Enfant)
{
	// Ajuste nb elmt branche existante
	Enfant->m_pRacine = Mere;
	Mere->m_pNoeud.Add(Enfant);
	return true;
}

CNoeud* CNoeud::NoeudParent(CNoeud *Enfant)
{
	// si noeud enfant non nul, renvoie le noeud parent
	if (Enfant != NULL)
		return Enfant->m_pRacine;
	else
		return NULL;
}	

// Vire tous les ss-noeuds du noeud racine
void CNoeud::FusillerSsNoeud()
{
	// Si ss-noeud existants on les efface
	if (this->m_pNoeud.GetSize() > 0)
	{	
		for (int i= 0;i<this->m_pNoeud.GetSize();i++)
		{
			delete  this->m_pNoeud[i];
		}
	}
}


// Recherche libelle dans un arbre / uniquemenu au niveau infèrieure
bool CNoeud ::RechercheNoeudViaAlias(CNoeud *pArbre,CString Alias,CNoeud *&pNoeud)
{
	bool NoeudTrouve = false;
	int NbElmt;

	NbElmt = pArbre->m_pNoeud.GetSize();
	if (NbElmt > 0 && Alias != "xxx" && Alias != "")
	{
		for(int i= 0;i<NbElmt;i++)
		{
			if (pArbre->m_pNoeud[i]->m_Alias == Alias)
			{
				pNoeud = pArbre->m_pNoeud[i];
				NoeudTrouve = true;
				break;
			}
		}
	}

	return NoeudTrouve;
}


/*
// Renvoie Date Exercice d'un noeud quelconque de la branche produit (groupe ou annonceur ou produit)
CString CNoeud ::DateExercice(CNoeud *Noeud)
{

}
*/	



