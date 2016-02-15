//=============================
// JFCDrawComposite.cpp
//
// date: 02/09/2001
// auteur: JB
//=============================

// on inclut les fichiers nécessaires
#include "stdafx.h"
#include "JFCDrawComposite.h"

//===============================================================
// le constructeur:
//===============================================================
DrawComposite::DrawComposite()
{
	// on ne fait rien
}

//===============================================================
// le constructeur de recopie:
//===============================================================
DrawComposite::DrawComposite(const DrawComposite & source)
{
	// on invalide les liens
	m_Parent = m_Suivant = m_Precedent = 0;

	// on liste les enfants
	if (source.m_Child != 0)
	{
		// on clone le premier enfant et on fait le lien
		m_Child = source.m_Child->Clone(); m_Child->m_Parent = this;

		// on prend un pointeur sur l'enfant de la source
		DrawComponent * pSrcChild = source.m_Child;

		// on prend un pointeur sur l'enfant copie
		DrawComponent * pChild = m_Child;

		// on boucle sur les enfants
		while(pSrcChild->m_Suivant!=0)
		{
			// on crée le clone
			pChild->m_Suivant = pSrcChild->m_Suivant->Clone();
			
			// on fait les liens
			pChild->m_Suivant->m_Precedent = pChild;
			pChild->m_Suivant->m_Parent = this;

			// on passe à l'élément suivant
			pSrcChild = pSrcChild->m_Suivant;
			pChild = pChild->m_Suivant;
		}
	}
	else
	{
		m_Child = 0;
	}
}

//===============================================================
// la fonction pour enlever un élément:
//===============================================================
DrawComponent *  DrawComposite::AddChild(DrawComponent & composant)
{
	// on crée un clone du composant
	DrawComponent * pCourant = composant.Clone();
	pCourant->m_Parent = this;
	pCourant->m_Suivant = 0;

	// on teste si il y a déjà un enfant
	if (m_Child != 0)
	{
		// on se place sur le dernier enfant
		DrawComponent * pChild = m_Child; while(pChild->m_Suivant != 0 ) pChild = pChild->m_Suivant;

		// on met à jour les liens
		pChild->m_Suivant = pCourant;
		pCourant->m_Precedent = pChild->m_Suivant;
	}
	else
	{
		// on met à jour les liens
		m_Child = pCourant;
		pCourant->m_Precedent = 0;
	}
	// on renvoie le pointeur sur l'élément crée
	return (pCourant);
}

//===============================================================
// la fonction pour enlever un élément:
//===============================================================
void DrawComposite::RemoveChild(DrawComponent * composant)
{
	// on teste si le composant est le premier enfant
	if (composant == composant->m_Parent->m_Child)
	{
		// on met à jour le lien sur l'enfant
		composant->m_Parent->m_Child = composant->m_Suivant;
	}
	// on met à jour les liens sur les suivants et précédent
	if (composant->m_Precedent != 0) composant->m_Precedent->m_Suivant = composant->m_Suivant;
	if (composant->m_Suivant != 0) composant->m_Suivant->m_Precedent = composant->m_Precedent;
		
	// on libère l'élément
	delete (composant);
}

//===============================================================
// le destructeur:
//===============================================================
DrawComposite::~DrawComposite()
{
	// on teste si le composite a des enfants
	if (m_Child!=0)
	{
		// on parcours la liste des enfants
		DrawComponent * pChild = m_Child;
		
		// on boucle sur les éléments
		while (pChild)
		{
			DrawComponent * pItem = pChild; 

			// on libère l'élément
			pChild = pChild->m_Suivant;
			delete (pItem); 
		}
	}
}