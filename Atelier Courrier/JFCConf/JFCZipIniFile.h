#ifndef JFCZIPINIFILE_H
#define JFCZIPINIFILE_H

// on inclut le header de la classe de base
#include "JFCIniFile.h"

class AFX_EXT_CLASS JFCZipIniFile : public JFCIniFile
{
	public:

		// le constructeur
		JFCZipIniFile ();

		// le desctructeur virtuel
		virtual ~JFCZipIniFile ();

		// Reads ini file specified using path.
		virtual bool ReadFile();

		// Writes data stored in class to ini file.
		virtual bool WriteFile(); 

};

#endif // JFCZIPINIFILE_H
