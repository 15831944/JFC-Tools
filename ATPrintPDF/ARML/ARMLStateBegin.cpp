//===============================
// fichier: ARMLStateBegin.cpp
//
// date: 06/09/2001
// auteur: JB
//===============================

// on inclut les fichiers nécessaires
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
// la fonction pour gérer l'état courant:
//=======================================================
bool StateBegin::Manage()
{
	// la ligne en cours
	CString Line;
	// le tableau de champs de ligne
	CStringArray FieldArray;
	// la commande
	CString Commande;
	
	// on lit la première ligne du fichier
	if (!m_Context->GetNextLine(Line)) return (GotoError());

	// on récupère les paramètres
	if (!m_Context->GetLineFields(FieldArray)) return (GotoError());

	// on teste le premier paramètre
	if (!LineField(FieldArray[0]).GetCommand(Commande)) return (GotoError());

	// on teste la commande
	if (Commande == "POLICE")
	{
		// on teste le nombre de champs
		if (FieldArray.GetSize() != 2) return (GotoError());

		// on récupère le second champ
		long idfont; if (!LineField(FieldArray[1]).GetInteger(idfont)) return (GotoError());

		// on teste l'identifiant
		switch(idfont) // à MODIFIER : 0,1,2,3
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

		// on récupère les paramètres
		if (!m_Context->GetLineFields(FieldArray)) return (GotoError());

		// on teste le premier paramètre
		if (!LineField(FieldArray[0]).GetCommand(Commande)) return (GotoError());
	}
	else
	{
		// on fixe la fonte par défaut
		m_Context->SetFont(0);
	}
	if (Commande == "HAUTPAGE")
	{
		// on crée un état header
		StateHeader * pStateHeader = new StateHeader(m_Context);

		// on teste le nombre de champs
		if (FieldArray.GetSize() != 2) return (GotoError());

		// on récupère le second champ
		long nblineheader; if (!LineField(FieldArray[1]).GetInteger(nblineheader)) return (GotoError());

		// on fixe le nombre de lignes de l'entete
		m_Context->SetNbLineHeader(nblineheader);

		// on libère l'état courant
		delete (this);

		// on gère l'état suivant
		return (pStateHeader->Manage());
	}
	// il n'y a pas de raison de se trouver ici
	return (GotoError());
}

//=========================================================================
// la fonction pour passer à l'état erreur:
//=========================================================================
bool StateBegin::GotoError()
{
	// on crée un état erreur
	StateError * pStateError = new StateError(m_Context);

	// on libère l'état actuel
	delete (this);

	// on gère l'état suivant
	return (pStateError->Manage());
}

//=======================================================
// le destructeur:
//=======================================================
StateBegin::~StateBegin()
{
	// on ne fait rien
}
