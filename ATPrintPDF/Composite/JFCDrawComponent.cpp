//===================================
// fichier: JFCDrawComponent.cpp
//
// date: 02/09/2001
// auteur: JB
//===================================

// on inclut les fichiers nécessaires
#include "stdafx.h"
#include "JFCDrawComponent.h"

//=============================================================
// le constructeur:
//=============================================================
DrawComponent::DrawComponent()
{
	// on initialise les liens
	m_Parent = 0;
	m_Child = 0;
	m_Precedent = 0;
	m_Suivant = 0;
}

//=============================================================
// le constructeur de recopie:
//=============================================================
DrawComponent::DrawComponent(const DrawComponent & source)
{
	// on recopie les paramètres de la source
	m_Parent = source.m_Parent;
	m_Child = source.m_Child;
	m_Precedent = source.m_Precedent;
	m_Suivant = source.m_Suivant;
}

//=============================================================
// les fonctions pour se déplacer dans le composite:
//=============================================================
DrawComponent * DrawComponent::GetRoot()
{
	// on remonte l'arborescence au maximum
	DrawComponent * pComponent = GetParent();
	DrawComponent * pRoot;
	if (pComponent == 0) pRoot = this;
	else
	{
		while(pComponent) 
		{
			// on met à jour le root
			pRoot = pComponent;

			// on recupère le parent
			pComponent = GetParent();
		}
	}
	// on renvoie le curseur sur la racine
	return (pRoot);
}

DrawComponent * DrawComponent::GetParent()
{
	// on renvoie le parent
	return (m_Parent);
}


DrawComponent * DrawComponent::GetChild()
{
	// on renvoie le curseur sur l'enfant
	return (m_Child);
}

DrawComponent * DrawComponent::GetNext()
{
	// on renvoie le curseur sur le composant suivant
	return (m_Suivant);
}

DrawComponent * DrawComponent::GetPrev()
{
	// on renvoie le curseur sur le composant précedent
	return (m_Precedent);
}

//=============================================================
// la fonction d'ajout d'un composant enfant:
//=============================================================
DrawComponent *  DrawComponent::AddChild(DrawComponent & composant)
{
	// on renvoie un pointeur null
	return (0);
}

//=============================================================
// la fonction de suppression d'un composant enfant:
//=============================================================
void DrawComponent::RemoveChild(DrawComponent * composant)
{
	// cette fonction n'est pas implémentée au niveau composant
}

/*
//=============================================================
// la fonction de clonage:
//=============================================================
DrawComponent * DrawComponent::Clone() const
{
	// on renvoie l'adresse d'une copie
	return (&(new DrawComponent(this));
}
*/
//=============================================================
// le destructeur:
//=============================================================
DrawComponent::~DrawComponent()
{
	// on ne fait rien
}