/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	CKeyUntUnt
//
//	Descriptif:
//	Clé de 2 entiers
*/

#if !defined(KEYUNTUNT_H__INCLUDED_)
#define KEYUNTUNT_H__INCLUDED_

#pragma once

class CKeyUntUnt
{
public:
	// les constructeurs
	CKeyUntUnt();
	CKeyUntUnt(const CKeyUntUnt & Source);

	// la fonction pour tester l'élément
	JBool IsValid() const;

	// l'opérateur pour recopier l'élément
	CKeyUntUnt & operator =(const CKeyUntUnt & Source);

	// les opérateurs pour comparer les éléments
	JBool operator <(const CKeyUntUnt & Reference) const;
	JBool operator >(const CKeyUntUnt & Reference) const;

	// la fonction pour réinitialiser l'élément
	JVoid Reset();

	// le destructeur
	~CKeyUntUnt();

public:
	JUnt32	m_Id1;	// l'identifiant du Support
	JUnt32	m_Id2;	// l'Identifiant du format	
};

#endif // !defined(KEYUNTUNT_H__INCLUDED_)
