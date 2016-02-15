// Cible.cpp: implementation of the CCible class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Cible.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Le nombre d'individu du panel est transmis par l'appellant
CCible::CCible()
{
	ASSERT(0);
}
CCible::CCible(int NbIndivPanel)
{
	m_NbIndividuPanel = NbIndivPanel;
	m_NrCible=-1;
	m_NbCasCible=0;
	m_Population=0;
	m_CoeffEx=NULL;
}

CCible::~CCible()
{
	UnloadCible();
}

bool CCible::LoadCible(LPCSTR NomFic)
{
	//UnloadCible();

	CStdioFile FicPDX;
	if(!FicPDX.Open(NomFic,CFile::modeRead  | CFile::typeBinary) )
	{
		CString MsgErr;
		MsgErr.Format("Erreur à l'ouverture du fichier %s ",NomFic);
		AfxMessageBox(MsgErr);
		return(0);
	}

	FicPDX.Seek(m_NrCible*sizeof(long)*m_NbIndividuPanel,CFile::begin);	

	// Lecture des coefficients d'extrapollation dans le fichier
	m_CoeffEx = new long[m_NbIndividuPanel]; 

	if(!FicPDX.Read(m_CoeffEx,sizeof(long)*m_NbIndividuPanel))
	{
		CString MsgErr;
		MsgErr.Format("Erreur à la lecture des poids de la cible:%s ",NomFic);
		AfxMessageBox(MsgErr);
		return(0);
	}
	FicPDX.Close();
	

	// Calcul de la population et du nombre de cas
	for(int idxindiv=0;idxindiv<m_NbIndividuPanel;idxindiv++)
	{
		m_Population+=m_CoeffEx[idxindiv];
		if(m_CoeffEx[idxindiv])m_NbCasCible++;
	}
	
	return 1;// OK
}

bool CCible::LoadCible(LPCSTR CheminData, LPCSTR CodeVague, int NumeroCible)
{

	// Les données de la cible ont déjà été lues
	if (NumeroCible==m_NrCible) return 1; //OK

	UnloadCible();


	m_NrCible=NumeroCible;
	CString NomFic;
	NomFic.Format("%s\\%s.PDX",CheminData,CodeVague);

	return LoadCible(NomFic);

}

void CCible::UnloadCible()
{
	if (m_NrCible==-1) return;

	if(m_CoeffEx!=NULL)delete[] m_CoeffEx;	
	m_NbCasCible=0;
	m_Population=0;
	m_NrCible=-1;
}
