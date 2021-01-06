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
//	CKeyTarifAuto
//
//	Descriptif:
//	Clé de tarif automatisé (IdSupport+IdFormat+Version)
*/


class CKeyTarifAuto
{
public:
	// les constructeurs
	CKeyTarifAuto();
	CKeyTarifAuto(const CKeyTarifAuto & Source);

	// la fonction pour tester l'élément
	JBool IsValid() const;

	// la fonction pour sérialiser l'élément
	JVoid Recv(JStream & Stream, JInt32 Version);

	// l'opérateur pour recopier l'élément
	CKeyTarifAuto & operator =(const CKeyTarifAuto & Source);

	// les opérateurs pour comparer les éléments
	JBool operator <(const CKeyTarifAuto & Reference) const;
	JBool operator >(const CKeyTarifAuto & Reference) const;

	// la fonction pour réinitialiser l'élément
	JVoid Reset();

	// le destructeur
	~CKeyTarifAuto();

public:
	JUnt32	m_IdSupport;						// l'identifiant du Support
	JUnt32	m_IdFormat;							// l'Identifiant du format	
	JUnt32	m_Version;							// l'Identifiant de la version
};
