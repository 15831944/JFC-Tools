#if !defined(AFX_ATPGraphCouv_METRIQUES_H__4393EBDA_8986_4493_A2BB_3C762DF20860__INCLUDED_)
#define AFX_ATPGraphCouv_METRIQUES_H__4393EBDA_8986_4493_A2BB_3C762DF20860__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CATPGraph_Metriques  
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
	static LONG CY_HS();
};

#endif // !defined(AFX_ATPGraphCouv_METRIQUES_H__4393EBDA_8986_4493_A2BB_3C762DF20860__INCLUDED_)
