
#pragma once

class CInsertion;
class CInsKeySDF;
#include "JATPPlateauSupports.h"

class CInsKey
 {
 public:
 	// Constructor
 	CInsKey()
 	{
 		m_IdSupport	=	0;
 		m_IdParent	=	0;
 		m_IdFormat	=	0;
		m_DateD		=	JDate::GetMin();
 	}
	CInsKey &			operator=(const CInsertion &rhs);
	virtual JVoid 		Send(JStream & Stream) const;	// Serialization
	virtual JVoid 		Recv(JStream & Stream);			// Serialization
 	
 	JUnt32	m_IdSupport; 	// 	Support Id
 	JUnt32	m_IdParent;		//	Parent Id
	JUnt32	m_IdFormat;		//	Format Id
	JDate	m_DateD;		// 	Date Debut
 };

// Search Key Support, Format & Date
class CInsKeySFD : public CInsKey
 {
 public:
 	CInsKeySFD & operator=(const CInsertion &rhs);
 	CInsKeySFD & operator=(const CInsKeySFD &rhs);
 	CInsKeySFD & operator=(const CInsKeySDF &rhs);
 	CInsKeySFD & operator=(const CInsKey &rhs);
 	// les opérateurs pour comparer les éléments
	JBool 		operator <(const CInsKeySFD & rhs) const;
	JBool 		operator >(const CInsKeySFD & rhs) const;
};
 
 // Search Key Support, Date & Format
 class CInsKeySDF : public CInsKey
 {
 public:
 	CInsKeySDF & operator=(const CInsertion &rhs);
 	CInsKeySDF & operator=(const CInsKeySDF &rhs);
 	CInsKeySDF & operator=(const CInsKeySFD &rhs);
 	CInsKeySDF & operator=(const CInsKey &rhs);
 	// les opérateurs pour comparer les éléments
	JBool 		operator <(const CInsKeySDF & rhs) const;
	JBool 		operator >(const CInsKeySDF & rhs) const;
 };

class CPlanInsertions
{
public:
 	// Constructor
	CPlanInsertions();
	// Copy constructor
	CPlanInsertions(const CPlanInsertions & rhs);
	// Assignment operator
	CPlanInsertions & operator=(const CPlanInsertions & rhs);
	// Operateur de comparaison
	JBool operator ==(const CPlanInsertions & rhs) const;
	
	// Swap Plans
	CPlanInsertions & Swap(CPlanInsertions & rhs);

	// Destructor
	virtual ~CPlanInsertions();
	JInt32			GetCount() const;						// Returns the Number of insertions
		
	JVoid 			Reset();								// Reset plan insertions
	JVoid 			Send(JStream & Stream) const;			// Serialization
	JVoid 			Recv(JStream & Stream);					// Serialization

	CInsertion * 	AddInsertion(CInsertion &Insertion, 
								 JDate &DebPeriode, 
								 JDate &FinPeriode); 	    // Adds an insertion
	
	bool			DeleteInsertion(CInsertion &Insertion,
		                            JDate &DebPeriode, 
									JDate &FinPeriode);		// Delete insertion
	bool			DeleteInsertion(CInsKey &Key, 
		                            JDate &DebPeriode, 
									JDate &FinPeriode);		// Delete insertion

	CInsertion * 	GetItem_SDF(); 							// 	Returns the Current Item
	bool			DeleteInsertion_SDF(JDate &DebPeriode,
		                                JDate &FinPeriode);	// 	Deletes the current insertion
	bool 			MoveFirst_SDF();						// 	Move First
	bool 			MoveNext_SDF();							// 	Move Next
	bool 			MoveLast_SDF();							// 	Move Last
	bool 			MovePrev_SDF();							// 	Move Previous
	bool			IsValid_SDF();							// 	Test if Current pos is valid
	bool 			MoveTo_SDF(CInsKeySDF &Key,JInt32 Move=0);	// 	Move To Direct

	CInsertion * 	GetItem_SFD();							// 	Returns the Current Item
	bool			DeleteInsertion_SFD(JDate &DebPeriode,
		                                JDate &FinPeriode); // 	Deletes the Current insertion
	bool 			MoveFirst_SFD();						// 	Move First
	bool 			MoveNext_SFD();							// 	Move Next
	bool 			MoveLast_SFD();							// 	Move Last
	bool 			MovePrev_SFD();							// 	Move Previous
	bool			IsValid_SFD();							// 	Test if Current pos is valid
	bool 			MoveTo_SFD(CInsKeySFD &Key,JInt32 Move=0);	// 	Move To Direct
	
	// Returns a list of insertions with the given date, support and parent
	// this does not take into account the Formats
	bool			FindInsertions(CInsKey & Key, JList<CInsKey> & List);
	bool			FindInsertions(CInsKey & Key, JDate DateF, JList<CInsKey> & List);
	CInsertion *	FindInsertion(CInsKey & Key);

	// Recherche insertion sur une période (indépendamment des formats)
	bool			FindPAInsertions(CInsKey & KeySupp,JDate DateDebut,JDate DateFin);
	
	JLabel GetLabel() const;

	void SetLabel(JLabel lbl);

public:
	JVoid SuppAllInsertion(LONG IdSupport,LONG IdFormat, JDate &DebPeriode, JDate &FinPeriode);

	JBool SuppLastInsertion(LONG IdSupport,LONG IdFormat, JDate &DebPeriode, JDate &FinPeriode);

	void GetNbInsertSupport(CKeySupport Support, LONG IdFormat, JDate DebPeriode, JDate FinPeriode, JInt32 & NbInsertions, JInt32 & NbInsertCpl, bool & bCpl);
	void GetNbInsertSupport(CKeySupport Support, JDate DebPeriode, JDate FinPeriode, JInt32 & NbInsertions, JInt32 & NbInsertCpl, bool & bCpl);

	void GetNbInsert(LONG IdFormat, JDate DebPeriode, JDate FinPeriode, JInt32 & NbInsertions, JInt32 & NbInsertCpl, bool & bCpl);
	void GetNbInsert(JDate DebPeriode, JDate FinPeriode, JInt32 & NbInsertions);
	void GetNbInsert(JDate DebPeriode, JDate FinPeriode, JInt32 & NbInsertions, JInt32 & NbInsertCpl, bool & bCpl);

private:
	JMap< CInsKeySFD,CInsertion *> 	m_KeySFDMap;	//	Map SFD d'insertions (Support->Format->Date)
	JMap< CInsKeySDF,CInsertion *> 	m_KeySDFMap;	// 	Map SDF d'insertions (Support->Date->Format)

	JList <CInsertion *> m_ListChronoInsertion;		//  Liste des insertions par ordre de création 


	JLabel m_label;	//Le nom de l'hypothèse

	// Ajout insertion à la liste création
	bool			AjouterInsertionListeCreation(CInsertion Insertion);
	// Vire l'insertion de la liste création
	bool			DeleteInsertionListeCreation(CInsertion *pInsertion);
};


typedef  JList<CPlanInsertions> LIST_HYPOTHESES;
