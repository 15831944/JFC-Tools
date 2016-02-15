// Ce fichier n'est utilisé dans ARZAPRNT
#ifdef UTZADICO_BUILD
#define UTZADICO_API __declspec(dllexport)
#else
#define UTZADICO_API 
//#define UTZADICO_API __declspec(dllimport)
#endif



// ********************** DICO STR ************************
extern "C" int UTZADICO_API InitDico(char *NomDico);
extern "C" char * UTZADICO_API GetDico(int NumDico, long Arg);

#define GetDico0(A) GetDico(NumDico,A)
// _extern int NumDico;

