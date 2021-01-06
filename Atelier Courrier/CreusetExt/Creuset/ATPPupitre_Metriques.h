#if !defined(AFX_ATPPUPITRE_METRIQUES_H__4CCFED52_4524_43C5_B70A_CBC0D7617B9B__INCLUDED_)
#define AFX_ATPPUPITRE_METRIQUES_H__4CCFED52_4524_43C5_B70A_CBC0D7617B9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CATPPupitre_Metriques  
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
	static LONG CX_x1();
	static LONG CX_x2();

	// la fonction pour récupérer les hauteurs des composants
	static LONG CY_y1();
	static LONG CY_y2();
	static LONG CY_y3();
	static LONG CY_y4();
	static LONG CY_y11();
};

#endif // !defined(AFX_ATPPUPITRE_METRIQUES_H__4CCFED52_4524_43C5_B70A_CBC0D7617B9B__INCLUDED_)
