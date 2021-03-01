#pragma once

/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	CKeySupportDate
//
//	Descriptif:
//	Clé Entre un identifiant support et une JDate
*/


class AFX_EXT_CLASS CKeySupportDate
{
public:
	// les constructeurs
	CKeySupportDate();
	CKeySupportDate(const CKeySupportDate & Source);

	// la fonction pour tester l'élément
	JBool IsValid() const;

	// la fonction pour sérialiser l'élément
	JVoid Recv(JStream & Stream, JInt32 Version);

	// l'opérateur pour recopier l'élément
	CKeySupportDate & operator =(const CKeySupportDate & Source);

	// les opérateurs pour comparer les éléments
	JBool operator <(const CKeySupportDate & Reference) const;
	JBool operator >(const CKeySupportDate & Reference) const;

	// la fonction pour réinitialiser l'élément
	JVoid Reset();

	// le destructeur
	~CKeySupportDate();

public:
	JUnt32	m_IdSupport;		// l'identifiant du Support
	JDate	m_Date;				// la date
};
