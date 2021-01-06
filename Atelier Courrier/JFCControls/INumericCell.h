#pragma once


#include "..\Config\BuildConfig.h"
#if !defined(BUILD_FOR_ATELIER_INTERNET)

class INumericCell
{
	public:

		INumericCell(CGXGridCore * pParentGrid);

		virtual ~INumericCell();

		////////////////////////////////////////////////////////////
		// les fonctions pour la gestion des nombres négatifs
		void SetNegativeNumbers (const bool bAcceptNegativeNumbers);

		const bool AcceptNegativeNumbers () const;

		////////////////////////////////////////////////////////////
		// les fonctions pour la gestion du nombre de décimales
		void SetNumberOfDecimals (const int iNumDecimals);

		const int GetNumberOfDecimals () const;

		////////////////////////////////////////////////////////////
		// les fonctions pour la gestion de la valeur maximale pouvant être saisie
		void SetMaxValue (const double dMaxValue);

		const double  GetMaxValue () const;

		////////////////////////////////////////////////////////////
		// les fonctions pour la gestion de la valeur minimale pouvant être saisie
		void SetMinValue (const double dMinValue);

		const double  GetMinValue () const;

	protected:

		/////////////////////////////////////////////////////////////////////////////
		// la fonction pour valider le text de l'EditBox avant de l'afficher
		bool ValidateValue (char nChar, const CString & Text, const int start_pos, const int end_pos);

		bool	m_AcceptNegativeNumbers;		// saisie de nombres négatifs (inactif par défaut)
		int		m_NumDecimals;					// nombre de décimales (2 par défaut)	

		double	m_dMaxValue;					// valeur maximale pouvant être saisie
		double	m_dMinValue;					// valeur minimale pouvant être saisie


	protected:

		CGXGridCore* m_pParentGrid;				//  pointer to the parent grid
};

#endif