#include "stdafx.h"
#include "JDateTime.h"


// Les constructeurs
JDateTime::JDateTime()
{
}

// Constructeur copie
JDateTime::JDateTime(const JDateTime & Source)
{
	if (this == &Source) throw JInvalidCall::GetInstance();

	*this = Source;
}


// Construction par date et heure
JDateTime::JDateTime(JInt32 Day, JInt32 Month, JInt32 Year, JInt32 Hour, JInt32 Minute, JInt32 Second)
{
	// La date
	m_Date.SetDate(Day,Month,Year);

	// L'heure
	m_Time.SetTime(Hour,Minute,Second); 
}


// Destructeur
JDateTime::~JDateTime()
{
}

// l'opérateur d'affectation	
JDateTime & JDateTime::operator=(const JDateTime & DateTime)
{
	m_Date = DateTime.m_Date;
	m_Time = DateTime.m_Time; 
	return *this;
}


// les fonctions pour sérialiser la date et l'heure
JVoid JDateTime::Send(JStream & Stream) const
{
	// Sérialise date
	m_Date.Send(Stream);
	// Sérialise Time
	m_Time.Send(Stream); 
}

JVoid JDateTime::Recv(JStream & Stream)
{
	// Sérialise date
	m_Date.Recv(Stream);
	// Sérialise Time
	m_Time.Recv(Stream); 
}

// la fonction pour tester la date et l'heure
JBool JDateTime::IsValid() const
{
	if (m_Date.IsValid() && m_Time.IsValid())
		return true;
	return false;
}

// Récupération date et heure système
void JDateTime::GetCurrentDateTime()
{
	// Date et Time courant
	COleDateTime DateTimeSyst = COleDateTime::GetCurrentTime();
	
	// Récupére la date
	m_Date.SetDate(DateTimeSyst.GetDay(), DateTimeSyst.GetMonth(), DateTimeSyst.GetYear());

	// Récupére heure/minute/seconde
	m_Time.SetTime(DateTimeSyst.GetHour(), DateTimeSyst.GetMinute() , DateTimeSyst.GetSecond());   

	//SYSTEMTIME DateTimeSyst;
	//::GetCurrentTime(&DateTimeSyst);
	//// Récupére la date
	//m_Date.SetDate((JInt32)DateTimeSyst.wDay, (JInt32)DateTimeSyst.wMonth, (JInt32)DateTimeSyst.wYear);

	//// Récupére heure/minute/seconde
	//m_Time.SetTime((JInt32)DateTimeSyst.wHour, (JInt32)DateTimeSyst.wMinute , (JInt32)DateTimeSyst.wSecond);   

}


// Opérateurs ajout Jours
JDateTime JDateTime:: operator +(JInt32 NbDays) const
{
	JDateTime DateTime;
	DateTime.m_Date  = m_Date + NbDays;
	DateTime.m_Time  = m_Time;
	return DateTime;
}


// Opérateurs suppression Jours
JDateTime JDateTime:: operator -(JInt32 NbDays) const
{
	JDateTime DateTime;
	DateTime.m_Date  = m_Date - NbDays;
	DateTime.m_Time  = m_Time;
	return DateTime;
}

// les opérateurs d'incrémentation et de décrémentation
JDateTime & JDateTime:: operator +=(JInt32 NbDays)
{
	m_Date += NbDays;
	return *this;
}

JDateTime & JDateTime:: operator -=(JInt32 NbDays)
{
	m_Date -= NbDays;
	return *this;
}

JDateTime & JDateTime:: operator ++()
{
	m_Date++;
	return *this;
}

JDateTime & JDateTime:: operator --()
{
	m_Date --;
	return *this;
}

JDateTime & JDateTime:: operator ++(int)
{
	++m_Date;
	return *this;
}

JDateTime & JDateTime:: operator --(int)
{
	--m_Date;
	return *this;
}

// Opérateur de comparaison
JBool JDateTime::operator ==(const JDateTime & Reference) const
{
	return (m_Date == Reference.m_Date && m_Time == Reference.m_Time);
}

// Opérateur de différence
JBool JDateTime::operator !=(const JDateTime & Reference) const
{
	// Test date et heure
	return (m_Date != Reference.m_Date || m_Time != Reference.m_Time); 
}

// Opérateur Infériorité stricte
JBool JDateTime::operator < (const JDateTime & Reference) const
{
	// Test date et heure
	if (m_Date < Reference.m_Date && m_Time < Reference.m_Time) 
		return true;
	
	return false;
}

// Opérateur Infériorité
JBool JDateTime::operator <=(const JDateTime & Reference) const
{
	// Test date et heure
	if (m_Date < Reference.m_Date) return true;
	if (m_Date > Reference.m_Date) return false;

	return (m_Time <= Reference.m_Time); 
}

// Opérateur Supériorité
JBool JDateTime::operator > (const JDateTime & Reference) const
{
	// Test date et heure
	if (m_Date > Reference.m_Date) return true;
	if (m_Date < Reference.m_Date) return false;

	return (m_Time > Reference.m_Time); 
}

// Opérateur Supériorité stricte
JBool JDateTime::operator >=(const JDateTime & Reference) const
{
	// Test date et heure
	if (m_Date > Reference.m_Date) return true;
	if (m_Date < Reference.m_Date) return false;

	return (m_Time >= Reference.m_Time); 
}
	
// la fonction pour réinitialiser la date
JVoid JDateTime::Reset()
{
	// Reset date
	m_Date.Reset();

	// Reset Time
	m_Time.Reset();

}

// Récupération Date
JDate & JDateTime::GetDate()
{
	return m_Date; 
}

// Récupération Time
JTime & JDateTime::GetTime()
{
	return m_Time; 
}
