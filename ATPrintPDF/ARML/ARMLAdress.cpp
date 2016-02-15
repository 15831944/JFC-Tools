// ARMLAdress.cpp: implementation of the CAdress class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "./../ATPrintPDF.h"
#include "./../Composite/JFCDrawMoveTo.h"
#include "./../Composite/JFCDrawText.h"
#include "./../Composite/JFCDrawSetFont.h"
#include "ARMLAdress.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//=====================================================================
// le constructeur:
//=====================================================================
CAdress::CAdress()
{
	// on initialise les paramètres
	m_Grayed = false;
	m_FieldArray.SetSize(0, 40);
}

//=====================================================================
// le constructeur de recopie:
//=====================================================================
CAdress::CAdress(const CAdress & source)
{
	// on recopie les paramètres
	m_Grayed = source.m_Grayed;
	m_FieldArray.Copy(source.m_FieldArray);
	
}

//=====================================================================
// l'opérateur d'affectation:
//=====================================================================
CAdress & CAdress::operator =(const CAdress & operande)
{
	// on recopie les paramètres
	m_Grayed = operande.m_Grayed;
	m_FieldArray.Copy(operande.m_FieldArray);

	// on renvoie la référence
	return (*this);
}

//=====================================================================
// la fonction pour fixer la couleur de fond
//=====================================================================
void CAdress::SetGrayed(bool grayed)
{
	// on fixe le paramètre grisé
	m_Grayed = grayed;
}

//=====================================================================
// la fonction pour ajouter un champ
//=====================================================================
void CAdress::AddField(CField & field)
{
	// on ajoute un champ au tableau
	m_FieldArray.Add(field);
}

//=====================================================================
// la fonction pour créer le composite d'un cadre
//=====================================================================
void CAdress::CreateAdress(long numpage, DrawComposite * composite)
{
	// on boucle sur les champs
	for (long i = 0, max = m_FieldArray.GetSize(); i < max; i++)
	{
		// on ajoute la commande moveto au composite
		composite->AddChild(DrawMoveTo (m_FieldArray[i].m_PosX, m_FieldArray[i].m_PosY));

		// on teste la police
		// changer la police par indice police
		if (m_FieldArray[i].m_Bold) composite->AddChild(DrawSetFont(1));
		else composite->AddChild(DrawSetFont(0));

		// on vire les @G @N
		m_FieldArray[i].m_Texte.Replace("@N", " ");
		m_FieldArray[i].m_Texte.Replace("@G", "");

		// on teste la justification
		switch(m_FieldArray[i].m_Justification)
		{
		case 'C':
			// on ajoute la commande DrawTexte
			composite->AddChild(DrawTexte(m_FieldArray[i].m_Texte, CENTER, CENTER));
			break;
		case 'D':
			// on ajoute la commande drawTexte
			composite->AddChild(DrawTexte(m_FieldArray[i].m_Texte, RIGHT, CENTER));
			break;
		case 'G':
			// on ajoute la commande drawtexte
			composite->AddChild(DrawTexte(m_FieldArray[i].m_Texte, LEFT, CENTER));
			break;
		default:
			composite->AddChild(DrawTexte(m_FieldArray[i].m_Texte, CENTER, CENTER));
			break;
		}
	}
}

//=====================================================================
// le destructeur:
//=====================================================================
CAdress::~CAdress()
{

}
