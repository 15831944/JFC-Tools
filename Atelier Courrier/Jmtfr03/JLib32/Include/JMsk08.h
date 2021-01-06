//
// Fichier: JMsk08.h
// Auteur:  Sylvain SAMMURI
// Date:    03/03/2003
//

// on débute l'inclusion conditionnelle
#ifndef JLIB_MSK08_H
#define JLIB_MSK08_H

// on inclut les définitions nécessaires
#include "..\Include\JTypes.h"

// on inclut les définitions nécessaires
#include "..\Include\JStream.h"

class JMsk08
{
public:
	// les constructeurs
	JMsk08();
	JMsk08(JUnt32 Source);
	JMsk08(const JMsk08 & Source);

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
	JMsk08 & operator =(JUnt32 Source);
	JMsk08 & operator =(const JMsk08 & Source);

	// les opérateurs logiques
	JMsk08 operator &(const JMsk08 & Source) const;
	JMsk08 operator |(const JMsk08 & Source) const;
	JMsk08 operator ^(const JMsk08 & Source) const;

	// les opérateurs de comparaison
	JBool operator ==(const JMsk08 & Reference) const;
	JBool operator !=(const JMsk08 & Reference) const;
	
	// les opérateurs logiques
	JMsk08 & operator &=(const JMsk08 & Source);
	JMsk08 & operator |=(const JMsk08 & Source);
	JMsk08 & operator ^=(const JMsk08 & Source);

	// les opérateurs logiques
	JMsk08 & operator &=(JBool Source);
	JMsk08 & operator |=(JBool Source);
	JMsk08 & operator ^=(JBool Source);

	// la fonction pour réinitialiser le masque
	JVoid Reset();

	// le destructeur
	~JMsk08();

private:
	JUnt08 m_Mask; // le masque
};

// on finit l'inclusion conditionnelle
#endif
