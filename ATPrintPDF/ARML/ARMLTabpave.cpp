// ARMLTabPave.cpp: implementation of the CTabPave class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "./../ATPrintPDF.h"
#include "ARMLTabPave.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//========================================================================
// le constructeur:
//========================================================================
CTabPave::CTabPave()
{
	// on initialise les param�tres
	m_PosY = 0;
	m_Height = 0;
	m_PosX = 0;
	m_WidthX = 0;
	m_HeightY = 0;
	m_BkColor = 0;
	m_Texte = "";
}

//========================================================================
// le constructeur de recopie:
//========================================================================
CTabPave::CTabPave(const CTabPave & source)
{
	// on initialise les param�tres
	m_PosY          = source.m_PosY;
	m_Height        = source.m_Height;
	m_PosX          = source.m_PosX ;
	m_WidthX        = source.m_WidthX;
	m_HeightY       = source.m_HeightY;
	m_BkColor       = source.m_BkColor;
	m_Texte         = source.m_Texte;
}

//========================================================================
// l'op�rateur d'affectation:
//========================================================================
CTabPave & CTabPave::operator=(const CTabPave & operande)
{
	// on initialise les param�tres
	m_PosY          = operande.m_PosY;
	m_Height        = operande.m_Height;
	m_PosX          = operande.m_PosX ;
	m_WidthX        = operande.m_WidthX;
	m_HeightY       = operande.m_HeightY;
	m_BkColor       = operande.m_BkColor;
	m_Texte         = operande.m_Texte;

	// on renvoie la r�f�rence sur l'�l�ment
	return (*this);
}


void CTabPave::GetLineFields(CString texte,CStringArray & FieldArray)
{
	FieldArray.SetSize(0,20);
	CString Field ="";

	// on boucle sur les caract�res de la ligne
	for (long i = 0, max = texte.GetLength(); i < max ; i++)
	{
		if (texte.GetAt(i) != '|')
		{
			Field += CString(texte.GetAt(i));
		}
		else
		{
			// on ajoute le champs au tableau
			if (Field != ""){
				FieldArray.Add(Field);
			}
				// on r�initialise le champ
			Field = "";
		}
	}
	// on ajoute le dernier champ si n�cessaire
	if (Field != ""){
		FieldArray.Add(Field);
	}
	// on renvoie le code de succ�s ou d'�chec
	return;
}

//========================================================================
// le destructeur:
//========================================================================
CTabPave::~CTabPave()
{

}
