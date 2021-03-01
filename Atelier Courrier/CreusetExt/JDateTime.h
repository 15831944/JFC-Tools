#pragma once
#include "JDate.h"
#include "JTime.h"

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JLIB_JDATETIME_H

// on définit la macro pour éviter les inclusions multiples
#define JLIB_JDATETIME_H

class JDateTime
{
public:
	// les constructeurs
	JDateTime();
	JDateTime(const JDateTime & Source);
	JDateTime(JInt32 Day, JInt32 Month, JInt32 Year, JInt32 Hour, JInt32 Minute, JInt32 Second);

	// les fonctions pour sérialiser la date et l'heure
	JVoid Send(JStream & Stream) const;
	JVoid Recv(JStream & Stream);

	// la fonction pour tester la date et l'heure
	JBool IsValid() const;

	// l'opérateur d'affectation	
	JDateTime & operator=(const JDateTime & DateTime);

	// Récupération de la date system
	void GetCurrentDateTime();

	// les opérateurs arithmétiques
	// JDateTime operator +(JInt32 NbDays) const;
	// JDateTime operator -(JInt32 NbDays) const;
	
	// les opérateurs d'incrémentation et de décrémentation
	JDateTime & operator +=(JInt32 NbDays);
	JDateTime & operator -=(JInt32 NbDays);
	JDateTime & operator ++();
	JDateTime & operator --();
	JDateTime & operator ++(int);
	JDateTime & operator --(int);
	JDateTime operator +(JInt32 NbDays) const;
	JDateTime operator -(JInt32 NbDays) const;

	// les opérateurs de comparaison
	JBool operator ==(const JDateTime & Reference) const;
	JBool operator !=(const JDateTime & Reference) const;
	JBool operator < (const JDateTime & Reference) const;
	JBool operator <=(const JDateTime & Reference) const;
	JBool operator > (const JDateTime & Reference) const;
	JBool operator >=(const JDateTime & Reference) const;
	
	// la fonction pour réinitialiser la date
	JVoid Reset();

	// Récupération Date
	JDate & GetDate();

	// Récupération Time
	JTime & GetTime();

	// le destructeur
	~JDateTime();


private:
	JDate	m_Date;
	JTime	m_Time;
};

// fin de l'inclusion conditionnelle
#endif
