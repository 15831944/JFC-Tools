// test2.cpp : Defines the entry point for the console application.
//

#include <stdafx.h>
#include <stdio.h>
#include <string.h>



extern "C"


/*void _stdcall FSUB 
                     (int *INT_ARG,
                      char *STR_IN,
                      int STR_IN_LEN,
                      char *STR_OUT,
                      int STR_OUT_LEN);*/

float _stdcall RAN3 (int*);


void main (int argc, char *argv[])
{

	int i;
	float v;
	for(i=1;i<=20;i++)
	{
		v=RAN3(&i);
		printf("value %d => %f\n",i,v);
	}

	getchar();


	
}

