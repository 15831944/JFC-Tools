//
// Fichier: JATPKEYEtude.h
// Auteur:  Alain Chambard & Eddie Gent
// Date:    20/08/2003
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JATP_KEY_ETUDE_H

// on définit la macro pour éviter les inclusions multiples
#define JATP_KEY_ETUDE_H

// on inclut les définitions nécessaires
#include "JLib.h"

class JATPKEYEtude
{
public:
	// le constructeur
	JATPKEYEtude();

	// les fonctions pour manipuler le nom de l'étude
	CString GetLabel() const;
	JVoid SetPath(CString path);
	CString GetPath() const;

	// les fonctions pour manipuler l'identifiant du produit
	JVoid  SetIdProduit(JUnt32 IdProduit);
	JUnt32 GetIdProduit() const;

	// les fonctions pour manipuler la date de début de l'étude
	JVoid SetDateD(JDate DateD);
	JDate GetDateD() const;	

	// les fonctions pour manipuler la date de fin de l'étude
	JVoid SetDateF(JDate DateF);
	JDate GetDateF() const;

	// les fonctions pour sérialiser l'élément
//	virtual void Serialize(CArchive& ar);

	JVoid Send(JStream & Stream) const;
	JVoid Recv(JStream & Stream);

	// la fonction pour permuter les éléments
	JVoid Swap(JATPKEYEtude & Source);

	// le destructeur
	~JATPKEYEtude();

private:
	// l'opérateur pour recopier l'élément
	JATPKEYEtude & operator =(const JATPKEYEtude & Source);

	// le constructeur copie
	JATPKEYEtude(const JATPKEYEtude & Source);

private:
	CString m_EtudeName;// le nom de l'etude
	CString m_path;
	JUnt32 m_IdProduit; // l'identifiant du produit
	JDate  m_DateD;     // la date de début
	JDate  m_DateF;     // la date de fin
};

// fin de l'inclusion conditionnelle
#endif
