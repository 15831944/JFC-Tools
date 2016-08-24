// TableComplexe.cpp: implementation of the CTableComplexe class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TableComplexe.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTableComplexe::CTableComplexe()
{
		m_NomFich="Complexe.table";
		SetName(m_NomFich);
		IsModified=0;

}
CTableComplexe::CTableComplexe(const  CTableComplexe & Data)
{	
		*this=Data;
};

CTableComplexe::~CTableComplexe()
{

}
bool CTableComplexe::LoadTable()
{
	RemoveAll();

	CFichierCinema m_Fichier;
	if(!m_Fichier.OpenFileForRead(m_NomFich,1))
	{
		return (0);
	} 

	// On lit tous les complexes que l'on place dans la table m_Complexes
	// lecture tant que la ligne n'est pas un '#'

	while(m_Fichier.ReadLine())
	{
		bool message=0;
		CString ChaineMessage="";
		CString Code;

		tagComplexe enr;
		// On stocke d'abord le code
		enr.m_Code  = m_Fichier.GetElement(0) ;
		// le code est il valide ?: 6 caractères
		if(enr.m_Code.GetLength()>6)
		{
			message=1;
			ChaineMessage="\nLe code est supérieur à 6 caractères";
			enr.m_Code = enr.m_Code.Left(6);
		}
		// le libellé est-il valide ?: 30 caractères
		enr.m_Libelle = m_Fichier.GetElement(1) ;
		if(enr.m_Libelle.GetLength()>30)
		{
			message=1;
			ChaineMessage+="\nLe libellé est supérieur à 30 caractères";
			enr.m_Libelle = enr.m_Libelle.Left(30);
			
		}

		// On enlève les espaces se trouvant après le libellé
		enr.m_Libelle.TrimRight();

		// le code BAC du complexe
		enr.m_CodeBAC  = m_Fichier.GetElement(2);
		if(enr.m_CodeBAC.GetLength()>6)
		{
			message=1;
			ChaineMessage+="\nLe code BAC est supérieur à 6 caractères";
			enr.m_CodeBAC = enr.m_CodeBAC.Left(6);
			
		}

		//On recupère la régie
		
		if(m_Fichier.GetNbElement() < 4 )
		{
			message=1;
			ChaineMessage+="\nLe code régie n'est pas présent dans la table";
			
		}
		else
		{
			CString regie = m_Fichier.GetElement(3);
			if (regie.GetLength()>1)
			{
				message=1;
				ChaineMessage+="\nLe code Régie est supérieur à 1 caractères";
				regie = regie.Left(1);
			}
			enr.m_Regie = regie.GetAt(0);
		}

		if(message)
		{
			AfxMessageBox("le format du fichier: "+m_NomFich+ " n'est pas valide"+ChaineMessage,MB_OK);   
		}

		// On ajoute l'enregistrement dans le tableau
		int index = Add(enr);

		// on ajoute l'index de l'élément dans un map mémoire
		Code = enr.GetCode();
		m_MapCodeToIndex.SetAt(enr.GetCode(),index);

	}
	if(GetSize()==0)
	{
		AfxMessageBox("La table des complexes est vide",MB_OK | MB_APPLMODAL);	
	}
	// fermeture du fichier
	m_Fichier.Close(); 
	return (1);
}




bool CTableComplexe::SaveTable()
{

	CString txt;
	// la table a été modifié
	IsModified = 0;

	// sauvegarde
	CFichierCinema Fichier;
	
	if(!Fichier.OpenFileForWrite(m_NomFich))
	{
		return (0);	
	}
	for(int i=0;i<GetSize();i++)
	{
		// sauvegarde du code de complexe
		txt=GetCode(i);
		// séparateur
		txt+="|";
		// sauvegarde du libellé de complexe
		CString Libelle;
		Libelle = GetLibelle(i);
		Libelle.Left(30);
		txt+=Libelle; 
		txt+="|";
		// sauvegarde du code BAC
		txt+=GetAt(i).GetCodeBAC();

		// sauvegarde de la régie
		txt+="|";
		txt+=GetAt(i).GetRegie(); 
		txt+="\n";
		if(!Fichier.WriteLine(txt))return(0);
		

	}
	// caractère de fin de fichier
	if(!Fichier.WriteLine("#"))return(0);
	// fermeture du fichier
	Fichier.Close(); 
	return (1);
}

LPCSTR CTableComplexe::GetCodeBac(int position)
{
	return GetAt(position).GetCodeBAC(); 
}

void CTableComplexe::Tri()
{
	// tableau pour le stockage des codes alphanumériques
	tagComplexe * tabAlpha = new tagComplexe[GetSize()];
	int count=0;
	
	// on boucle sur les complexes
	for(int c=0;c<GetSize();c++)
	{

		tagComplexe enr;
		enr = GetAt(c);
		for(int c2=c+1;c2<GetSize();c2++)
		{
			tagComplexe enr2;
			enr2 = GetAt(c2);
			int code1,code2;
			CString chainecode1,chainecode2;

			chainecode1 = enr.m_Code.Right(5);
			chainecode2 = enr.m_Code.Right(5);
			
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

	delete [] tabAlpha;


}


void CTableComplexe::Decale(int idcomplexe)
{
	for(int id=idcomplexe+1;id<GetSize();id++)
	{
		tagComplexe enr=GetAt(id);
		SetAt(id-1,enr);
	}

}





