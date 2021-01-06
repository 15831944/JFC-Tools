#if !defined(AFX_ATPECHELLE_METRIQUES_H__6870DF7E_6D9D_45CB_945C_573A3528B9B7__INCLUDED_)
#define AFX_ATPECHELLE_METRIQUES_H__6870DF7E_6D9D_45CB_945C_573A3528B9B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CATPEchelle_Metriques  
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
	static LONG CX_Ex();
	static LONG CX_VS();

	// les fonctions pour récupérer les hauteurs des composants
	static LONG CY_T1();
	static LONG CY_T2();
	static LONG CY_x1();
	static LONG CY_x2();
	static LONG CY_x3();
	static LONG CY_x4();
	static LONG CY_HS();
};

#endif // !defined(AFX_ATPECHELLE_METRIQUES_H__6870DF7E_6D9D_45CB_945C_573A3528B9B7__INCLUDED_)
