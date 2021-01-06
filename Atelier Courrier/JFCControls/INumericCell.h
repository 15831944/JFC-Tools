#pragma once


#include "..\Config\BuildConfig.h"
#if !defined(BUILD_FOR_ATELIER_INTERNET)

class INumericCell
{
	public:

		INumericCell(CGXGridCore * pParentGrid);

		virtual ~INumericCell();

		////////////////////////////////////////////////////////////
		// les fonctions pour la gestion des nombres n�gatifs
		void SetNegativeNumbers (const bool bAcceptNegativeNumbers);

		const bool AcceptNegativeNumbers () const;

		////////////////////////////////////////////////////////////
		// les fonctions pour la gestion du nombre de d�cimales
		void SetNumberOfDecimals (const int iNumDecimals);

		const int GetNumberOfDecimals () const;

		////////////////////////////////////////////////////////////
		// les fonctions pour la gestion de la valeur maximale pouvant �tre saisie
		void SetMaxValue (const double dMaxValue);

		const double  GetMaxValue () const;

		////////////////////////////////////////////////////////////
		// les fonctions pour la gestion de la valeur minimale pouvant �tre saisie
		void SetMinValue (const double dMinValue);

		const double  GetMinValue () const;

	protected:

		/////////////////////////////////////////////////////////////////////////////
		// la fonction pour valider le text de l'EditBox avant de l'afficher
		bool ValidateValue (char nChar, const CString & Text, const int start_pos, const int end_pos);

		bool	m_AcceptNegativeNumbers;		// saisie de nombres n�gatifs (inactif par d�faut)
		int		m_NumDecimals;					// nombre de d�cimales (2 par d�faut)	

		double	m_dMaxValue;					// valeur maximale pouvant �tre saisie
		double	m_dMinValue;					// valeur minimale pouvant �tre saisie


	protected:

		CGXGridCore* m_pParentGrid;				//  pointer to the parent grid
};

#endif