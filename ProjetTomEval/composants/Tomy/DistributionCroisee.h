// DistributionCroisee.h: interface for the CDistributionCroisee class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DISTRIBUTIONCROISEE_H__26E1222D_4DD5_4FB8_A9D4_4847B012DB68__INCLUDED_)
#define AFX_DISTRIBUTIONCROISEE_H__26E1222D_4DD5_4FB8_A9D4_4847B012DB68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDistributionCroisee  ;

class CDistribution  
{
public:
	void RecalculNonTouche();
	void RedresseDistribution(double Coefficient);
	CDistribution(){};
	CDistribution(CDistribution& Data){*this=Data;};
	virtual ~CDistribution(){};

	double CalculCouverture_nPlus(int n)
	{
		double result=0;
		for(int i=n;i<m_Distribution.GetSize();i++)
		{
			result+=m_Distribution[i];
		}
		return(result);
	}
	void Initialise()
	{
		for(int i=0;i<m_Distribution.GetSize();i++)
		{
			m_Distribution[i]=+0.0;
		}
	}

	void CreateNewDistribution(const int NbElement){m_Distribution.SetSize(NbElement);};
	void CreateNewDistribution(CDistribution & Distrib)
	{
		m_Distribution.SetSize(Distrib.GetNbElement());
		for(int e=0;e<m_Distribution.GetSize();e++)
			m_Distribution[e]=Distrib.GetValue(e);
	};
	CDistribution & operator*=(const double coeff)
	{
		for(int e=0;e<m_Distribution.GetSize();e++)
			m_Distribution[e]*=coeff;
		return *this;
	};
	CDistribution & operator/=(const double coeff)
	{
		if(coeff==0)
		{
			ASSERT(0);
			for(int e=0;e<m_Distribution.GetSize();e++)
				m_Distribution[e]=0;
		}
		else
		{
			for(int e=0;e<m_Distribution.GetSize();e++)
				m_Distribution[e]/=coeff;
		}
		return *this;
	};
	CDistribution & operator=(CDistribution & Data)
	{
		CreateNewDistribution(Data);
		return * this;
	};

	double GetValue(const int x){return(m_Distribution[x]);};
	void SetValue(const int x,const double value){m_Distribution[x]=value;};
	void AddValue(int x,double value){m_Distribution[x]+=value;};
	int GetNbElement(){return(m_Distribution.GetSize());}

	// Opération interne
private:
	void CopyDistribution(const double **Distribution);
	void CopyDistribution(CDistributionCroisee & CrossDistrib);

	void ResetAttributs();

// Attributs
private:
	CArray<double,double&>m_Distribution;
};


class CDistributionCroisee  
{
public:
	void TraceContent();
	void GetAsText(CString & txt);
	double CalculCouverture_nPlus(int n);
	void Initialise();
	CDistributionCroisee();
	virtual ~CDistributionCroisee();

	void CreateNewDistribution(int NbElementXY);
	void CreateNewDistribution(const int NbX,const int NbY);
	void CreateNewDistribution(int NbX,const int NbY,const double ** Distribution);
	void CreateNewDistribution(CDistributionCroisee & CrossDistrib);
	CDistributionCroisee & operator*=(const double coeff);
	CDistributionCroisee & operator/=(const double coeff);
	CDistributionCroisee & operator=(CDistributionCroisee & Data)
	{
		CreateNewDistribution(Data);
		return * this;
	};

	double GetValue(const int x,const int y);
	void SetValue(const int x,const int y,const double value);
	void AddValue(int x, int y,double value);
	int GetNbElement(const int Dimension){return((Dimension==0)?m_NbClasse1:m_NbClasse2);}

	// Opération interne
private:
	void CopyDistribution(const double **Distribution);
	void CopyDistribution(CDistributionCroisee & CrossDistrib);
	void ResetAttributs();
	void FreeMemory();
	void AllocMemory();


// Attributs
protected:
	int m_NbClasse1;
	int m_NbClasse2;
	double ** m_Distribution;
};

#endif // !defined(AFX_DISTRIBUTIONCROISEE_H__26E1222D_4DD5_4FB8_A9D4_4847B012DB68__INCLUDED_)
