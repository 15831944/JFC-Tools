//
// Fichier: JDate.h
// Auteur:  Sylvain SAMMURI
// Date:    04/06/2002
//

// on d�bute l'inclusion conditionnelle
#ifndef JLIB_DATE_H
#define JLIB_DATE_H

// on inclut les d�finitions n�cessaires
#include "..\Include\JTypes.h"

// on inclut les d�finitions n�cessaires
#include "..\Include\JStream.h"

class JDate
{
public:
	// les constructeurs
	JDate();
	JDate(const JDate & Source);
	JDate(JInt32 Day, JInt32 Month, JInt32 Year);

	// la fonction pour tester la date
	JBool IsValid() const;

	// la fonction pour r�cup�rer la cl� de hashage
	JInt32 GetHashKey(JInt32 Size) const;

	// les fonctions pour s�rialiser la date
	JVoid Send(JStream & Stream) const;
	JVoid Recv(JStream & Stream);

	// les fonctions pour manipuler la date
	JVoid SetDate(JInt32   Day, JInt32   Month, JInt32   Year);
	JVoid GetDate(JInt32 & Day, JInt32 & Month, JInt32 & Year) const;

	// la fonction pour r�cup�rer le jour de la semaine
	JInt32 GetDayOfWeek() const;

	// la fonction pour r�cup�rer le num�ro de la semaine
	JVoid GetNoOfWeek(JInt32 & Week, JInt32 & Year) const;

	// les fonctions pour r�cup�rer les dates particuli�res
	JDate GetDateOfWeek() const;
	JDate GetDateOfMonth() const;
	JDate GetDateOfYear() const;

	// l'op�rateur d'affectation
	JDate & operator =(const JDate & Source);

	// les op�rateurs arithm�tiques
	JDate operator +(JInt32 NbDays) const;
	JDate operator -(JInt32 NbDays) const;

	// l'op�rateur de soustraction de deux dates
	JInt32 operator -(const JDate & Reference) const;

	// les op�rateurs d'incr�mentation et de d�cr�mentation
	JDate & operator +=(JInt32 NbDays);
	JDate & operator -=(JInt32 NbDays);
	JDate & operator ++();
	JDate & operator --();
	JDate & operator ++(int);
	JDate & operator --(int);

	// les op�rateurs de comparaison
	JBool operator ==(const JDate & Reference) const;
	JBool operator !=(const JDate & Reference) const;
	JBool operator < (const JDate & Reference) const;
	JBool operator <=(const JDate & Reference) const;
	JBool operator > (const JDate & Reference) const;
	JBool operator >=(const JDate & Reference) const;
	
	// la fonction pour r�initialiser la date
	JVoid Reset();

	// le destructeur
	~JDate();

public:
	// les fonctions pour r�cup�rer les bornes
	static JDate GetMin();
	static JDate GetMax();

public:
	// la fonction pour tester la validit� d'une date
	static JBool Check(JInt32 Day, JInt32 Month, JInt32 Year);

private:
	// le constructeur
	JDate(JInt32 Index);

private:
	// la fonction pour v�rifier les jours
	JBool OnCheckNbDays(JInt32 NbDays) const;

private:
	// les fonctions pour manipuler les indices
	JVoid OnComputeDateToIndex(JInt32 Day, JInt32 Month, JInt32 Year, JInt32 & Index) const;
	JVoid OnComputeIndexToDate(JInt32 Index, JInt32 & Day, JInt32 & Month, JInt32 & Year) const;

private:
	// les fonctions pour ajouter des jours
	JBool OnAddDay(JInt32 & Index, JInt32 NbDays) const;
	JBool OnSubDay(JInt32 & Index, JInt32 NbDays) const;

private:
	JInt32 m_Index; // l'indice de la date
};

// on finit l'inclusion conditionnelle
#endif
