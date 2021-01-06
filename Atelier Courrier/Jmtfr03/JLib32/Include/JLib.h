//
// Fichier: JLib.h
// Auteur:  Sylvain SAMMURI
// Date:    04/06/2002
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JLIB_H

// on d�finit la macro pour �viter les inclusions multiples
#define JLIB_H

// on inclut la d�finition des types
#include "..\Include\JTypes.h"

// on inclut la d�finition des limites
#include "..\Include\JLimits.h"

// on inclut la d�finition des conversions
#include "..\Include\JConvert.h"

// on inclut la d�finition des exceptions
#include "..\Include\JInternalError.h"
#include "..\Include\JSystemError.h"
#include "..\Include\JNetworkError.h"
#include "..\Include\JInvalidCall.h"
#include "..\Include\JMemoryFull.h"
#include "..\Include\JBadSchema.h"
#include "..\Include\JTimeout.h"

// on inclut la d�finition des grandeurs
#include "..\Include\JRef32.h"
#include "..\Include\JInt08x.h"
#include "..\Include\JUnt08x.h"
#include "..\Include\JInt16x.h"
#include "..\Include\JUnt16x.h"
#include "..\Include\JInt32x.h"
#include "..\Include\JUnt32x.h"
#include "..\Include\JFlt32x.h"
#include "..\Include\JFlt64x.h"
#include "..\Include\JMsk08.h"
#include "..\Include\JMsk16.h"
#include "..\Include\JMsk32.h"

// on inclut la d�finition des composants
#include "..\Include\JFpu.h"
#include "..\Include\JMath.h"
#include "..\Include\JDate.h"
#include "..\Include\JTime.h"
#include "..\Include\JLabel.h"
#include "..\Include\JString.h"
#include "..\Include\JStringEx.h"
#include "..\Include\JHost.h"
#include "..\Include\JPath.h"

// on inclut la d�finition des interfaces de progression
#include "..\Include\JProgress.h"

// on inclut la d�finition des collections
#include "..\Include\JList.h"
#include "..\Include\JVector.h"
#include "..\Include\JHash.h"
#include "..\Include\JMap.h"

// on inclut la d�finition des conteneurs
#include "..\Include\JArray.h"
#include "..\Include\JMatrix.h"
#include "..\Include\JMultix.h"

// on inclut la d�finition du champ de bits
#include "..\Include\JBitArray.h"

// on inclut la d�finition du gestionnaire des chemins
#include "..\Include\JPathManager.h"

// on inclut la d�finition de la s�rialisation des flots
#include "..\Include\JMemory.h"
#include "..\Include\JFile.h"
#include "..\Include\JPipe.h"

// on inclut la d�finition des fa�ades des flots
#include "..\Include\JArchive.h"
#include "..\Include\JConnection.h"
#include "..\Include\JMemento.h"

// on inclut la d�finition des imports / exports
#include "..\Include\JReader.h"
#include "..\Include\JParser.h"
#include "..\Include\JExport.h"

// fin de l'inclusion conditionnelle
#endif
