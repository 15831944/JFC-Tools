// Saisonnalite.cpp: implementation of the CSaisonnalite class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Saisonnalite.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#include "CAlendrierHebdoCinema.h"
#include "CFichierCinema.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSaisonnalite::CSaisonnalite()
{
	m_IsModified=0;
}

CSaisonnalite::~CSaisonnalite()
{

}
CSaisonnalite::CSaisonnalite(const CSaisonnalite & data)
{
	*this=data;
};

bool CSaisonnalite::ImportFile()
{
	return(1);
}

double CSaisonnalite::GetSaisonnalite(int semaine)
{
	double saisonnnalite;
	saisonnnalite = (double)(m_CoeffSaisonnalite.GetAt(semaine))/100.0; 
	return saisonnnalite;
}

bool CSaisonnalite::InitialiseExercice(COleDateTime date, int nbsemaines)
{
	// Le fichier
	CFichierCinema Fichier;
	// l'année
	int annee;
	// la semaine
	int semaine;
	int semDepart;

	m_CoeffSaisonnalite.SetSize(0);

	
	COleDateTime newDate;
	// tableau temporaire pour le chargement
	CWordArray coeffs;
	coeffs.SetSize(53);

	// le calendrier pour savoir quelle semaine on est
	CCAlendrierHebdoCinema cal;

	CString nomFich; 
	// initalisation de la date de l'exercice
	m_Exercice = date;
	// initalisation de la durée de l'exercice
	m_Duree = nbsemaines;

	semDepart = cal.GetNumSemaine(date);
	annee = -1;

	// On boucle sur les semaines pour chaque ligne	
	for(int i=0;i<nbsemaines;i++)	
	{	
		COleDateTimeSpan span(7*i,0,0,0);	
		newDate = date + span;	
		if(annee!=cal.GetAnnee(newDate))	
		{	
			annee =  cal.GetAnnee(newDate);	
			// on charge le fichier	
			nomFich.Format("Saison%04d.table",annee);

			if(!Fichier.OpenFileForRead(nomFich,1))		
			{		
				// préparation d'un tableau avec un coeff neutre = 100
				for(int j=0;j<53;j++)
				{
					coeffs.SetAt(j,100);
				}
			}
			else
			{

				for(int count=0;count<53;count++)
				{

					if(!Fichier.ReadLine())
					{
						// la fin du fichier est atteinte
						// on controle que l'on a bien lu 53 ligne
						if(count!=53)
						{
							nomFich.Format("le fichier de saisonnalité:Saison%04d.table ne comporte pas 53 lignes",annee); 
							AfxMessageBox(nomFich);

							// préparation d'un tableau avec un coeff neutre = 100
							for(int j=0;j<53;j++)
							{
								coeffs.SetAt(j,100);
							}
						}
			
					}
					else
					{
						coeffs.SetAt(count,(WORD)atoi(Fichier.GetElement(0)));
					}
					
				}
				
				// fermeture du fichier
				Fichier.Close(); 
			}
		}

		semaine = cal.GetIndexSemaine(newDate);
		m_CoeffSaisonnalite.Add(coeffs[semaine]); 
	}

	return (1);
}		

int CSaisonnalite::LoadSaison()
{
	CFichierCinema m_Fichier;
	if(!m_Fichier.OpenFileForRead(m_NomFich,1))
	{
		return (0);
	} 
	int count=0;
	while(m_Fichier.ReadLine())
	{
		count++;
		m_CoeffSaisonnalite.Add((WORD)atoi(m_Fichier.GetElement(0))); 
	}	
	// On vérifie que l'on a bien 53 saisonnalités
	ASSERT(count==53);
	m_Fichier.Close();
	return(1);
}

void CSaisonnalite::SetSaisonnalite(int saisonnalite,int pos)
{
	if(pos>=m_CoeffSaisonnalite.GetSize())m_CoeffSaisonnalite.Add((WORD)saisonnalite);
	else m_CoeffSaisonnalite.SetAt(pos,(WORD)saisonnalite); 
	m_IsModified=1;
}

int CSaisonnalite::SaveSaisonnalite()
{
	CFichierCinema m_Fichier;	
	if(m_IsModified)	
	{	

		if(!m_Fichier.OpenFileForWrite(m_NomFich))		
		{		
			return (0);		
		} 		
		// bouclage sur les semaines	
		for(int sem=0;sem<53;sem++)	
		{	
			CString coeff;	
			coeff.Format("%.0f",GetSaisonnalite(sem)*100);	
			coeff+="\n";	
			m_Fichier.WriteLine(coeff); 	
		}	
		m_Fichier.WriteLine("#"); 	
		m_Fichier.Close(); 	
	}
	m_IsModified=0;
	return (1);		

}

int CSaisonnalite::IsModif()
{
	return m_IsModified;
}

void CSaisonnalite::SetModified()
{
	m_IsModified=1;
}