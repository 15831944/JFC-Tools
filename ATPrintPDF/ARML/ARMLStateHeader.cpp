//================================
// fichier: ARMLStateHeader.cpp
// 
// date: 06/09/2001
// auteur: JB
//================================

// on inclut les fichiers nécessaires
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
// la fonction pour traiter l'état courant:
//===================================================================
bool StateHeader::Manage()
{
	// la variable pour la ligne du fichier
	CString Ligne;
	// la variable pour les champs de la ligne
	CStringArray FieldArray;
	// la variable pour la commande courante
	CString Commande;

	// on fixe le numéro de ligne
	m_Context->SetNumLine(1);

	// on lit la ligne suivante du fichier
	if (!m_Context->GetNextLine(Ligne)) return (GotoError());
	// on récupère la liste de champs
	if (!m_Context->GetLineFields(FieldArray)) return (GotoError());

	// on récupère la commande
	if (!LineField(FieldArray[0]).GetCommand(Commande)) return (GotoError());

	// on boucle sur les cadres
	while(Commande == "CADRE")
	{
		// on crée une structure cadre
		CFrame frame;

		// on crée un état frame
		StateFrame * pStateFrame = new StateFrame(m_Context, &frame);

		// on gère cet état
		if (pStateFrame->Manage())
		{
			// on ajoute le cadre au header
			m_Context->AddHeader(frame);

			// on fixe l'état courant
			m_Context->SetState(this);
		}
		else return (false);

		// on lit la ligne suivante du fichier
		if (!m_Context->GetNextLine(Ligne)) return (GotoError());
		
		// on récupère la liste de champs
		if (!m_Context->GetLineFields(FieldArray)) return (GotoError());

		// on récupère la commande
		if (!LineField(FieldArray[0]).GetCommand(Commande)) return (GotoError());

	}
	if (Commande != "BASPAGE") return (GotoError());
	else
	{
		// on teste le nombre de champs dans la ligne
		if (FieldArray.GetSize() != 2) return (GotoError());

		// on récupère le nombre de ligne du pied de page
		long nblinefooter; if(!LineField(FieldArray[1]).GetInteger(nblinefooter)) return (GotoError());
		
		// on fixe le nombre de ligne du pied de page
		m_Context->SetNbLineFooter(nblinefooter);

		// on crée un état footer
		StateFooter * pStateFooter = new StateFooter(m_Context);

		// on libère l'état courant
		delete (this);

		// on gère l'état footer
		return (pStateFooter->Manage());
		//return (true);
	}
}

//=========================================================================
// la fonction pour passer à l'état erreur:
//=========================================================================
bool StateHeader::GotoError()
{
	// on crée un état erreur
	StateError * pStateError = new StateError(m_Context);

	// on libère l'état actuel
	delete (this);

	// on gère l'état suivant
	return (pStateError->Manage());
}

//===================================================================
// le destructeur:
//===================================================================
StateHeader::~StateHeader()
{
	
}