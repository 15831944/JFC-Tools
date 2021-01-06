
#pragma once

#include "JATPDATInsertion.h"
#include "PlanInsertions.h"

class CInsertion
{
public:
 	// Constructor
	CInsertion();
	// Copy constructor
	CInsertion(const CInsertion & rhs);
	// Assignment operator
	CInsertion & operator=(const CInsertion & rhs);
	// Assignment operator
	CInsertion & operator=(const CInsKey & rhs);
	// Assignment operator
	CInsertion & operator=(const CInsKeySFD & rhs);
	// Assignment operator
	CInsertion & operator=(const CInsKeySDF & rhs);
	// Swap Plans
	CInsertion & Swap(CInsertion & rhs);
	// Destructor
	virtual ~CInsertion();
	
	JVoid 						Reset();						// Reset insertion
	JVoid 						Send(JStream & Stream) const;	// Serialization
	JVoid 						Recv(JStream & Stream);			// Serialization

	bool						IsCouplage() const;				// Test if Couplage
	CPlanInsertions & 			SetCouplageInsertions(CPlanInsertions &Insertions); 	// Sets the Couplage insertions
	CPlanInsertions & 			GetCouplageInsertions() ; 	// Recuperer les insertions du couplage
	CInsertion & 				operator++();					// Add an Insertion
	CInsertion &				operator+=(const CInsertion & rhs);
	CInsertion & 				operator--();					// Remove an Insertion
	bool						operator==(const CInsKeySDF &rhs); // Test Egalité
	bool 						operator==(const CInsKeySFD &rhs); // Test Egalité
	bool						operator==(const CInsertion &rhs); // Test Egalité
	bool						HasInsertions() const;			// Check if any insertions
	
	// Elements servant pour les clés
	JUnt32				m_IdSupport; 	// 	Support Id
	JUnt32				m_IdParent;		//	Parent Id > 0 = Support Id Couplage
	JUnt32				m_IdFormat;		//	Format Id
	JDate				m_DateD;		// 	Date Debut
	
	// Data insertion
	JUnt32				m_NbInsertions;	//	Nombre d'insertions

	JUnt32				m_Duree;		//	Duree de l'insertion
	CPlanInsertions		m_Couplage; 	//  Seulement si Couplage

	JATPDATInsertion	m_Data;			//	Data
	
};

