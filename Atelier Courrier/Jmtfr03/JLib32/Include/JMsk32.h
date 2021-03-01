//
// Fichier: JMsk32.h
// Auteur:  Sylvain SAMMURI
// Date:    03/03/2003
//

// on débute l'inclusion conditionnelle
#ifndef JLIB_MSK32_H
#define JLIB_MSK32_H

// on inclut les définitions nécessaires
#include "..\Include\JTypes.h"

// on inclut les définitions nécessaires
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

	// les fonctions pour sérialiser le masque
	JVoid Send(JStream & Stream) const;
	JVoid Recv(JStream & Stream);

	// la fonction pour récupérer le masque
	JUnt32 AsJUnt32() const;

	// les opérateurs d'affectation
	JMsk32 & operator =(JUnt32 Source);
	JMsk32 & operator =(const JMsk32 & Source);

	// les opérateurs logiques
	JMsk32 operator &(const JMsk32 & Source) const;
	JMsk32 operator |(const JMsk32 & Source) const;
	JMsk32 operator ^(const JMsk32 & Source) const;

	// les opérateurs de comparaison
	JBool operator ==(const JMsk32 & Reference) const;
	JBool operator !=(const JMsk32 & Reference) const;

	// les opérateurs logiques
	JMsk32 & operator &=(const JMsk32 & Source);
	JMsk32 & operator |=(const JMsk32 & Source);
	JMsk32 & operator ^=(const JMsk32 & Source);

	// les opérateurs logiques
	JMsk32 & operator &=(JBool Source);
	JMsk32 & operator |=(JBool Source);
	JMsk32 & operator ^=(JBool Source);

	// la fonction pour réinitialiser le masque
	JVoid Reset();

	// le destructeur
	~JMsk32();

private:
	JUnt32 m_Mask; // le masque
};

// on finit l'inclusion conditionnelle
#endif
