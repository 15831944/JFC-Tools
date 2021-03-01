// BagMining.cpp: implementation of the CBagMining class.
//
//////////////////////////////////////////////////////////////////////

// on inclut les définitions nécessaires
#include "stdafx.h"
#include "BagMining.h"
#include "DlgMapping.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// le constructeur
//////////////////////////////////////////////////////////////////////

CBagMining::CBagMining()
{
}

//////////////////////////////////////////////////////////////////////
// Fonction de récupération document
JVoid CBagMining::SetDocument(JATPDocChoixSupport* pDoc, CDlgMapping *pDlg)
{
	// Passe le document pour crétion liste des points
	m_Liste.SetDocument(pDoc,pDlg);
}

//////////////////////////////////////////////////////////////////////
// Update points mapping
JBool CBagMining::UpdatePoints(JBool InitPoint)
{
	return m_Liste.Update(InitPoint); 
}

//////////////////////////////////////////////////////////////////////
// la fonction pour récupérer le nombre d'élément
//////////////////////////////////////////////////////////////////////

JInt32 CBagMining::GetCount()
{
	// on initialise le nombre d'élément
	JInt32 taille = m_Liste.GetTaille();

	// on renvoie le nombre d'élément
	return (taille);
}

//////////////////////////////////////////////////////////////////////
// la fonction pour tester la validité de l'élément
//////////////////////////////////////////////////////////////////////

JBool CBagMining::IsItemValid(JInt32 Index)
{
	// on renvoie l'indicateur de validité
	return (true);
}

//////////////////////////////////////////////////////////////////////
// les fonctions pour récupérer le libellé des grandeurs
//////////////////////////////////////////////////////////////////////

const JString CBagMining::GetLabelXAxe()
{
	// on initialise le libellé de la grandeur
	JString Libelle;

	// on récupère le libellé
	//Libelle = "X";
	Libelle = m_Liste.GetLibAbscisse();

	// on renvoie le libellé
	return (Libelle);
}

const JString CBagMining::GetLabelYAxe()
{
	// on initialise le libellé de la grandeur
	JString Libelle;

	// on récupère le libellé
	//Libelle = "Y";
	Libelle = m_Liste.GetLibOrdonnee();

	// on renvoie le libellé
	return (Libelle);
}

//////////////////////////////////////////////////////////////////////
// les fonctions pour manipuler la visibilité de l'élément
//////////////////////////////////////////////////////////////////////

JVoid CBagMining::SetShowItem(JInt32 Index, JBool Show) 
{
	m_Liste.SetShowElt(Index, Show);
}

JBool CBagMining::GetShowItem(JInt32 Index)
{
	// on renvoie la visibilité de l'élément
	return (m_Liste.GetShowElt(Index));
}

// Positionne visibilité élmt via son ident
JVoid CBagMining::SetShowItemById(JUnt32 IdElmt, JBool Show)
{
	m_Liste.SetShowEltById(IdElmt, Show);
}

//////////////////////////////////////////////////////////////////////
// la fonction pour récupérer le libellé de l'élément
//////////////////////////////////////////////////////////////////////
const JString CBagMining::GetLabelItem(JInt32 Index)
{
	// on initialise le libellé de l'élément
	JString Libelle;

	// on récupère le libellé de l'élément
	// Libelle.Format("Element %d", indice);
	Libelle = m_Liste.GetLibPoint(Index);
	
	// on renvoie le libellé de l'élément
	return (Libelle);
}

//////////////////////////////////////////////////////////////////////
// les fonctions pour manipuler la sélection de l'élément
//////////////////////////////////////////////////////////////////////
JVoid CBagMining::SetSelItem(JInt32 Index, JBool Selection)
{
	// Mise à jour sélection des points
	m_Liste.SetSelElt(Index, Selection);

	// Mise à jour des supports sélectionnés
	// Voir A REMETTRE m_Liste.MajSelElt();
}

JBool CBagMining::GetSelItem(JInt32 Index)
{
	// on renvoie la sélection de l'élément
	return (m_Liste.GetSelElt(Index));
}

//////////////////////////////////////////////////////////////////////
// Mise à jour de la sélection en cours
JVoid CBagMining::MajSelSet()
{
	m_Liste.MajSelElt();
}

//////////////////////////////////////////////////////////////////////
// Mise à jour libellés en cours
JVoid CBagMining::MajLibelleSet()
{
	m_Liste.MajLibelleElt();
}


//////////////////////////////////////////////////////////////////////
// la fonction pour manipuler la visibilité du libellé
//////////////////////////////////////////////////////////////////////
JVoid CBagMining::SetShowLabel(JInt32 Index, JBool Show)
{
	m_Liste.SetShowLib(Index, Show);
}

JBool CBagMining::GetShowLabel(JInt32 Index)
{
	// on renvoie la visibilité du libellé
	return (m_Liste.GetShowLib(Index));
}

//////////////////////////////////////////////////////////////////////
// les fonctions pour récupérer la position de l'élément
//////////////////////////////////////////////////////////////////////
JFlt64 CBagMining::GetPosXItem(JInt32 Index)
{
	return ((double)m_Liste.GetdXPosElt(Index));
}

JFlt64 CBagMining::GetPosYItem(JInt32 Index)
{
	return ((double)m_Liste.GetdYPosElt(Index));
}

//////////////////////////////////////////////////////////////////////
// les fonctions pour manipuler le déplacement horizontal du libellé
//////////////////////////////////////////////////////////////////////
JVoid CBagMining::SetDxLabel(JInt32 Index, JFlt32 Dx)
{
	m_Liste.SetdXPosLib(Index, Dx);
}

JFlt32 CBagMining::GetDxLabel(JInt32 Index)
{
	// on renvoie le déplacement
	return ((float)m_Liste.GetdXPosLib(Index));
}
	
//////////////////////////////////////////////////////////////////////
// les fonctions pour manipuler le déplacement vertical du libellé
//////////////////////////////////////////////////////////////////////
JVoid  CBagMining::SetDyLabel(JInt32 Index, JFlt32 Dy)
{
	m_Liste.SetdYPosLib(Index, Dy);
}

JFlt32 CBagMining::GetDyLabel(JInt32 Index)
{
	// on renvoie le déplacement
	return ((float)m_Liste.GetdYPosLib(Index));
}

////////////////////////////////////////////////////////////////////////
// fonction pour positionner en X et Y le libellé
JVoid CBagMining::SetPosLibelle(JInt32 indice, JFlt32 dx, JFlt32 dy)
{
	m_Liste.SetPosLib(indice, dx, dy);
}

/*
////////////////////////////////////////////////////////////////////////
// fonction pour récupérer position en X du libellé
JFlt64 CBagMining::GetPosXLibelle(JInt32 indice)
{
	return m_Liste.GetXPosLib(indice);
}

////////////////////////////////////////////////////////////////////////
// fonction pour récupérer position en Y du libellé
JFlt64 CBagMining::GetPosYLibelle(JInt32 indice)
{
	return m_Liste.GetYPosLib(indice);
}
*/

//////////////////////////////////////////////////////////////////////
// la fonction pour manipuler l'appartenance a la courbe generale de degré 2
//////////////////////////////////////////////////////////////////////
JVoid CBagMining::SetIsCourbeElt(JInt32 indice, JBool etat)
{
	m_Liste.SetIsCourbeElt(indice, etat);
}

JBool CBagMining::GetIsCourbeElt(JInt32 indice)
{
	bool etat = m_Liste.GetIsCourbeElt(indice);
	return (etat);
}

//////////////////////////////////////////////////////////////////////
// la fonction pour mettre à jour le zoom
//////////////////////////////////////////////////////////////////////

JVoid CBagMining::OnUpdateZoom(JInt32 indice)
{

}

//////////////////////////////////////////////////////////////////////
// la fonction pour propager les mises à jour
//////////////////////////////////////////////////////////////////////

JVoid CBagMining::OnUpdate(JInt32 Update)
{
	if (Update == 2 || Update == 1)
		// Mise à jour sélection des points
		this->MajSelSet();

	else if (Update == 3)
		// Mise à jour libellé uniquement
		this->MajLibelleSet();
}

JVoid CBagMining::MajSelection()
{
	this->MajSelSet();
}

///////////////////////////////////////////////////////////////////////////////////////////
// Affichage ou pas de tous les libellés points
JVoid CBagMining::SetShowAllLibelle(JBool etat)
{
	long taille = GetCount();

	// on parcourt tous les éléments
	for (long indice = 0; indice < taille; indice += 1)
	{
		// on vérifie la validité et la visibilité de l'élément
		if (IsItemValid(indice) && GetShowItem(indice))
		{
			// on montre le libellé
			SetShowLabel(indice, etat);
		}
	}
}


///////////////////////////////////////////////////////////////////////////////////////////
// récupération état affichage libellés
JBool CBagMining::GetLibAfficher()
{
	return m_Liste.GetLibAfficher();
}

///////////////////////////////////////////////////////////////////////////////////////////
// positionne état d'affichage des libellés
JVoid CBagMining::SetLibAfficher(JBool etat)
{
	m_Liste.SetLibAfficher(etat);
}

//////////////////////////////////////////////////////////////////////
// récupére ident élmt
JUnt32 CBagMining::GetIdent(JInt32 indice)
{
	return m_Liste.GetIdent(indice);
}

//////////////////////////////////////////////////////////////////////
// les fonctions pour corriger la position des axes
JFlt64 CBagMining::GetPosXAxe(JFlt64 XAxePos)
{
	// Ici pas de changement X Axe
	return XAxePos;
}

JFlt64 CBagMining::GetPosYAxe(JFlt64 YAxePos)
{
	// Ici pas de changement Y Axe
	return YAxePos;
}

////////////////////////////////////////////////////////////////
// Affichage uniquement des points sélectionnés
JVoid CBagMining::ShowSelItem()
{
	m_Liste.ShowSelItem();
}

////////////////////////////////////////////////////////////////
// Affichage uniquement des points non sélectionnés
JVoid CBagMining::HideSelItem()
{
	m_Liste.HideSelItem();
}

////////////////////////////////////////////////////////////////
// Affichage de tous les points
JVoid CBagMining::ShowAllItem()
{
	m_Liste.ShowAllItem();
}

//////////////////////////////////////////////////////////////////////
// le destructeur
//////////////////////////////////////////////////////////////////////

CBagMining::~CBagMining()
{
	// on ne fait rien
}

