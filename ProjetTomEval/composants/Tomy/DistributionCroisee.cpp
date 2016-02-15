// DistributionCroisee.cpp: implementation of the CDistributionCroisee class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DistributionCroisee.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDistributionCroisee::CDistributionCroisee()
{
	ResetAttributs();
}

CDistributionCroisee::~CDistributionCroisee()
{
	FreeMemory();
}

void CDistributionCroisee::AllocMemory()
{
	FreeMemory();
	ASSERT(m_NbClasse1>0);
	ASSERT(m_NbClasse2>0);
	m_Distribution=new double * [m_NbClasse1];
	for(int i=0;i<m_NbClasse1;i++)
		m_Distribution[i]=new double [m_NbClasse2];
}

void CDistributionCroisee::FreeMemory()
{
	if (m_Distribution==NULL) return;

	for(int i=0;i<m_NbClasse1;i++)
		delete [] m_Distribution[i];
	delete [] m_Distribution;

	ResetAttributs();
}

void CDistributionCroisee::ResetAttributs()
{
	m_NbClasse1=0;
	m_NbClasse2=0;
	m_Distribution=NULL;
}

double CDistributionCroisee::GetValue(int x, int y)
{
	ASSERT(m_Distribution);
	if(x>=m_NbClasse1 || x<0)
	{
		AfxThrowUserException();
		return 0;
	}
	if(y>=m_NbClasse2 || y<0)
	{
		AfxThrowUserException();
		return 0;
	}
	return m_Distribution[x][y];
}

void CDistributionCroisee::SetValue(int x, int y,double value)
{
	ASSERT(m_Distribution);
	if(x>=m_NbClasse1 || x<0)
	{
		AfxThrowUserException();
		return;
	}
	if(y>=m_NbClasse2 || y<0)
	{
		AfxThrowUserException();
		return;
	}
	m_Distribution[x][y]=value;
}

void CDistributionCroisee::AddValue(int x, int y,double value)
{
	ASSERT(m_Distribution);
	if(x>=m_NbClasse1 || x<0)
	{
		AfxThrowUserException();
		return;
	}
	if(y>=m_NbClasse2 || y<0)
	{
		AfxThrowUserException();
		return;
	}
	m_Distribution[x][y]+=value;
}

void CDistributionCroisee::CreateNewDistribution(int NbX, int NbY)
{
	if((NbX==m_NbClasse1) && (NbY==m_NbClasse2))return;
	FreeMemory();
	m_NbClasse1=NbX;
	m_NbClasse2=NbY;
	AllocMemory();
}

void CDistributionCroisee::CreateNewDistribution(int NbElementXY)
{
	CreateNewDistribution(NbElementXY,NbElementXY);
}

void CDistributionCroisee::CreateNewDistribution(const int NbX,const int NbY,const double ** Distribution)
{
	CreateNewDistribution(NbX,NbY);
	CopyDistribution(Distribution);
}

void CDistributionCroisee::CreateNewDistribution(CDistributionCroisee & CrossDistrib)
{
	CreateNewDistribution(CrossDistrib.GetNbElement(0),CrossDistrib.GetNbElement(1));
	CopyDistribution(CrossDistrib);
}

void CDistributionCroisee::CopyDistribution(const double **Distribution)
{
	for(int x=0;x<m_NbClasse1;x++)
	{
		for(int y=0;y<m_NbClasse2;y++)
		{
			SetValue(x,y,Distribution[x][y]);
		}
	}
}

void CDistributionCroisee::CopyDistribution(CDistributionCroisee & CrossDistrib)
{
	for(int x=0;x<m_NbClasse1;x++)
	{
		for(int y=0;y<m_NbClasse2;y++)
		{
			SetValue(x,y,CrossDistrib.GetValue(x,y));
		}
	}
}

void CDistributionCroisee::Initialise()
{
	for(int x=0;x<m_NbClasse1;x++)
	{
		for(int y=0;y<m_NbClasse2;y++)
		{
			m_Distribution[x][y]=0;
		}
	}
}

CDistributionCroisee & CDistributionCroisee::operator*=(const double coeff)
{
	for(int x=0;x<m_NbClasse1;x++)
	{
		for(int y=0;y<m_NbClasse2;y++)
		{
			m_Distribution[x][y]*=coeff;
		}
	}
	return * this;
}

CDistributionCroisee & CDistributionCroisee::operator/=(const double coeff)
{
	if(coeff==0)
	{
		AfxMessageBox("Erreur de calcul: division par zéro dans la classe CDistributionCroisee !");
		return * this;
	}
	const double diviseur=1/coeff;
	return((*this)*=diviseur);
}

double CDistributionCroisee::CalculCouverture_nPlus(int n)
{
	double result=0;
	for(int x=0;x<m_NbClasse1;x++)
	{
		for(int y=0;y<m_NbClasse2;y++)
		{
			if (x+y>=n)
				result+=m_Distribution[x][y];
		}
	}
	return result;
}


void CDistribution::RedresseDistribution(double Coefficient)
{
	double sigma=CalculCouverture_nPlus(0);

	for (int i=1;i<m_Distribution.GetSize();i++)
	{
		double v=m_Distribution[i]*Coefficient;
		sigma-=v;
		m_Distribution[i]=v;
	}
	m_Distribution[0]=sigma;
}

void CDistributionCroisee::GetAsText(CString &txt)
{
	txt="";
	for(int x=0;x<m_NbClasse1;x++)
	{
		for(int y=0;y<m_NbClasse2;y++)
		{
			CString val;
			val.Format("%6.3f	",m_Distribution[x][y]);
			txt+=val;
		}
		txt+="\n";
	}
}

void CDistributionCroisee::TraceContent()
{
#ifdef _DEBUG
	CString txt;
	for(int x=0;x<m_NbClasse1;x++)
	{
		txt="";
		for(int y=0;y<m_NbClasse2;y++)
		{
			CString val;
			val.Format("%6.3f	",m_Distribution[x][y]);
			txt+=val;
		}
		txt+="\n";
		TRACE(txt);
	}
#endif
}

void CDistribution::RecalculNonTouche()
{
	m_Distribution[0]=100-CalculCouverture_nPlus(1);
}
