//
// Fichier: JFCException.h
// Date:    12/01/2000
// Auteur:  Sylvain SAMMURI
//

// on v�rifie que le fichier n'a pas d�j� �t� inclu
#ifndef EXCEPTION_H

// on d�finit la macro pour �viter les inclusions multiples
#define EXCEPTION_H

class AFX_EXT_CLASS Exception  
{
public:
	// les constructeurs
	Exception();
	Exception(long code);
	Exception(const Exception & source);

	// l'op�rateur d'affectation
	void operator =(const Exception & source);

	// la fonction pour r�initialiser l'exception
	void Reset();

	// le destructeur
	~Exception();

protected:
	long m_Code; // le code de l'exception
};

// fin de l'inclusion conditionnelle
#endif
