// Fichier: CPlanMasters.cpp
// Auteur:  Alain Chambard & Eddie Gent
// Date:    18/08/2003
// Classe stockage des diférents étapes plan master
//
#pragma once
#include "PlanInsertions.h"
#include "JDateTime.h"

class CPlanMasters
{
public:
	// Constructeur
	CPlanMasters(void);

	// Destructeur
	virtual ~CPlanMasters(void);

	// Copie operator
	CPlanMasters(const CPlanMasters & rhs);
	
	// Assignment operator
	CPlanMasters & operator=(const CPlanMasters & rhs);

	// Swap Plans master
	CPlanMasters & Swap(CPlanMasters & rhs);

	// Reset plan master
	JVoid Reset();

	// Serialization / Send
	JVoid Send(JStream & Stream) const;

	// Serialization / Receive
	JVoid Recv(JStream & Stream);

public:

	// Plan insertion
	CPlanInsertions				m_Plan;			

	// Nom du plan sauvegardé					(si sauvegarde volontaire)
	JLabel  					m_Libelle;

	// Date archivage (jj-mm-aa + hh-mm)		(si sauvegarde automatique)
	JDateTime					m_DateArchive;

};

typedef  JList<CPlanMasters> LIST_PLANMASTERS;
