#include "StdAfx.h"

#include "SupportListElt.h"
#include "SupportListFunctors.h"
#include "IElem.h"

//=============================================================================	
// classe spécifique à la liste d'affichage des titres
SupportListElt::SupportListElt(IElem * pElem_, eSupportListEltType type, bool bIsApparie_)
//=============================================================================	
	:	bIsTitre		(type == sletTitre)
	,	bIsCouplage		(type == sletCouplagefixe || type == sletCouplagelibre)
	,	bIsFamille		(type == sletFamille)
	,	bIsApparie		(bIsApparie_)
	,	bIsCouplageFixe	(type == sletCouplagefixe)
	,	bIsCouplageLibre(type == sletCouplagelibre)
{
	ASSERT(pElem_);
	if (bIsTitre)			ASSERT(!bIsCouplage && !bIsFamille);
	if (bIsCouplage)		ASSERT(!bIsTitre	&& !bIsFamille);
	if (bIsFamille)			ASSERT(!bIsTitre	&& !bIsCouplage);
	if (bIsCouplageFixe)	ASSERT(!bIsCouplageLibre);
	if (bIsCouplageLibre)	ASSERT(!bIsCouplageFixe);

	pElem		= pElem_;
	pcszLabel	= pElem->GetLabel().AsJCharPtr();
	labelLen	= strlen(pcszLabel);
	Id			= pElem->GetID();
	iSelected	= FALSE;

	bIsSupportFromSource = true;
}

//=============================================================================	
SupportListElt::~SupportListElt()
//=============================================================================	
{

}

//=============================================================================	
const JChar* SupportListElt::GetLabel() const
//=============================================================================	
{
	return pcszLabel;
}
