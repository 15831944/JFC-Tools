// TableBAC.cpp: implementation of the CTableBAC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TableBAC.h"
#include "TableComplexe.h"
#include "TableNbEntreeComplexe.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTableBAC::CTableBAC()
{
	m_NomFich="bac.table";
	// m_NomFich="bacs.table";
	SetName(m_NomFich);
	IsModified=0;

}

// copy constructor
CTableBAC::CTableBAC(const CTableBAC &Data)
{
	*this = Data;
};

CTableBAC::~CTableBAC()
{

}

bool CTableBAC::LoadTable()
{
	RemoveAll();

	CFichierCinema Fichier;
	if(!Fichier.OpenFileForRead(m_NomFich,1))
	{
		return (0);
	} 
	bool message=0;
	CString ChaineMessage;
	// On lit tous les bacs que l'on place dans la table m_Bacs
	// lecture tant que la ligne n'est pas vide
	while(Fichier.ReadLine())
	{
		tagBAC enr;
		// On stocke d'abord le code
		enr.m_Code  = Fichier.GetElement(0) ;
		// le code est il valide ?: 6 caractères
		if(enr.m_Code.GetLength()>6)
		{
			message=1;
			ChaineMessage="\nLe code est supérieur à 6 caractères";
			enr.m_Code = enr.m_Code.Left(6);
			enr.m_CodeComposite=enr.m_Code;
		}
		// le libellé est-il valide ?: 30 caractères
		enr.m_Libelle = Fichier.GetElement(1) ;
		if(enr.m_Libelle.GetLength()>30)
		{
			message=1;
			ChaineMessage+="\nLe libellé est supérieur à 30 caractères";
			enr.m_Libelle = enr.m_Libelle.Left(30);
			
		}
		// On enlève les espaces se trouvant après le libellé
		enr.m_Libelle.TrimRight();

		// On ajoute l'enregistrement dans le tableau
		int index = Add(enr);

		// on ajoute l'index de l'élément dans un map mémoire
		CString CodeBac;
		CodeBac = enr.GetCode();
		m_MapCodeToIndex.SetAt(enr.GetCode(),index);
	}
	if(GetSize()==0)
	{
			AfxMessageBox("La table des bacs est vide",MB_OK | MB_APPLMODAL);	
			return (0);
	}
	if(message)
	{
		AfxMessageBox("le format du fichier: "+m_NomFich+" n'est pas valide"+ChaineMessage,MB_OK | MB_APPLMODAL);   
	}
	// fermeture du fichier
	Fichier.Close(); 
	return (1);
}






bool CTableBAC::SaveTable()
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
		// sauvegarde du code
		txt=GetCode(i);
		// séparateur
		txt+="|";
		// sauvegarde du libellé
		CString Libelle;
		Libelle = GetLibelle(i);
		// troncature du libellé à 30 caractères
		txt+=Libelle.Left(30) ;
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
	// fermeture du fichier
	Fichier.Close(); 
	return (1);
}

void CTableBAC::Tri()
{
	// tableau pour le stockage des codes alphanumériques
	tagBAC * tabAlpha = new tagBAC[GetSize()];
	int count=0;
	
	// on boucle sur les bacs
	for(int bac=0;bac<GetSize();bac++)
	{

		tagBAC enr;
		enr = GetAt(bac);
		for(int bac2=bac+1;bac2<GetSize();bac2++)
		{
			tagBAC enr2;
			enr2 = GetAt(bac2);
			int code1,code2;
			CString chainecode1,chainecode2;

			chainecode1 = enr.m_Code.Right(5);
			chainecode2 = enr.m_Code.Right(5);
			
			code1 = atoi(chainecode1);
			code2 = atoi(chainecode2);

			for(int c=0;c<5;c++)
			{
				// les codes alphanumériques sont ajoutés à la fin
				if((chainecode1.GetAt(c)<=47)||(chainecode1.GetAt(c)>=58))
				{
					tabAlpha[count]=enr;
					Decale(bac);
					count++;
					IsModified=1;
					goto label;
				}
				if((chainecode2.GetAt(c)<=47)||(chainecode2.GetAt(c)>=58))
				{
					tabAlpha[count]=enr2;
					Decale(bac2);
					count++;
					IsModified=1;
					break;


				}
				
			}

			if(code2<code1)
			{
				SetAt(bac,enr2);
				SetAt(bac2,enr);
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

	delete[] tabAlpha;

	

}

// décale les index de la table de idbac+1 à la fin
void CTableBAC::Decale(int idbac)
{
	for(int id=idbac+1;id<GetSize();id++)
	{
		tagBAC enr=GetAt(id);
		SetAt(id-1,enr);
	}
}

// Récup bac d'après le code bac
tagBAC CTableBAC::GetBac(CString codebac)
{
	for(int id=0;id<GetSize();id++)
	{
		tagBAC enr=GetAt(id);
		if (enr.GetCode() == codebac)
			return enr;
	}
	ASSERT(0);
}	

// retourne l'index du bac à partir de son code composite
/*int GetIndexFromCodeComposite(CString codecomposite)
{
#ifdef _DEBUG
	if(codecomposite.GetLength()!=7)
	{
		AfxMessageBox("Ceci n'est pas un code composite");
	}
#endif
	if(codecomposite[6]=='P')
	{
		codecomposite = codecomposite.Left(codecomposite.GetLength()-1);
		return FindIndexFromCode(codecomposite);
	}
	else
	{
		return FindIndexFromCode(codecomposite);
	}
}*/
