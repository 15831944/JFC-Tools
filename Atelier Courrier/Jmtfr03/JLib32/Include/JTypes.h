//
// Fichier: JTypes.h
// Auteur:  Sylvain SAMMURI
// Date:    04/06/2002
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JLIB_JTYPES_H

// on d�finit la macro pour �viter les inclusions multiples
#define JLIB_JTYPES_H

// la d�finition des types de base
typedef void           JVoid;  // void
typedef bool           JBool;  // bool�en
typedef char           JChar;  // caract�re
typedef signed   char  JInt08; // entier     sign�  8 bits
typedef unsigned char  JUnt08; // entier non sign�  8 bits
typedef signed   short JInt16; // entier     sign� 16 bits
typedef unsigned short JUnt16; // entier non sign� 16 bits
typedef signed   long  JInt32; // entier     sign� 32 bits
typedef unsigned long  JUnt32; // entier non sign� 32 bits
typedef float          JFlt32; // flottant         32 bits
typedef double         JFlt64; // flottant         64 bits

// fin de l'inclusion conditionnelle
#endif
