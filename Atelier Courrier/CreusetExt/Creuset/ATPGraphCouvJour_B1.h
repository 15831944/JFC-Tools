#if !defined(AFX_ATPGraphCouvJour_B1_H__F98DB27B_0EF5_497D_9020_C611D83937DC__INCLUDED_)
#define AFX_ATPGraphCouvJour_B1_H__F98DB27B_0EF5_497D_9020_C611D83937DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les d�finitions n�cessaires
#include "JATPDocCreuset.h"
#include "ATPGraphHisto.h"


/////////////////////////////////////////////////////////////////////////////
// CATPGraphCouvJour_B1

class CATPGraphCouvJour_B1 : public CATPGraphHisto
{
public:
	// le constructeur
	CATPGraphCouvJour_B1();

	// le destructeur
	virtual ~CATPGraphCouvJour_B1();


protected:
	virtual void CalcValues(const JITPDATHypoFormat & DATHypoFormat, int i, JFlt64 &Value);


};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATPGraphCouvJour_B1_H__F98DB27B_0EF5_497D_9020_C611D83937DC__INCLUDED_)
