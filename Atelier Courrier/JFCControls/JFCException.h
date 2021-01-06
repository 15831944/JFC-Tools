//
// Fichier: JFCException.h
// Date:    12/01/2000
// Auteur:  Sylvain SAMMURI
//

// on vérifie que le fichier n'a pas déjà été inclu
#ifndef EXCEPTION_H

// on définit la macro pour éviter les inclusions multiples
#define EXCEPTION_H

class AFX_EXT_CLASS Exception  
{
public:
	// les constructeurs
	Exception();
	Exception(long code);
	Exception(const Exception & source);

	// l'opérateur d'affectation
	void operator =(const Exception & source);

	// la fonction pour réinitialiser l'exception
	void Reset();

	// le destructeur
	~Exception();

protected:
	long m_Code; // le code de l'exception
};

// fin de l'inclusion conditionnelle
#endif
