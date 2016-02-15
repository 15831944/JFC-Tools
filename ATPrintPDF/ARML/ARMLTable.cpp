// ARMLTable.cpp: implementation of the CTable class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "./../ATPrintPDF.h"
#include "ARMLTable.h"
#include "./../composite/JFCDrawTableau.h"
#include "./../composite/JFCDrawMoveTo.h"
#include "./../composite/JFCDrawSetLineWidth.h"
#include "./../composite/JFCDrawLineTo.h"
#include "./../composite/JFCDrawBeginPath.h"
#include "./../composite/JFCDrawEndPath.h"
#include "./../composite/JFCDrawSetFillColor.h"
#include "./../composite/JFCDrawText.h"
#include "./../composite/JFCDrawSetColor.h"
#include "./../composite/JFCDrawSetFont.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//=====================================================================
// le constructeur:
//=====================================================================
CTable::CTable()
{
	// on initialise les paramètres
	m_ArrayHStroke.SetSize(0, 12);
	m_ArrayTabCell.SetSize(0, 500);
	m_ArrayTabPave.SetSize(0, 500);
}

//=====================================================================
// le constructeur de recopie:
//=====================================================================
CTable::CTable(const CTable & source)
{
	// on recopie les paramètres
	m_ArrayHStroke.Copy(source.m_ArrayHStroke);
	m_ArrayTabCell.Copy(source.m_ArrayTabCell);
	m_ArrayTabPave.Copy(source.m_ArrayTabPave);
}

//=====================================================================
// l'opérateur d'affectation:
//=====================================================================
CTable & CTable::operator =(const CTable & operande)
{
	// on recopie les paramètres
	m_ArrayHStroke.Copy(operande.m_ArrayHStroke);
	m_ArrayTabCell.Copy(operande.m_ArrayTabCell);	
	m_ArrayTabPave.Copy(operande.m_ArrayTabPave);	

	// on renvoie la référence
	return (*this);
}

//=====================================================================
// les fonctions pour ajouter des éléments dans les tableaux:
//=====================================================================
void CTable::AddHStroke(CHStroke & stroke)
{
	// on ajoute le trait dans le tableau de traits
	m_ArrayHStroke.Add(stroke);
}

void CTable::AddTabCell(CTabCell & tabcell)
{
	// on ajoute la cellule dans le tableau de cellule
	m_ArrayTabCell.Add(tabcell);
}
void CTable::AddTabPave(CTabPave & tabpave)
{
	// on ajoute la cellule dans le tableau de cellule
	m_ArrayTabPave.Add(tabpave);
}

//=====================================================================
// la fonction pour créer les commandes:
//=====================================================================
void CTable::CreateTable(long numpage, DrawComposite * composite)
{
	// on boucle sur le tableau de cellules
	for (long i=0, max = m_ArrayTabPave.GetSize(); i < max; i++)
	{
		DrawTable table;
		
			// on ajoute le chemin
		table.AddChild(DrawSetColor(m_ArrayTabPave[i].m_BkColor+100));
		table.AddChild(DrawSetFillColor(m_ArrayTabPave[i].m_BkColor+100));
		table.AddChild(DrawBeginPath());
		table.AddChild(DrawMoveTo(m_ArrayTabPave[i].m_PosX, (m_ArrayTabPave[i].m_PosY-(m_ArrayTabPave[i].m_Height/2))));
		table.AddChild(DrawLineTo(m_ArrayTabPave[i].m_PosX+m_ArrayTabPave[i].m_WidthX, (m_ArrayTabPave[i].m_PosY-(m_ArrayTabPave[i].m_Height/2))));
		table.AddChild(DrawLineTo(m_ArrayTabPave[i].m_PosX+m_ArrayTabPave[i].m_WidthX, 
				(m_ArrayTabPave[i].m_PosY-(m_ArrayTabPave[i].m_Height/2)+m_ArrayTabPave[i].m_HeightY)));
		table.AddChild(DrawLineTo(m_ArrayTabPave[i].m_PosX,
				(m_ArrayTabPave[i].m_PosY-(m_ArrayTabPave[i].m_Height/2)+m_ArrayTabPave[i].m_HeightY)));
		table.AddChild(DrawLineTo(m_ArrayTabPave[i].m_PosX, (m_ArrayTabPave[i].m_PosY-(m_ArrayTabPave[i].m_Height/2))));
		table.AddChild(DrawEndPath());

			// on remet la couleur de trait normale noir
		table.AddChild(DrawSetColor(0));
		// on se place sur le centre de la cellule
		if (m_ArrayTabPave[i].m_Texte.GetLength() != 0) {
			CStringArray PaveArray;
			int nb,posY;
			m_ArrayTabPave[i].GetLineFields(m_ArrayTabPave[i].m_Texte,PaveArray);
			nb = PaveArray.GetSize();
			if (nb*m_ArrayTabPave[i].m_Height <=m_ArrayTabPave[i].m_HeightY) {
				posY = m_ArrayTabPave[i].m_PosY+(m_ArrayTabPave[i].m_HeightY-nb*m_ArrayTabPave[i].m_Height)/2;
				for (int j=0;j<nb;j++) {
					table.AddChild(DrawMoveTo(m_ArrayTabPave[i].m_PosX+(m_ArrayTabPave[i].m_WidthX/2), 
										  posY+(j*m_ArrayTabPave[i].m_Height)));
					table.AddChild(DrawSetFont(0));
					table.AddChild(DrawTexte(PaveArray[j], CENTER, CENTER));
				}
			}
		}
		composite->AddChild(table);
	}
	// on boucle sur le tableau de traits horizontaux
	for (long i=0, max = m_ArrayHStroke.GetSize(); i <  max; i++)
	{
		// on teste le style de trait
		switch(m_ArrayHStroke[i].m_StrokeStyle)
		{
		case 0:
			// on ne fait rien
			break;
		case 1:
			// on se place au début du trait
			composite->AddChild(DrawMoveTo(m_ArrayHStroke[i].m_StartPosX, m_ArrayHStroke[i].m_PosY));
			
			// on fixe l'épaisseur de trait
			composite->AddChild(DrawSetLineWidth(2));

			// on trace le trait
			composite->AddChild(DrawLineTo(m_ArrayHStroke[i].m_EndPosX, m_ArrayHStroke[i].m_PosY));

			// on quitte
			break;
		case 2:
			// on fixe l'épaisseur de trait
			composite->AddChild(DrawSetLineWidth(2));

			// on trace 2 traits
			composite->AddChild(DrawMoveTo(m_ArrayHStroke[i].m_StartPosX, m_ArrayHStroke[i].m_PosY + 2));
			composite->AddChild(DrawLineTo(m_ArrayHStroke[i].m_EndPosX, m_ArrayHStroke[i].m_PosY + 2));

			composite->AddChild(DrawMoveTo(m_ArrayHStroke[i].m_StartPosX, m_ArrayHStroke[i].m_PosY - 2));
			composite->AddChild(DrawLineTo(m_ArrayHStroke[i].m_StartPosX, m_ArrayHStroke[i].m_PosY - 2));

			// on quitte
			break;
		}
	}
	// on boucle sur le tableau de cellules
	for (long i=0, max = m_ArrayTabCell.GetSize(); i < max; i++)
	{
		DrawTable table;
		
		// on teste le fond
		if (m_ArrayTabCell[i].m_BkColor != 2) // si le fond est  != blanc
		{
			// on ajoute le chemin
			table.AddChild(DrawSetColor(m_ArrayTabCell[i].m_BkColor));
			table.AddChild(DrawSetFillColor(m_ArrayTabCell[i].m_BkColor));
			table.AddChild(DrawBeginPath());
			table.AddChild(DrawMoveTo(m_ArrayTabCell[i].m_PosX, (m_ArrayTabCell[i].m_PosY-(m_ArrayTabCell[i].m_Height/2))));
			table.AddChild(DrawLineTo(m_ArrayTabCell[i].m_PosX+m_ArrayTabCell[i].m_Width, (m_ArrayTabCell[i].m_PosY-(m_ArrayTabCell[i].m_Height/2))));
			table.AddChild(DrawLineTo(m_ArrayTabCell[i].m_PosX+m_ArrayTabCell[i].m_Width, (m_ArrayTabCell[i].m_PosY+(m_ArrayTabCell[i].m_Height/2))));
			table.AddChild(DrawLineTo(m_ArrayTabCell[i].m_PosX, (m_ArrayTabCell[i].m_PosY+(m_ArrayTabCell[i].m_Height/2))));
			table.AddChild(DrawLineTo(m_ArrayTabCell[i].m_PosX, (m_ArrayTabCell[i].m_PosY-(m_ArrayTabCell[i].m_Height/2))));
			table.AddChild(DrawEndPath());

			// on remet la couleur de trait normale noir
			table.AddChild(DrawSetColor(0));
		}
		// on teste le style de trait Gauche
		switch(m_ArrayTabCell[i].m_StyleTG)
		{
		case 0:
			break;
		case 1:
			// on fixe l'épaisseur de trait
			table.AddChild(DrawSetLineWidth(2));

			// on dessine le trait gauche
			table.AddChild(DrawMoveTo(m_ArrayTabCell[i].m_PosX, (m_ArrayTabCell[i].m_PosY-(m_ArrayTabCell[i].m_Height))));
			table.AddChild(DrawLineTo(m_ArrayTabCell[i].m_PosX, (m_ArrayTabCell[i].m_PosY+(m_ArrayTabCell[i].m_Height))));

			break;
		case 2:
			// on fixe l'épaisseur de trait
			table.AddChild(DrawSetLineWidth(2));
		
			// on dessine les traits gauches
			table.AddChild(DrawMoveTo(m_ArrayTabCell[i].m_PosX - 2, (m_ArrayTabCell[i].m_PosY-(m_ArrayTabCell[i].m_Height))));
			table.AddChild(DrawLineTo(m_ArrayTabCell[i].m_PosX - 2, (m_ArrayTabCell[i].m_PosY+(m_ArrayTabCell[i].m_Height))));

			table.AddChild(DrawMoveTo(m_ArrayTabCell[i].m_PosX + 2, (m_ArrayTabCell[i].m_PosY-(m_ArrayTabCell[i].m_Height))));
			table.AddChild(DrawLineTo(m_ArrayTabCell[i].m_PosX + 2, (m_ArrayTabCell[i].m_PosY+(m_ArrayTabCell[i].m_Height))));
			break;
		}
		// on teste le style de trait droit
		switch(m_ArrayTabCell[i].m_StyleTD)
		{
		case 0:
			break;
		case 1:
			// on fixe l'épaisseur de trait
			table.AddChild(DrawSetLineWidth(2));
		
			// on dessine le trait droit
			table.AddChild(DrawMoveTo(m_ArrayTabCell[i].m_PosX+m_ArrayTabCell[i].m_Width, (m_ArrayTabCell[i].m_PosY-(m_ArrayTabCell[i].m_Height))));
			table.AddChild(DrawLineTo(m_ArrayTabCell[i].m_PosX+m_ArrayTabCell[i].m_Width, (m_ArrayTabCell[i].m_PosY+(m_ArrayTabCell[i].m_Height))));
			
			break;
		case 2:
			// on fixe l'épaisseur de trait
			table.AddChild(DrawSetLineWidth(2));
		
			// on dessine les traits droits
			table.AddChild(DrawMoveTo(m_ArrayTabCell[i].m_PosX+m_ArrayTabCell[i].m_Width - 2, (m_ArrayTabCell[i].m_PosY-(m_ArrayTabCell[i].m_Height))));
			table.AddChild(DrawLineTo(m_ArrayTabCell[i].m_PosX+m_ArrayTabCell[i].m_Width - 2, (m_ArrayTabCell[i].m_PosY+(m_ArrayTabCell[i].m_Height))));

			table.AddChild(DrawMoveTo(m_ArrayTabCell[i].m_PosX+m_ArrayTabCell[i].m_Width + 2, (m_ArrayTabCell[i].m_PosY-(m_ArrayTabCell[i].m_Height))));
			table.AddChild(DrawLineTo(m_ArrayTabCell[i].m_PosX+m_ArrayTabCell[i].m_Width + 2, (m_ArrayTabCell[i].m_PosY+(m_ArrayTabCell[i].m_Height))));
			
			break;
		}
		// on positionne le texte en fonction de la justification
		switch(m_ArrayTabCell[i].m_Justification)
		{
		case 'D':
			// on se place sur la droite de la cellule
			table.AddChild(DrawMoveTo(m_ArrayTabCell[i].m_PosX+m_ArrayTabCell[i].m_Width - 10, m_ArrayTabCell[i].m_PosY));
			// on teste le paramètre gras
			if (m_ArrayTabCell[i].m_Bold)
			{
				table.AddChild(DrawSetFont(1));
			}
			else
			{
				table.AddChild(DrawSetFont(0));
			}			
			// on écrit le texte
			table.AddChild(DrawTexte(m_ArrayTabCell[i].m_Texte, RIGHT, CENTER));
			break;
		case 'G':
			// on se place sur la gauche de la cellule
			table.AddChild(DrawMoveTo(m_ArrayTabCell[i].m_PosX + 10, m_ArrayTabCell[i].m_PosY));
			// on teste le paramètre gras
			if (m_ArrayTabCell[i].m_Bold)
			{
				table.AddChild(DrawSetFont(1));
			}
			else
			{
				table.AddChild(DrawSetFont(0));
			}
			table.AddChild(DrawTexte(m_ArrayTabCell[i].m_Texte, LEFT, CENTER));
			break;
		case 'C':
			// on se place sur le centre de la cellule
			table.AddChild(DrawMoveTo(m_ArrayTabCell[i].m_PosX+(m_ArrayTabCell[i].m_Width/2), m_ArrayTabCell[i].m_PosY));
			if (m_ArrayTabCell[i].m_Bold)
			{
				table.AddChild(DrawSetFont(1));
			}
			else
			{
				table.AddChild(DrawSetFont(0));
			}
			table.AddChild(DrawTexte(m_ArrayTabCell[i].m_Texte, CENTER, CENTER));
			break;
		}
		composite->AddChild(table);

	}
}

//=====================================================================
// le destructeur:
//=====================================================================
CTable::~CTable()
{

}
