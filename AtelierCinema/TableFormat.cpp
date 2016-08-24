// TableFormat.cpp: implementation of the CTableFormat class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TableFormat.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTableFormat::CTableFormat()
{
	m_NomFich = "format.table"; 
	SetName(m_NomFich);
	IsModified=0;

}

CTableFormat::~CTableFormat()
{

}
CTableFormat::CTableFormat(const CTableFormat & Data)
{
	*this=Data;
};

bool CTableFormat::LoadTable()
{
	RemoveAll();

	CFichierCinema m_Fichier;

	if(!m_Fichier.OpenFileForRead(m_NomFich,1))
	{
		return (0);
	} 

	// On lit tous les tempos que l'on place dans la table m_TableTempo
	// lecture tant que la ligne n'est pas un '#'

	while(m_Fichier.ReadLine())
	{
		bool message=0;
		CString ChaineMessage="";
		CString coeff;
		tagFormat enr;

		// On stocke d'abord le libellé
		CString libelle;
		libelle = m_Fichier.GetElement(0) ;

		// le libellé est il valide ?: 3 caractères max
		if(libelle.GetLength()>3)
		{
			message=1;
			ChaineMessage="\nLe libellé du format n'est pas défini sur trois caractères";
			libelle= libelle.Left(3);
		}
		// On enlève les blancs
		libelle.TrimRight();
		enr.m_Libelle = libelle;
		coeff = m_Fichier.GetElement(1);
		enr.m_Coeff = atoi(coeff);

		if(coeff.GetLength()!=3)
		{
			message=1;
			ChaineMessage="\nLe coefficient du format n'est pas défini sur trois caractères";

		}
				
		if(message)
		{
			AfxMessageBox("le format du fichier: "+m_NomFich+ " n'est pas valide"+ChaineMessage,MB_OK);   
		}

		// On ajoute l'enregistrement dans le tableau
		int index = Add(enr);
	}
	if(GetSize()==0)
	{
		AfxMessageBox("La table des formats est vide",MB_OK | MB_APPLMODAL);	
	}
	// fermeture du fichier
	m_Fichier.Close(); 
	return (1);

	// Pour l'instant, la table des formats est construite en dur
	/*tagFormat frm;
	frm.m_Libelle = "20";
	frm.m_Coeff = 80;
	Add(frm);
	frm.m_Libelle = "25";
	frm.m_Coeff = 90;
	Add(frm);
	frm.m_Libelle = "30";
	frm.m_Coeff = 100;
	Add(frm);
	frm.m_Libelle = "35";
	frm.m_Coeff = 115;
	Add(frm);
	frm.m_Libelle = "40";
	frm.m_Coeff = 135;
	Add(frm);
	frm.m_Libelle = "45";
	frm.m_Coeff = 150;
	Add(frm);
	frm.m_Libelle = "50";
	frm.m_Coeff = 165;
	Add(frm);
	frm.m_Libelle = "60";
	frm.m_Coeff = 200;
	Add(frm);
	frm.m_Libelle = "70";
	frm.m_Coeff = 235;
	Add(frm);
	frm.m_Libelle = "80";
	frm.m_Coeff = 265;
	Add(frm);
	frm.m_Libelle = "85";
	frm.m_Coeff = 285;
	Add(frm);
	frm.m_Libelle = "90";
	frm.m_Coeff = 300;
	Add(frm);
	return (1);*/
	
}

double CTableFormat::GetCoefficient(LPCSTR libelle)
{
	CString message;
	// index du libellé dans la table des formats
	int index = FindIndexFromCode(libelle);
	
	if(index<0)
	{
		message.Format("Ce format:%s n'existe pas!!",libelle);
		AfxMessageBox(message);
		return 1.0;
	}
	// On récupère le coefficient correspondant
	return GetAt(index).m_Coeff/100; 
}

int CTableFormat::FindIndexFromCode(LPCSTR code)
{
	// recherche de l'index correspondant au code
	for(int i=0;i<GetSize();i++)
	{	
		CString ref = GetAt(i).GetLibelle();	
		if(ref==code)return(i);	
	}	

	// Pas trouvé ?	
	return(-1);	

}

int CTableFormat::SaveTable()
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
		txt="";
		CString coefficient;
		CString libelle;

		// sauvegarde du libellé coefficient
		libelle = GetLibelle(i);
		if(libelle.GetLength()<3)
		{
			while(libelle.GetLength()!=3)
			{
				libelle+=" ";
			}
		}
		txt+=libelle;
		txt+="|";
		// sauvegarde du coefficient
		coefficient.Format("%2.0f",GetAt(i).m_Coeff);
		if(coefficient.GetLength()<3)
		{
			while(coefficient.GetLength()!=3)
			{
				coefficient+=" ";
			}
		}
		txt+=coefficient;
		txt+="\n";
		if(!Fichier.WriteLine(txt))return(0);
		

	}
	// caractère de fin de fichier
	if(!Fichier.WriteLine("#"))return(0);
	// fermeture du fichier
	Fichier.Close(); 
	return (1);

}
