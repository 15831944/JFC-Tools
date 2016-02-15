//===============================
// fichier: ARMLStateBody.cpp
//
// date: 07/09/2001
// auteur: JB
//===============================

// on inclut les fichiers n�cessaires
#include "stdafx.h"
#include "ARMLStateBody.h"
#include "ARMLStateEnd.h"
#include "ARMLLineField.h"
#include "ARMLStateFrame.h"
#include "ARMLStateTable.h"
#include "./../Composite/JFCDrawBeginPage.h"
#include "./../Composite/JFCDrawEndPage.h"


//===========================================================
// le constructeur:
//===========================================================
StateBody::StateBody(Context * contexte) : State(contexte)
{

}

//===========================================================
// la fonction pour g�rer l'�tat "Corps du document":
//===========================================================
bool StateBody::Manage()
{
	// la variable pour la ligne du fichier
	CString Ligne;
	// la variable pour les champs de la ligne
	CStringArray FieldArray;
	// la variable pour la commande courante
	CString Commande = "";
	
	// on fixe la ligne courante
	m_Context->SetNumLine(m_Context->GetNbLineHeader());
	
	while (Commande != "FIN")
	{
		// on lit la ligne suivante dans le fichier
		if (!m_Context->GetNextLine(Ligne)) return (GotoError());
		
		// on r�cup�re les champs
		if (!m_Context->GetLineFields(FieldArray)) return (GotoError());

		// on r�cup�re la commande
		if (!LineField(FieldArray[0]).GetCommand(Commande)) return (GotoError());

		//on teste la commande
		if (Commande == "CADRE")
		{
			// on cr�e un cadre
			CFrame frame;

			// on cr�e un etat frame
			StateFrame * pStateFrame = new StateFrame(m_Context, &frame);

			m_Context->SetNumLine(m_Context->GetNbLineHeader());

			// on teste la gestion de l'�tat
			if (pStateFrame->Manage())
			{
				// on ajoute le cadre au tableau de cadre du corps
				m_Context->AddBody(frame);
			}
			else
			{
				// on passe � l'�tat erreur
				return (GotoError());
			}
			m_Context->SetNumLine(m_Context->GetStartBodyLine());

		}
		else if (Commande == "TAB")
		{
			// on cr�e un tableau
			CTable table;

			// on cr�e un �tat tableau
			StateTable * pStateTable = new StateTable(m_Context, &table);

			// on teste la gestion
			if (pStateTable->Manage())
			{
				// on ajoute le cadre au tableau de tableau du corps
				m_Context->AddBody(table);
			}
			else
			{
				// on passe � l'�tat erreur
				return (GotoError());
			}
		}
		else if (Commande == "SAUTDEPAGE")
		{
			// on ajoute la page dans le document
			m_Context->AddPage();

			m_Context->SetNumLine(m_Context->GetStartBodyLine());
		}
	}
	// on teste la commande
	if (Commande == "FIN")
	{
		// on ajoute la page dans le document
		m_Context->AddPage();

		// on cr�e un �tat fin
		StateEnd * pStateEnd = new StateEnd(m_Context);

		// on lib�re l'�tat courant
		delete (this);

		// on g�re l'�tat fin
		return (pStateEnd->Manage());
	}
	else
	{
		return (GotoError());	
	}

}

//=========================================================================
// la fonction pour passer � l'�tat erreur:
//=========================================================================
bool StateBody::GotoError()
{
	// on cr�e un �tat erreur
	StateError * pStateError = new StateError(m_Context);

	// on lib�re l'�tat actuel
	delete (this);

	// on g�re l'�tat suivant
	return (pStateError->Manage());
}

//===========================================================
// le destructeur:
//===========================================================
StateBody::~StateBody()
{
	
}