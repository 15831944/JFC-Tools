/* Main program written in C++ that calls a Fortran routine */
#include <stdio.h>
#include <string.h>

/* Declare the Fortran routine. 

   The "C" attribute prevents C++ name mangling  Remove it
   if the file type is .c
*/

#ifdef __cplusplus
extern "C" 
#endif

/* This source file can be used with the Fortran routine
   being in a DLL or directly in the project.  The 
   USEDLL project defines a USEDLL preprocessor symbol to
   indicate that  DLL is being used; the NODLL project 
   omits this.  If we're using the Fortran DLL, add the
   dllimport specification.
*/

#ifdef USEDLL
__declspec(dllimport)
#endif

/* In the following declaration of FSUB, note that there
   are two additional arguments defined.  By default,
   Fortran code expects CHARACTER arguments to be passed
   using two argument positions, an address and an integer
   length (passed by value).  When calling from 
   non-Fortran code, we need to explicitly pass the
   length arguments unless other arrangements have been 
   made for the Fortran code not to expect them (C routine
   attribute and REFERENCE argument attribute.)

   The _stdcall attribute is used to match the Fortran
   default calling and naming conventions.  The routine
   name must be in uppercase unless the Fortran code 
   includes an appropriate ALIAS attribute.
*/

void _stdcall FSUB 
                     (int *INT_ARG,
                      char *STR_IN,
                      int STR_IN_LEN,
                      char *STR_OUT,
                      int STR_OUT_LEN);

void main (int argc, char *argv[])
{
char instring[40];
char outstring[40];
int intarg;

strcpy(instring,"Testing...");
intarg = 123;
/* Call Fortran routine - pass intarg by reference,
pass length of outstring explicitly */
FSUB(&intarg,instring,strlen(instring),outstring,40);
printf("%s\n",outstring);
} 
