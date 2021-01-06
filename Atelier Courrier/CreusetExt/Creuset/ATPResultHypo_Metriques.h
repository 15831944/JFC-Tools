#if !defined(AFX_ATPResultHypo_METRIQUES_H__8FE88ADD_0678_4978_9C92_CEB62460E4F9__INCLUDED_)
#define AFX_ATPResultHypo_METRIQUES_H__8FE88ADD_0678_4978_9C92_CEB62460E4F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CATPResultHypo_Metriques  
{
public:
	// les fonctions pour récupérer les dimensions des bordures
	static LONG CX_Border();
	static LONG CY_Border();

	// les fonctions pour récupérer les dimensions des interstices
	static LONG CX_Interstice();
	static LONG CY_Interstice();

public:
	// les fonctions pour récupérer les largeurs des composants
	static LONG CX_Ax();
	static LONG CX_Bx();
	static LONG CX_VS();

	// les fonctions pour récupérer les hauteurs des composants
	static LONG CY_T1();
	static LONG CY_x1();
	static LONG CY_x2();
};

#endif // !defined(AFX_ATPResultHypo_METRIQUES_H__8FE88ADD_0678_4978_9C92_CEB62460E4F9__INCLUDED_)
