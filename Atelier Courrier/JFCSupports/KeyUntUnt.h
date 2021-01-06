/*
//	JFC Informatique et M�dia
//	2003
//
//	Auteur:
//	J�r�my DEVILERS
//
//
//	Classe:
//	CKeyUntUnt
//
//	Descriptif:
//	Cl� de 2 entiers
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

	// la fonction pour tester l'�l�ment
	JBool IsValid() const;

	// l'op�rateur pour recopier l'�l�ment
	CKeyUntUnt & operator =(const CKeyUntUnt & Source);

	// les op�rateurs pour comparer les �l�ments
	JBool operator <(const CKeyUntUnt & Reference) const;
	JBool operator >(const CKeyUntUnt & Reference) const;

	// la fonction pour r�initialiser l'�l�ment
	JVoid Reset();

	// le destructeur
	~CKeyUntUnt();

public:
	JUnt32	m_Id1;	// l'identifiant du Support
	JUnt32	m_Id2;	// l'Identifiant du format	
};

#endif // !defined(KEYUNTUNT_H__INCLUDED_)
