#pragma once
#include "JDate.h"
#include "JTime.h"

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JLIB_JDATETIME_H

// on d�finit la macro pour �viter les inclusions multiples
#define JLIB_JDATETIME_H

class JDateTime
{
public:
	// les constructeurs
	JDateTime();
	JDateTime(const JDateTime & Source);
	JDateTime(JInt32 Day, JInt32 Month, JInt32 Year, JInt32 Hour, JInt32 Minute, JInt32 Second);

	// les fonctions pour s�rialiser la date et l'heure
	JVoid Send(JStream & Stream) const;
	JVoid Recv(JStream & Stream);

	// la fonction pour tester la date et l'heure
	JBool IsValid() const;

	// l'op�rateur d'affectation	
	JDateTime & operator=(const JDateTime & DateTime);

	// R�cup�ration de la date system
	void GetCurrentDateTime();

	// les op�rateurs arithm�tiques
	// JDateTime operator +(JInt32 NbDays) const;
	// JDateTime operator -(JInt32 NbDays) const;
	
	// les op�rateurs d'incr�mentation et de d�cr�mentation
	JDateTime & operator +=(JInt32 NbDays);
	JDateTime & operator -=(JInt32 NbDays);
	JDateTime & operator ++();
	JDateTime & operator --();
	JDateTime & operator ++(int);
	JDateTime & operator --(int);
	JDateTime operator +(JInt32 NbDays) const;
	JDateTime operator -(JInt32 NbDays) const;

	// les op�rateurs de comparaison
	JBool operator ==(const JDateTime & Reference) const;
	JBool operator !=(const JDateTime & Reference) const;
	JBool operator < (const JDateTime & Reference) const;
	JBool operator <=(const JDateTime & Reference) const;
	JBool operator > (const JDateTime & Reference) const;
	JBool operator >=(const JDateTime & Reference) const;
	
	// la fonction pour r�initialiser la date
	JVoid Reset();

	// R�cup�ration Date
	JDate & GetDate();

	// R�cup�ration Time
	JTime & GetTime();

	// le destructeur
	~JDateTime();


private:
	JDate	m_Date;
	JTime	m_Time;
};

// fin de l'inclusion conditionnelle
#endif
