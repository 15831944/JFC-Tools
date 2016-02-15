//===============================
// fichier: ARMLStateBegin.cpp
//
// date: 06/09/2001
// auteur: JB
//===============================

// on inclut les fichiers n�cessaires
#include "stdafx.h"
#include "ARMLLineField.h"
#include "ARMLStateBegin.h"
#include "ARMLStateHeader.h"
#include "./../Composite/JFCDrawHeader.h"
#include "./../Composite/JFCDrawSetFont.h"

//=======================================================
// le constructeur:
//=======================================================
StateBegin::StateBegin(Context * contexte) : State(contexte)
{
	// on ne fait rien
	
}

//=======================================================
// la fonction pour g�rer l'�tat courant:
//=======================================================
bool StateBegin::Manage()
{
	// la ligne en cours
	CString Line;
	// le tableau de champs de ligne
	CStringArray FieldArray;
	// la commande
	CString Commande;
	
	// on lit la premi�re ligne du fichier
	if (!m_Context->GetNextLine(Line)) return (GotoError());

	// on r�cup�re les param�tres
	if (!m_Context->GetLineFields(FieldArray)) return (GotoError());

	// on teste le premier param�tre
	if (!LineField(FieldArray[0]).GetCommand(Commande)) return (GotoError());

	// on teste la commande
	if (Commande == "POLICE")
	{
		// on teste le nombre de champs
		if (FieldArray.GetSize() != 2) return (GotoError());

		// on r�cup�re le second champ
		long idfont; if (!LineField(FieldArray[1]).GetInteger(idfont)) return (GotoError());

		// on teste l'identifiant
		switch(idfont) // � MODIFIER : 0,1,2,3
		{
		case 0:	// Mode portrait en 80c
			m_Context->SetFont(0);
			break;
		case 1:		// Mode portrait en 100c
			m_Context->SetFont(0);
			break;
		case 2:		// Mode paysage en 160c , case 3: en 240
			m_Context->SetFont(0);
			break;
		default:
			return (GotoError());
		}
		// on lit la ligne suivante dans le fichier
		if (!m_Context->GetNextLine(Line)) return (GotoError());

		// on r�cup�re les param�tres
		if (!m_Context->GetLineFields(FieldArray)) return (GotoError());

		// on teste le premier param�tre
		if (!LineField(FieldArray[0]).GetCommand(Commande)) return (GotoError());
	}
	else
	{
		// on fixe la fonte par d�faut
		m_Context->SetFont(0);
	}
	if (Commande == "HAUTPAGE")
	{
		// on cr�e un �tat header
		StateHeader * pStateHeader = new StateHeader(m_Context);

		// on teste le nombre de champs
		if (FieldArray.GetSize() != 2) return (GotoError());

		// on r�cup�re le second champ
		long nblineheader; if (!LineField(FieldArray[1]).GetInteger(nblineheader)) return (GotoError());

		// on fixe le nombre de lignes de l'entete
		m_Context->SetNbLineHeader(nblineheader);

		// on lib�re l'�tat courant
		delete (this);

		// on g�re l'�tat suivant
		return (pStateHeader->Manage());
	}
	// il n'y a pas de raison de se trouver ici
	return (GotoError());
}

//=========================================================================
// la fonction pour passer � l'�tat erreur:
//=========================================================================
bool StateBegin::GotoError()
{
	// on cr�e un �tat erreur
	StateError * pStateError = new StateError(m_Context);

	// on lib�re l'�tat actuel
	delete (this);

	// on g�re l'�tat suivant
	return (pStateError->Manage());
}

//=======================================================
// le destructeur:
//=======================================================
StateBegin::~StateBegin()
{
	// on ne fait rien
}
