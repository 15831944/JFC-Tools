//==============================
// fichier: ARMLStateFrame.cpp
//
// date: 06/09/2001
// auteur: JB
//==============================

// on inclut les fichiers n�cessaires
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
// la fonction pour g�rer l'�tat actuel:
//========================================================
bool StateFrame::Manage()
{
	// la variable pour contenir la ligne
	CString Ligne;
	// le tableau de param�tres
	CStringArray FieldArray;
	// la commande
	CString Commande;
	
	// on r�cup�re les champs de la ligne
	if (!m_Context->GetLineFields(FieldArray)) return (GotoError());

	// on teste le nombre de champs
	if (FieldArray.GetSize() != 5) return (GotoError());

	// on r�cup�re le param�tres de ligne de d�part
	long currentline; if (!LineField(FieldArray[1]).GetInteger(currentline)) return (GotoError());
	long numline = m_Context->GetNumLine() + currentline;

	// on r�cup�re le nombre de ligne du cadre
	long nbline; if (!LineField(FieldArray[2]).GetInteger(nbline)) return (GotoError());

	// on r�cup�re la position de d�part
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
	
	// on r�cup�re les param�tres
	if (!m_Context->GetLineFields(FieldArray)) return (GotoError());
	
	// on teste la commande
	if (!LineField(FieldArray[0]).GetCommand(Commande) || Commande != "TRAIT") return (GotoError());

	// on r�cup�re les param�tres de style de trait
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
			if (tagfond == 1) m_pFrame->SetGrayed(TRUE);
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

				// on incr�mente le compteur de champ
				cptfield += 1;
			}
			// on incr�mente la position
			numline += 1;
			
			// on incr�mente le compteur de ligne
			cptline += 1;
		}
	}
	// on met � jour la ligne dans le contexte
	m_Context->SetStartBodyLine(numline);
	
	// on lib�re l'�tat courant
	delete (this);

	// on renvoie le code de succ�s
	return (true);
}

//=========================================================================
// la fonction pour passer � l'�tat erreur:
//=========================================================================
bool StateFrame::GotoError()
{
	// on cr�e un �tat erreur
	StateError * pStateError = new StateError(m_Context);

	// on lib�re l'�tat actuel
	delete (this);

	// on g�re l'�tat suivant
	return (pStateError->Manage());
}

//========================================================
// le destructeur:
//========================================================
StateFrame::~StateFrame()
{
	// on ne fait rien
}