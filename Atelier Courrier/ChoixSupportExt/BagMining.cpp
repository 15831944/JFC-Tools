// BagMining.cpp: implementation of the CBagMining class.
//
//////////////////////////////////////////////////////////////////////

// on inclut les d�finitions n�cessaires
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
// Fonction de r�cup�ration document
JVoid CBagMining::SetDocument(JATPDocChoixSupport* pDoc, CDlgMapping *pDlg)
{
	// Passe le document pour cr�tion liste des points
	m_Liste.SetDocument(pDoc,pDlg);
}

//////////////////////////////////////////////////////////////////////
// Update points mapping
JBool CBagMining::UpdatePoints(JBool InitPoint)
{
	return m_Liste.Update(InitPoint); 
}

//////////////////////////////////////////////////////////////////////
// la fonction pour r�cup�rer le nombre d'�l�ment
//////////////////////////////////////////////////////////////////////

JInt32 CBagMining::GetCount()
{
	// on initialise le nombre d'�l�ment
	JInt32 taille = m_Liste.GetTaille();

	// on renvoie le nombre d'�l�ment
	return (taille);
}

//////////////////////////////////////////////////////////////////////
// la fonction pour tester la validit� de l'�l�ment
//////////////////////////////////////////////////////////////////////

JBool CBagMining::IsItemValid(JInt32 Index)
{
	// on renvoie l'indicateur de validit�
	return (true);
}

//////////////////////////////////////////////////////////////////////
// les fonctions pour r�cup�rer le libell� des grandeurs
//////////////////////////////////////////////////////////////////////

const JString CBagMining::GetLabelXAxe()
{
	// on initialise le libell� de la grandeur
	JString Libelle;

	// on r�cup�re le libell�
	//Libelle = "X";
	Libelle = m_Liste.GetLibAbscisse();

	// on renvoie le libell�
	return (Libelle);
}

const JString CBagMining::GetLabelYAxe()
{
	// on initialise le libell� de la grandeur
	JString Libelle;

	// on r�cup�re le libell�
	//Libelle = "Y";
	Libelle = m_Liste.GetLibOrdonnee();

	// on renvoie le libell�
	return (Libelle);
}

//////////////////////////////////////////////////////////////////////
// les fonctions pour manipuler la visibilit� de l'�l�ment
//////////////////////////////////////////////////////////////////////

JVoid CBagMining::SetShowItem(JInt32 Index, JBool Show) 
{
	m_Liste.SetShowElt(Index, Show);
}

JBool CBagMining::GetShowItem(JInt32 Index)
{
	// on renvoie la visibilit� de l'�l�ment
	return (m_Liste.GetShowElt(Index));
}

// Positionne visibilit� �lmt via son ident
JVoid CBagMining::SetShowItemById(JUnt32 IdElmt, JBool Show)
{
	m_Liste.SetShowEltById(IdElmt, Show);
}

//////////////////////////////////////////////////////////////////////
// la fonction pour r�cup�rer le libell� de l'�l�ment
//////////////////////////////////////////////////////////////////////
const JString CBagMining::GetLabelItem(JInt32 Index)
{
	// on initialise le libell� de l'�l�ment
	JString Libelle;

	// on r�cup�re le libell� de l'�l�ment
	// Libelle.Format("Element %d", indice);
	Libelle = m_Liste.GetLibPoint(Index);
	
	// on renvoie le libell� de l'�l�ment
	return (Libelle);
}

//////////////////////////////////////////////////////////////////////
// les fonctions pour manipuler la s�lection de l'�l�ment
//////////////////////////////////////////////////////////////////////
JVoid CBagMining::SetSelItem(JInt32 Index, JBool Selection)
{
	// Mise � jour s�lection des points
	m_Liste.SetSelElt(Index, Selection);

	// Mise � jour des supports s�lectionn�s
	// Voir A REMETTRE m_Liste.MajSelElt();
}

JBool CBagMining::GetSelItem(JInt32 Index)
{
	// on renvoie la s�lection de l'�l�ment
	return (m_Liste.GetSelElt(Index));
}

//////////////////////////////////////////////////////////////////////
// Mise � jour de la s�lection en cours
JVoid CBagMining::MajSelSet()
{
	m_Liste.MajSelElt();
}

//////////////////////////////////////////////////////////////////////
// Mise � jour libell�s en cours
JVoid CBagMining::MajLibelleSet()
{
	m_Liste.MajLibelleElt();
}


//////////////////////////////////////////////////////////////////////
// la fonction pour manipuler la visibilit� du libell�
//////////////////////////////////////////////////////////////////////
JVoid CBagMining::SetShowLabel(JInt32 Index, JBool Show)
{
	m_Liste.SetShowLib(Index, Show);
}

JBool CBagMining::GetShowLabel(JInt32 Index)
{
	// on renvoie la visibilit� du libell�
	return (m_Liste.GetShowLib(Index));
}

//////////////////////////////////////////////////////////////////////
// les fonctions pour r�cup�rer la position de l'�l�ment
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
// les fonctions pour manipuler le d�placement horizontal du libell�
//////////////////////////////////////////////////////////////////////
JVoid CBagMining::SetDxLabel(JInt32 Index, JFlt32 Dx)
{
	m_Liste.SetdXPosLib(Index, Dx);
}

JFlt32 CBagMining::GetDxLabel(JInt32 Index)
{
	// on renvoie le d�placement
	return ((float)m_Liste.GetdXPosLib(Index));
}
	
//////////////////////////////////////////////////////////////////////
// les fonctions pour manipuler le d�placement vertical du libell�
//////////////////////////////////////////////////////////////////////
JVoid  CBagMining::SetDyLabel(JInt32 Index, JFlt32 Dy)
{
	m_Liste.SetdYPosLib(Index, Dy);
}

JFlt32 CBagMining::GetDyLabel(JInt32 Index)
{
	// on renvoie le d�placement
	return ((float)m_Liste.GetdYPosLib(Index));
}

////////////////////////////////////////////////////////////////////////
// fonction pour positionner en X et Y le libell�
JVoid CBagMining::SetPosLibelle(JInt32 indice, JFlt32 dx, JFlt32 dy)
{
	m_Liste.SetPosLib(indice, dx, dy);
}

/*
////////////////////////////////////////////////////////////////////////
// fonction pour r�cup�rer position en X du libell�
JFlt64 CBagMining::GetPosXLibelle(JInt32 indice)
{
	return m_Liste.GetXPosLib(indice);
}

////////////////////////////////////////////////////////////////////////
// fonction pour r�cup�rer position en Y du libell�
JFlt64 CBagMining::GetPosYLibelle(JInt32 indice)
{
	return m_Liste.GetYPosLib(indice);
}
*/

//////////////////////////////////////////////////////////////////////
// la fonction pour manipuler l'appartenance a la courbe generale de degr� 2
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
// la fonction pour mettre � jour le zoom
//////////////////////////////////////////////////////////////////////

JVoid CBagMining::OnUpdateZoom(JInt32 indice)
{

}

//////////////////////////////////////////////////////////////////////
// la fonction pour propager les mises � jour
//////////////////////////////////////////////////////////////////////

JVoid CBagMining::OnUpdate(JInt32 Update)
{
	if (Update == 2 || Update == 1)
		// Mise � jour s�lection des points
		this->MajSelSet();

	else if (Update == 3)
		// Mise � jour libell� uniquement
		this->MajLibelleSet();
}

JVoid CBagMining::MajSelection()
{
	this->MajSelSet();
}

///////////////////////////////////////////////////////////////////////////////////////////
// Affichage ou pas de tous les libell�s points
JVoid CBagMining::SetShowAllLibelle(JBool etat)
{
	long taille = GetCount();

	// on parcourt tous les �l�ments
	for (long indice = 0; indice < taille; indice += 1)
	{
		// on v�rifie la validit� et la visibilit� de l'�l�ment
		if (IsItemValid(indice) && GetShowItem(indice))
		{
			// on montre le libell�
			SetShowLabel(indice, etat);
		}
	}
}


///////////////////////////////////////////////////////////////////////////////////////////
// r�cup�ration �tat affichage libell�s
JBool CBagMining::GetLibAfficher()
{
	return m_Liste.GetLibAfficher();
}

///////////////////////////////////////////////////////////////////////////////////////////
// positionne �tat d'affichage des libell�s
JVoid CBagMining::SetLibAfficher(JBool etat)
{
	m_Liste.SetLibAfficher(etat);
}

//////////////////////////////////////////////////////////////////////
// r�cup�re ident �lmt
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
// Affichage uniquement des points s�lectionn�s
JVoid CBagMining::ShowSelItem()
{
	m_Liste.ShowSelItem();
}

////////////////////////////////////////////////////////////////
// Affichage uniquement des points non s�lectionn�s
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

