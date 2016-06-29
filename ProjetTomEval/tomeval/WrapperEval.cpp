#include "stdafx.h"
#include "WrapperEval.h"

#include <string>
#include "eval.h"

using namespace std;

int DummyFuction(int i)
{


	return i+1;
}

int FullEvaluation(char* tomJobName, char* cibjob,char*  restomName)
{
	CEval* leval = new CEval();
	leval->LanceEval(0, 0, tomJobName, cibjob,restomName);
//	AfficheErreur(tomJobName);


	delete leval;
	return 0;
};
