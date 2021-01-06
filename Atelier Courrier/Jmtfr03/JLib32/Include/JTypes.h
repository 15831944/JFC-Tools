//
// Fichier: JTypes.h
// Auteur:  Sylvain SAMMURI
// Date:    04/06/2002
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JLIB_JTYPES_H

// on définit la macro pour éviter les inclusions multiples
#define JLIB_JTYPES_H

// la définition des types de base
typedef void           JVoid;  // void
typedef bool           JBool;  // booléen
typedef char           JChar;  // caractère
typedef signed   char  JInt08; // entier     signé  8 bits
typedef unsigned char  JUnt08; // entier non signé  8 bits
typedef signed   short JInt16; // entier     signé 16 bits
typedef unsigned short JUnt16; // entier non signé 16 bits
typedef signed   long  JInt32; // entier     signé 32 bits
typedef unsigned long  JUnt32; // entier non signé 32 bits
typedef float          JFlt32; // flottant         32 bits
typedef double         JFlt64; // flottant         64 bits

// fin de l'inclusion conditionnelle
#endif
