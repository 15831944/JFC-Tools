#pragma once

/*
//	JFC Informatique et M�dia
//	2003
//
//	Auteur:
//	J�r�my DEVILERS
//
//
//	Classe:
//	CKeySupportDate
//
//	Descriptif:
//	Cl� Entre un identifiant support et une JDate
*/


class AFX_EXT_CLASS CKeySupportDate
{
public:
	// les constructeurs
	CKeySupportDate();
	CKeySupportDate(const CKeySupportDate & Source);

	// la fonction pour tester l'�l�ment
	JBool IsValid() const;

	// la fonction pour s�rialiser l'�l�ment
	JVoid Recv(JStream & Stream, JInt32 Version);

	// l'op�rateur pour recopier l'�l�ment
	CKeySupportDate & operator =(const CKeySupportDate & Source);

	// les op�rateurs pour comparer les �l�ments
	JBool operator <(const CKeySupportDate & Reference) const;
	JBool operator >(const CKeySupportDate & Reference) const;

	// la fonction pour r�initialiser l'�l�ment
	JVoid Reset();

	// le destructeur
	~CKeySupportDate();

public:
	JUnt32	m_IdSupport;		// l'identifiant du Support
	JDate	m_Date;				// la date
};
