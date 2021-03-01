// on inclut les définitions nécessaires
#include "stdafx.h"
#include "ATPCreusetPlan_Metriques.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////
// les fonctions pour récupérer les dimensions des bordures

LONG CATPCreusetPlan_Metriques::CX_Border() { return (5); }
LONG CATPCreusetPlan_Metriques::CY_Border() { return (5); }

//////////////////////////////////////////////////////////////
// les fonctions pour récupérer les dimensions des interstices

LONG CATPCreusetPlan_Metriques::CX_Interstice() { return (2); }
LONG CATPCreusetPlan_Metriques::CY_Interstice() { return (2); }

///////////////////////////////////////////////////////////
// les fonctions pour récupérer les largeurs des composants

LONG CATPCreusetPlan_Metriques::CX_Ax() { return (168); }
LONG CATPCreusetPlan_Metriques::CX_Bx() { return (54); }
LONG CATPCreusetPlan_Metriques::CX_Cx() { return (54); }
LONG CATPCreusetPlan_Metriques::CX_Dx() { return (74); }
LONG CATPCreusetPlan_Metriques::CX_VS() { return (16); }

///////////////////////////////////////////////////////////
// les fonctions pour récupérer les	hauteurs des composants

LONG CATPCreusetPlan_Metriques::CY_x0() { return (25); }
LONG CATPCreusetPlan_Metriques::CY_x1() { return (52); }
LONG CATPCreusetPlan_Metriques::CY_x2() { return (18); }   //22
LONG CATPCreusetPlan_Metriques::CY_x3() { return (22); }
LONG CATPCreusetPlan_Metriques::CY_x4() { return (22); }
LONG CATPCreusetPlan_Metriques::CY_HS() { return (16); }




