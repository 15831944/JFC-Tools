//
// Fichier: JMsk16.h
// Auteur:  Sylvain SAMMURI
// Date:    03/03/2003
//

// on débute l'inclusion conditionnelle
#ifndef JLIB_MSK16_H
#define JLIB_MSK16_H

// on inclut les définitions nécessaires
#include "..\Include\JTypes.h"

// on inclut les définitions nécessaires
#include "..\Include\JStream.h"

class JMsk16
{
public:
	// les constructeurs
	JMsk16();
	JMsk16(JUnt32 Source);
	JMsk16(const JMsk16 & Source);

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
	JMsk16 & operator =(JUnt32 Source);
	JMsk16 & operator =(const JMsk16 & Source);

	// les opérateurs logiques
	JMsk16 operator &(const JMsk16 & Source) const;
	JMsk16 operator |(const JMsk16 & Source) const;
	JMsk16 operator ^(const JMsk16 & Source) const;

	// les opérateurs de comparaison
	JBool operator ==(const JMsk16 & Reference) const;
	JBool operator !=(const JMsk16 & Reference) const;
	
	// les opérateurs logiques
	JMsk16 & operator &=(const JMsk16 & Source);
	JMsk16 & operator |=(const JMsk16 & Source);
	JMsk16 & operator ^=(const JMsk16 & Source);

	// les opérateurs logiques
	JMsk16 & operator &=(JBool Source);
	JMsk16 & operator |=(JBool Source);
	JMsk16 & operator ^=(JBool Source);

	// la fonction pour réinitialiser le masque
	JVoid Reset();

	// le destructeur
	~JMsk16();

private:
	JUnt16 m_Mask; // le masque
};

// on finit l'inclusion conditionnelle
#endif
