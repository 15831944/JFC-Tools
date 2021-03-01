#if !defined(AFX_ATPCreusetHypo_METRIQUES_H__C0F25CD6_ED1A_4E6D_98BC_984968BE1E48__INCLUDED_)
#define AFX_ATPCreusetHypo_METRIQUES_H__C0F25CD6_ED1A_4E6D_98BC_984968BE1E48__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
 * \ingroup AtelierPresse
 *
 *
 * \par requirements
 * win98 or later\n
 * win2k or later\n
 * MFC\n
 *
 * \version 1.0
 * first version
 *
 * \date 12-09-2003
 *
 * \author alain
 *
 * \par license
 * This code is absolutely free to use and modify. The code is provided "as is" with
 * no expressed or implied warranty. The author accepts no liability if it causes
 * any damage to your computer, causes your pet to fall ill, increases baldness
 * or makes your car start emitting strange noises when you start it up.
 * This code has no bugs, just undocumented features!
 * 
 * \todo 
 *
 * \bug 
 *
 */
class CATPCreusetHypo_Metriques  
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
	static LONG CX_AxQP();
	static LONG CX_Bx();
	static LONG CX_Cx();
	static LONG CX_CxQP();
	static LONG CX_Dx();
	static LONG CX_VS();

	// les fonctions pour récupérer les hauteurs des composants
	static LONG CY_x1();
	static LONG CY_x2();
	static LONG CY_x3();
	static LONG CY_x4();
	static LONG CY_T1();
	static LONG CY_HS();
};

#endif // !defined(AFX_ATPCreusetHypo_METRIQUES_H__C0F25CD6_ED1A_4E6D_98BC_984968BE1E48__INCLUDED_)
