// armlfield.cpp: implementation of the CField class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "./../ATPrintPDF.h"
#include "armlfield.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//=========================================================================
// le constructeur:
//=========================================================================
CField::CField()
{
	// on initialise les param�tres
	m_Bold = false;
	m_Justification = 'C';
	m_PosY = 0;
	m_PosX = 0;
	m_Texte = "";
}

//=========================================================================
// le constructeur de recopie:
//=========================================================================
CField::CField(const CField & source)
{
	// on recopie les param�tres de la source
	m_Bold = source.m_Bold;
	m_Justification = source.m_Justification;
	m_PosY = source.m_PosY;
	m_PosX = source.m_PosX;
	m_Texte = source.m_Texte;
}

//=========================================================================
// l'operateur d'�galit�:
//=========================================================================
CField & CField::operator =(const CField & operande)
{
	// on recopie les param�tres
	m_Bold = operande.m_Bold;
	m_Justification = operande.m_Justification;
	m_PosY = operande.m_PosY;
	m_PosX = operande.m_PosX;
	m_Texte = operande.m_Texte;

	// on renvoie une r�f�rence sur l'�l�ment
	return (*this);
}

//=========================================================================
// la fonction pour fixer la position en X
//=========================================================================
void CField::SetPosX(long posX)
{
	// on fixe le num�ro de la ligne
	m_PosX = posX;
}

//=========================================================================
// la fonction pour fixer la position en Y
//=========================================================================
void CField::SetPosY(long posY)
{
	// on fixe le num�ro de la ligne
	m_PosY = posY;
}

//=========================================================================
// la fonction pour fixer le param�tre de gras
//=========================================================================
void CField::SetBold(bool bold)
{
	// on fixe le param�tre de gras
	m_Bold = bold;
}

//=========================================================================
// la fonction pour fixer la justification
//=========================================================================
void CField::SetJustification(char justification)
{
	// on fixe le param�tre de justification
	m_Justification = justification;
}

//=========================================================================
// la fonction pour fixer le texte
//=========================================================================
void CField::SetText(const CString & texte)
{
	// on fixe le param�tre de texte
	m_Texte  = texte;
}

//=========================================================================
// le destructeur:
//=========================================================================
CField::~CField()
{

}
