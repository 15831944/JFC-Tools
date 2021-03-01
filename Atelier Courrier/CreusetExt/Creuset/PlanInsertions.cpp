
#include "Stdafx.h"
#include "Insertion.h"
#include "PlanInsertions.h"

JVoid 	CInsKey::Send(JStream & Stream) const
{
	// Serialization
	Stream.Send(m_IdSupport); 
	Stream.Send(m_IdParent);  
	Stream.Send(m_IdFormat);  
	m_DateD.Send(Stream);
}   
    
JVoid 	CInsKey::Recv(JStream & Stream)
{
	// Serialization
	Stream.Recv(m_IdSupport); 
	Stream.Recv(m_IdParent);  
	Stream.Recv(m_IdFormat);  
	m_DateD.Recv(Stream);
}

CInsKey & CInsKey::operator=(const CInsertion &rhs)
{
	m_IdSupport	=	rhs.m_IdSupport;
	m_IdParent	=	rhs.m_IdParent;
	m_IdFormat	=	rhs.m_IdFormat;
	m_DateD		=	rhs.m_DateD;
	return *this;
}


/////////////////////////////////////////////////////////////////////////////////////


CInsKeySFD & CInsKeySFD::operator=(const CInsertion &rhs)
{
	m_IdSupport	=	rhs.m_IdSupport;
	m_IdParent	=	rhs.m_IdParent;
	m_IdFormat	=	rhs.m_IdFormat;
	m_DateD		=	rhs.m_DateD;
	return *this;
}

CInsKeySFD & CInsKeySFD::operator=(const CInsKeySFD &rhs)
{
	m_IdSupport	=	rhs.m_IdSupport;
	m_IdParent	=	rhs.m_IdParent;
	m_IdFormat	=	rhs.m_IdFormat;
	m_DateD		=	rhs.m_DateD;
	return *this;
}

CInsKeySFD & CInsKeySFD::operator=(const CInsKeySDF &rhs)
{
	m_IdSupport	=	rhs.m_IdSupport;
	m_IdParent	=	rhs.m_IdParent;
	m_IdFormat	=	rhs.m_IdFormat;
	m_DateD		=	rhs.m_DateD;
	return *this;
}

CInsKeySFD & CInsKeySFD::operator=(const CInsKey &rhs)
{
	m_IdSupport	=	rhs.m_IdSupport;
	m_IdParent	=	rhs.m_IdParent;
	m_IdFormat	=	rhs.m_IdFormat;
	m_DateD		=	rhs.m_DateD;
	return *this;
}

// les opérateurs pour comparer les éléments
JBool 	CInsKeySFD::operator <(const CInsKeySFD & rhs) const
{
	if(m_IdSupport	<	rhs.m_IdSupport) return true;
	if(m_IdSupport	>	rhs.m_IdSupport) return false;
	if(m_IdParent	<	rhs.m_IdParent) return true;
	if(m_IdParent	>	rhs.m_IdParent) return false;
	if(m_IdFormat	<	rhs.m_IdFormat) return true;
	if(m_IdFormat	>	rhs.m_IdFormat) return false;
	if(m_DateD		<	rhs.m_DateD) return true;
	return false;
}

JBool 	CInsKeySFD::operator >(const CInsKeySFD & rhs) const
{
	if(m_IdSupport	>	rhs.m_IdSupport) return true;
	if(m_IdSupport	<	rhs.m_IdSupport) return false;
	if(m_IdParent	>	rhs.m_IdParent) return true;
	if(m_IdParent	<	rhs.m_IdParent) return false;
	if(m_IdFormat	>	rhs.m_IdFormat) return true;
	if(m_IdFormat	<	rhs.m_IdFormat) return false;
	if(m_DateD		>	rhs.m_DateD) return true;
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////

CInsKeySDF & CInsKeySDF::operator=(const CInsertion &rhs)
{
	m_IdSupport	=	rhs.m_IdSupport;
	m_IdParent	=	rhs.m_IdParent;
	m_IdFormat	=	rhs.m_IdFormat;
	m_DateD		=	rhs.m_DateD;
	return *this;
}

CInsKeySDF & CInsKeySDF::operator=(const CInsKeySDF &rhs)
{
	m_IdSupport	=	rhs.m_IdSupport;
	m_IdParent	=	rhs.m_IdParent;
	m_IdFormat	=	rhs.m_IdFormat;
	m_DateD		=	rhs.m_DateD;
	return *this;
}

CInsKeySDF & CInsKeySDF::operator=(const CInsKeySFD &rhs)
{
	m_IdSupport	=	rhs.m_IdSupport;
	m_IdParent	=	rhs.m_IdParent;
	m_IdFormat	=	rhs.m_IdFormat;
	m_DateD		=	rhs.m_DateD;
	return *this;
}

CInsKeySDF & CInsKeySDF::operator=(const CInsKey &rhs)
{
	m_IdSupport	=	rhs.m_IdSupport;
	m_IdParent	=	rhs.m_IdParent;
	m_IdFormat	=	rhs.m_IdFormat;
	m_DateD		=	rhs.m_DateD;
	return *this;
}

// les opérateurs pour comparer les éléments
JBool 	CInsKeySDF::operator <(const CInsKeySDF & rhs) const
{
	if(m_IdSupport	<	rhs.m_IdSupport) return true;
	if(m_IdSupport	>	rhs.m_IdSupport) return false;
	if(m_IdParent	<	rhs.m_IdParent) return true;
	if(m_IdParent	>	rhs.m_IdParent) return false;
	if(m_DateD		<	rhs.m_DateD) return true;
	if(m_DateD		>	rhs.m_DateD) return false;
	if(m_IdFormat	<	rhs.m_IdFormat) return true;
	return false;
}

JBool 	CInsKeySDF::operator >(const CInsKeySDF & rhs) const
{
	if(m_IdSupport	>	rhs.m_IdSupport) return true;
	if(m_IdSupport	<	rhs.m_IdSupport) return false;
	if(m_IdParent	>	rhs.m_IdParent) return true;
	if(m_IdParent	<	rhs.m_IdParent) return false;
	if(m_DateD		>	rhs.m_DateD) return true;
	if(m_DateD		<	rhs.m_DateD) return false;
	if(m_IdFormat	>	rhs.m_IdFormat) return true;
	return false;
}


/////////////////////////////////////////////////////////////////////////////////////:


// Constructor
CPlanInsertions::CPlanInsertions()
{
	//	Reset Map SFD d'insertions (Support->Format->Date)
	m_KeySFDMap.Reset();	
	
	// 	Reset Map SDF d'insertions (Support->Date->Format)
	m_KeySDFMap.Reset();	

	//  Reset Liste création des insertions
	m_ListChronoInsertion.Reset(); 
}

// Copy constructor
CPlanInsertions::CPlanInsertions(const CPlanInsertions & rhs)
{
	*this = rhs;	
}

// Assignment operator
CPlanInsertions & CPlanInsertions::operator=(const CPlanInsertions & rhs)
{
	//	Copie Map SFD d'insertions (Support->Format->Date)
	m_KeySFDMap.Reset();
	m_KeySDFMap.Reset();
	m_ListChronoInsertion.Reset(); 

	// 	Copie Map SDF d'insertions (Support->Date->Format)
	for(rhs.m_KeySFDMap.MoveFirst(); rhs.m_KeySFDMap.IsValid(); rhs.m_KeySFDMap.MoveNext())
	{
		// Récup clé origine
		CInsKeySFD KeySFD			= rhs.m_KeySFDMap.GetKey();
		CInsKeySDF KeySDF; 
		KeySDF						= KeySFD;

		// Créer un nouveau pointeur insertion
		CInsertion *pInsertion		= new CInsertion(*rhs.m_KeySFDMap.GetItem());
		m_KeySFDMap.Add(KeySFD)		= pInsertion;	
		m_KeySDFMap.Add(KeySDF)		= pInsertion;
	}

	//  Copie Liste création des insertions
	for(rhs.m_ListChronoInsertion.MoveFirst(); rhs.m_ListChronoInsertion.IsValid();rhs.m_ListChronoInsertion.MoveNext())
	{
		// Créer nouveau pointeur insertion et récup info insertion
		CInsertion *pInsertion		= new CInsertion(*rhs.m_ListChronoInsertion.GetItem());

		// Ajoute élément à nouvelle liste
		m_ListChronoInsertion.AddTail() = pInsertion;
	}

	m_label = rhs.m_label;

	return *this;
}

// Operateur de comparaison
JBool CPlanInsertions::operator ==(const CPlanInsertions & rhs) const
{
	// Teste déja nb insertions
	if (m_KeySFDMap.GetCount() != rhs.m_KeySFDMap.GetCount() ||
		m_KeySDFMap.GetCount() != rhs.m_KeySDFMap.GetCount())
		return false;

	// Test chaque insertion
	for(m_KeySFDMap.MoveFirst(); m_KeySFDMap.IsValid(); m_KeySFDMap.MoveNext())
	{
		// Récup clef et insertion
		CInsKeySFD Key		= m_KeySFDMap.GetKey(); 
		CInsertion *pInsert1	= m_KeySFDMap.GetItem(); 

		// Il faut que les 2 soient identiques sur l'autre plan*
		rhs.m_KeySFDMap.MoveTo(Key);
		if (rhs.m_KeySFDMap.IsValid()) 
		{
			// Test les valeur insertions
			// A FAIRE !!!!!
			CInsertion *pInsert2 = rhs.m_KeySFDMap.GetItem();
			if (pInsert1 == pInsert2)
				return false;
		}
		else
			return false;

	}

	// Egalité parfaite
	return true;
}


// Swap Plans
CPlanInsertions & CPlanInsertions::Swap(CPlanInsertions & rhs)
{
	//	Swap Map SFD d'insertions (Support->Format->Date)
	m_KeySFDMap.Swap(rhs.m_KeySFDMap);	
	
	// 	Swap Map SDF d'insertions (Support->Date->Format)
	m_KeySDFMap.Swap(rhs.m_KeySDFMap);

	// Swap Liste Chrono Insertions
	m_ListChronoInsertion.Swap(rhs.m_ListChronoInsertion); 

	//On ne copie pas le nom de l'hypothèse
	//JLabel lbl = m_label;
	//m_label = rhs.m_label;
	//rhs.m_label = lbl;

	return *this;
}

// Destructor
CPlanInsertions::~CPlanInsertions()
{
	// Destruction des élmts Map SFD
	for (m_KeySFDMap.MoveFirst();m_KeySFDMap.IsValid();m_KeySFDMap.MoveNext())
		delete m_KeySFDMap.GetItem();

	// Destruction des élmts liste création insertions
	for (m_ListChronoInsertion.MoveFirst();m_ListChronoInsertion.IsValid();m_ListChronoInsertion.MoveNext())
		delete m_ListChronoInsertion.GetItem(); 
}

// Returns the Number of insertions
JInt32	CPlanInsertions::GetCount() const
{
	// Via le Map SFD (on pourrait le faire idem avec Map SDF)
	return m_KeySFDMap.GetCount();
}

// Reset plan insertions
JVoid	CPlanInsertions::Reset()
{
	//	Reset Map SFD d'insertions (Support->Format->Date)
	for(m_KeySFDMap.MoveFirst(); m_KeySFDMap.IsValid(); m_KeySFDMap.MoveNext())
		delete m_KeySFDMap.GetItem();

	m_KeySFDMap.Reset();	
	// 	Reset Map SDF d'insertions (Support->Date->Format)
	m_KeySDFMap.Reset();	

	// Reset Liste Création Insertion
	for(m_ListChronoInsertion.MoveFirst(); m_ListChronoInsertion.IsValid();m_ListChronoInsertion.MoveNext())
		delete m_ListChronoInsertion.GetItem();
	m_ListChronoInsertion.Reset();
}

// Serialization
JVoid	CPlanInsertions::Send(JStream & Stream) const
{
	// Les maps insertions
	JUnt32 Count  = m_KeySFDMap.GetCount();
	Stream.Send(Count);
	for(m_KeySFDMap.MoveFirst(); m_KeySFDMap.IsValid (); m_KeySFDMap.MoveNext())
	{
		CInsertion *pInsertion = m_KeySFDMap.GetItem();
		pInsertion->Send(Stream);
	}

	// la liste création chronologique insertions
	Count = m_ListChronoInsertion.GetCount(); 
	Stream.Send(Count);
	for (m_ListChronoInsertion.MoveFirst();m_ListChronoInsertion.IsValid();m_ListChronoInsertion.MoveNext())
	{
		CInsertion *pInsertion = m_ListChronoInsertion.GetItem();				
		pInsertion->Send(Stream);
	}

	m_label.Send(Stream);

}

// Serialization
JVoid	CPlanInsertions::Recv(JStream & Stream)
{
	// Les maps insertions
	JMap< CInsKeySFD,CInsertion *> 	KeySFDMap;	//	Map SFD d'insertions (Support->Format->Date)
	JMap< CInsKeySDF,CInsertion *> 	KeySDFMap;	// 	Map SDF d'insertions (Support->Date->Format)
	JList<CInsertion *> ListChronoInsertion;	//  Liste création des insertions	
	JUnt32 Count  = 0;
	Stream.Recv(Count);
	for(JUnt32 x = 0; x < Count; x++)
	{
		CInsertion *pInsertion = new CInsertion;
		pInsertion->Recv(Stream);
		CInsKeySDF KeySDF;
		KeySDF = *pInsertion;
		CInsKeySFD KeySFD;
		KeySFD = *pInsertion;
		KeySDFMap.Add(KeySDF) = pInsertion;
		KeySFDMap.Add(KeySFD) = pInsertion;
	}

	// La Liste création insertion
	Stream.Recv(Count);
	for(JUnt32 x = 0; x < Count; x++)
	{
		CInsertion *pInsertion = new CInsertion;
		pInsertion->Recv(Stream);
		ListChronoInsertion.AddTail() = pInsertion; 
	}

	m_label.Recv(Stream);

	// Swap des maps et liste
	m_KeySDFMap.Swap(KeySDFMap);  
	m_KeySFDMap.Swap(KeySFDMap);  
	m_ListChronoInsertion.Swap(ListChronoInsertion);

}

// Adds an insertion
CInsertion *	CPlanInsertions::AddInsertion(CInsertion & Insertion, JDate &DebPeriode, JDate &FinPeriode)
{
	// Bornes de la nouvelle insertion à ajouter
	JDate 		DateD = Insertion.m_DateD;
	JDate 		DateF = DateD + Insertion.m_Duree-1;
	JDate 		DateFinInsPrev = DateD-1;
	JDate 		DateDebInsNext = DateF+1;
	// Positionne la clef SFD
	CInsKeySFD KeySFD; KeySFD	= Insertion;
	CInsKeySDF KeySDF; KeySDF	= Insertion;
	CInsertion *pInsertion		= 0;
	

	bool bAdd = true;

	//    <============>...............................<===========>
	//    DDP		 DFP	     ^insertion			   DDN        DFN 	
	// recherche avant pour trouver fin insertion avant (DateFinInsPrev = DFP)
	
	
	// recherche après pour trouver début insertion apres (DateDebutInsNext = DDN)
	
	// Recherche insertion exact
	if (MoveTo_SDF(KeySDF))
	{	
		// Récup insertion 
		pInsertion = GetItem_SDF();
		if (pInsertion)
		{
			if(*pInsertion == KeySDF)
			{
				// Ajout insertion sur insertion déjà existante, c'est possible
				(*pInsertion)+=Insertion;

				// Ajout dans liste création
				AjouterInsertionListeCreation(Insertion);

				return pInsertion;
			}
		}
		return 0;
	}
	
	if (MoveTo_SDF(KeySDF,-1))
	{	
		// Récup insertion précédente
		pInsertion = GetItem_SDF();
		if (pInsertion)
		{
			if(pInsertion->m_IdSupport == KeySDF.m_IdSupport)
			{
				if(pInsertion->m_DateD == KeySDF.m_DateD)
				{
					// Insertion found with same date but different format
					DateFinInsPrev = DateD-1;
					bAdd = true;
				}
				else
					// Récup Date Fin Insertion Précédente
					DateFinInsPrev = pInsertion->m_DateD + pInsertion->m_Duree - 1;  		
			}
		}
	}		

	if(MoveTo_SDF(KeySDF,+1))
	{
		// Récup insertion suivante
		pInsertion = GetItem_SDF();
		if (pInsertion)
		{
			if(pInsertion->m_IdSupport == KeySDF.m_IdSupport)
			{
				if(pInsertion->m_DateD == KeySDF.m_DateD)
				{
					// Insertion found with same date but different format
					DateDebInsNext = DateF+1;
					bAdd = true;
				}
				else
					// Récup Date Debut Insertion next
					DateDebInsNext = pInsertion->m_DateD;  		
			}
		}
	}
		
	// il faut que DateD > DateFinInsPrev && que DateF < DateDebutInsNext 
	if (DateD > DateFinInsPrev && DateF < DateDebInsNext && bAdd)
	{
		// Création nouvelle insertion
		pInsertion = new CInsertion(Insertion);

		// Ajout insertion aux maps
		if(pInsertion->m_NbInsertions==0)
			pInsertion->m_NbInsertions=1;
		m_KeySFDMap.Add(KeySFD) = pInsertion;
		m_KeySDFMap.Add(KeySDF) = pInsertion;

		// Ajout insertion à la liste création
		AjouterInsertionListeCreation(Insertion);

		return pInsertion;
	}		 
	else
		// Pas possible chevauchement
		return 0;
}

// Ajout insertion à la liste création
bool	CPlanInsertions::AjouterInsertionListeCreation(CInsertion Insertion)
{
	CInsertion *pInsertChrono	= 0;
	
	// Créer nvlle insertion pour insérer dans liste
	pInsertChrono = new CInsertion(Insertion);

	// Ajoute à la liste
	m_ListChronoInsertion.AddTail() = pInsertChrono;

	return true;
}

// Delete insertion
bool	CPlanInsertions::DeleteInsertion(CInsertion & Insertion, JDate &DebPeriode, JDate &FinPeriode)
{
	// Positionne la clef 
	CInsKey Key;
	Key = Insertion;
	// Delete the insertion
	return DeleteInsertion(Key,DebPeriode,FinPeriode);
}

// Delete insertion
bool	CPlanInsertions::DeleteInsertion(CInsKey &Key, JDate &DebPeriode, JDate &FinPeriode)
{
	// Positionne la clef SFD
	CInsKeySDF KeySDF; KeySDF = Key;
	CInsKeySFD KeySFD; KeySFD = Key;
	if(MoveTo_SFD(KeySFD))
	{
		// Récup insertion
		CInsertion *pInsertion = GetItem_SFD();
		if (pInsertion)
		{
			// Décrémente nb insertions pour cette insertion
			--(*pInsertion);
			if(!pInsertion->HasInsertions())
			{
				// Vire l'insertion de la liste création
				DeleteInsertionListeCreation(pInsertion);

				// Ici nb insertion = 0 pour cette insertion
				m_KeySFDMap.Remove();
				if(MoveTo_SDF(KeySDF))
					m_KeySDFMap.Remove();
				delete pInsertion;
			}
			else
			{
				// Il reste des insertions pour cette insertion
				// il faut retirer cette insertion de la liste création insertion (à partir de la fin)
				DeleteInsertionListeCreation(pInsertion);
			}

			return true;
		}
	}
	return false;	
}

// Vire l'insertion de la liste création
bool CPlanInsertions::DeleteInsertionListeCreation(CInsertion *pInsertion)
{
	for (m_ListChronoInsertion.MoveLast();m_ListChronoInsertion.IsValid();m_ListChronoInsertion.MovePrev())
	{
		if (pInsertion->m_IdSupport == m_ListChronoInsertion.GetItem()->m_IdSupport &&
			pInsertion->m_IdParent  == m_ListChronoInsertion.GetItem()->m_IdParent &&
			pInsertion->m_IdFormat  == m_ListChronoInsertion.GetItem()->m_IdFormat &&
			pInsertion->m_DateD     == m_ListChronoInsertion.GetItem()->m_DateD)
		{
			// Ici on se trouve sur la dernière insertion de cette nature
			CInsertion *pInsert = m_ListChronoInsertion.GetItem(); 
			m_ListChronoInsertion.Remove(); 
			delete pInsert;
			break;
		}
	}
	return true;
}

// 	Returns the Current Item
CInsertion *	CPlanInsertions::GetItem_SDF()
{
	if(m_KeySDFMap.IsValid())
		return m_KeySDFMap.GetItem();
	else
		return 0;
}

// 	Deletes the current insertion
bool	CPlanInsertions::DeleteInsertion_SDF(JDate &DebPeriode, JDate &FinPeriode)
{
	if(m_KeySDFMap.IsValid())
	{
		CInsKeySFD KeySFD; KeySFD = m_KeySDFMap.GetKey();
		// Récup insertion
		CInsertion *pInsertion = GetItem_SDF();
		if (pInsertion)
		{
			// Test si suppression possible (date début inclue dans intervalle date période action)
			if 	(pInsertion->m_DateD >= DebPeriode && pInsertion->m_DateD <= FinPeriode) 
			{

				--(*pInsertion);
				if(!pInsertion->HasInsertions())
				{
					// Vire l'insertion de la liste création
					DeleteInsertionListeCreation(pInsertion);

					// Vire insertion dans les maps
					m_KeySDFMap.Remove();
					if(MoveTo_SFD(KeySFD))
						m_KeySFDMap.Remove();
					delete pInsertion;
				}
				else
				{
					// Il reste des insertions pour cette insertion
					// il faut retirer cette insertion de la liste création insertion (à partir de la fin)
					DeleteInsertionListeCreation(pInsertion);
				}

				return true;
			}
		}
	}

	return false;
}

// 	Move First
bool	CPlanInsertions::MoveFirst_SDF()
{
	m_KeySDFMap.MoveFirst();
	return m_KeySDFMap.IsValid();
}

// 	Move Next
bool	CPlanInsertions::MoveNext_SDF()
{
	m_KeySDFMap.MoveNext();
	return m_KeySDFMap.IsValid();
}

// 	Move Last
bool	CPlanInsertions::MoveLast_SDF()
{
	m_KeySDFMap.MoveLast();
	return m_KeySDFMap.IsValid();
}

// 	Move Previous
bool	CPlanInsertions::MovePrev_SDF()
{
	m_KeySDFMap.MovePrev();
	return m_KeySDFMap.IsValid();
}

// 	Test if Current pos is valid
bool	CPlanInsertions::IsValid_SDF()
{
	return (m_KeySDFMap.IsValid());
}

// 	Move To Direct
bool	CPlanInsertions::MoveTo_SDF(CInsKeySDF &Key,JInt32 Move)
{
	m_KeySDFMap.MoveTo(Key,Move);
	return (m_KeySDFMap.IsValid());
}

// 	Returns the Current Item
CInsertion * 	CPlanInsertions::GetItem_SFD()
{
	if(m_KeySFDMap.IsValid())
		return m_KeySFDMap.GetItem();
	else
		return 0;
}

// Delete the current insertions
bool	CPlanInsertions::DeleteInsertion_SFD(JDate &DebPeriode, JDate &FinPeriode)
{
	if(m_KeySFDMap.IsValid())
	{
		CInsKeySDF KeySDF; KeySDF = m_KeySFDMap.GetKey();

		// Récup insertion
		CInsertion *pInsertion = GetItem_SFD();

		if (pInsertion)
		{

			// Test si suppression possible (date début inclue dans intervalle date période action)
			if 	(pInsertion->m_DateD >= DebPeriode && pInsertion->m_DateD <= FinPeriode) 
			{
				--(*pInsertion);
				if(!pInsertion->HasInsertions())
				{
					// Vire de la liste création
					DeleteInsertionListeCreation(pInsertion);

					// Vire des 2 maps
					m_KeySFDMap.Remove();
					if(MoveTo_SDF(KeySDF))
						m_KeySDFMap.Remove();
					delete pInsertion;
				
				}
				else
				{
					// Il reste des insertions pour cette insertion
					// il faut retirer cette insertion de la liste création insertion (à partir de la fin)
					DeleteInsertionListeCreation(pInsertion);
				}				
				return true;
			}
		}
	}

	return false;
}

// 	Move First
bool	CPlanInsertions::MoveFirst_SFD()
{
	m_KeySFDMap.MoveFirst();
	return m_KeySFDMap.IsValid();
}

// Move Next
bool	CPlanInsertions::MoveNext_SFD()
{
	m_KeySFDMap.MoveNext();
	return m_KeySFDMap.IsValid();
}

// Move Last
bool 	CPlanInsertions::MoveLast_SFD()
{
	m_KeySFDMap.MoveLast();
	return m_KeySFDMap.IsValid();
}

// 	Move Previous
bool 	CPlanInsertions::MovePrev_SFD()
{
	m_KeySFDMap.MovePrev();
	return m_KeySFDMap.IsValid();
}

// 	Test if Current pos is valid
bool	CPlanInsertions::IsValid_SFD()
{
	return m_KeySFDMap.IsValid();
}

// 	Move To Direct
bool 	CPlanInsertions::MoveTo_SFD(CInsKeySFD &Key,JInt32 Move)
{
	m_KeySFDMap.MoveTo(Key,Move);
	return m_KeySFDMap.IsValid();
}

// Returns a list of insertions with the given date, support and parent
// this does not take into account the Formats
bool	CPlanInsertions::FindInsertions(CInsKey & Key, JList<CInsKey> & List)
{
	CInsertion *pInsertion = 0;
	CInsKeySDF KeySDF;
	KeySDF = Key;

    // Vide la liste
	if(KeySDF.m_IdParent)
	{
		CInsKey TmpKey = Key;
		TmpKey.m_IdSupport = Key.m_IdParent; 
		TmpKey.m_IdParent = 0;

		JList<CInsKey> ListP;
		if(FindInsertions(TmpKey,ListP))
		{
			for(ListP.MoveFirst(); ListP.IsValid (); ListP.MoveNext ())
			{
				CInsKeySFD KeySFD;
				KeySFD = ListP.GetItem();
				if(MoveTo_SFD(KeySFD))
				{
					pInsertion = GetItem_SFD();
					CPlanInsertions & PLNCouplage = pInsertion->GetCouplageInsertions();
					PLNCouplage.FindInsertions(Key,List);
				}
			}
			if(List.GetCount())
				return true;
			else
				return false;
		}
	}

	// Recherche insertion
	if (!MoveTo_SDF(KeySDF,-1))
		if(!MoveTo_SDF(KeySDF,+1))
			return false;

	while(IsValid_SDF())
	{	
		// Récup insertion 
		pInsertion = GetItem_SDF();
		if (pInsertion)
		{
			if(pInsertion->m_IdSupport == KeySDF.m_IdSupport)
			{
				// voir si dans intervalle date
				if (KeySDF.m_DateD >= pInsertion->m_DateD &&
					KeySDF.m_DateD < (pInsertion->m_DateD + pInsertion->m_Duree))
				{
					List.AddTail() = *pInsertion; 	
				}
			}
			else
			{
				if(pInsertion->m_IdSupport > KeySDF.m_IdSupport ||
				    pInsertion->m_DateD > KeySDF.m_DateD)
					break;
			}
		}
		MoveNext_SDF();
	}

	if(List.GetCount())
		return true;
	return false;
}

// Returns a list of insertions with the given date, support and parent
// this does not take into account the Formats
bool	CPlanInsertions::FindInsertions(CInsKey & Key, JDate DateF, JList<CInsKey> &List)
{
	CInsertion *pInsertion = 0;
	CInsKeySDF KeySDF;
	KeySDF = Key;

    // Vide la liste
	if(KeySDF.m_IdParent)
	{
		CInsKey TmpKey = Key;
		TmpKey.m_IdSupport = Key.m_IdParent; 
		TmpKey.m_IdParent = 0;

		JList<CInsKey> ListP;
		if(FindInsertions(TmpKey, DateF, ListP))
		{
			for(ListP.MoveFirst(); ListP.IsValid (); ListP.MoveNext ())
			{
				CInsKeySFD KeySFD;
				KeySFD = ListP.GetItem();
				if(MoveTo_SFD(KeySFD))
				{
					pInsertion = GetItem_SFD();
					CPlanInsertions & PLNCouplage = pInsertion->GetCouplageInsertions();
					PLNCouplage.FindInsertions(Key, DateF, List);
				}
			}
			if(List.GetCount())
				return true;
			else
				return false;
		}
	}

	// Recherche insertion
	//if (!MoveTo_SDF(KeySDF,-1))
	//	if(!MoveTo_SDF(KeySDF,+1))
	//		return false;

	//if(!MoveTo_SDF(KeySDF));
	//	if(!MoveTo_SDF(KeySDF,-1))
	//		MoveTo_SDF(KeySDF,+1);
	//while(IsValid_SDF())
	//{	
	//	// Récup insertion 
	//	pInsertion = GetItem_SDF();
	//	if (pInsertion)
	//	{
	//		if(pInsertion->m_IdSupport == KeySDF.m_IdSupport)
	//		{
	//			// voir si dans intervalle date
	//			/*
	//			if (pInsertion->m_DateD >= KeySDF.m_DateD &&
	//				(pInsertion->m_DateD + pInsertion->m_Duree-1) <= DateF)
	//			*/
	//			// !!!!!! Attention maintenant insertion présente si au moins 1 jour inclu dans période
	//			if (!(pInsertion->m_DateD > DateF || pInsertion->m_DateD + pInsertion->m_Duree-1 < KeySDF.m_DateD))
	//			{
	//				List.AddTail() = *pInsertion; 	
	//			}
	//		}
	//		else
	//		{
	//			if(pInsertion->m_IdSupport > KeySDF.m_IdSupport ||
	//			    pInsertion->m_DateD > DateF)
	//				break;
	//		}
	//	}
	//	MoveNext_SDF();
	//}

	//MoveTo_SDF(KeySDF,+1);
	//while(IsValid_SDF())
	//{	
	//	// Récup insertion 
	//	pInsertion = GetItem_SDF();
	//	if (pInsertion)
	//	{
	//		if(pInsertion->m_IdSupport == KeySDF.m_IdSupport)
	//		{
	//			// voir si dans intervalle date
	//			/*
	//			if (pInsertion->m_DateD >= KeySDF.m_DateD &&
	//				(pInsertion->m_DateD + pInsertion->m_Duree-1) <= DateF)
	//			*/
	//			// !!!!!! Attention maintenant insertion présente si au moins 1 jour inclu dans période
	//			if (!(pInsertion->m_DateD > DateF || pInsertion->m_DateD + pInsertion->m_Duree-1 < KeySDF.m_DateD))
	//			{
	//				List.AddTail() = *pInsertion; 	
	//			}
	//		}
	//		else
	//		{
	//			if(pInsertion->m_IdSupport > KeySDF.m_IdSupport ||
	//			    pInsertion->m_DateD > DateF)
	//				break;
	//		}
	//	}
	//	MovePrev_SDF();
	//}


	for(MoveFirst_SDF(); IsValid_SDF(); MoveNext_SDF())
	{
		pInsertion = GetItem_SDF();
		if (pInsertion)
		{
			if(pInsertion->m_IdSupport == KeySDF.m_IdSupport)
			{
				// voir si dans intervalle date
				/*
				if (pInsertion->m_DateD >= KeySDF.m_DateD &&
					(pInsertion->m_DateD + pInsertion->m_Duree-1) <= DateF)
				*/
				// !!!!!! Attention maintenant insertion présente si au moins 1 jour inclu dans période
				//if (!(pInsertion->m_DateD > DateF || pInsertion->m_DateD + pInsertion->m_Duree-1 < Key.m_DateD))
				if( ( Key.m_DateD <=pInsertion->m_DateD+ pInsertion->m_Duree-1 && pInsertion->m_DateD+ pInsertion->m_Duree-1<= DateF)
					||( Key.m_DateD <=pInsertion->m_DateD && pInsertion->m_DateD<= DateF)
					||( pInsertion->m_DateD<=Key.m_DateD &&  pInsertion->m_DateD+ pInsertion->m_Duree-1 >= DateF )
					)
				{
					List.AddTail() = *pInsertion; 	
				}
			}
		}
	}

	if(List.GetCount())
		return true;
	return false;
}

CInsertion *	CPlanInsertions::FindInsertion(CInsKey & Key)
{
	CInsertion *pInsertion = 0;

	CInsKeySFD KeySFD;
	KeySFD = Key;

	if(MoveTo_SFD(KeySFD))
		return GetItem_SFD();

    // Voir si insertion fils
	if(Key.m_IdParent)
	{
		CInsKeySFD KeySFDP;
		KeySFDP				=	Key;
		KeySFDP.m_IdSupport	=	Key.m_IdParent;
		KeySFDP.m_IdParent	=	0;
		// Aller sur insertion parent
		if(MoveTo_SFD(KeySFDP,-1))
		{
			// Récup plan insertions parent
			CInsertion * pInsertion = GetItem_SFD();
			CPlanInsertions & PLNCouplage = pInsertion->GetCouplageInsertions();
			return PLNCouplage.FindInsertion(Key);
		}
	}
	return 0;
}

// Recherche insertion sur période action (indépendamment des formats)
bool CPlanInsertions::FindPAInsertions(CInsKey & KeySupp,JDate DateDebut,JDate DateFin)
{
	if(KeySupp.m_IdParent == 0)
	{
		// on est bien sur un support parent
		for (m_KeySFDMap.MoveFirst();m_KeySFDMap.IsValid();m_KeySFDMap.MoveNext())
		{
			// Récup insertion
			CInsertion *pInsertion = m_KeySFDMap.GetItem();
			if (pInsertion->m_IdSupport == KeySupp.m_IdSupport)
			{
				// voir si insertion en dehors de la période
				if (pInsertion->m_DateD > DateFin || (pInsertion->m_DateD + pInsertion->m_Duree -1) < DateDebut)
					continue;

				// au moins 1 insertion trouvée pour cette période
				return true;
			}
		}
	}

	// pas d'insertions trouvées
	return false;

}


JVoid CPlanInsertions::SuppAllInsertion(LONG IdSupport,LONG IdFormat, JDate &DebPeriode, JDate &FinPeriode)
{
	JBool bret;
	do
	{
		bret = SuppLastInsertion(IdSupport,IdFormat, DebPeriode, FinPeriode);
	}while(bret);

}

JBool CPlanInsertions::SuppLastInsertion(LONG IdSupport,LONG IdFormat, JDate &DebPeriode, JDate &FinPeriode)
{
	// calage sur la dernière insertion entrée de ce support
	for (m_ListChronoInsertion.MoveLast();m_ListChronoInsertion.IsValid();m_ListChronoInsertion.MovePrev())   
	{
		if (m_ListChronoInsertion.IsValid())
		{
			// Si elle existe on la supprime (attention si insertion multiple)
			CInsKey Key;
			CInsertion *pInsertion = m_ListChronoInsertion.GetItem();

			CInsKeySFD KeySFD;
			KeySFD.m_IdSupport			= pInsertion->m_IdSupport;
			KeySFD.m_IdParent			= pInsertion->m_IdParent; 
			KeySFD.m_IdFormat			= pInsertion->m_IdFormat;
			KeySFD.m_DateD				= pInsertion->m_DateD;

			// Si c'est le bon support et format
			if (pInsertion->m_IdSupport    == IdSupport 
				&& pInsertion->m_IdParent  == 0
				&& pInsertion->m_IdFormat  == IdFormat)
			{
				// On supprime cette insertion
				if (MoveTo_SFD(KeySFD,-1))
				{
					// Suppression insertion
					if (DeleteInsertion_SFD(DebPeriode,FinPeriode))
						return true;
				}
			}
		}
	}

	// si pas de suppression
	return false;
}

void CPlanInsertions::GetNbInsertSupport(CKeySupport Support, LONG IdFormat, JDate DebPeriode, JDate FinPeriode, JInt32 & NbInsertions, JInt32 & NbInsertCpl, bool & bCpl)
{
	// on crée la clé de l'insertion
	NbInsertions = 0;
	NbInsertCpl = 0;
	CInsKey Key;

	Key.m_IdSupport		=	Support.m_IdSupport;
	Key.m_IdParent		=	Support.m_IdParent; 
	Key.m_DateD			=	DebPeriode;
	Key.m_IdFormat		=	0;

	JList<CInsKey> List;

	if(FindInsertions(Key,FinPeriode,List))
	{
		// Filtre avec les formats actifs
		JList<CInsKey> Tmp;
		for(List.MoveFirst(); List.IsValid(); List.MoveNext())
		{
			if((List.GetItem().m_IdFormat == IdFormat) && (List.GetItem().m_IdParent == Support.m_IdParent))
				Tmp.AddTail() = List.GetItem();
		}

		if(Tmp.GetCount())
		{
			for(Tmp.MoveFirst(); Tmp.IsValid(); Tmp.MoveNext())
			{
				CInsKey InsKey = Tmp.GetItem();
				if(InsKey.m_IdParent)
					bCpl = true;
				CInsertion *pInsertion = FindInsertion(InsKey);
				if(pInsertion == 0)
					continue;
				NbInsertions += pInsertion->m_NbInsertions;
				if(pInsertion->IsCouplage())
				{
					CPlanInsertions & PLNInserCpl = pInsertion->GetCouplageInsertions();
					for(PLNInserCpl.MoveFirst_SDF(); PLNInserCpl.IsValid_SDF(); PLNInserCpl.MoveNext_SDF())
					{
						CInsertion *pInsCpl = PLNInserCpl.GetItem_SDF();
						if( ( DebPeriode <= pInsCpl->m_DateD && pInsCpl->m_DateD <= FinPeriode) ||
							(pInsCpl->m_DateD <= DebPeriode && DebPeriode < pInsCpl->m_DateD + pInsCpl->m_Duree) ||
							(pInsCpl->m_DateD <= FinPeriode && FinPeriode < pInsCpl->m_DateD + pInsCpl->m_Duree ))
							NbInsertCpl += pInsCpl->m_NbInsertions;
					}
				}
			}
		}
	}
}

void CPlanInsertions::GetNbInsertSupport(CKeySupport Support, JDate DebPeriode, JDate FinPeriode, JInt32 & NbInsertions, JInt32 & NbInsertCpl, bool & bCpl)
{
	// on crée la clé de l'insertion
	NbInsertions = 0;
	NbInsertCpl = 0;
	CInsKey Key;

	Key.m_IdSupport		=	Support.m_IdSupport;
	Key.m_IdParent		=	Support.m_IdParent; 
	Key.m_DateD			=	DebPeriode;
	Key.m_IdFormat		=	0;

	JList<CInsKey> List;

	if(FindInsertions(Key,FinPeriode,List))
	{
		// Filtre avec les formats actifs
		JList<CInsKey> Tmp;
		for(List.MoveFirst(); List.IsValid(); List.MoveNext())
		{
			if(List.GetItem().m_IdParent == Support.m_IdParent)
				Tmp.AddTail() = List.GetItem();
		}

		if(Tmp.GetCount())
		{
			for(Tmp.MoveFirst(); Tmp.IsValid(); Tmp.MoveNext())
			{
				CInsKey InsKey = Tmp.GetItem();
				if(InsKey.m_IdParent)
					bCpl = true;
				CInsertion *pInsertion = FindInsertion(InsKey);
				if(pInsertion == 0)
					continue;
				NbInsertions += pInsertion->m_NbInsertions;
				if(pInsertion->IsCouplage())
				{
					//NbInsertCpl += pInsertion->GetCouplageInsertions().GetCount();
					CPlanInsertions & PLNInserCpl = pInsertion->GetCouplageInsertions();
					for(PLNInserCpl.MoveFirst_SDF(); PLNInserCpl.IsValid_SDF(); PLNInserCpl.MoveNext_SDF())
					{
						CInsertion *pInsCpl = PLNInserCpl.GetItem_SDF();
						if( ( DebPeriode <= pInsCpl->m_DateD && pInsCpl->m_DateD <= FinPeriode) ||
							(pInsCpl->m_DateD <= DebPeriode && DebPeriode < pInsCpl->m_DateD + pInsCpl->m_Duree) ||
							(pInsCpl->m_DateD <= FinPeriode && FinPeriode < pInsCpl->m_DateD + pInsCpl->m_Duree ))
							NbInsertCpl += pInsCpl->m_NbInsertions;
					}
				}
			}
		}
	}
}
void CPlanInsertions::GetNbInsert(LONG IdFormat, JDate DebPeriode, JDate FinPeriode, JInt32 & NbInsertions, JInt32 & NbInsertCpl, bool & bCpl)
{
	for(MoveFirst_SDF(); IsValid_SDF(); MoveNext_SDF())
	{
		CInsertion *pInsertion = GetItem_SDF();
		if(pInsertion == 0)
			continue;
		if(pInsertion->m_IdFormat != IdFormat)
			continue;
		if( ( DebPeriode <= pInsertion->m_DateD && pInsertion->m_DateD <= FinPeriode) ||
			(pInsertion->m_DateD <= DebPeriode && DebPeriode < pInsertion->m_DateD + pInsertion->m_Duree) ||
			(pInsertion->m_DateD <= FinPeriode && FinPeriode < pInsertion->m_DateD + pInsertion->m_Duree ))
		{
			NbInsertions += pInsertion->m_NbInsertions;
			if(pInsertion->IsCouplage())
			{
				//NbInsertCpl += pInsertion->GetCouplageInsertions().GetCount();
				CPlanInsertions & PLNInserCpl = pInsertion->GetCouplageInsertions();
				for(PLNInserCpl.MoveFirst_SDF(); PLNInserCpl.IsValid_SDF(); PLNInserCpl.MoveNext_SDF())
				{
					CInsertion *pInsCpl = PLNInserCpl.GetItem_SDF();
					if( ( DebPeriode <= pInsCpl->m_DateD && pInsCpl->m_DateD <= FinPeriode) ||
						(pInsCpl->m_DateD <= DebPeriode && DebPeriode < pInsCpl->m_DateD + pInsCpl->m_Duree) ||
						(pInsCpl->m_DateD <= FinPeriode && FinPeriode < pInsCpl->m_DateD + pInsCpl->m_Duree ))
						NbInsertCpl += pInsCpl->m_NbInsertions;
				}
			}
		}
	}

}

void CPlanInsertions::GetNbInsert(JDate DebPeriode, JDate FinPeriode, JInt32 & NbInsertions)
{
	bool bCpl = false;
	JInt32 NbInsertCpl = 0;
	GetNbInsert(DebPeriode, FinPeriode, NbInsertions, NbInsertCpl, bCpl);
}

void CPlanInsertions::GetNbInsert(JDate DebPeriode, JDate FinPeriode, JInt32 & NbInsertions, JInt32 & NbInsertCpl, bool & bCpl)
{
	for(MoveFirst_SDF(); IsValid_SDF(); MoveNext_SDF())
	{
		CInsertion *pInsertion = GetItem_SDF();
		if(pInsertion == 0)
			continue;
		if( ( DebPeriode <= pInsertion->m_DateD && pInsertion->m_DateD <= FinPeriode) ||
			(pInsertion->m_DateD <= DebPeriode && DebPeriode < pInsertion->m_DateD + pInsertion->m_Duree) ||
			(pInsertion->m_DateD <= FinPeriode && FinPeriode <= pInsertion->m_DateD + pInsertion->m_Duree ))
		{
			NbInsertions += pInsertion->m_NbInsertions;
			if(pInsertion->IsCouplage())
			{
				//NbInsertCpl += pInsertion->GetCouplageInsertions().GetCount();
				CPlanInsertions & PLNInserCpl = pInsertion->GetCouplageInsertions();
				for(PLNInserCpl.MoveFirst_SDF(); PLNInserCpl.IsValid_SDF(); PLNInserCpl.MoveNext_SDF())
				{
					CInsertion *pInsCpl = PLNInserCpl.GetItem_SDF();
					if( ( DebPeriode <= pInsCpl->m_DateD && pInsCpl->m_DateD <= FinPeriode) ||
						(pInsCpl->m_DateD <= DebPeriode && DebPeriode < pInsCpl->m_DateD + pInsCpl->m_Duree) ||
						(pInsCpl->m_DateD <= FinPeriode && FinPeriode < pInsCpl->m_DateD + pInsCpl->m_Duree ))
						NbInsertCpl += pInsCpl->m_NbInsertions;
				}
			}
		}
	}
}
JLabel CPlanInsertions::GetLabel() const
{
	return m_label;
}

void CPlanInsertions::SetLabel(JLabel lbl)
{
	m_label = lbl;
}


