// ARMLAdress.h: interface for the CAdress class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADRESS_H__5F99B8D5_30AC_4FD0_95D9_BE9F78FD9A6B__INCLUDED_)
#define AFX_ADRESS_H__5F99B8D5_30AC_4FD0_95D9_BE9F78FD9A6B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les fichiers nécessaires
#include "AfxTempl.h"
#include "ARMLField.h"
#include "./../Composite/JFCDrawComposite.h"

class CAdress  
{
public:
	// le constructeur
	CAdress();

	// le constructeur de recopie
	CAdress(const CAdress & source);

	// l'opérateur d'égalité
	CAdress & operator = (const CAdress & source);

	// la fonction pour fixer la couleur de fond
	void SetGrayed(bool grayed);

	// la fonction pour ajouter un champ
	void AddField(CField & field);

	// la fonction pour créer le composite d'un cadre
	void CreateAdress(long numpage, DrawComposite * composant);

	// le destructeur
	virtual ~CAdress();
protected:
	// le flag grisé ou non
	bool m_Grayed;
	// le tableau de champs
	CArray<CField, CField&> m_FieldArray;
};

#endif // !defined(AFX_ADRESS_H__5F99B8D5_30AC_4FD0_95D9_BE9F78FD9A6B__INCLUDED_)
