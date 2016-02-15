//================================
// fichier: ARMLStateHeader.cpp
// 
// date: 06/09/2001
// auteur: JB
//================================

// on inclut les fichiers n�cessaires
#include "stdafx.h"
#include "ARMLLineField.h"
#include "ARMLStateHeader.h"
#include "ARMLStateFrame.h"
#include "ARMLStateFooter.h"

//===================================================================
// le constructeur:
//===================================================================
StateHeader::StateHeader(Context * contexte) : State(contexte)
{

}

//===================================================================
// la fonction pour traiter l'�tat courant:
//===================================================================
bool StateHeader::Manage()
{
	// la variable pour la ligne du fichier
	CString Ligne;
	// la variable pour les champs de la ligne
	CStringArray FieldArray;
	// la variable pour la commande courante
	CString Commande;

	// on fixe le num�ro de ligne
	m_Context->SetNumLine(1);

	// on lit la ligne suivante du fichier
	if (!m_Context->GetNextLine(Ligne)) return (GotoError());
	// on r�cup�re la liste de champs
	if (!m_Context->GetLineFields(FieldArray)) return (GotoError());

	// on r�cup�re la commande
	if (!LineField(FieldArray[0]).GetCommand(Commande)) return (GotoError());

	// on boucle sur les cadres
	while(Commande == "CADRE")
	{
		// on cr�e une structure cadre
		CFrame frame;

		// on cr�e un �tat frame
		StateFrame * pStateFrame = new StateFrame(m_Context, &frame);

		// on g�re cet �tat
		if (pStateFrame->Manage())
		{
			// on ajoute le cadre au header
			m_Context->AddHeader(frame);

			// on fixe l'�tat courant
			m_Context->SetState(this);
		}
		else return (false);

		// on lit la ligne suivante du fichier
		if (!m_Context->GetNextLine(Ligne)) return (GotoError());
		
		// on r�cup�re la liste de champs
		if (!m_Context->GetLineFields(FieldArray)) return (GotoError());

		// on r�cup�re la commande
		if (!LineField(FieldArray[0]).GetCommand(Commande)) return (GotoError());

	}
	if (Commande != "BASPAGE") return (GotoError());
	else
	{
		// on teste le nombre de champs dans la ligne
		if (FieldArray.GetSize() != 2) return (GotoError());

		// on r�cup�re le nombre de ligne du pied de page
		long nblinefooter; if(!LineField(FieldArray[1]).GetInteger(nblinefooter)) return (GotoError());
		
		// on fixe le nombre de ligne du pied de page
		m_Context->SetNbLineFooter(nblinefooter);

		// on cr�e un �tat footer
		StateFooter * pStateFooter = new StateFooter(m_Context);

		// on lib�re l'�tat courant
		delete (this);

		// on g�re l'�tat footer
		return (pStateFooter->Manage());
		//return (true);
	}
}

//=========================================================================
// la fonction pour passer � l'�tat erreur:
//=========================================================================
bool StateHeader::GotoError()
{
	// on cr�e un �tat erreur
	StateError * pStateError = new StateError(m_Context);

	// on lib�re l'�tat actuel
	delete (this);

	// on g�re l'�tat suivant
	return (pStateError->Manage());
}

//===================================================================
// le destructeur:
//===================================================================
StateHeader::~StateHeader()
{
	
}