// ProbaSupport.cpp: implementation of the CProbaSupport class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ProbaSupport.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProbaSupport::CProbaSupport()
{
	m_NbCasAlloue=m_NbCas=0;
	m_NrIndividu=NULL;
	m_Proba=NULL;
}

CProbaSupport::~CProbaSupport()
{
	if(m_NbCasAlloue<=0)return;
	if(m_NrIndividu) delete [] m_NrIndividu;
	if(m_Proba) delete [] m_Proba;
	m_NrIndividu=NULL;
	m_Proba=NULL;
	m_NbCasAlloue=m_NbCas=0;
	
}

void CProbaSupport::SetNbCas(unsigned short NbCas)
{
	ASSERT(m_NbCasAlloue==0);
	m_NbCasAlloue=m_NbCas=NbCas;
	if(m_NbCas<=0)return;
	m_NrIndividu=new unsigned short [NbCas];
	m_Proba=new unsigned short [NbCas];
}

void CProbaSupport::AddNouveauCas(unsigned short NrIndividu, unsigned short Proba)
{
	// Est-il nécessaire de réallouer ?
	ASSERT(m_NbCasAlloue>=m_NbCas);
	if(m_NbCasAlloue==m_NbCas)
	{
		// On réalloue 10 en plus
		m_NbCasAlloue+=10;
		unsigned short * NrIndividu=new unsigned short [m_NbCasAlloue];
		unsigned short * Proba=new unsigned short [m_NbCasAlloue];

		// On copie ceux qui existent
		for(int c=0;c<m_NbCas;c++)
		{
			NrIndividu[c]=m_NrIndividu[c];
			Proba[c]=m_Proba[c];
		}

		// on libère le vecteur
		if(m_NrIndividu) delete [] m_NrIndividu;
		if(m_Proba) delete [] m_Proba;

		// Et on place le nouveau vecteur
		m_NrIndividu=NrIndividu;
		m_Proba=Proba;
	}

	// Ajour du nouveau cas dans la proba
	m_NrIndividu[m_NbCas]=NrIndividu;
	m_Proba[m_NbCas]=Proba;
	m_NbCas++;
}

void CEverReach::SetNbCas(unsigned short NbCas,unsigned short NrSta)
{
	m_NbCas=NbCas;
	m_NrStation = NrSta;
	if(NbCas<=0)return;
	m_NrIndividu=new unsigned short [NbCas];
}

CEverReach::CEverReach()
{
	m_NbCas=0;
	m_NrIndividu=NULL;
}

CEverReach::~CEverReach()
{
	if(m_NrIndividu) delete [] m_NrIndividu;
	m_NrIndividu=NULL;
	m_NbCas=0;
}
