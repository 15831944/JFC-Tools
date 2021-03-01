#if !defined(AFX_ATPGraphGRPCumul_B1_H__1C1B2C6F_064D_4189_BDC5_840D293EC7FE__INCLUDED_)
#define AFX_ATPGraphGRPCumul_B1_H__1C1B2C6F_064D_4189_BDC5_840D293EC7FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les définitions nécessaires
#include "JATPDocCreuset.h"
#include "ATPGraphCrb.h"
/////////////////////////////////////////////////////////////////////////////
// CATPGraphGRPCumul_B1
typedef	JArray<JFlt64> JArrayJFlt64;

class CATPGraphGRPCumul_B1 : public CATPGraphCrb
{
public:
	// le constructeur
	CATPGraphGRPCumul_B1();
	// le destructeur
	virtual ~CATPGraphGRPCumul_B1();

	virtual JVoid Update();

protected:
	virtual void CalcValues(const JITPDATHypoFormat & DATHypoFormat, int idxHypo, int i, JFlt64 &A2, JFlt64 &A3, JFlt64 &A4);
	// la fonction pour appliquer les mises à jour

private:
	JMap<JInt32, JArrayJFlt64> m_MapArrayGRP;

};


/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATPGraphGRPCumul_B1_H__1C1B2C6F_064D_4189_BDC5_840D293EC7FE__INCLUDED_)
