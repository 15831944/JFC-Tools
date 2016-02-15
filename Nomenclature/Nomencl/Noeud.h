// Noeud.h: interface for the CNoeud class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NOEUD_H__2380181A_6EB6_4E21_9193_AD2A13BDA06A__INCLUDED_)
#define AFX_NOEUD_H__2380181A_6EB6_4E21_9193_AD2A13BDA06A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxtempl.h>


// Classe noeud d'un arbre
class CNoeud  
{
public:
	CString m_DateExercice;
	CString m_ExPathFichier;
	CString m_Buffer;

	CNoeud();
	CNoeud(CNoeud *Mere);
	virtual ~CNoeud();

	CString m_Libelle;					//libelle de la branche
	CString m_Alias;					//alias de la branche
	CString m_Codif;					//code client pour cette elmt
	CString m_Marque;					//marque associée au produit (si c'est un produit)
	CString m_Famille;					//famille associé au produit (si c'est un produit)	
	CString m_Secteur;					//secteur associé au produit (si c'est un produit)
	CString m_PathFichier;				//path des fichiers à charger

	UINT m_CodeImage;					//stockage id image noeud et image aprés
	int m_Niveau;						//niveau de la branche

	// Les infos EDI pour Annonceur
	CString m_TypeTiers;			// type tiers (PO/centrale,BY/annonceur,DS/Regie)
	CString m_CodeTiers;			// ** obligatoire (max 35 alphanum)
	CString m_TypeCodeTiers;		// tjrs 107 (faut pas chercher à comprendre)
	CString m_NomTiers;				// ** obligatoire en fait nom annonceur ou centrale ou régie (max 35 alphanum)
	CString m_AdresseTiers1;		// 1ere partie adresse tiers ((max 35 alphanum)
	CString m_AdresseTiers2;		// 1ere partie adresse tiers ((max 35 alphanum)
	CString m_CodePostalTiers;		// (max 9 alphanum)
	CString m_VilleTiers;			// (max 35 alphanum)
	CString m_CodePaysTiers;		// (max 3 alphanum via un combo-box pays de correspondance)

	// Interlocuteur(s) tiers
	CString m_NomContactTiers;		// ** obligatoire (max 35 alphanum)
	CString m_PrenomContactTiers;	// ** obligatoire (max 35 alphanum)
	CString m_AdrMailContactTiers;	// ** obligatoire (max 35 alphanum)
	
	CString m_NoTelTiers;			// (max 35 alphanum)
	CString m_NoFaxTiers;			// (max 35 alphanum)

	CArray<CNoeud*,CNoeud*> m_pNoeud;	//tableau des feuilles pour cette branche 
	CNoeud * m_pRacine;					//pointeur sur racine mere
	CNoeud* m_pRacineMarque;			//pointeur sur racine marque associée (pour produit)
	
	// Chargement en mémoire de l'arborescence marque à partir de fichier
	bool AjoutArborescenceMarque(CPWFile &File,CNoeud *Noeud,CString &Txt,int &Niveau);
	bool AjoutArborescenceProduit(CPWFile &File,CNoeud *Noeud,CString &Txt,int &Niveau,CNoeud *ArbreMarque);
	bool AjoutArborescenceSecteur(CPWFile &File,CNoeud *Noeud,CString &Txt,int &Niveau);

	bool LierNoeud(CNoeud *Mere,CNoeud *Enfant);
	void FusillerSsNoeud();

	bool RechercheNoeudViaAlias(CNoeud *pArbre,CString Alias,CNoeud *&pNoeud);
	CNoeud* NoeudParent(CNoeud *Enfant);

	// Renvoie Date Exercice d'un noeud quelconque de la branche produit (groupe ou annonceur ou produit)
	//CString DateExercice(CNoeud *Noeud);	


};

#endif // !defined(AFX_NOEUD_H__2380181A_6EB6_4E21_9193_AD2A13BDA06A__INCLUDED_)
