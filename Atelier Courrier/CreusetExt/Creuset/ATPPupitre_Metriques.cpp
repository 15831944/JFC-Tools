// on inclut les définitions nécessaires
#include "stdafx.h"
#include "ATPPupitre_Metriques.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////
// les fonctions pour récupérer les dimensions des bordures

LONG CATPPupitre_Metriques::CX_Border() { return (5); }
LONG CATPPupitre_Metriques::CY_Border() { return (5); }

//////////////////////////////////////////////////////////////
// les fonctions pour récupérer les dimensions des interstices

LONG CATPPupitre_Metriques::CX_Interstice() { return (2); }
LONG CATPPupitre_Metriques::CY_Interstice() { return (2); }

///////////////////////////////////////////////////////////
// les fonctions pour récupérer les largeurs des composants

LONG CATPPupitre_Metriques::CX_Ax() { return (138);}
LONG CATPPupitre_Metriques::CX_x1() { return (96); }
LONG CATPPupitre_Metriques::CX_x2() { return (22); }

/////////////////////////////////////////////////////////
// la fonction pour récupérer les hauteurs des composants

LONG CATPPupitre_Metriques::CY_y1() { return (22); }			// Hauteur des buttons
LONG CATPPupitre_Metriques::CY_y2() { return (3*24 + 3*2); }			// Hauteur ButtonBox Terrain  // 110
LONG CATPPupitre_Metriques::CY_y3() { return (2*28); }			// Hauteur ButtonBox Cible    // 110 
LONG CATPPupitre_Metriques::CY_y4() { return (3*24 + 3*2); }		// Hauteur ButtonBox Format   // (4*22)+(3*2)
LONG CATPPupitre_Metriques::CY_y11() { return (28); }		// Hauteur ButtonBox Format   // (4*22)+(3*2)
