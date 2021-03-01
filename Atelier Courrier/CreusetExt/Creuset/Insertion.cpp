#include "Stdafx.h"
#include "Insertion.h"

///////////////////////////////////////////////// CINSERTION //////////////////////////////


// Constructor
CInsertion::CInsertion()
{
	m_IdSupport		= 0; 	
	m_IdParent		= 0;		
	m_IdFormat		= 0;
	m_DateD			= JDate::GetMin();		
	                
	m_NbInsertions	= 0;		 
	m_Duree			= 0;				
	m_Data.Reset();			
	
	m_Couplage.Reset();
}	

// Copy constructor
CInsertion::CInsertion(const CInsertion & rhs)
{
	*this = rhs;
}

bool	CInsertion::operator==(const CInsKeySDF &rhs)
{
	if(m_IdSupport 	!= rhs.m_IdSupport) return false;
	if(m_IdParent 	!= rhs.m_IdParent) return false;
	if(m_IdFormat 	!= rhs.m_IdFormat) return false;
	if(m_DateD 	!= rhs.m_DateD) return false;		
	return true;
}

bool 	CInsertion::operator==(const CInsKeySFD &rhs)
{
	if(m_IdSupport 	!= rhs.m_IdSupport) return false;
	if(m_IdParent 	!= rhs.m_IdParent) return false;
	if(m_IdFormat 	!= rhs.m_IdFormat) return false;
	if(m_DateD 	!= rhs.m_DateD) return false;		
	return true;
}

bool	CInsertion::operator==(const CInsertion &rhs)
{
	if(m_IdSupport 	!= rhs.m_IdSupport) return false;
	if(m_IdParent 	!= rhs.m_IdParent) return false;
	if(m_IdFormat 	!= rhs.m_IdFormat) return false;
	if(m_DateD 	!= rhs.m_DateD) return false;		
	return true;
}



// Assignment operator
CInsertion & CInsertion::operator=(const CInsertion & rhs)
{
	m_IdSupport		= rhs.m_IdSupport; 	
	m_IdParent		= rhs.m_IdParent;		
	m_IdFormat		= rhs.m_IdFormat;
	m_DateD			= rhs.m_DateD;		
	                
	m_NbInsertions	= rhs.m_NbInsertions;		 
	m_Duree			= rhs.m_Duree;				
	m_Data			= rhs.m_Data;			
	
	m_Couplage		= rhs.m_Couplage;
	
	return *this;
}

CInsertion & CInsertion::operator=(const CInsKey & rhs)
{
	m_IdSupport		= rhs.m_IdSupport; 	
	m_IdParent		= rhs.m_IdParent;		
	m_IdFormat		= rhs.m_IdFormat;
	m_DateD			= rhs.m_DateD;		
	return *this;
}

CInsertion & CInsertion::operator=(const CInsKeySFD & rhs)
{
	m_IdSupport		= rhs.m_IdSupport; 	
	m_IdParent		= rhs.m_IdParent;		
	m_IdFormat		= rhs.m_IdFormat;
	m_DateD			= rhs.m_DateD;		
	return *this;
}

CInsertion & CInsertion::operator=(const CInsKeySDF & rhs)
{
	m_IdSupport		= rhs.m_IdSupport; 	
	m_IdParent		= rhs.m_IdParent;		
	m_IdFormat		= rhs.m_IdFormat;
	m_DateD			= rhs.m_DateD;		
	return *this;
}

// Swap Plans
CInsertion & CInsertion::Swap(CInsertion & rhs)
{
	JUnt32 Tmp;
	
	Tmp 			= m_IdSupport;
	m_IdSupport		= rhs.m_IdSupport;
	rhs.m_IdSupport	= Tmp;
	
	Tmp 			= m_IdParent;
	m_IdParent		= rhs.m_IdParent;		
	rhs.m_IdParent 	= Tmp;
	
	Tmp 			= m_IdFormat;
	m_IdFormat 		= rhs.m_IdFormat;
	rhs.m_IdFormat 	= Tmp;
	
	JDate DTmp;
	DTmp 			= m_DateD;
	m_DateD			= rhs.m_DateD;
	rhs.m_DateD 	= DTmp;
	                
	Tmp 				= m_NbInsertions;
	m_NbInsertions		= rhs.m_NbInsertions;		 
	rhs.m_NbInsertions 	= Tmp;
	
	Tmp 			= m_Duree;
	m_Duree			= rhs.m_Duree;
	rhs.m_Duree 	= Tmp;
	
	m_Data		.Swap(rhs.m_Data);			
	
	m_Couplage	.Swap(rhs.m_Couplage);
	
	return *this;
}

// Destructor
CInsertion::~CInsertion()
{
}

// Reset insertion	
JVoid CInsertion::Reset()						
{
	m_IdSupport		= 0; 	
	m_IdParent		= 0;		
	m_IdFormat		= 0;
	m_DateD			= JDate::GetMin();		
	                
	m_NbInsertions	= 0;		 
	m_Duree			= 0;				
	m_Data.Reset();			
	
	m_Couplage.Reset();
}

// Sérialise
JVoid CInsertion::Send(JStream & Stream) const
{
	Stream.Send(m_IdSupport); 	
	Stream.Send(m_IdParent);		
	Stream.Send(m_IdFormat);
	m_DateD.Send(Stream);
	                
	Stream.Send(m_NbInsertions);
	Stream.Send(m_Duree);
	m_Data.Send(Stream);
	
	m_Couplage.Send(Stream);
}

// Sérialise
JVoid CInsertion::Recv(JStream & Stream)
{
	Stream.Recv(m_IdSupport); 	
	Stream.Recv(m_IdParent);		
	Stream.Recv(m_IdFormat);
	m_DateD.Recv(Stream);
	                
	Stream.Recv(m_NbInsertions);
	Stream.Recv(m_Duree);
	m_Data.Recv(Stream);
	
	m_Couplage.Recv(Stream);
}

// Test if Couplage
bool CInsertion::IsCouplage() const
{
	if(m_Couplage.GetCount())
		return true;
	return false;
}

// Sets the Couplage insertions
CPlanInsertions & CInsertion::SetCouplageInsertions(CPlanInsertions &Insertions)
{
	m_Couplage = Insertions;
	return m_Couplage;
}

// Recuperer les insertions du couplage
CPlanInsertions & CInsertion::GetCouplageInsertions()
{
	return m_Couplage;
}


// Add an Insertion
CInsertion & CInsertion:: operator++()
{
	if(m_Couplage.GetCount())
	{
		// ici c'est un couplage 
		CPlanInsertions &PlanInsertion = GetCouplageInsertions();
		for (PlanInsertion.MoveFirst_SDF();PlanInsertion.IsValid_SDF();PlanInsertion.MoveNext_SDF())
		{
			// on récupére les fils et on incrémente
			CInsertion *pInsertion = PlanInsertion.GetItem_SDF();
			if(pInsertion)
			{
				int Count = pInsertion->m_NbInsertions/m_NbInsertions;
				for(int x=0;x< Count; x++)
					++(*pInsertion);
			}
		}							
	}		
		
	// on incrémente nb insertions du couplage parent
	m_NbInsertions++;
	
	// renvoi élmt
	return *this;
}
CInsertion & CInsertion::operator+=(const CInsertion & rhs)
{
	if(m_Couplage.GetCount())
	{
		// ici c'est un couplage 
		CPlanInsertions &PlanInsertion = GetCouplageInsertions();
		for (PlanInsertion.MoveFirst_SDF();PlanInsertion.IsValid_SDF();PlanInsertion.MoveNext_SDF())
		{
			// on récupére les fils et on incrémente
			CInsertion *pInsertion = PlanInsertion.GetItem_SDF();
			if(pInsertion)
			{
				int Count = (pInsertion->m_NbInsertions/m_NbInsertions)*rhs.m_NbInsertions;
				for(int x=0;x< Count; x++)
					++(*pInsertion);
			}
		}							
	}		
		
	// on incrémente nb insertions du couplage parent
	m_NbInsertions+= rhs.m_NbInsertions;
	
	// renvoi élmt
	return *this;
}
// Remove an Insertion
CInsertion & CInsertion:: operator--()
{
	if(m_NbInsertions)
	{
		if(m_Couplage.GetCount())
		{
			// ici c'est un couplage 
			CPlanInsertions &PlanInsertion = GetCouplageInsertions();
			for (PlanInsertion.MoveFirst_SDF();PlanInsertion.IsValid_SDF();PlanInsertion.MoveNext_SDF())
			{
				// on récupére les fils et on incrémente
				CInsertion *pInsertion = PlanInsertion.GetItem_SDF();
				if(pInsertion)
				{
					int Count = pInsertion->m_NbInsertions/m_NbInsertions;
					for(int x=0;x< Count; x++)
						--(*pInsertion);
				}
			}							
		}		
			
		// on décrémente nb insertions du couplage parent
		m_NbInsertions--;
	}
	
	// If no insertions remove all couplage insertions
	if(!m_NbInsertions)
		m_Couplage.Reset();
	
	// renvoi élmt
	return *this;
}

// Check if any insertions
bool CInsertion::HasInsertions() const
{
	if(m_NbInsertions)
		return true;
	return false;
}



