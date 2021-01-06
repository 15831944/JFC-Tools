#if !defined(AFX_ATPGraphGRP_B1_H__F98DB27B_0EF5_497D_9020_C611D83937DC__INCLUDED_)
#define AFX_ATPGraphGRP_B1_H__F98DB27B_0EF5_497D_9020_C611D83937DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les définitions nécessaires
#include "JATPDocCreuset.h"
#include "ATPGraphHisto.h"

/////////////////////////////////////////////////////////////////////////////
// CATPGraphGRP_B1

class CATPGraphGRP_B1 : public CATPGraphHisto
{
public:
	// le constructeur
	CATPGraphGRP_B1();

	// le destructeur
	virtual ~CATPGraphGRP_B1();

protected:
	virtual void CalcValues(const JITPDATHypoFormat & DATHypoFormat, int i, JFlt64 &Value);

};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATPGraphGRP_B1_H__F98DB27B_0EF5_497D_9020_C611D83937DC__INCLUDED_)
