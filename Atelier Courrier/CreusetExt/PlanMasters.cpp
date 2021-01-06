//
// Fichier: CPlanMasters.cpp
// Auteur:  Alain Chambard & Eddie Gent
// Date:    18/08/2003
// Classe stockage des diférents étapes plan master
//
#include "Stdafx.h"
#include "PlanMasters.h"


// Constructeur
CPlanMasters::CPlanMasters(void)
{
	// Date du jour et hh:mm
	m_DateArchive.GetCurrentDateTime();    

	// par défaut pas de nom
	m_Libelle = "";
}

// Destructeur
CPlanMasters::~CPlanMasters(void)
{
}

// Copie operator
CPlanMasters::CPlanMasters(const CPlanMasters & rhs)
{
	*this = rhs;	
}

// Assignment operator
CPlanMasters & CPlanMasters :: operator=(const CPlanMasters & rhs)
{
	m_Plan			= rhs.m_Plan;
	m_Libelle		= rhs.m_Libelle;
	m_DateArchive	= rhs.m_DateArchive;
	return *this;
}

// Swap Plans master
CPlanMasters & CPlanMasters::Swap(CPlanMasters & rhs)
{
	// Swap plan
	m_Plan.Swap(rhs.m_Plan);

	// Swap libellé
	JLabel LibTmp;
	LibTmp				= rhs.m_Libelle;
	m_Libelle			= rhs.m_Libelle;
	rhs.m_Libelle		= LibTmp;

	// Swap date archivage
	JDateTime DateTmp;
	DateTmp				= rhs.m_DateArchive;
	m_DateArchive		= rhs.m_DateArchive;
	rhs.m_DateArchive	= DateTmp;

	return *this;
}

// Reset plan master
JVoid	CPlanMasters::Reset()
{
	//	Reset Plan Insertions
	m_Plan.Reset();

}

// Serialization / Send
JVoid	CPlanMasters::Send(JStream & Stream) const
{
	// Le plan
	m_Plan.Send(Stream);

	// Libellé du plan
	m_Libelle.Send(Stream); 

	// Les infos date/time
	m_DateArchive.Send(Stream);
}

// Serialization / Receive
JVoid	CPlanMasters::Recv(JStream & Stream)
{
	// Le plan
	m_Plan.Recv(Stream);
	
	// Le libellé du plan
	m_Libelle.Recv(Stream); 

	// Les infos date/time
	m_DateArchive.Recv(Stream);
}