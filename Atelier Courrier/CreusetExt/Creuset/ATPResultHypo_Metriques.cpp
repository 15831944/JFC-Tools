// on inclut les définitions nécessaires
#include "stdafx.h"
#include "ATPResultHypo_Metriques.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////
// les fonctions pour récupérer les dimensions des bordures

LONG CATPResultHypo_Metriques::CX_Border() { return (5); }
LONG CATPResultHypo_Metriques::CY_Border() { return (5); }

//////////////////////////////////////////////////////////////
// les fonctions pour récupérer les dimensions des interstices

LONG CATPResultHypo_Metriques::CX_Interstice() { return (2); }
LONG CATPResultHypo_Metriques::CY_Interstice() { return (2); }

///////////////////////////////////////////////////////////
// les fonctions pour récupérer les largeurs des composants

LONG CATPResultHypo_Metriques::CX_Ax() { return (202); }
LONG CATPResultHypo_Metriques::CX_Bx() { return (70); }   // 50
LONG CATPResultHypo_Metriques::CX_VS() { return (16); }

///////////////////////////////////////////////////////////
// les fonctions pour récupérer les	hauteurs des composants

LONG CATPResultHypo_Metriques::CY_T1() { return (24); }
LONG CATPResultHypo_Metriques::CY_x1() { return (18); }//18
//LONG CATPResultHypo_Metriques::CY_x1() { return (36); }//18
LONG CATPResultHypo_Metriques::CY_x2() { return (60); }
