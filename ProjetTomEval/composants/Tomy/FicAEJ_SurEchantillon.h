// FicAEJ_SurEchantillon.h: interface for the CFicAEJ_SurEchantillon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FICAEJ_SURECHANTILLON_H__3FCF6314_AFEF_45FF_B8D9_714DA74DE235__INCLUDED_)
#define AFX_FICAEJ_SURECHANTILLON_H__3FCF6314_AFEF_45FF_B8D9_714DA74DE235__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ficAEJ.h"
//##ModelId=3D9AB7D6016B
class CFicAEJ_SurEchantillon : public CFicAEJ
{
public:
	//##ModelId=3D9AB7D60176
	CFicAEJ_SurEchantillon();
	//##ModelId=3D9AB7D60177
	virtual ~CFicAEJ_SurEchantillon();
	//##ModelId=3D9AB7D60179
	bool BindPanelSurEchntillonOffreElargie(LPCSTR NomPanelFusion,CFicProba * FicProbaBase,CFicProba * FicProbaSurEchantillon,char * filtreOffreElargie);
	//##ModelId=3D9AB7D6018A
	int GetNrIndivBase(int indiv){ASSERT(indiv<m_NbIndivBase);return(m_NrIndivBase[indiv]);};
	//##ModelId=3D9AB7D60193
	int GetNrIndivSurEchantillon(int indiv){ASSERT(indiv<m_NbIndivSurEchantillon);return(m_NrIndivSurEchantillon[indiv]);};
	//##ModelId=3D9AB7D60195
	void CalculRatioConsomation(int NbRatio);
	//##ModelId=3D9AB7D6019E
	short GetNrPanelParIndividu(int indiv) // 0:Mediadata 1:Mediacabsat
	{
		if(indiv<m_NbIndivBase)
			return 0;
		else
			return 1;
	}
	//##ModelId=3D9AB7D601A0
	long GetNbSupportParIndividu(int indiv){return(m_NbSupportMoyenParPanelParJour[GetNrPanelParIndividu(indiv)]);
	}
private:
	//##ModelId=3D9AB7D601A8
	char * m_FiltreOffreElargie;
	//##ModelId=3D9AB7D601B1
	int * m_NrIndivBase;
	//##ModelId=3D9AB7D601BB
	int * m_NrIndivSurEchantillon;
	int m_NbIndivBase,m_NbIndivSurEchantillon;
	//##ModelId=3D9AB7D601BC
	long  m_NbSupportMoyenParPanelParJour[2]; // 0:Mediadata 1:Mediacabsat
};

#endif // !defined(AFX_FICAEJ_SURECHANTILLON_H__3FCF6314_AFEF_45FF_B8D9_714DA74DE235__INCLUDED_)
