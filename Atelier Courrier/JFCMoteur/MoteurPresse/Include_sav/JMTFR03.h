//
// Fichier: JLib.h
// Auteur:  Sylvain SAMMURI
// Date:    07/12/2003
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JMTFR03_H

// on définit la macro pour éviter les inclusions multiples
#define JMTFR03_H

// on inclut la définition des sources
#include "JFR03Sources.h"

// on inclut la définition des exceptions
#include "JMTFR03Exception.h"

// on inclut la définition des terrains, des modulations et des cibles
#include "JMTFR03TBLTerrains.h"
#include "JMTFR03TBLModulations.h"
#include "JMTFR03TBLCibles.h"

// on inclut les définitions des effectifs et des audiences
#include "JMTFR03TBLEffectifs.h"
#include "JMTFR03TBLAudiences.h"

// on inclut les définitions des moteurs d'évaluation
#include "JMTFR03FullBinomial.h"
#include "JMTFR03BetaBinomial.h"
#include "JMTFR03FullBinomialCum.h"

// on inclut la définition des couplages et des mailles
#include "JMTFR03Couplage.h"
#include "JMTFR03Maille.h"

// on inclut la définition des options
#include "JMTFR03Options.h"

// on inclut la définition des traces
#include "JMTFR03Logs.h"

// fin de l'inclusion conditionnelle
#endif
