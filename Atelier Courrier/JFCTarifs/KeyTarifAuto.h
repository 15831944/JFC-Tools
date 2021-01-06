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
//	CKeyTarifAuto
//
//	Descriptif:
//	Cl� de tarif automatis� (IdSupport+IdFormat+Version)
*/


class CKeyTarifAuto
{
public:
	// les constructeurs
	CKeyTarifAuto();
	CKeyTarifAuto(const CKeyTarifAuto & Source);

	// la fonction pour tester l'�l�ment
	JBool IsValid() const;

	// la fonction pour s�rialiser l'�l�ment
	JVoid Recv(JStream & Stream, JInt32 Version);

	// l'op�rateur pour recopier l'�l�ment
	CKeyTarifAuto & operator =(const CKeyTarifAuto & Source);

	// les op�rateurs pour comparer les �l�ments
	JBool operator <(const CKeyTarifAuto & Reference) const;
	JBool operator >(const CKeyTarifAuto & Reference) const;

	// la fonction pour r�initialiser l'�l�ment
	JVoid Reset();

	// le destructeur
	~CKeyTarifAuto();

public:
	JUnt32	m_IdSupport;						// l'identifiant du Support
	JUnt32	m_IdFormat;							// l'Identifiant du format	
	JUnt32	m_Version;							// l'Identifiant de la version
};
