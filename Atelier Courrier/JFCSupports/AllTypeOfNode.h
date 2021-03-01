/*
//	JFC Informatique et M�dia
//	2003
//
//	Auteur:
//	J�r�my DEVILERS
//
//
//	Descriptif:
//	Permet d'inclure tous les types d�finis dans cette DLL
*/

// AllTypeOfNode.h
//
//////////////////////////////////////////////////////////////////////
#if !defined(ALLTYPEOFNODE_H)
#define ALLTYPEOFNODE_H

#pragma once

#include ".\ClassesComposites\Regroupements.h"
#include "..\JFCTables\IElem.h"

//Composite
#include ".\ClassesComposites\Famille.h"
#include ".\ClassesComposites\Centre.h"

#include ".\ClassesComposites\Lot.h"
#include ".\ClassesComposites\CouplageLibre.h"

//Leaf
#include ".\ClassesComposites\Titre.h"
#include ".\ClassesComposites\TitreApparente.h"

#include ".\ClassesComposites\TitreLot.h"
#include ".\ClassesComposites\TitreCouplageLibre.h"

#include ".\ClassesComposites\TitresApparies.h"
#include ".\ClassesComposites\TitreApparie.h"


#endif // !defined(ALLTYPEOFNODE_H)