//
// Fichier: JTime.h
// Auteur:  Sylvain SAMMURI
// Date:    04/06/2002
//

// on d�bute l'inclusion conditionnelle
#ifndef JLIB_TIME_H
#define JLIB_TIME_H

// on inclut les d�finitions n�cessaires
#include "..\Include\JTypes.h"

// on inclut les d�finitions n�cessaires
#include "..\Include\JStream.h"

class JTime
{
public:
	// les constructeurs
	JTime();
	JTime(const JTime & Source);
	JTime(JInt32 Hour, JInt32 Minute, JInt32 Second);

	// la fonction pour tester l'heure
	JBool IsValid() const;

	// la fonction pour r�cup�rer la cl� de hashage
	JInt32 GetHashKey(JInt32 Size) const;

	// les fonctions pour s�rialiser l'heure
	JVoid Send(JStream & Stream) const;
	JVoid Recv(JStream & Stream);

	// les fonctions pour manipuler l'heure
	JVoid SetTime(JInt32   Hour, JInt32   Minute, JInt32   Second);
	JVoid GetTime(JInt32 & Hour, JInt32 & Minute, JInt32 & Second) const;

	// les fonctions pour r�cup�rer les heures particuli�res
	JTime GetTimeOfHour() const;
	JTime GetTimeOfMinute() const;

	// l'op�rateur d'affectation
	JTime & operator =(const JTime & Source);

	// les op�rateurs arithm�tiques
	JTime operator +(JInt32 NbSeconds) const;
	JTime operator -(JInt32 NbSeconds) const;

	// l'op�rateur de soustraction de deux heures
	JInt32 operator -(const JTime & Reference) const;

	// les op�rateurs d'incr�mentation et de d�cr�mentation
	JTime & operator +=(JInt32 NbSeconds);
	JTime & operator -=(JInt32 NbSeconds);
	JTime & operator ++();
	JTime & operator --();
	JTime & operator ++(int);
	JTime & operator --(int);

	// les op�rateurs de comparaison
	JBool operator ==(const JTime & Reference) const;
	JBool operator !=(const JTime & Reference) const;
	JBool operator < (const JTime & Reference) const;
	JBool operator <=(const JTime & Reference) const;
	JBool operator > (const JTime & Reference) const;
	JBool operator >=(const JTime & Reference) const;

	// la fonction pour r�initialiser l'heure
	JVoid Reset();

	// le destructeur
	~JTime();

public:
	// les fonctions pour r�cup�rer les bornes
	static JTime GetMin();
	static JTime GetMax();

public:
	// la fonction pour tester la validit� d'une heure
	static JBool Check(JInt32 Hour, JInt32 Minute, JInt32 Second);

private:
	// le constructeur
	JTime(JInt32 Index);

private:
	// la fonction pour v�rifier les secondes
	JBool OnCheckNbSeconds(JInt32 NbSeconds) const;

private:
	// les fonctions pour manipuler les indices
	JVoid OnComputeTimeToIndex(JInt32 Hour, JInt32 Minute, JInt32 Second, JInt32 & Index) const;
	JVoid OnComputeIndexToTime(JInt32 Index, JInt32 & Hour, JInt32 & Minute, JInt32 & Second) const;

private:
	// les fonctions pour ajouter des secondes
	JBool OnAddSecond(JInt32 & Index, JInt32 NbSeconds) const;
	JBool OnSubSecond(JInt32 & Index, JInt32 NbSeconds) const;

private:
	JInt32 m_Index; // l'indice de l'heure
};

// on finit l'inclusion conditionnelle
#endif
