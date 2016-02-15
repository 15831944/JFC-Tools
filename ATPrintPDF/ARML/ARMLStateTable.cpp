//===============================
// fichier: ARMLStateTable.cpp
// 
// date: 07/09/2001
// auteur: JB
//===============================

// on inclut les fichiers nécessaires
#include "stdafx.h"
#include "ARMLLineField.h"
#include "ARMLStateTable.h"

//=======================================================================
// le constructeur:
//=======================================================================
StateTable::StateTable(Context * contexte, CTable * ptable) : State(contexte)
{
	// on fixe le pointeur sur la table
	m_pTable = ptable;
}

//=======================================================================
// la fonction pour gérer l'état table:
//=======================================================================
bool StateTable::Manage()
{
	// la variable pour la ligne du fichier
	CString Ligne;
	// la variable pour les champs de la ligne
	CStringArray FieldArray;
	// la variable pour la commande courante
	CString Commande;
	// le tableau de traits horizontaux
	CArray<CHStroke, CHStroke&> ArrayHeaderHStroke;ArrayHeaderHStroke.SetSize(0, 10);
	// le tableau de cellules
	CArray<CTabCell, CTabCell&> ArrayHeaderTabCell;ArrayHeaderTabCell.SetSize(0, 100);
	// le tableau de paves
	CArray<CTabPave, CTabPave&> ArrayHeaderTabPave;ArrayHeaderTabPave.SetSize(0, 100);
	// le tableau de paves de la page suivante
	CArray<CTabPave, CTabPave&> ArrayHeaderTabPaveNext;ArrayHeaderTabPaveNext.SetSize(0, 100);
	// le nombre de ligne de l'entête du tableau
	long nblinehead = 0;
	// le flag pour savoir si une ligne a été dessinée
	bool linedrawed = false;

	// on récupère le numéro de la ligne courante
	long numline = m_Context->GetNumLine();
	long startheadline = numline;
	long startlinePagePave = 0; // N° ligne du 1er Pavé dans la page
	long endlinePagePave = 0;	// N° ligne du dernière ligne
	long NbLineWritePave = 0;	// Nb de ligne du pavé déjà écrire
	long HeightPagePave = 0;

	
	// on récupère les paramètres de la ligne
	if (!m_Context->GetLineFields(FieldArray)) return (GotoError());

	// on teste le nombre de champs
	if (FieldArray.GetSize() != 2) return (GotoError());

	// on récupère le second paramètre
	long nbcol = 0;
	if (!LineField(FieldArray[1]).GetInteger(nbcol)) return (GotoError());
	
	// on alloue les tableaux
	m_ArrayStrokeStyle.SetSize(nbcol+1);
	m_ArrayJustification.SetSize(nbcol);
	m_ArrayColWidth.SetSize(nbcol);

	// on lit la ligne suivante du fichier
	if (!m_Context->GetNextLine(Ligne)) return (GotoError());

	// on récupère les paramètres
	if (!m_Context->GetLineFields(FieldArray)) return (GotoError());

	// on teste le nombre de paramètres
	if (FieldArray.GetSize() != (nbcol+2)) return (GotoError());

	// on récupère la commande, on la teste
	if (!LineField(FieldArray[0]).GetCommand(Commande)) return (GotoError());
	if (Commande != "TRAIT") return (GotoError());

	// on boucle sur les styles de traits
	for(long i = 1; i <= (nbcol+1); i++)
	{
		// on récupère les styles de trait
		if (!LineField(FieldArray[i]).GetInteger(m_ArrayStrokeStyle[i-1])) return (GotoError());
	}

	// on lit la ligne suivante du fichier
	if(!m_Context->GetNextLine(Ligne)) return (GotoError());

	// on récupère les champs
	if (!m_Context->GetLineFields(FieldArray)) return (GotoError());

	// on boucle sur les champs
	for(long i=0; i < nbcol; i++)
	{
		// on récupère les champs
		if(!LineField(FieldArray[i]).GetTextPosition(m_ArrayJustification[i], m_ArrayColWidth[i])) return (GotoError());
	}

	// on lit la ligne suivante du fichier
	if (!m_Context->GetNextLine(Ligne)) return (GotoError());

	// on récupère les paramètres
	if (!m_Context->GetLineFields(FieldArray)) return (GotoError());

	// on teste la commande
	if(!LineField(FieldArray[0]).GetCommand(Commande)) return (GotoError()); 		

	// on teste la présence d'un header
	if (Commande == "LIGNELIBCOL")
	{
		// on teste le nombre de paramètres
		if (FieldArray.GetSize() != 2) return(GotoError());

		// on récupère le nombre de lignes de l'entête
		if(!LineField(FieldArray[1]).GetInteger(nblinehead)) return (GotoError());

		// on boucle sur les lignes d'entete
		long cptlinehead = 0;
		while (cptlinehead < nblinehead)
		{
			// on lit la ligne suivante du fichier
			if (!m_Context->GetNextLine(Ligne)) return (GotoError());

			// on récupère les champs de cette ligne
			if (!m_Context->GetLineFields(FieldArray)) return (GotoError());

			// on récupère la commande
			if (LineField(FieldArray[0]).GetCommand(Commande))
			{
				// on teste la commande
				if (Commande == "TRAITHOR")
				{
					// on teste le nombre de paramètres
					if (FieldArray.GetSize() != 4) return (GotoError());

					// on récupère le type de trait
					long strokestyle; if (!LineField(FieldArray[1]).GetInteger(strokestyle)) return (GotoError());

					// on récupère la colonne de départ
					long startcol; if (!LineField(FieldArray[2]).GetInteger(startcol)) return (GotoError());

					// on récupère la colonne d'arrivée
					long endcol; if (!LineField(FieldArray[3]).GetInteger(endcol)) return (GotoError());

					//  on crée un objet CHStroke
					CHStroke hStroke;

					// on remplit ses paramètres
					hStroke.m_StrokeStyle = strokestyle;
					hStroke.m_PosY = m_Context->GetVertPos(numline);
					hStroke.m_StartPosX = this->GetStartPosX(startcol);
					hStroke.m_EndPosX = this->GetEndPosX(endcol);

					// on ajoute le trait au tableau
					ArrayHeaderHStroke.Add(hStroke);

					// on incrémente le compteur de ligne
					cptlinehead += 1;
					numline += 1;
				}
				else if (Commande == "TRAIT0HOR")
				{
					// on teste le nombre de paramètres
					if (FieldArray.GetSize() != 4) return (GotoError());

					// on récupère le type de trait
					long strokestyle; if (!LineField(FieldArray[1]).GetInteger(strokestyle)) return (GotoError());

					// on récupère la colonne de départ
					long startcol; if (!LineField(FieldArray[2]).GetInteger(startcol)) return (GotoError());

					// on récupère la colonne d'arrivée
					long endcol; if (!LineField(FieldArray[3]).GetInteger(endcol)) return (GotoError());

					
					//  on crée un objet CHStroke
					CHStroke hStroke;

					// on remplit ses paramètres
					hStroke.m_StrokeStyle = strokestyle;
					hStroke.m_PosY = m_Context->GetVertPos(numline);
					hStroke.m_StartPosX = this->GetStartPosX(startcol);
					hStroke.m_EndPosX = this->GetEndPosX(endcol);

					// on ajoute le trait au tableau
					ArrayHeaderHStroke.Add(hStroke);
				}
				else if (Commande == "LIGNE")
				{
					// on teste le nombre de paramètres
					if (FieldArray.GetSize() != 2) return (GotoError());

					// on récupère le nombre de champs
					long nbfield; if(!LineField(FieldArray[1]).GetInteger(nbfield)) return (GotoError());

					
					// on initialise le compteur de champs
					long cptfield = 0;

					// on boucle sur les champs
					while (cptfield < nbfield)
					{
						// on récupère la ligne suivante du fichier
						if(!m_Context->GetNextLine(Ligne)) return (GotoError());

						// on récupère les champs de la ligne
						if (!m_Context->GetLineFields(FieldArray)) return (GotoError());

						// on teste le nombre de champs
						if (FieldArray.GetSize() == 3 && !LineField(FieldArray[0]).IsText())
						{
							// on récupère la colonne de début
							long startcol; if (!LineField(FieldArray[0]).GetInteger(startcol)) return (GotoError());

							// on récupère la colonne de fin
							long endcol; if (!LineField(FieldArray[1]).GetInteger(endcol)) return (GotoError());

							// on récupère le texte
							CString texte; if (!LineField(FieldArray[2]).GetText(texte)) return (GotoError());

							// on crée une cellule
							CTabCell tabcell;
							tabcell.m_BkColor = 2;
							tabcell.m_Height = m_Context->GetLineHeight();
//							if (startcol == endcol) tabcell.m_Justification = m_ArrayJustification[startcol-1];
//							else tabcell.m_Justification = 'C';
							tabcell.m_Justification = 'C';
							tabcell.m_PosX = this->GetStartPosX(startcol);
							tabcell.m_Width = this->GetEndPosX(endcol) - tabcell.m_PosX;
							tabcell.m_PosY = m_Context->GetVertPos(numline);
							tabcell.m_StyleTD = this->m_ArrayStrokeStyle[endcol];
							tabcell.m_StyleTG = this->m_ArrayStrokeStyle[startcol-1];
							tabcell.m_Texte = texte;

							// on ajoute la cellule au tableau
							ArrayHeaderTabCell.Add(tabcell);
							
							// on incrémente le compteur de champs
							cptfield += 1;
						}
						else if (FieldArray.GetSize() == nbfield && LineField(FieldArray[0]).IsText())
						{
							// on boucle sur tous les champs
							for(long i = 0, max = FieldArray.GetSize(); i < max ; i++)
							{
								// on récupère les colonnes de début et de fin
								long startcol = i+1; long endcol = i+1;

								// on récupère le texte
								CString texte; if (!LineField(FieldArray[i]).GetText(texte)) return (GotoError());

								// on crée une cellule
								CTabCell tabcell;
								tabcell.m_BkColor = 2;
								tabcell.m_Height = m_Context->GetLineHeight();
//								tabcell.m_Justification = m_ArrayJustification[startcol-1];
								tabcell.m_Justification = 'C';
								tabcell.m_PosX = this->GetStartPosX(startcol);
								tabcell.m_Width = this->GetEndPosX(endcol) - tabcell.m_PosX;
								tabcell.m_PosY = m_Context->GetVertPos(numline);
								tabcell.m_StyleTD = this->m_ArrayStrokeStyle[endcol];
								tabcell.m_StyleTG = this->m_ArrayStrokeStyle[startcol-1];
								tabcell.m_Texte = texte;

								// on ajoute la cellule au tableau
								ArrayHeaderTabCell.Add(tabcell);
								
								// on incrémente le compteur de champs
								cptfield += 1;
							}
						}
						else return (GotoError());
					}
					// on incrémente le compteur de ligne
					cptlinehead += 1;
					numline += 1;
				}
			}
			else
			{
				if(FieldArray.GetSize() == nbcol && LineField(FieldArray[0]).IsText())
				{
					// on boucle sur tous les champs
					for(long i = 0, max = FieldArray.GetSize(); i < max ; i++)
					{
						// on récupère les colonnes de début et de fin
						long startcol = i+1; long endcol = i+1;

						// on récupère le texte
						CString texte; if (!LineField(FieldArray[i]).GetText(texte)) return (GotoError());

						// on crée une cellule
						CTabCell tabcell;
						tabcell.m_BkColor = 2;
						tabcell.m_Height = m_Context->GetLineHeight();
						tabcell.m_Justification = m_ArrayJustification[startcol-1];
						tabcell.m_PosX = this->GetStartPosX(startcol);
						tabcell.m_Width = this->GetEndPosX(endcol) - tabcell.m_PosX;
						tabcell.m_PosY = m_Context->GetVertPos(numline);
						tabcell.m_StyleTD = this->m_ArrayStrokeStyle[endcol];
						tabcell.m_StyleTG = this->m_ArrayStrokeStyle[startcol-1];
						tabcell.m_Texte = texte;

						// on ajoute la cellule au tableau
						ArrayHeaderTabCell.Add(tabcell);

						// on incrémente le compteur de ligne
						cptlinehead += 1;
					}
				}
				else return (GotoError());
			}

		}
		// on récupère la ligne
		if (!m_Context->GetNextLine(Ligne)) return (GotoError());

		// on récupère les champs
		if (!m_Context->GetLineFields(FieldArray)) return (GotoError());

		// on récupère la commande
		if (!LineField(FieldArray[0]).GetCommand(Commande)) return (GotoError());
	}
	if (Commande[0] == 'D' && Commande[1] == 'O')
	{
		bool fondgrise = false;
		long startcolgrise = 0;
		long endcolgrise = 0;		// Sauvegarder StartLineDonnee = numline

		startlinePagePave = numline;// m_Context->GetNumLine();

		// on boucle sur les lignes
		while (Commande !=  "TABFIN")
		{
			// on récupère la ligne
			if (!m_Context->GetNextLine(Ligne)) return (GotoError());

			// on récupère les champs de cette ligne
			if (!m_Context->GetLineFields(FieldArray)) return (GotoError());

			// on récupère la commande
			if (LineField(FieldArray[0]).GetCommand(Commande))
			{
				// on teste la commande
				if (Commande == "FONDGRISE")
				{
					// on teste le nombre de paramètres
					if (FieldArray.GetSize() != 3) return (GotoError());
					if (!LineField(FieldArray[1]).GetInteger(startcolgrise)) return (GotoError());
					if (!LineField(FieldArray[2]).GetInteger(endcolgrise)) return (GotoError());
					fondgrise = true;
				}
				if (Commande == "TRAITHOR")
				{
					if (fondgrise) return (GotoError());

					// on teste le nombre de paramètres
					if (FieldArray.GetSize() != 4) return (GotoError());

					// on récupère le type de trait
					long strokestyle; if (!LineField(FieldArray[1]).GetInteger(strokestyle)) return (GotoError());

					// on récupère la colonne de départ
					long startcol; if (!LineField(FieldArray[2]).GetInteger(startcol)) return (GotoError());

					// on récupère la colonne d'arrivée
					long endcol; if (!LineField(FieldArray[3]).GetInteger(endcol)) return (GotoError());

					//  on crée un objet CHStroke
					CHStroke hStroke;

					// on remplit ses paramètres
					hStroke.m_StrokeStyle = strokestyle;
					hStroke.m_PosY = m_Context->GetVertPos(numline);
					hStroke.m_StartPosX = this->GetStartPosX(startcol);
					hStroke.m_EndPosX = this->GetEndPosX(endcol);

					// on ajoute le trait au tableau
					m_pTable->AddHStroke(hStroke);

					// on incrémente le compteur de ligne
					numline += 1;
				}
				if (Commande == "TRAIT0HOR")
				{
					if (fondgrise) return (GotoError());

					// on teste le nombre de paramètres
					if (FieldArray.GetSize() != 4) return (GotoError());

					// on récupère le type de trait
					long strokestyle; if (!LineField(FieldArray[1]).GetInteger(strokestyle)) return (GotoError());

					// on récupère la colonne de départ
					long startcol; if (!LineField(FieldArray[2]).GetInteger(startcol)) return (GotoError());

					// on récupère la colonne d'arrivée
					long endcol; if (!LineField(FieldArray[3]).GetInteger(endcol)) return (GotoError());

					//  on crée un objet CHStroke
					CHStroke hStroke;

					// on remplit ses paramètres
					hStroke.m_StrokeStyle = strokestyle;
					hStroke.m_PosY = m_Context->GetVertPos(numline);
					hStroke.m_StartPosX = this->GetStartPosX(startcol);
					hStroke.m_EndPosX = this->GetEndPosX(endcol);

					// on ajoute le trait au tableau
					m_pTable->AddHStroke(hStroke);
				}
				if (Commande == "PAVE")
				{
					CString texte="";
					// on teste le nombre de paramètres
					if (FieldArray.GetSize() < 6) return (GotoError());
					long color; if (!LineField(FieldArray[1]).GetInteger(color)) return (GotoError());
					long startcol; if (!LineField(FieldArray[2]).GetInteger(startcol)) return (GotoError());
					long endcol; if (!LineField(FieldArray[3]).GetInteger(endcol)) return (GotoError());
					long startline; if (!LineField(FieldArray[4]).GetInteger(startline)) return (GotoError());
					long endline; if (!LineField(FieldArray[5]).GetInteger(endline)) return (GotoError());
					if (FieldArray.GetSize() ==7)
						if (!LineField(FieldArray[6]).GetText(texte)) return (GotoError());

					CTabPave tabpave;
					tabpave.m_BkColor = color%17;	// Limité à 15 couleurs
					tabpave.m_Texte   = texte;
					tabpave.m_Height  = m_Context->GetLineHeight();
					tabpave.m_PosX    = this->GetStartPosX(startcol);
					tabpave.m_WidthX  = this->GetEndPosX(endcol) - tabpave.m_PosX;
					tabpave.m_PosY    = startline;		// Ligne début du pavé
					tabpave.m_HeightY  = (1+endline-startline);	// Nb ligne du pavé
					ArrayHeaderTabPave.Add(tabpave);

				}
				if (Commande == "SAUTDEPAGE")
				{
					CTabPave *Ptabpave;
					CHStroke hStroke;
					// on remplit ses paramètres
					hStroke.m_StrokeStyle = 1;
					hStroke.m_PosY = m_Context->GetVertPos(numline);
					hStroke.m_StartPosX = this->GetStartPosX(1);
					hStroke.m_EndPosX = this->GetEndPosX(nbcol);
					// on ajoute le trait au tableau
					m_pTable->AddHStroke(hStroke);
					// on met à jour le flag
					linedrawed = false;
					// on ajoute les éléments du header au tableau
					for(long i=0, max = ArrayHeaderHStroke.GetSize(); i < max; i++)
					{
						m_pTable->AddHStroke(ArrayHeaderHStroke[i]);
					}
					for(long i = 0, max = ArrayHeaderTabCell.GetSize(); i < max; i++)
					{
						m_pTable->AddTabCell(ArrayHeaderTabCell[i]);
					}
					HeightPagePave = numline-startlinePagePave;
					for(long i = 0, max = ArrayHeaderTabPave.GetSize(); i < max; i++)
					{
						Ptabpave = &ArrayHeaderTabPave[i];
						long LineDebutPave = Ptabpave->m_PosY-NbLineWritePave;
						if (LineDebutPave+Ptabpave->m_HeightY <= HeightPagePave) {
							// Le pave est dans la page
							Ptabpave->m_PosY = m_Context->GetVertPos(LineDebutPave+startlinePagePave);
							Ptabpave->m_HeightY  = Ptabpave->m_Height*(Ptabpave->m_HeightY);
							m_pTable->AddTabPave(ArrayHeaderTabPave[i]);
						}
						else if (LineDebutPave >  HeightPagePave) {
							// Le pave est en dehors de la page,conserver pour les pages prochaines
							ArrayHeaderTabPaveNext.Add(ArrayHeaderTabPave[i]);
							continue;
						}
						else {
							// Le pave est à cheval sur la page
							long Height = Ptabpave->m_HeightY;
							Ptabpave->m_PosY = m_Context->GetVertPos(LineDebutPave+startlinePagePave);
							Ptabpave->m_HeightY  = Ptabpave->m_Height*(HeightPagePave-LineDebutPave);
							m_pTable->AddTabPave(ArrayHeaderTabPave[i]);
							Ptabpave->m_PosY = NbLineWritePave+HeightPagePave;
							Ptabpave->m_HeightY = Height-(HeightPagePave-LineDebutPave);
							ArrayHeaderTabPaveNext.Add(ArrayHeaderTabPave[i]);
						}
					}
					// on ajoute le tableau au corps du document
					m_Context->AddBody(*m_pTable);

						// on modifie les positions en y des tableaux
					for(long i=0, max = ArrayHeaderHStroke.GetSize(); i < max; i++)
					{
						ArrayHeaderHStroke[i].m_PosY -= (startheadline - m_Context->GetStartBodyLine()) * m_Context->GetLineHeight();
					}
					for(long i = 0, max = ArrayHeaderTabCell.GetSize(); i < max; i++)
					{
						ArrayHeaderTabCell[i].m_PosY -= (startheadline - m_Context->GetStartBodyLine()) * m_Context->GetLineHeight();
					}
					m_pTable->m_ArrayHStroke.SetSize(0, 20);
					m_pTable->m_ArrayTabCell.SetSize(0, 500);
					m_pTable->m_ArrayTabPave.SetSize(0, 500);
					// on ajoute la page au document et on met à jour le numéro de ligne
					m_Context->AddPage();
					numline = m_Context->GetStartBodyLine() + nblinehead;
					startheadline = m_Context->GetStartBodyLine();

					startlinePagePave = numline;
					NbLineWritePave += HeightPagePave;
					ArrayHeaderTabPave.SetSize(0, 500);
					for(long i = 0, max = ArrayHeaderTabPaveNext.GetSize(); i < max; i++)
					{
						ArrayHeaderTabPave.Add(ArrayHeaderTabPaveNext[i]);
					}
					ArrayHeaderTabPaveNext.SetSize(0, 500);
				}
			}
			else
			{
				// on teste le nombre de paramètres
				if (FieldArray.GetSize() == nbcol)
				{
					CTabPave *Ptabpave;
					HeightPagePave = numline-startlinePagePave;
					// on teste la ligne en cours
					if (numline >= (m_Context->GetNbLinePage() - m_Context->GetNbLineFooter()-1))
					{
						// on teste si on est sur la dernière ligne
						if (numline == (m_Context->GetNbLinePage() - m_Context->GetNbLineFooter()-1) && linedrawed == true)
						{
							//  on crée un objet CHStroke
							CHStroke hStroke;
	
							// on remplit ses paramètres
							hStroke.m_StrokeStyle = 1;
							hStroke.m_PosY = m_Context->GetVertPos(numline);
							hStroke.m_StartPosX = this->GetStartPosX(1);
							hStroke.m_EndPosX = this->GetEndPosX(nbcol);

							// on ajoute le trait au tableau
							m_pTable->AddHStroke(hStroke);

							// on met à jour le flag
							linedrawed = false;
						}
						// on ajoute les éléments du header au tableau
						for(long i=0, max = ArrayHeaderHStroke.GetSize(); i < max; i++)
						{
							m_pTable->AddHStroke(ArrayHeaderHStroke[i]);
						}
						for(long i = 0, max = ArrayHeaderTabCell.GetSize(); i < max; i++)
						{
							m_pTable->AddTabCell(ArrayHeaderTabCell[i]);
						}
						for(long i = 0, max = ArrayHeaderTabPave.GetSize(); i < max; i++)
						{

							Ptabpave = &ArrayHeaderTabPave[i];
							long LineDebutPave = Ptabpave->m_PosY-NbLineWritePave;
							if (LineDebutPave+Ptabpave->m_HeightY <= HeightPagePave) {
								// Le pave est dans la page
								Ptabpave->m_PosY = m_Context->GetVertPos(LineDebutPave+startlinePagePave);
								Ptabpave->m_HeightY  = Ptabpave->m_Height*(Ptabpave->m_HeightY);
								m_pTable->AddTabPave(ArrayHeaderTabPave[i]);
							}
							else if (LineDebutPave >=  HeightPagePave) {
								// Le pave est en dehors de la page,conserver pour les pages prochaines
								ArrayHeaderTabPaveNext.Add(ArrayHeaderTabPave[i]);
								continue;
							}
							else {
								// Le pave est à cheval sur la page
								long Height = Ptabpave->m_HeightY;
								Ptabpave->m_PosY = m_Context->GetVertPos(LineDebutPave+startlinePagePave);
								Ptabpave->m_HeightY  = Ptabpave->m_Height*(HeightPagePave-LineDebutPave);
								m_pTable->AddTabPave(ArrayHeaderTabPave[i]);
								Ptabpave->m_PosY = NbLineWritePave+HeightPagePave;
								Ptabpave->m_HeightY = Height-(HeightPagePave-LineDebutPave);
								ArrayHeaderTabPaveNext.Add(ArrayHeaderTabPave[i]);
							}

						}
						// on ajoute le tableau au corps du document
						m_Context->AddBody(*m_pTable);

						// on modifie les positions en y des tableaux
						for(long i=0, max = ArrayHeaderHStroke.GetSize(); i < max; i++)
						{
							ArrayHeaderHStroke[i].m_PosY -= (startheadline - m_Context->GetStartBodyLine()) * m_Context->GetLineHeight();
						}
						for(long i = 0, max = ArrayHeaderTabCell.GetSize(); i < max; i++)
						{
							ArrayHeaderTabCell[i].m_PosY -= (startheadline - m_Context->GetStartBodyLine()) * m_Context->GetLineHeight();
						}

						m_pTable->m_ArrayHStroke.SetSize(0, 20);
						m_pTable->m_ArrayTabCell.SetSize(0, 500);
						m_pTable->m_ArrayTabPave.SetSize(0, 500);

						// on ajoute la page au document et on met à jour le numéro de ligne
						m_Context->AddPage();
						numline = m_Context->GetStartBodyLine() + nblinehead;
						startheadline = m_Context->GetStartBodyLine();
						startlinePagePave = numline;
						NbLineWritePave += HeightPagePave;

						ArrayHeaderTabPave.SetSize(0, 500);
						for(long i = 0, max = ArrayHeaderTabPaveNext.GetSize(); i < max; i++)
						{
							ArrayHeaderTabPave.Add(ArrayHeaderTabPaveNext[i]);
						}
						ArrayHeaderTabPaveNext.SetSize(0, 500);
					}
					// on boucle sur tous les champs
					for(long i = 0, max = FieldArray.GetSize(); i < max ; i++)
					{
						// on récupère les colonnes de début et de fin
						long startcol = i+1; long endcol = i+1;

						// on récupère le texte
						CString texte; if (!LineField(FieldArray[i]).GetText(texte)) return (GotoError());

						// on crée une cellule
						CTabCell tabcell;
						if ((texte.GetLength() > 1) && texte[0] == '@')
						{
							if (texte[1] == 'G')
							{
								tabcell.m_Bold = true;
								texte = texte.Right(texte.GetLength() - 2);
							}
							else return (GotoError());
						}								
						
						// on fixe la couleur de fond
						if (fondgrise)
						{
							// on teste le numéro de colonne
							if (i >= startcolgrise-1 && i < endcolgrise)
							{
								tabcell.m_BkColor = 1; // ligne grise
							}
							else
							{
								tabcell.m_BkColor = 2; // ligne blanche
							}
						}
						else
						{
							tabcell.m_BkColor = 2; // ligne blanche
						}
						tabcell.m_Height = m_Context->GetLineHeight();
						tabcell.m_Justification = m_ArrayJustification[startcol-1];
						tabcell.m_PosX = this->GetStartPosX(startcol);
						tabcell.m_Width = this->GetEndPosX(endcol) - tabcell.m_PosX;
						tabcell.m_PosY = m_Context->GetVertPos(numline);
						tabcell.m_StyleTD = this->m_ArrayStrokeStyle[endcol];
						tabcell.m_StyleTG = this->m_ArrayStrokeStyle[startcol-1];
						tabcell.m_Texte = texte;

						// on ajoute la cellule au tableau
						m_pTable->AddTabCell(tabcell);
					}
					// on incrémente le compteur de ligne
					numline += 1;
					endlinePagePave = numline;
					linedrawed = true;
				
					// on réinitialise l'indicateur de fond grisé
					fondgrise = false;
				}
				else return (GotoError());
			}
		} 
	}
	// on ajoute les éléments du header au tableau
	long max = 0;
	for(long i=0, max = ArrayHeaderHStroke.GetSize(); i < max; i++)
	{
		m_pTable->AddHStroke(ArrayHeaderHStroke[i]);
	}
	for(long i = 0, max = ArrayHeaderTabCell.GetSize(); i < max; i++)
	{
		m_pTable->AddTabCell(ArrayHeaderTabCell[i]);
	}
	for(long i = 0, max = ArrayHeaderTabPave.GetSize(); i < max; i++)
	{
		CTabPave *Ptabpave;
		Ptabpave = &ArrayHeaderTabPave[i];
		HeightPagePave = endlinePagePave-startlinePagePave;
		long LineDebutPave = Ptabpave->m_PosY-NbLineWritePave;
		if (LineDebutPave+Ptabpave->m_HeightY <= HeightPagePave) {
			// Le pave est dans la page
			Ptabpave->m_PosY = m_Context->GetVertPos(LineDebutPave+startlinePagePave);
			Ptabpave->m_HeightY  = Ptabpave->m_Height*(Ptabpave->m_HeightY);
			m_pTable->AddTabPave(ArrayHeaderTabPave[i]);
		}
		else if (LineDebutPave >  HeightPagePave) {
			continue;
		}
		else {
			// Le pave est à cheval sur la page
			long Height = Ptabpave->m_HeightY;
			Ptabpave->m_PosY = m_Context->GetVertPos(LineDebutPave+startlinePagePave);
			Ptabpave->m_HeightY  = Ptabpave->m_Height*(HeightPagePave-LineDebutPave);
			m_pTable->AddTabPave(ArrayHeaderTabPave[i]);
		}
	}
	// on met à jour la ligne courante
	m_Context->SetNumLine(numline);

	// on libère l'état courant
	delete (this);

	// on renvoie le code de succés
	return (true);		
}

//=========================================================================
// la fonction pour passer à l'état erreur:
//=========================================================================
bool StateTable::GotoError()
{
	// on crée un état erreur
	StateError * pStateError = new StateError(m_Context);

	// on libère l'état actuel
	delete (this);

	// on gère l'état suivant
	return (pStateError->Manage());
}

long StateTable::GetStartPosX(long startcol)
{
	long width = 0;

	// on récupère la largeur du tableau
	for (long i = 0, max = m_ArrayColWidth.GetSize(); i < max; i++)
	{
		width += m_ArrayColWidth[i];
	}
	// on teste la valeur de la colonne
	if (startcol >= 0 && startcol <= m_ArrayColWidth.GetSize())
	{
		long posx = 0;
		for (long i = 0; i < (startcol - 1); i++)
		{
			posx += m_ArrayColWidth[i];
		}
		// on renvoie la position réelle
		return (m_Context->GetHorzPos(posx)*100/width);
	}
	else return (-1);
}

long StateTable::GetEndPosX(long endcol)
{
	long width = 0;

	// on récupère la largeur du tableau
	for (long i = 0, max = m_ArrayColWidth.GetSize(); i < max; i++)
	{
		width += m_ArrayColWidth[i];
	}

	// on teste la valeur de la colonne
	if (endcol >= 0 && endcol <= m_ArrayColWidth.GetSize())
	{
		long posx = 0;
		for (long i = 0; i < endcol; i++)
		{
			posx += m_ArrayColWidth[i];
		}
		// on renvoie la position réelle
		return (m_Context->GetHorzPos(posx)*100/width);
	}
	else return (-1);
}
//=======================================================================
// le destructeur:
//=======================================================================
StateTable::~StateTable()
{
	// on ne fait rien
}