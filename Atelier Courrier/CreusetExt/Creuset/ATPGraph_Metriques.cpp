// on inclut les définitions nécessaires
#include "stdafx.h"
#include "ATPGraph_Metriques.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////
// les fonctions pour récupérer les dimensions des bordures

LONG CATPGraph_Metriques::CX_Border() { return (5); }
LONG CATPGraph_Metriques::CY_Border() { return (5); }

//////////////////////////////////////////////////////////////
// les fonctions pour récupérer les dimensions des interstices

LONG CATPGraph_Metriques::CX_Interstice() { return (2); }
LONG CATPGraph_Metriques::CY_Interstice() { return (2); }

///////////////////////////////////////////////////////////
// les fonctions pour récupérer les largeurs des composants

LONG CATPGraph_Metriques::CX_Ax() { return (50); } 
LONG CATPGraph_Metriques::CX_Bx() { return (42); }
LONG CATPGraph_Metriques::CX_VS() { return (16); }

///////////////////////////////////////////////////////////
// les fonctions pour récupérer les	hauteurs des composants

LONG CATPGraph_Metriques::CY_T1() { return (24); }
LONG CATPGraph_Metriques::CY_x1() { return (50); }
LONG CATPGraph_Metriques::CY_HS() { return (16); }
