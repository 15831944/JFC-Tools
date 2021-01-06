// on inclut les définitions nécessaires
#include "stdafx.h"
#include "ATPEchelle_Metriques.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////
// les fonctions pour récupérer les dimensions des bordures

LONG CATPEchelle_Metriques::CX_Border() { return (5); }
LONG CATPEchelle_Metriques::CY_Border() { return (5); }

//////////////////////////////////////////////////////////////
// les fonctions pour récupérer les dimensions des interstices

LONG CATPEchelle_Metriques::CX_Interstice() { return (2); }
LONG CATPEchelle_Metriques::CY_Interstice() { return (2); }

///////////////////////////////////////////////////////////
// les fonctions pour récupérer les largeurs des composants

LONG CATPEchelle_Metriques::CX_Ax() { return (168);}
LONG CATPEchelle_Metriques::CX_Bx() { return (54); }
LONG CATPEchelle_Metriques::CX_Cx() { return (70); }
// Largeur des colonnes Résultat
LONG CATPEchelle_Metriques::CX_Dx() { return (70); }	
LONG CATPEchelle_Metriques::CX_Ex() { return (62); }
LONG CATPEchelle_Metriques::CX_VS() { return (16); }

///////////////////////////////////////////////////////////
// les fonctions pour récupérer les	hauteurs des composants

LONG CATPEchelle_Metriques::CY_T1() { return (24); }
LONG CATPEchelle_Metriques::CY_T2() { return (24); }
LONG CATPEchelle_Metriques::CY_x1() { return (24); }
LONG CATPEchelle_Metriques::CY_x2() { return (18); }
LONG CATPEchelle_Metriques::CY_x3() { return (18); }
LONG CATPEchelle_Metriques::CY_x4() { return (22); }
LONG CATPEchelle_Metriques::CY_HS() { return (16); }
