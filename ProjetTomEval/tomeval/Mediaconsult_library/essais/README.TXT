ReadMe for C_CALL_F workspace

This workspace demonstrates a mixed-language application in
which a C (or C++) main program calls a Fortran subroutine.
It is presented in two different styles, one where the 
Fortran and C code are built together in the same project
and one where the Fortran code is built separately into
a DLL which is then called from C.

The C main program assigns the string "Testing..." to a
string variable and 123 to an integer variable.  These are
then passed to a Fortran subroutine along with a string
output argument.  The Fortran routine converts the integer
to a decimal string, concatenates it with the input string,
and stores the result in the output string argument.  Upon
return, the C main program displays the output string. 
This example also demonstrates how to pass character/string
arguments to Fortran.

Project NODLL demonstrates mixing Fortran and C source in
the same project.  The only non-default setting is for the
Debug confguration where C/C++..Code Generation..Use
run-time library is set to Single-Threaded.  The C/C++
default of Debug Single-Threaded conflicts with Fortran's
default of using the non-debug C libraries.  This setting
change is not required in a Release configuration.

Project FDLL builds the Fortran source as a DLL.  It is
made a dependency of project USEDLL so that when USEDLL
is built, FDLL will automatically be built first and the
FDLL export library will be automatically searched when
USEDLL is linked.  Note the use of the predefined 
preprocessor symbol _DLL in the Fortran code to 
conditionally cause the declaration of FSUB to be exported.
One setting change made for this project was to
Link..General..Output file name.  By default, the DLL file
is put in the Debug (or Release) subfolder, but this would
require manually copying the DLL to the folder of the
USEDLL executable.  What we have done here is modify the
setting so that the DLL is automatically written into the
proper folder.  In a real application, this would 
typically not be done, the DLL being placed during its
"installation".

Project USEDLL contains the C++ main program and has a
dependency on the FDLL subproject.  The non-default 
settings for this project are to add a USEDLL preprocessor
symbol to C/C++..Preprocessor..Preprocessor Definitions
(this is used to include the __declspec(dllimport)
directive), and C/C++..Code Generation..Use run-time
library..Multithreaded DLL.  Whenever building a main
program that links against a Fortran or C DLL, you should
make sure that the main program also links against the
DLL forms of the run-time libraries.
