#if !defined(AFX_ATPCreusetPlan_METRIQUES_H__C0F25CD6_ED1A_4E6D_98BC_984968BE1E48__INCLUDED_)
#define AFX_ATPCreusetPlan_METRIQUES_H__C0F25CD6_ED1A_4E6D_98BC_984968BE1E48__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CATPCreusetPlan_Metriques  
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
	static LONG CX_Cx();
	static LONG CX_Dx();
	static LONG CX_VS();

	// les fonctions pour récupérer les hauteurs des composants
	static LONG CY_x0();
	static LONG CY_x1();
	static LONG CY_x2();
	static LONG CY_x3();
	static LONG CY_x4();
	static LONG CY_HS();
};

#endif // !defined(AFX_ATPCreusetPlan_METRIQUES_H__C0F25CD6_ED1A_4E6D_98BC_984968BE1E48__INCLUDED_)
