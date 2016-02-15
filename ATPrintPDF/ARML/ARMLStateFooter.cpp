//================================
// fichier: ARMLStateFooter.cpp
//
// date: 07/09/2001
// auteur: JB
//================================

// on inclut les fichiers n�cessaires
#include "stdafx.h"
#include "ARMLStateFooter.h"
#include "ARMLStateBody.h"
#include "ARMLLineField.h"
#include "./../Composite/JFCDrawAdress.h"
#include "./../Composite/JFCDrawLogo.h"

//=======================================================================
// le constructeur:
//=======================================================================
StateFooter::StateFooter(Context * contexte) : State(contexte)
{

}

//=======================================================================
// la fonction pour g�rer l'�tat pied de page:
//=======================================================================
bool StateFooter::Manage()
{
	// la variable pour la ligne du fichier
	CString Ligne;
	// la variable pour les champs de la ligne
	CStringArray FieldArray;
	// la variable pour la commande courante
	CString Commande;
	CString LogoPath;
	long nbline; 

	LogoPath = "";
	nbline = 0;
	// on r�cup�re la ligne suivante 
	if (!m_Context->GetNextLine(Ligne)) return (GotoError());

	// on r�cup�re les champs de la ligne
	if (!m_Context->GetLineFields(FieldArray)) return (GotoError());

	// on r�cup�re la commande
	if (!LineField(FieldArray[0]).GetCommand(Commande)) return (GotoError());

	// on teste la commande
	//if (Commande != "LOGO") return (GotoError());
	if (Commande == "LOGO") {
		// on teste le nombre de param�tre
		if (FieldArray.GetSize() != 2) return (GotoError());

		// on r�cup�re le chemin du logo
//		CString LogoPath;
		if (!LineField(FieldArray[1]).GetText(LogoPath)) return (GotoError());

		// on lit la ligne suivante du fichier
		if (!m_Context->GetNextLine(Ligne)) return (GotoError());

		// on r�cup�re les param�tres de la ligne
		if (!m_Context->GetLineFields(FieldArray)) return (GotoError());

		// on r�cup�re la commande
		if (!LineField(FieldArray[0]).GetCommand(Commande)) return (GotoError());
	}

//	if (Commande != "ADRESSE") return (GotoError());
	if (Commande == "ADRESSE") {
		// on teste le nombre de param�tres
		if (FieldArray.GetSize() != 2) return (GotoError());

		// on r�cup�re le nombre de lignes
		if (!LineField(FieldArray[1]).GetInteger(nbline)) return (GotoError());

		// on place les coordonn�es en y
		m_Context->SetNumLine(m_Context->GetNbLinePage() - m_Context->GetNbLineFooter());
	}
	
		// on a suffisament d'�l�ments pour cr�er le logo
	CLogo logo;
	logo.m_StartPosY = m_Context->GetVertPos(m_Context->GetNumLine());
	logo.m_EndPosY = m_Context->GetVertPos(m_Context->GetNumLine() + m_Context->GetNbLineFooter() - nbline - 1);
	logo.m_StartPosX = m_Context->GetHorzPos(0);
	logo.m_EndPosX = m_Context->GetHorzPos(100);
	logo.m_LogoPath = LogoPath;

	// on fixe le logo dans le contexte
	m_Context->SetLogo(logo);

	// on d�place la ligne de dessin
	m_Context->SetNumLine(m_Context->GetNbLinePage() - nbline);


	CAdress adress;

	// on boucle sur les lignes
	long cptline = 0;
	while(cptline < nbline)
	{
		// on lit la ligne suivante
		if (!m_Context->GetNextLine(Ligne)) return (GotoError());
	
		// on r�cup�re les param�tres
		if (!m_Context->GetLineFields(FieldArray)) return (GotoError());
	
		// on teste la commande
		if (!LineField(FieldArray[0]).GetCommand(Commande)) return (GotoError());

		if (Commande == "FOND")
		{
			// on teste le nombre de param�tres
			if (FieldArray.GetSize() != 2) return (GotoError());

			// on r�cup�re le second param�tre
			long tagfond; if (!LineField(FieldArray[1]).GetInteger(tagfond)) return (GotoError());

			// on fixe le param�tre de fond
			if (tagfond == 1) adress.SetGrayed(TRUE);
		}
		if (Commande == "LIGNE")
		{
			// on teste le nombre de param�tres
			if (FieldArray.GetSize() != 2) return (GotoError());

			// on r�cup�re le second param�tre
			long nbfield; if (!LineField(FieldArray[1]).GetInteger(nbfield)) return (GotoError());

			// on boucle sur les champs
			long cptfield = 0;
			while (cptfield < nbfield)
			{
				long position = 0; char justification = 0; CString texte = ""; bool bold = false; CField field;

				// on r�cup�re la ligne suivante
				if (!m_Context->GetNextLine(Ligne)) return (GotoError());

				// on r�cup�re les param�tres
				if (!m_Context->GetLineFields(FieldArray)) return (GotoError());

				// on teste le premier param�tre
				if (LineField(FieldArray[0]).GetCommand(Commande))
				{
					if (Commande == "GRAS")
					{
						// on initialise le flag
						bold = true;
					}
					else return (GotoError());

					// on r�cup�re la ligne suivante
					if (!m_Context->GetNextLine(Ligne)) return (GotoError());

					// on r�cup�re les param�tres
					if (!m_Context->GetLineFields(FieldArray)) return (GotoError());
				}
				if (LineField(FieldArray[0]).GetTextPosition(justification, position))
				{
					// on teste ne nombre de champs
					if (FieldArray.GetSize() != 2) return (GotoError());
					
					// on r�cup�re le texte
					if (!LineField(FieldArray[1]).GetText(texte)) return (GotoError());
					
				}
				else return (GotoError());
				
				// on initialise le champ
				field.m_Bold = bold;
				field.m_Justification = justification;
				field.m_Texte = texte;
				field.m_PosY = m_Context->GetVertPos(m_Context->GetNumLine());
				if (position == 0)
				{
					switch(justification)
					{
					case 'C' :
						field.m_PosX = m_Context->GetHorzPos(50);
						break;
					case 'D':
						field.m_PosX = m_Context->GetHorzPos(100);
						break;
					case 'G':
						field.m_PosX = m_Context->GetHorzPos(0);
						break;
					}
				}
				else 
				{
					field.m_PosX = m_Context->GetHorzPos(position);
				}
				
				// on ajoute le champ au cadre
				adress.AddField(field);

				// on incr�mente le compteur de champ
				cptfield += 1;
			}
			// on incr�mente la position
			m_Context->SetNumLine(m_Context->GetNumLine() + 1);
			
			// on incr�mente le compteur de ligne
			cptline += 1;
		}
	}
	// on fixe l'adresse
	m_Context->SetAdress(adress);
	
	// on cr�e un �tat body
	StateBody * pStateBody = new StateBody(m_Context);

	// on lib�re l'�tat courant
	delete (this);

	// on g�re l'�tat body
	return (pStateBody->Manage());
}

//=========================================================================
// la fonction pour passer � l'�tat erreur:
//=========================================================================
bool StateFooter::GotoError()
{
	// on cr�e un �tat erreur
	StateError * pStateError = new StateError(m_Context);

	// on lib�re l'�tat actuel
	delete (this);

	// on g�re l'�tat suivant
	return (pStateError->Manage());
}

//=======================================================================
// le destructeur:
//=======================================================================
StateFooter::~StateFooter()
{

}
