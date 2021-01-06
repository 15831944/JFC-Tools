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

// l'op�rateur d'affectation	
JDateTime & JDateTime::operator=(const JDateTime & DateTime)
{
	m_Date = DateTime.m_Date;
	m_Time = DateTime.m_Time; 
	return *this;
}


// les fonctions pour s�rialiser la date et l'heure
JVoid JDateTime::Send(JStream & Stream) const
{
	// S�rialise date
	m_Date.Send(Stream);
	// S�rialise Time
	m_Time.Send(Stream); 
}

JVoid JDateTime::Recv(JStream & Stream)
{
	// S�rialise date
	m_Date.Recv(Stream);
	// S�rialise Time
	m_Time.Recv(Stream); 
}

// la fonction pour tester la date et l'heure
JBool JDateTime::IsValid() const
{
	if (m_Date.IsValid() && m_Time.IsValid())
		return true;
	return false;
}

// R�cup�ration date et heure syst�me
void JDateTime::GetCurrentDateTime()
{
	// Date et Time courant
	COleDateTime DateTimeSyst = COleDateTime::GetCurrentTime();
	
	// R�cup�re la date
	m_Date.SetDate(DateTimeSyst.GetDay(), DateTimeSyst.GetMonth(), DateTimeSyst.GetYear());

	// R�cup�re heure/minute/seconde
	m_Time.SetTime(DateTimeSyst.GetHour(), DateTimeSyst.GetMinute() , DateTimeSyst.GetSecond());   

	//SYSTEMTIME DateTimeSyst;
	//::GetCurrentTime(&DateTimeSyst);
	//// R�cup�re la date
	//m_Date.SetDate((JInt32)DateTimeSyst.wDay, (JInt32)DateTimeSyst.wMonth, (JInt32)DateTimeSyst.wYear);

	//// R�cup�re heure/minute/seconde
	//m_Time.SetTime((JInt32)DateTimeSyst.wHour, (JInt32)DateTimeSyst.wMinute , (JInt32)DateTimeSyst.wSecond);   

}


// Op�rateurs ajout Jours
JDateTime JDateTime:: operator +(JInt32 NbDays) const
{
	JDateTime DateTime;
	DateTime.m_Date  = m_Date + NbDays;
	DateTime.m_Time  = m_Time;
	return DateTime;
}


// Op�rateurs suppression Jours
JDateTime JDateTime:: operator -(JInt32 NbDays) const
{
	JDateTime DateTime;
	DateTime.m_Date  = m_Date - NbDays;
	DateTime.m_Time  = m_Time;
	return DateTime;
}

// les op�rateurs d'incr�mentation et de d�cr�mentation
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

// Op�rateur de comparaison
JBool JDateTime::operator ==(const JDateTime & Reference) const
{
	return (m_Date == Reference.m_Date && m_Time == Reference.m_Time);
}

// Op�rateur de diff�rence
JBool JDateTime::operator !=(const JDateTime & Reference) const
{
	// Test date et heure
	return (m_Date != Reference.m_Date || m_Time != Reference.m_Time); 
}

// Op�rateur Inf�riorit� stricte
JBool JDateTime::operator < (const JDateTime & Reference) const
{
	// Test date et heure
	if (m_Date < Reference.m_Date && m_Time < Reference.m_Time) 
		return true;
	
	return false;
}

// Op�rateur Inf�riorit�
JBool JDateTime::operator <=(const JDateTime & Reference) const
{
	// Test date et heure
	if (m_Date < Reference.m_Date) return true;
	if (m_Date > Reference.m_Date) return false;

	return (m_Time <= Reference.m_Time); 
}

// Op�rateur Sup�riorit�
JBool JDateTime::operator > (const JDateTime & Reference) const
{
	// Test date et heure
	if (m_Date > Reference.m_Date) return true;
	if (m_Date < Reference.m_Date) return false;

	return (m_Time > Reference.m_Time); 
}

// Op�rateur Sup�riorit� stricte
JBool JDateTime::operator >=(const JDateTime & Reference) const
{
	// Test date et heure
	if (m_Date > Reference.m_Date) return true;
	if (m_Date < Reference.m_Date) return false;

	return (m_Time >= Reference.m_Time); 
}
	
// la fonction pour r�initialiser la date
JVoid JDateTime::Reset()
{
	// Reset date
	m_Date.Reset();

	// Reset Time
	m_Time.Reset();

}

// R�cup�ration Date
JDate & JDateTime::GetDate()
{
	return m_Date; 
}

// R�cup�ration Time
JTime & JDateTime::GetTime()
{
	return m_Time; 
}
