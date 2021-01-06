//
// Fichier: JLimits.h
// Auteur:  Sylvain SAMMURI
// Date:    09/10/2002
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JLIB_JLIMITS_H

// on définit la macro pour éviter les inclusions multiples
#define JLIB_JLIMITS_H

// les limites des entiers signés
#define JINT08_MAX (127l)
#define JINT16_MAX (32767l)
#define JINT32_MAX (2147483647l)
#define JINT08_MIN (-JINT08_MAX-1l)
#define JINT16_MIN (-JINT16_MAX-1l)
#define JINT32_MIN (-JINT32_MAX-1l)

// les limites des entiers non signés
#define JUNT08_MAX (255ul)
#define JUNT16_MAX (65535ul)
#define JUNT32_MAX (4294967295ul)
#define JUNT08_MIN (0ul)
#define JUNT16_MIN (0ul)
#define JUNT32_MIN (0ul)

// la taille des tampons des flots
#define JMEMORY_MAX (1024l)
#define JFILE_MAX   (65536l)

// la taille limite des composants
#define JLEXEME_MAX (256l)
#define JSTRING_MAX (256l)
#define JLABEL_MAX  (256l)
#define JPATH_MAX   (128l)
#define JHOST_MAX   (32l)

// fin de l'inclusion conditionnelle
#endif
