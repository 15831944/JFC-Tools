// on inclut les définitions nécessaires
#include "stdafx.h"
#include "ATPCreusetHypo_Metriques.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////
// les fonctions pour récupérer les dimensions des bordures

LONG CATPCreusetHypo_Metriques::CX_Border() { return (5); }
LONG CATPCreusetHypo_Metriques::CY_Border() { return (5); }

//////////////////////////////////////////////////////////////
// les fonctions pour récupérer les dimensions des interstices

LONG CATPCreusetHypo_Metriques::CX_Interstice() { return (2); }
LONG CATPCreusetHypo_Metriques::CY_Interstice() { return (2); }

///////////////////////////////////////////////////////////
// les fonctions pour récupérer les largeurs des composants

LONG CATPCreusetHypo_Metriques::CX_Ax() { return (200); }   // 196
LONG CATPCreusetHypo_Metriques::CX_AxQP() { return (180); }   // 196
LONG CATPCreusetHypo_Metriques::CX_Bx() { return (50); }
LONG CATPCreusetHypo_Metriques::CX_Cx() { return (70); }
LONG CATPCreusetHypo_Metriques::CX_CxQP() { return (90); }
LONG CATPCreusetHypo_Metriques::CX_Dx() { return (83); }
LONG CATPCreusetHypo_Metriques::CX_VS() { return (16); }

///////////////////////////////////////////////////////////
// les fonctions pour récupérer les	hauteurs des composants

LONG CATPCreusetHypo_Metriques::CY_x1() { return (36); }
LONG CATPCreusetHypo_Metriques::CY_x2() { return (18); }
LONG CATPCreusetHypo_Metriques::CY_x3() { return (22); }
LONG CATPCreusetHypo_Metriques::CY_x4() { return (22); }
LONG CATPCreusetHypo_Metriques::CY_T1() { return (24); }

LONG CATPCreusetHypo_Metriques::CY_HS() { return (16); }
