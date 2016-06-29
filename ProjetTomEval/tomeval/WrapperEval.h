#pragma once

#include <string>

using namespace std;

extern "C" {
	_declspec(dllexport) int DummyFuction(int i);


	_declspec(dllexport) int FullEvaluation(char* tomJobName,char* cibjob, char* restomName);
}