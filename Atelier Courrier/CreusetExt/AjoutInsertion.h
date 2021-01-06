#pragma once

#include "Insertion.h"

class CAjoutInsertion
{
	CAjoutInsertion(void){};
	~CAjoutInsertion(void){};

public:

	static bool AjoutInsertionSuccessif(LONG IdSupport, LONG IdFormat, LONG IdHypothese, JATPDocCreuset * pATPDocCreuset);
	static bool AjoutInsertionSuccessif(LONG IdSupport, LONG IdFormat, CPlanInsertions & PLNInsertions, JDate &DebPeriode, JDate &FinPeriode, int nbInsert, JATPDocCreuset * pATPDocCreuset);


	static bool AddInsertionCouplage(CInsertion & Insertion, JDate DateMin, JDate DateMax, JATPDocCreuset * pATPDocCreuset, bool bNext = false);

	static bool SupprLastInsertion(LONG IdSupport, LONG IdFormat, LONG IdHypothese, JATPDocCreuset * pATPDocCreuset);

	static bool IsCouplage(JUnt32 IdSupport);
};
