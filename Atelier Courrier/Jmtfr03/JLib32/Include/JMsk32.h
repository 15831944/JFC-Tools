//
// Fichier: JMsk32.h
// Auteur:  Sylvain SAMMURI
// Date:    03/03/2003
//

// on d�bute l'inclusion conditionnelle
#ifndef JLIB_MSK32_H
#define JLIB_MSK32_H

// on inclut les d�finitions n�cessaires
#include "..\Include\JTypes.h"

// on inclut les d�finitions n�cessaires
#include "..\Include\JStream.h"

class JMsk32
{
public:
	// les constructeurs
	JMsk32();
	JMsk32(JUnt32 Source);
	JMsk32(const JMsk32 & Source);

	// la fonction pour tester le masque
	JBool IsEmpty() const;

	// les fonctions pour manipuler le masque
	JVoid SetAt(JInt32 Index, JBool State);
	JBool GetAt(JInt32 Index) const;

	// les fonctions pour s�rialiser le masque
	JVoid Send(JStream & Stream) const;
	JVoid Recv(JStream & Stream);

	// la fonction pour r�cup�rer le masque
	JUnt32 AsJUnt32() const;

	// les op�rateurs d'affectation
	JMsk32 & operator =(JUnt32 Source);
	JMsk32 & operator =(const JMsk32 & Source);

	// les op�rateurs logiques
	JMsk32 operator &(const JMsk32 & Source) const;
	JMsk32 operator |(const JMsk32 & Source) const;
	JMsk32 operator ^(const JMsk32 & Source) const;

	// les op�rateurs de comparaison
	JBool operator ==(const JMsk32 & Reference) const;
	JBool operator !=(const JMsk32 & Reference) const;

	// les op�rateurs logiques
	JMsk32 & operator &=(const JMsk32 & Source);
	JMsk32 & operator |=(const JMsk32 & Source);
	JMsk32 & operator ^=(const JMsk32 & Source);

	// les op�rateurs logiques
	JMsk32 & operator &=(JBool Source);
	JMsk32 & operator |=(JBool Source);
	JMsk32 & operator ^=(JBool Source);

	// la fonction pour r�initialiser le masque
	JVoid Reset();

	// le destructeur
	~JMsk32();

private:
	JUnt32 m_Mask; // le masque
};

// on finit l'inclusion conditionnelle
#endif
