#include "StdAfx.h"
#include "INumericCell.h"

#include <limits>

#include "..\Config\BuildConfig.h"
#if !defined(BUILD_FOR_ATELIER_INTERNET)

INumericCell::~INumericCell(void)
{
}

/////////////////////////////////////////////////////////////////////////////
// la fonction pour valider le text de l'EditBox avant de l'afficher
bool INumericCell::ValidateValue (char nChar, const CString & Text, const int start_pos, const int end_pos)
{
	// on cr�e une chaine temporaire
	CString Temp; Temp = Text;

	// si une partie du texte est selectionn�e
	if (start_pos != end_pos)
	{
		Temp = Temp.Left(start_pos) + Temp.Right(Temp.GetLength() - end_pos);
	}

	// on ajoute � la chaine le caract�re num�rique saisi
	Temp.Insert (start_pos, nChar);

	// on teste si la valeur obtenue est bien born�e
	const double dValue = atof(Temp);
	if (dValue < m_dMinValue)
	{
		return false;
	}
	else if (dValue > m_dMaxValue )
	{
		return false;
	}

	// r�cup�re la position de la virgule
	int dot_pos = Temp.FindOneOf (",.");

	// il y a une virgule
	if (dot_pos != -1)
	{
		// on s'assure qu'il n'y a qu'une
		CString tmp = Temp.Mid(dot_pos+1);
		if (tmp.FindOneOf (",.") != -1)
		{
			return false;
		}

		// on compte le nombre de d�cimales
		int decimals = Temp.GetLength() - 1 - dot_pos;

		if (decimals > m_NumDecimals)
		{
			return false;
		}
	}
	//on v�rifie la position du '-' pour les nombres n�gatifs
	int minus_pos = Temp.Find("-");	
	if (m_AcceptNegativeNumbers == false && minus_pos != -1)
	{
		return false;
	}
	// on v�rifie que le - est bien le 1er caract�re
	else if (m_AcceptNegativeNumbers && minus_pos != 0)
	{
		return false;
	}

	// le texte est valid�, on quitte
	return true;
}


////////////////////////////////////////////////////////////
// les fonctions pour la gestion de la valeur maximale pouvant �tre saisie
void INumericCell::SetMaxValue (const double dMaxValue)
{
	m_dMaxValue = dMaxValue;
}

const double  INumericCell::GetMaxValue () const
{
	return m_dMaxValue;
}

////////////////////////////////////////////////////////////
// les fonctions pour la gestion de la valeur minimale pouvant �tre saisie
void INumericCell::SetMinValue (const double dMinValue)
{
	m_dMinValue = dMinValue;
}

const double  INumericCell::GetMinValue () const
{
	return m_dMinValue;
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour la gestion des nombres n�gatifs

void INumericCell::SetNegativeNumbers (const bool bAcceptNegativeNumbers)
{
	m_AcceptNegativeNumbers = bAcceptNegativeNumbers;
}

const bool INumericCell::AcceptNegativeNumbers () const
{
	return m_AcceptNegativeNumbers;
}

////////////////////////////////////////////////////////////
// les fonctions pour la gestion du nombre de d�cimales
void INumericCell::SetNumberOfDecimals (const int iNumDecimals)
{
	m_NumDecimals = iNumDecimals;
}

const int INumericCell::GetNumberOfDecimals () const
{
	return m_NumDecimals;
}

#undef min
#undef max

INumericCell::INumericCell(CGXGridCore * pParentGrid)
	:	m_pParentGrid(pParentGrid)
{
	// on donne la valeur par d�faut
	m_AcceptNegativeNumbers = false;

	// on donne la valeur par d�faut
	m_NumDecimals = 1;

	// on donne les valeurs mini et maxi possibles pour des double aux valeurs des bornes
	m_dMinValue = std::numeric_limits<double>::min();
	m_dMaxValue = std::numeric_limits<double>::max();
}

#endif