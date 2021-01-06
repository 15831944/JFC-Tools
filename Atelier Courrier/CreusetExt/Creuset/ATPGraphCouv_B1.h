#if !defined(AFX_ATPGraphCouv_B1_H__1C1B2C6F_064D_4189_BDC5_840D293EC7FE__INCLUDED_)
#define AFX_ATPGraphCouv_B1_H__1C1B2C6F_064D_4189_BDC5_840D293EC7FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les définitions nécessaires
#include "JATPDocCreuset.h"
#include "stdafx.h"
#include "ATPGraphCrb.h"

/////////////////////////////////////////////////////////////////////////////
// CATPGraphCouv_B1

class CATPGraphCouv_B1 : public CATPGraphCrb
{
public:
	// le constructeur
	CATPGraphCouv_B1();
	virtual ~CATPGraphCouv_B1();

protected:
	virtual void CalcValues(const JITPDATHypoFormat & DATHypoFormat, int idxHypo, int i, JFlt64 &A2, JFlt64 &A3, JFlt64 &A4);
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATPGraphCouv_B1_H__1C1B2C6F_064D_4189_BDC5_840D293EC7FE__INCLUDED_)
