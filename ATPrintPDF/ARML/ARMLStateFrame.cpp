//==============================
// fichier: ARMLStateFrame.cpp
//
// date: 06/09/2001
// auteur: JB
//==============================

// on inclut les fichiers nécessaires
#include "stdafx.h"
#include "ARMLLineField.h"
#include "ARMLStateFrame.h"


//========================================================
// le constructeur:
//========================================================
StateFrame::StateFrame(Context * contexte, CFrame * pframe) : State(contexte)
{
	// on fixe le pointeur sur le cadre
	m_pFrame = pframe;
}

//========================================================
// la fonction pour gérer l'état actuel:
//========================================================
bool StateFrame::Manage()
{
	// la variable pour contenir la ligne
	CString Ligne;
	// le tableau de paramètres
	CStringArray FieldArray;
	// la commande
	CString Commande;
	
	// on récupère les champs de la ligne
	if (!m_Context->GetLineFields(FieldArray)) return (GotoError());

	// on teste le nombre de champs
	if (FieldArray.GetSize() != 5) return (GotoError());

	// on récupère le paramètres de ligne de départ
	long currentline; if (!LineField(FieldArray[1]).GetInteger(currentline)) return (GotoError());
	long numline = m_Context->GetNumLine() + currentline;

	// on récupère le nombre de ligne du cadre
	long nbline; if (!LineField(FieldArray[2]).GetInteger(nbline)) return (GotoError());

	// on récupère la position de départ
	long startposx; if (!LineField(FieldArray[3]).GetInteger(startposx)) return (GotoError());
	long endposx;   if (!LineField(FieldArray[4]).GetInteger(endposx)) return (GotoError());

	// on fixe les position horizontale du cadre
	m_pFrame->SetStartPosX(m_Context->GetHorzPos(startposx));
	m_pFrame->SetEndPosX(m_Context->GetHorzPos(endposx));
	
	// on fixe les positions verticales du cadre
	m_pFrame->SetStartPosY(m_Context->GetVertPos(numline));
	m_pFrame->SetEndPosY(m_Context->GetVertPos(numline+nbline-1));

	// on lit la ligne suivante
	if (!m_Context->GetNextLine(Ligne)) return (GotoError());
	
	// on récupère les paramètres
	if (!m_Context->GetLineFields(FieldArray)) return (GotoError());
	
	// on teste la commande
	if (!LineField(FieldArray[0]).GetCommand(Commande) || Commande != "TRAIT") return (GotoError());

	// on récupère les paramètres de style de trait
	if (!LineField(FieldArray[1]).GetInteger(m_pFrame->m_StyleTG)) return (GotoError());
	if (!LineField(FieldArray[2]).GetInteger(m_pFrame->m_StyleTH)) return (GotoError());
	if (m_pFrame->m_StyleTH > 0) { numline += 1; nbline -=1;}
	if (!LineField(FieldArray[3]).GetInteger(m_pFrame->m_StyleTD)) return (GotoError());
	if (!LineField(FieldArray[4]).GetInteger(m_pFrame->m_StyleTB)) return (GotoError());
	if (m_pFrame->m_StyleTH > 0) nbline -=1;

	// on boucle sur les lignes
	long cptline = 0;
	while(cptline < nbline)
	{
		// on lit la ligne suivante
		if (!m_Context->GetNextLine(Ligne)) return (GotoError());
	
		// on récupère les paramètres
		if (!m_Context->GetLineFields(FieldArray)) return (GotoError());
	
		// on teste la commande
		if (!LineField(FieldArray[0]).GetCommand(Commande)) return (GotoError());

		if (Commande == "FOND")
		{
			// on teste le nombre de paramètres
			if (FieldArray.GetSize() != 2) return (GotoError());

			// on récupère le second paramètre
			long tagfond; if (!LineField(FieldArray[1]).GetInteger(tagfond)) return (GotoError());

			// on fixe le paramètre de fond
			if (tagfond == 1) m_pFrame->SetGrayed(TRUE);
		}
		if (Commande == "LIGNE")
		{
			// on teste le nombre de paramètres
			if (FieldArray.GetSize() != 2) return (GotoError());

			// on récupère le second paramètre
			long nbfield; if (!LineField(FieldArray[1]).GetInteger(nbfield)) return (GotoError());

			// on boucle sur les champs
			long cptfield = 0;
			while (cptfield < nbfield)
			{
				long position = 0; char justification = 0; CString texte = ""; bool bold = false; CField field;

				// on récupère la ligne suivante
				if (!m_Context->GetNextLine(Ligne)) return (GotoError());

				// on récupère les paramètres
				if (!m_Context->GetLineFields(FieldArray)) return (GotoError());

				// on teste le premier paramètre
				if (LineField(FieldArray[0]).GetCommand(Commande))
				{
					if (Commande == "GRAS")
					{
						// on initialise le flag
						bold = true;
					}
					else return (GotoError());

					// on récupère la ligne suivante
					if (!m_Context->GetNextLine(Ligne)) return (GotoError());
					// on récupère les paramètres
					if (!m_Context->GetLineFields(FieldArray)) return (GotoError());
				}
				if (LineField(FieldArray[0]).GetTextPosition(justification, position))
				{
					// on teste ne nombre de champs
					if (FieldArray.GetSize() != 2) return (GotoError());
					
					// on récupère le texte
					if (!LineField(FieldArray[1]).GetText(texte)) return (GotoError());
				}
				else 
				{
					FieldArray[0].Remove(' ');
					if (FieldArray[0] == "\"\"")
					{
						justification = 'C';
						position = 0;
						texte = " ";
					}
					else return (GotoError());
				}
				
				// on initialise le champ
				field.m_Bold = bold;
				field.m_Justification = justification;
				field.m_Texte = texte;
				field.m_PosY = m_Context->GetVertPos(numline);
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
				m_pFrame->AddField(field);

				// on incrémente le compteur de champ
				cptfield += 1;
			}
			// on incrémente la position
			numline += 1;
			
			// on incrémente le compteur de ligne
			cptline += 1;
		}
	}
	// on met à jour la ligne dans le contexte
	m_Context->SetStartBodyLine(numline);
	
	// on libère l'état courant
	delete (this);

	// on renvoie le code de succés
	return (true);
}

//=========================================================================
// la fonction pour passer à l'état erreur:
//=========================================================================
bool StateFrame::GotoError()
{
	// on crée un état erreur
	StateError * pStateError = new StateError(m_Context);

	// on libère l'état actuel
	delete (this);

	// on gère l'état suivant
	return (pStateError->Manage());
}

//========================================================
// le destructeur:
//========================================================
StateFrame::~StateFrame()
{
	// on ne fait rien
}