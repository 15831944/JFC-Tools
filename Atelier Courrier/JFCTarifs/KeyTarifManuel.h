#pragma once

/*
//	Classe:
//	CKeyTarifManuel
//
//	Descriptif:
//	Clé Recherche Tarif Manuel
*/

// Classe pour la clé recherche tarif manuel
class AFX_EXT_CLASS CKeyTarifManuel
{
public:
	// Constructor
 	CKeyTarifManuel()
 	{
 		m_IdSupport	 =	0;
		m_IdFormat	 =  0;
 	}

 	// les opérateurs pour comparer les éléments
	JBool operator <(const CKeyTarifManuel & rhs) const
	{
		if(m_IdSupport < rhs.m_IdSupport)
			return true;
		if(m_IdFormat  < rhs.m_IdFormat)
			return true;
		return false;
	};

	JBool operator >(const CKeyTarifManuel & rhs) const
	{
		if(m_IdSupport > rhs.m_IdSupport)
			return true;
		if(m_IdFormat  > rhs.m_IdFormat)
			return true;
		return false;
	};

	// Les éléments de la clé recherche tarif manuel
 	JUnt32		m_IdSupport; 	// 	Ident Support
 	JUnt32		m_IdFormat;		//	Ident Format

	// Destructeur
	~CKeyTarifManuel();
};
