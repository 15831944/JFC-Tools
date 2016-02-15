// ARMLFrame.cpp: implementation of the CFrame class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "./../ATPrintPDF.h"
#include "ARMLFrame.h"
#include "./../Composite/JFCDrawLineTo.h"
#include "./../Composite/JFCDrawMoveTo.h"
#include "./../Composite/JFCDrawSetLineWidth.h"
#include "./../Composite/JFCDrawSetFont.h"
#include "./../Composite/JFCDrawText.h"
#include "./../Composite/JFCDrawSetFillColor.h"
#include "./../Composite/JFCDrawBeginPath.h"
#include "./../Composite/JFCDrawEndPath.h"
#include "./../Composite/JFCDrawSetColor.h"
#include "./../Draw/GestionCouleurs.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//=======================================================================
// le constructeur:
//=======================================================================
CFrame::CFrame()
{
	// on initialise les paramètres
	m_Grayed = false;
	m_StartPosX = 0;
	m_EndPosX = 0;
	m_StartPosY = 0;
	m_EndPosY = 0;
	m_StyleTG = 0;
	m_StyleTH = 0;
	m_StyleTD = 0;
	m_StyleTB = 0;
	m_FieldArray.SetSize(0, 50);
}

//=======================================================================
// le constructeur de recopie:
//=======================================================================
CFrame::CFrame(const CFrame & source)
{
	// on recopie les paramètres
	m_Grayed = source.m_Grayed;
	m_StartPosX = source.m_StartPosX;
	m_EndPosX = source.m_EndPosY;
	m_StartPosY = source.m_StartPosY;
	m_EndPosY = source.m_EndPosY;

	m_StyleTG = source.m_StyleTG;
	m_StyleTH = source.m_StyleTH;
	m_StyleTD = source.m_StyleTD;
	m_StyleTB = source.m_StyleTB;
	m_FieldArray.Copy(source.m_FieldArray);
}

//=======================================================================
// l'opérateur d'affectation:
//=======================================================================
CFrame & CFrame::operator = (const CFrame & operande)
{
	// on recopie les paramètres
	m_Grayed = operande.m_Grayed;
	m_StartPosX = operande.m_StartPosX;
	m_EndPosX   = operande.m_EndPosX;
	m_StartPosY = operande.m_StartPosY;
	m_EndPosY   = operande.m_EndPosY;
	m_StyleTG = operande.m_StyleTG;
	m_StyleTH = operande.m_StyleTH;
	m_StyleTD = operande.m_StyleTD;
	m_StyleTB = operande.m_StyleTB;
	m_FieldArray.Copy(operande.m_FieldArray);

	// on renvoie la référence
	return (*this);
}

//======================================================================
// la fonction pour fixer la position de départ en abscisses
//======================================================================
void CFrame::SetStartPosX(long startposX)
{
	// on fixe la position de départ
	this->m_StartPosX = startposX;
}

//======================================================================
// la fonction pour fixer la position d'arrivée en abscisses:
//======================================================================
void CFrame::SetEndPosX(long endposX)
{
	// on fixe la position d'arrivée
	this->m_EndPosX = endposX;
}

//======================================================================
// la fonction pour fixer la position d'arrivée en ordonnées:
//======================================================================
void CFrame::SetEndPosY(long endposY)
{
	// on fixe le nombre de ligne
	this->m_EndPosY = endposY;
}

//======================================================================
// la fonction pour fixer la position de départ en ordonnées:
//======================================================================
void CFrame::SetStartPosY(long startposY)
{
	// on fixe le numéro de la ligne de départ
	this->m_StartPosY = startposY;
}

//======================================================================
// la fonction pour fixer les styles de traits
//======================================================================
void CFrame::SetStrokeStyles(long traitG, long traitH, long traitD, long traitB)
{
	// on fixe les styles de trait
	this->m_StyleTB = traitB;
	this->m_StyleTD = traitD;
	this->m_StyleTG = traitG;
	this->m_StyleTH = traitH;
}

//======================================================================
// la fonction pour récupérer les styles de trait:
//======================================================================
long CFrame::GetStyleTH()
{
	// on renvoie le style du trait haut
	return (m_StyleTH);
}

//======================================================================
// la fonction pour fixer la couleur de fond
//======================================================================
void CFrame::SetGrayed(bool grayed)
{
	// on fixe le paramètre grisé
	m_Grayed = grayed;
}

//======================================================================
// la fonction pour ajouter un champ
//======================================================================
void CFrame::AddField(CField & field)
{
	// on ajoute le champ
	m_FieldArray.Add(field);
}

//======================================================================
// la fonction pour créer le composite d'un cadre
//======================================================================
void CFrame::CreateFrame(long numpage, DrawComposite * composite)
{
	if (this->m_Grayed)
	{
		composite->AddChild(DrawSetFillColor(3));
		composite->AddChild(DrawBeginPath());
		composite->AddChild(DrawMoveTo(m_StartPosX, m_EndPosY));
		composite->AddChild(DrawLineTo(m_StartPosX, m_StartPosY));
		composite->AddChild(DrawLineTo(m_EndPosX, m_StartPosY));
		composite->AddChild(DrawLineTo(m_EndPosX, m_EndPosY));
		composite->AddChild(DrawLineTo(m_StartPosX, m_EndPosY));
		composite->AddChild(DrawEndPath());
	}
	// Contour entete et police noir 
	composite->AddChild(DrawSetColor(0));
	composite->AddChild(DrawMoveTo(m_StartPosX, m_EndPosY));

	if (this->m_StyleTG > 0) 
	{
		composite->AddChild(DrawSetLineWidth(this->m_StyleTG * 2));
		composite->AddChild(DrawLineTo(m_StartPosX, m_StartPosY));
	}
	if (this->m_StyleTH > 0)
	{
		composite->AddChild(DrawSetLineWidth(this->m_StyleTH * 2));
		composite->AddChild(DrawLineTo(m_EndPosX, m_StartPosY));
	}
	if (this->m_StyleTD > 0)
	{
		composite->AddChild(DrawSetLineWidth(this->m_StyleTD *2));
		composite->AddChild(DrawLineTo(m_EndPosX, m_EndPosY));
	}
	if (this->m_StyleTB > 0)
	{
		composite->AddChild(DrawSetLineWidth(this->m_StyleTB * 2));
		composite->AddChild(DrawLineTo(m_StartPosX, m_EndPosY));
	}

	// on boucle sur les champs
	for (long i = 0, max = m_FieldArray.GetSize(); i < max; i++)
	{
		// on ajoute la commande moveto au composite
		composite->AddChild(DrawMoveTo (m_FieldArray[i].m_PosX, m_FieldArray[i].m_PosY));

		// on teste la police
		if (m_FieldArray[i].m_Bold) composite->AddChild(DrawSetFont(1));
		else composite->AddChild(DrawSetFont(0));

		// on récupère le texte
		CString texte = m_FieldArray[i].m_Texte;

		// on cherche les paramètres de modification du texte
		if (texte == "@NOPAGE") texte.Format("%d", numpage);

		// on teste la justification
		switch(m_FieldArray[i].m_Justification)
		{
		case 'C':
			// on ajoute la commande DrawTexte
			composite->AddChild(DrawTexte(texte, CENTER, CENTER));
			break;
		case 'D':
			// on ajoute la commande drawTexte
			composite->AddChild(DrawTexte(texte, RIGHT, CENTER));
			break;
		case 'G':
			// on ajoute la commande drawtexte
			composite->AddChild(DrawTexte(texte, LEFT, CENTER));
			break;
		default:
			composite->AddChild(DrawTexte(texte, CENTER, CENTER));
			break;
		}
	}
}

//=======================================================================
// le destructeur:
//=======================================================================
CFrame::~CFrame()
{
	// 
}
