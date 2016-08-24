// TableNbEntreeComplexe.cpp: implementation of the CTableNbEntreeComplexe class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TableNbEntreeComplexe.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTableNbEntreeComplexe::CTableNbEntreeComplexe()
{
	m_NomFich="EntreeComplexe.table";
	IsModified=0;
	SetName(m_NomFich);

}

CTableNbEntreeComplexe::~CTableNbEntreeComplexe()
{

}
CTableNbEntreeComplexe::CTableNbEntreeComplexe(const  CTableNbEntreeComplexe & Data)
{	
		*this=Data;
};


bool CTableNbEntreeComplexe::LoadTable()
{
	RemoveAll();

	// On charge le fichier des entrées dans la table
	CFichierCinema m_Fichier;

	if(!m_Fichier.OpenFileForRead(m_NomFich,1))
	{
		return (0);
	} 

	// On lit toutes les entrées que l'on place dans la table 
	// lecture tant que la ligne n'est pas un '#'

	while(m_Fichier.ReadLine())
	{
		CString ChaineMessage;
		CString Code;
		bool message=0;
		tagNbEntree enr;
		// On stocke d'abord le code complexe
		CString code;
		code = m_Fichier.GetElement(0) ;
		// le code est il valide ?: 6 caractères
		if(code.GetLength()>6)
		{
			message=1;
			code = code.Left(6); 
			ChaineMessage="\nLe code est supérieur à 6 caractères";
		}
		enr.m_CodeComplexe = code;

		// le nombre d'entrée limité à 10 caractères
		CString nbentree = m_Fichier.GetElement(1);
		if(nbentree.GetLength()>10)
		{
			message=1;
			// correction
			nbentree=nbentree.Left(10); 
			ChaineMessage+="\nLe nombre d'entrée est supérieur à 10 caractères";
		}
		enr.m_NbEntree = atoi(nbentree);
		if(message)
		{
			AfxMessageBox("le format du fichier n'est pas valide"+ChaineMessage,MB_OK | MB_APPLMODAL);   
		}

		// On ajoute l'enregistrement dans le tableau
		int index = Add(enr);
		// on ajoute l'index de l'élément dans un map mémoire
		Code = enr.GetCode();
		m_MapCodeToIndex.SetAt(enr.GetCode(),index);
	}
	if(GetSize()==0)
	{
		AfxMessageBox("La table des entrées est vide",MB_OK | MB_APPLMODAL);	
	}
	// fermeture du fichier
	m_Fichier.Close(); 
	return (1);
}

bool CTableNbEntreeComplexe::SaveTable()
{
	// sauvegarde
	m_NomFich = "EntreeComplexe.table";
	// la table a été modifié
	IsModified = 0;

	CString txt;
	CFichierCinema Fichier;

	if(!Fichier.OpenFileForWrite(m_NomFich))
	{
		return (0);	
	}
	for(int i=0;i<GetSize();i++)
	{
		// sauvegarde du code
		txt=GetCode(i);
		// séparateur
		txt+="|";
		// sauvegarde du nombre d'entrées
		txt.Format(txt+"%d",GetAt(i).GetNbEntree());
		txt+="\n";
		
		if(!Fichier.WriteLine(txt))
		{
			return (0);
		}
		
	}
	// caractère de fin de fichier
	if(!Fichier.WriteLine("#"))
	{
		return (0);
	}
	IsModified=0;
	// fermeture du fichier
	Fichier.Close(); 
	return (1);

}

void CTableNbEntreeComplexe::Tri()
{
	// on boucle sur les complexes

	// tableau pour le stockage des codes alphanumériques
	tagNbEntree * tabAlpha = new tagNbEntree[GetSize()];
	int count=0;
	
	for(int c=0;c<GetSize();c++)
	{

		tagNbEntree enr;
		enr = GetAt(c);
		for(int c2=c+1;c2<GetSize();c2++)
		{
			tagNbEntree enr2;
			enr2 = GetAt(c2);
			int code1,code2;
			CString chainecode1,chainecode2;

			chainecode1 = enr.m_CodeComplexe.Right(5);
			chainecode2 = enr.m_CodeComplexe.Right(5);
			
			code1 = atoi(chainecode1);
			code2 = atoi(chainecode2);

			for(int car=0;car<5;car++)
			{
				// les codes alphanumériques sont ajoutés à la fin
				if((chainecode1.GetAt(car)<=47)||(chainecode1.GetAt(car)>=58))
				{
					tabAlpha[count]=enr;
					Decale(c);
					count++;
					IsModified=1;
					goto label;
				}
				if((chainecode2.GetAt(car)<=47)||(chainecode2.GetAt(car)>=58))
				{
					tabAlpha[count]=enr2;
					Decale(c2);
					count++;
					IsModified=1;
					break;


				}
				
			}

			if(code2<code1)
			{
				SetAt(c,enr2);
				SetAt(c2,enr);
				IsModified=1;

			}
		}
label:;
	}
	// On ajoute ensuite les codes alphanumériques
	for(int code=0;code<count;code++)
	{
		SetAt(GetSize()-code-1,tabAlpha[code]);
	}

	/*	for(int c2=0;c2<GetSize();c2++)
	{
		TRACE("code n°%d:%s\n",c2,GetAt(c2).m_Code);
	}*/
	delete [] tabAlpha;



}

void CTableNbEntreeComplexe::Decale(int idnbentree)
{
	for(int id=idnbentree+1;id<GetSize();id++)
	{
		tagNbEntree enr=GetAt(id);
		SetAt(id-1,enr);
	}

}
