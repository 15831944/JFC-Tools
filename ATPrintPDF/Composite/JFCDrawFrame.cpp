//=============================
// fichier: JFCDrawFrame.cpp
// 
// date: 04/09/2001
// auteur: JB
//=============================

// on inclut les fichiers nécessaires
#include "stdafx.h"
#include "JFCDrawText.h"
#include "JFCDrawFrame.h"
#include "JFCDrawDocument.h"
#include "JFCDrawMoveTo.h"
#include "JFCDrawSetLineWidth.h"
#include "JFCDrawLineTo.h"
#include "JFCDrawSetFillColor.h"
#include "JFCDrawBeginPath.h"
#include "JFCDrawEndPath.h"
#include "JFCDrawSetFont.h"

//********************************
// la sous-classe Champ:
//********************************
// le constructeur:
DrawFrame::Champ::Champ()
{
	// on initialise les paramètres
	m_Justification = 0;
	m_Position = 0;
	m_Texte = "";
	m_Bold = false;
}

DrawFrame::Champ::Champ(char justification, long position, CString texte, bool bold)
{
	// on initialise les paramètres
	m_Justification = justification;
	m_Position = position;
	m_Texte = texte;
	m_Bold = bold;
}

// le constructeur de recopie
DrawFrame::Champ::Champ(const Champ & source)
{
	// on recopie les paramètres
	m_Justification = source.m_Justification;
	m_Position = source.m_Position;
	m_Texte = source.m_Texte;
	m_Bold = source.m_Bold;
}

// l'opérateur d'égalité
DrawFrame::Champ & DrawFrame::Champ::operator =(const Champ & operande)
{
	// on recopie les paramètres
	m_Justification = operande.m_Justification;
	m_Position = operande.m_Position;
	m_Texte = operande.m_Texte;
	m_Bold = operande.m_Bold;

	// on renvoie la référence
	return (*this);
}

// le destructeur
DrawFrame::Champ::~Champ()
{
	// on ne fait rien
}

//********************************
// la sous-classe Ligne:
//********************************
// le constructeur:
DrawFrame::Ligne::Ligne()
{
	// on initialise les paramètres
	m_Donnees.SetSize(0);
}

// le constructeur de recopie
DrawFrame::Ligne::Ligne(const Ligne & source)
{
	// on fixe la taille du tableau
	m_Donnees.SetSize(source.m_Donnees.GetSize());

	// on recopie les éléments
	for (long i = 0, imax = source.m_Donnees.GetSize(); i < imax; i++)
	{
		// on recopie l'élément
		m_Donnees[i] = source.m_Donnees[i];
	}
}

// l'opérateur d'égalité
DrawFrame::Ligne & DrawFrame::Ligne::operator =(const Ligne & operande)
{
	// on fixe la taille du tableau
	m_Donnees.SetSize(operande.m_Donnees.GetSize());

	
	// on recopie les éléments
	for(long i = 0, imax = operande.m_Donnees.GetSize(); i < imax; i++)
	{
		// on recopie l'élément
		m_Donnees[i] = operande.m_Donnees[i];
	}
	// on renvoie la référence
	return (*this);
}

// le destructeur
DrawFrame::Ligne::~Ligne()
{
	// on ne fait rien
}

//===========================================================================================
// le constructeur:
//===========================================================================================
DrawFrame::DrawFrame()
{
	// on initialise les paramètres du cadre
	m_StartLigne = 0;
	m_NbLigne = 0;
	m_StartCol = 0;
	m_NbCol = 0;
	m_WidthTG = 0;
	m_WidthTH = 0;
	m_WidthTD = 0;
	m_WidthTB = 0;
	m_BackgroundColor = 2;
}

//============================================================================================
// le constructeur de recopie:
//============================================================================================
DrawFrame::DrawFrame(const DrawFrame & source) : DrawComposite(source)
{
	// on recopie les paramètres
	m_StartLigne = source.m_StartLigne;
	m_NbLigne    = source.m_NbLigne;
	m_StartCol   = source.m_StartCol;
	m_NbCol      = source.m_StartCol;
	m_WidthTG    = source.m_WidthTG;
	m_WidthTH    = source.m_WidthTG;
	m_WidthTD    = source.m_WidthTD;
	m_WidthTB    = source.m_WidthTB;

	// on fixe la taille
	m_FrameData.SetSize(source.m_FrameData.GetSize());
	
	// on boucle sur les lignes
	for (long i = 0, imax = source.m_FrameData.GetSize(); i < imax; i++)
	{
		// on fixe la taille de la ligne
		m_FrameData[i].m_Donnees.SetSize(source.m_FrameData[i].m_Donnees.GetSize());
		
		// on boucle sur les champs
		for (long j = 0, jmax =  source.m_FrameData[i].m_Donnees.GetSize(); j < jmax; j++)
		{
			// on recopie les paramètres
			m_FrameData[i].m_Donnees[j].m_Justification = source.m_FrameData[i].m_Donnees[j].m_Justification;
			m_FrameData[i].m_Donnees[j].m_Position      = source.m_FrameData[i].m_Donnees[j].m_Position;
			m_FrameData[i].m_Donnees[j].m_Texte         = source.m_FrameData[i].m_Donnees[j].m_Texte;
		}
	}
}

//=============================================================================================
// la fonction pour dessiner un composant:
//=============================================================================================
void DrawFrame::Draw(JFCDraw * pDraw, long firstpage, long lastpage)
{
	// on boucle sur les enfants
	DrawComponent * pChild = this->GetChild();

	while(pChild != 0)
	{
		// on lance la commande de dessin de l'enfant
		pChild->Draw(pDraw, firstpage, lastpage);

		// on passe a l'enfant suivant
		pChild = pChild->GetNext();
	}
}

//==============================================================================================
// la fonction de clonage:
//==============================================================================================
DrawComponent * DrawFrame::Clone() const
{
	// on renvoie l'adresse d'une copie
	return (new DrawFrame(*this));
}

//==============================================================================================
// les fonctions de remplissage du cadre:
//==============================================================================================
void DrawFrame::SetFrameParams(long lignedepart, long nbligne, long colonnedepart, long nbcolonne)
{
	// on fixe les paramètres de position et de dimension du cadre
	m_StartLigne = lignedepart;
	m_NbLigne = nbligne;
	m_StartCol = colonnedepart;
	m_NbCol = nbcolonne;
}

void DrawFrame::SetStrokeStyles(long widthTG, long widthTH, long widthTD, long widthTB)
{
	// on fixe les paramètres de largeur des traits
	m_WidthTG = widthTG;
	m_WidthTH = widthTH;
	m_WidthTD = widthTD;
	m_WidthTB = widthTB;
}

void DrawFrame::SetBkColor(long bkcolor)
{
	// on fixe la couleur de fond
	m_BackgroundColor = bkcolor;
}

void DrawFrame::AddField(long numligne, char justification, long position, CString texte, bool bold)
{
	// on teste le numéro de ligne
	if (numligne > (this->m_FrameData.GetSize())) 
	{
		// on sort de la fonction
		return;
	}
	// on fixe les paramètres pour la chaine sélectionnée
	m_FrameData[numligne-1].m_Donnees.Add(DrawFrame::Champ(justification, position, texte, bold));
}

void DrawFrame::SetNbLine(long nbline)
{
	// on fixe le nombre de lignes
	m_FrameData.SetSize(nbline);
}

//=========================================================================================
// la fonction pour créer les composants enfants:
//=========================================================================================
void DrawFrame::CreateFrame()
{
	// on récupère la largeur d'une commande
	long ColWidth = 20;		
	long RowHeight = 50;	


	long LineStart = m_StartLigne*RowHeight + RowHeight/2;

	// on crée la commande moveto
	DrawMoveTo moveto(m_StartCol*ColWidth, LineStart);
	
	// on ajoute la commande au composite
	this->AddChild(moveto);

	// on teste le trait
	if(m_WidthTH > 0)
	{
		// on crée une largeur de trait, on l'ajoute au composite
		DrawSetLineWidth setlinewidth(m_WidthTB*5); this->AddChild(setlinewidth);
		
		// on calcule la position en X
		long X = GetPosX(this->m_NbCol);
		
		// on crée la commande lineto, on l'ajoute au composite
		DrawLineTo lineto(X, LineStart);
		this->AddChild(lineto);
	}
	if(m_WidthTD > 0)
	{
		// on crée une largeur de trait, on l'ajoute au composite
		DrawSetLineWidth setlinewidth(m_WidthTD*5); this->AddChild(setlinewidth);
		
		// on calcule la position en X
		long X = GetPosX(this->m_NbCol);
		
		// on crée la commande lineto, on l'ajoute au composite
		DrawLineTo lineto(X, LineStart + (m_NbLigne-1)*RowHeight);
		this->AddChild(lineto);
	}
	if(m_WidthTB > 0)
	{
		// on crée une largeur de trait, on l'ajoute au composite
		DrawSetLineWidth setlinewidth(m_WidthTB*5); this->AddChild(setlinewidth);
		
		// on calcule la position en X
		long X = GetPosX(this->m_StartCol);
		
		// on crée la commande lineto, on l'ajoute au composite
		DrawLineTo lineto(X, LineStart + (m_NbLigne-1)*RowHeight);
		this->AddChild(lineto);
	}
	if(m_WidthTG > 0)
	{
		// on crée une largeur de trait, on l'ajoute au composite
		DrawSetLineWidth setlinewidth(m_WidthTG*5); this->AddChild(setlinewidth);
		
		// on calcule la position en X
		long X = GetPosX(this->m_StartCol);
		
		//on crée la commande lineto, on l'ajoute au composite
		DrawLineTo lineto(X, LineStart);
		this->AddChild(lineto);
	}

	// on positionne le pointeur de ligne sur la première ligne de texte
	if(m_WidthTH > 0) (LineStart += RowHeight);

	// on boucle sur les données
	for (long i = 0, imax = m_FrameData.GetSize(); i< imax; i++)
	{
		// on boucle sur les éléments
		for ( long j = 0, jmax = m_FrameData[i].m_Donnees.GetSize(); j < jmax; j++)
		{
			// on teste la position
			if (m_FrameData[i].m_Donnees[j].m_Position == 0)
			{
				// on teste la justification
				if (m_FrameData[i].m_Donnees[j].m_Justification == 'C')
				{
					// on calcule la position du champ
					long X = this->GetPosX(50);

					// on crée la commande moveto, on l'ajoute au composite
					DrawMoveTo moveto(X, LineStart+i*RowHeight); AddChild(moveto);

					// on teste si le texte est en gras
					if (m_FrameData[i].m_Donnees[j].m_Bold == true)
					{
						// on change la police, on l'ajoute au composite
						DrawSetFont setfont(3);
						AddChild(setfont);
					}
					else
					{
						// on change la police, on l'ajoute au composite
						DrawSetFont setfont(2);
						AddChild(setfont);
					}

					// on crée la commande text, on l'ajoute au composite
					DrawTexte text(m_FrameData[i].m_Donnees[j].m_Texte, CENTER, CENTER);
					AddChild(text);
				}
				else if (m_FrameData[i].m_Donnees[j].m_Justification == 'D')
				{
					// on calcule la position du champ
					long X = this->GetPosX(100);
					
					// on teste si le texte est en gras
					if (m_FrameData[i].m_Donnees[j].m_Bold == true)
					{
						// on change la police, on l'ajoute au composite
						DrawSetFont setfont(3);
						AddChild(setfont);
					}
					else
					{
						// on change la police, on l'ajoute au composite
						DrawSetFont setfont(2);
						AddChild(setfont);
					}

					// on crée la commande moveto, on l'ajoute au composite
					DrawMoveTo moveto(X, LineStart+i*RowHeight);AddChild(moveto);

					// on crée la commande text, on l'ajoute au composite
					DrawTexte text(m_FrameData[i].m_Donnees[j].m_Texte, RIGHT, CENTER); AddChild(text);
				}
				else if (m_FrameData[i].m_Donnees[j].m_Justification == 'G')
				{
					// on calcule la position du champ
					long X = this->GetPosX(0);

					// on teste si le texte est en gras
					if (m_FrameData[i].m_Donnees[j].m_Bold == true)
					{
						// on change la police, on l'ajoute au composite
						DrawSetFont setfont(3);
						AddChild(setfont);
					}
					else
					{
						// on change la police, on l'ajoute au composite
						DrawSetFont setfont(2);
						AddChild(setfont);
					}

					// on crée la commande moveto, on l'ajoute au composite
					DrawMoveTo moveto(X, LineStart+i*RowHeight); AddChild(moveto);

					// on crée la commande text, on l'ajoute au composite
					DrawTexte text(m_FrameData[i].m_Donnees[j].m_Texte, LEFT, CENTER); AddChild(text);
				}
			}
			else
			{
				// on calcule la position du champ
				long X = this->GetPosX(m_FrameData[i].m_Donnees[j].m_Position);

				// on crée la commande moveto, on l'ajoute au composite
				DrawMoveTo moveto(X, LineStart + i * RowHeight); AddChild(moveto);

				// on teste si le texte est en gras
				if (m_FrameData[i].m_Donnees[j].m_Bold == true)
				{
					// on change la police, on l'ajoute au composite
					DrawSetFont setfont(3);
					AddChild(setfont);
				}
				else
				{
					// on change la police, on l'ajoute au composite
					DrawSetFont setfont(2);
					AddChild(setfont);
				}

				// on récupère l'alignement horizontal
				Align halign = CENTER;
				if (m_FrameData[i].m_Donnees[j].m_Justification == 'C') halign = CENTER;
				else if(m_FrameData[i].m_Donnees[j].m_Justification == 'D') halign = RIGHT;
				else if(m_FrameData[i].m_Donnees[j].m_Justification == 'G') halign = LEFT;

				// on crée la commande text, on l'ajoute au composite
				DrawTexte text(m_FrameData[i].m_Donnees[j].m_Texte, halign, CENTER); AddChild(text);
			}
		}
	}
}

//=========================================================================================
// la fonction pour récupérer la position absolue en fonction du pourcentage:
//==========================================================================================
long DrawFrame::GetPosX(long posrelative)
{
	// la taille de la page
	long PageWidth = 2000;

	// on renvoie la position relative
	return (posrelative*PageWidth/100);
}

//=========================================================================================
// le destructeur:
//==========================================================================================
DrawFrame::~DrawFrame()
{
	// on ne fait rien
}
