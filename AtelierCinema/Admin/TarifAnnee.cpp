// TarifAnnee.cpp: implementation of the CTarifAnnee class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "admin.h"
#include "TarifAnnee.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
// L'Application
extern CAdminApp theApp;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CTarifAnnee::~CTarifAnnee()
{
	m_Ligne.RemoveAll();
}

// chargement des tarifs de l'année
int CTarifAnnee::LoadTarif()
{
	CFichierCinema m_Fichier;
#ifdef _DEBUG
	if(!m_Fichier.OpenFileForRead(m_NomFich,1))
#else
	if(!m_Fichier.OpenFileForRead(m_NomFich,0))
#endif
	{
		return (0);
	} 

	// On lit tous les tarifs que l'on place dans les tableaux m_TarifBac,m_TarifComplexe,m_TarifProduit en fonction du code
	// lecture tant que la ligne n'est pas un '#'
	// MODIF FRED : 16/05/2002 : BAC REGIE

	double * tarifs= new double[53];
	while(m_Fichier.ReadLine())
	{
		CString ChaineMessage="";
		CString code = m_Fichier.GetElement(0);
		//CString radical;
		int message=0;

		// le code est il valide ?: 6 ou 7 caractères pour un bac. 
		if((code[0]=='B')&&(code.GetLength()!=7))
		{
			message=1;
			ChaineMessage="\nLe code bac n'est pas défini sur 7 caractères";
		}
		if((code[0]!='B')&&(code.GetLength()!=6))
		{
			message=1;
			ChaineMessage="\nLe code n'est pas défini sur 6 caractères";
		}
		// Lorsque le tarif d'un code bac a été chargé, on controle que le tarif du code lui succédant est un code bac.
		// On controle que le seconde code
		/*if((count!=0)&&(code[0]!='B'))
		{
			message=1;
			ChaineMessage.Format("La table des tarifs est invalide:\nle tarif du bac %s est incorrect (il manque une régie)",codebac); 
		}*/
		if(message)
		{
			AfxMessageBox(ChaineMessage);
			ASSERT(0);
			delete [] tarifs;
			return(0);
		}
		if(code[0]=='B')
		{
			int erreur=0;
			if((code[6]!='A')&&(code[6]!='M')&&(code[6]!='P')&&(code[6]!='N'))erreur=1;
			if(erreur)
			{
				CString message;
				message.Format("Le fichier d'import du tarif est invalide:\nle code du bac %s n'est pas un code composite",code); 
				AfxMessageBox(message);
				delete [] tarifs;
				return(0);
			}
			/*if(count==2)count=0;
			else count++;*/

		}

		// ajout des tarifs de l'année
		for(int s=0;s<53;s++)
		{
			double prix = atof(m_Fichier.GetElement(s+1));
			tarifs[s] = prix;
		}
		AddTarif(code,tarifs,0);
	}
	delete [] tarifs;
	
	// fermeture du fichier
	m_Fichier.Close(); 
	return (1);
}

int CTarifAnnee::SaveTarif()
{
	if(!m_flagModified)return 1; //OK

	CFichierCinema m_Fichier;
	if(!m_Fichier.OpenFileForWrite(m_NomFich))
	{
		return (0);
	} 
	int size = m_Ligne.GetSize();
	// sauvegarde des tarifs
	for(int p=0;p<size;p++)
	{
		// Le code
		CString txt;
		txt+= m_Ligne[p].m_Code;
		txt+="|";

		// les tarifs
		for(int sem=0;sem<53;sem++)
		{
			CString prix;
			prix.Format("%10.2f",m_Ligne[p].m_Tarif[sem]); 
			
			txt+=prix; 
			txt+='|';
		}	
		txt+="\n";
		if(!m_Fichier.WriteLine(txt))return(0);
	}
	// caractère de fin de fichier
	if(!m_Fichier.WriteLine("#"))return(0);

	// fermeture du fichier
	m_Fichier.Close();
	m_flagModified=0;
	return 1; //OK
}


void CTarifAnnee::RemoveTarif(LPCSTR Code)
{
	int idx=GetIndexForCode(Code);
	if(idx>=0)
	{
		m_Ligne.RemoveAt(idx);
		m_flagModified=1;
	}
}

int CTarifAnnee::GetIndexForCode(LPCSTR Code)
{
	for(int i=0;i<m_Ligne.GetSize();i++)
		if(m_Ligne[i].m_Code==Code)return(i);
	return(-1);
}

int CTarifAnnee::AddTarif(LPCSTR Code, double *Tarif,bool flagSetModify)
{
	tagTarif T;
	T.m_Code=Code;
	for(int i=0;i<53;i++)
		T.m_Tarif[i]=Tarif[i];
	if(flagSetModify)m_flagModified=1;
	return(m_Ligne.Add(T));
}

void CTarifAnnee::ModifyCode(LPCSTR Code, LPCSTR NewCode)
{
	int idx=GetIndexForCode(Code);
	if(idx>=0)
	{
		m_Ligne[idx].m_Code=NewCode;
		m_flagModified=1;
	}
}

void CTarifAnnee::ModifyTarif(LPCSTR Code,double *Tarif)
{
	// index du code concerné
	int idx=GetIndexForCode(Code);

	// si trouvé pas de problème mise à jour tarif
	if(idx>=0)
	{
		for(int sem=0;sem<53;sem++)
			m_Ligne[idx].m_Tarif[sem] = Tarif[sem];
		m_flagModified=1;
	}

}

bool CTarifAnnee::IsModified()
{
	return (m_flagModified!=0);

}

double CTarifAnnee::GetTarif(int ligne, int sem)
{
	return m_Ligne[ligne].m_Tarif[sem]; 
}
