#include "StdAfx.h"
#include ".\atpmapaffhypo.h"

CATPMapAffHypo::CATPMapAffHypo(void)
{
}

CATPMapAffHypo::~CATPMapAffHypo(void)
{
}

void CATPMapAffHypo::Init(JUnt32 nb, bool etat)
{
	Reset();
	for(JUnt32 id = 0; id <= nb; id++)
		Add(id) = etat;
}

void CATPMapAffHypo::Set(JUnt32 id, bool etat)
{
	MoveTo(id);
	if(IsValid())
		GetItem() = etat;
}

void CATPMapAffHypo::ResetSet(JUnt32 id)
{
	for(MoveFirst(); IsValid(); MoveNext())
		GetItem() = false;

	MoveTo(id);
	if(IsValid())
		GetItem() = true;
}

void CATPMapAffHypo::Invert(JUnt32 id)
{
	MoveTo(id);
	if(IsValid())
		GetItem() = !GetItem();
}

bool CATPMapAffHypo::GetState(JUnt32 id)
{
	MoveTo(id);
	if(IsValid())
		return GetItem();
	return false;
}

JUnt32 CATPMapAffHypo::GetNbAff()
{
	JUnt32 count = 0;
	for(MoveFirst(); IsValid(); MoveNext())
	{
		if (GetItem() == true)
			count++;
	}
	return count;
}
