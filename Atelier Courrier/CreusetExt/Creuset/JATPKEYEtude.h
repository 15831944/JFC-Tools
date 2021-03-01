//
// Fichier: JATPKEYEtude.h
// Auteur:  Alain Chambard & Eddie Gent
// Date:    20/08/2003
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JATP_KEY_ETUDE_H

// on d�finit la macro pour �viter les inclusions multiples
#define JATP_KEY_ETUDE_H

// on inclut les d�finitions n�cessaires
#include "JLib.h"

class JATPKEYEtude
{
public:
	// le constructeur
	JATPKEYEtude();

	// les fonctions pour manipuler le nom de l'�tude
	CString GetLabel() const;
	JVoid SetPath(CString path);
	CString GetPath() const;

	// les fonctions pour manipuler l'identifiant du produit
	JVoid  SetIdProduit(JUnt32 IdProduit);
	JUnt32 GetIdProduit() const;

	// les fonctions pour manipuler la date de d�but de l'�tude
	JVoid SetDateD(JDate DateD);
	JDate GetDateD() const;	

	// les fonctions pour manipuler la date de fin de l'�tude
	JVoid SetDateF(JDate DateF);
	JDate GetDateF() const;

	// les fonctions pour s�rialiser l'�l�ment
//	virtual void Serialize(CArchive& ar);

	JVoid Send(JStream & Stream) const;
	JVoid Recv(JStream & Stream);

	// la fonction pour permuter les �l�ments
	JVoid Swap(JATPKEYEtude & Source);

	// le destructeur
	~JATPKEYEtude();

private:
	// l'op�rateur pour recopier l'�l�ment
	JATPKEYEtude & operator =(const JATPKEYEtude & Source);

	// le constructeur copie
	JATPKEYEtude(const JATPKEYEtude & Source);

private:
	CString m_EtudeName;// le nom de l'etude
	CString m_path;
	JUnt32 m_IdProduit; // l'identifiant du produit
	JDate  m_DateD;     // la date de d�but
	JDate  m_DateF;     // la date de fin
};

// fin de l'inclusion conditionnelle
#endif
