// Cible.cpp: implementation of the CCible class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cinema.h"
extern CCinemaApp theApp;
#include "Cible.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCible::CCible()
{
	// cf Init
}

CCible::~CCible()
{
	delete [] m_LibClasse;
}



BOOL CCible::ChargerDico(void)
{
	FILE	*fp_Dico;
	FILE	*fp_Lib;
	short	x;

	char txt[200];

	CString nomfic;
	nomfic=theApp.m_Chemin+"\\SOURCE\\VAGUE.DIC";
	if((fp_Dico=fopen(nomfic,"rt"))==NULL) return(FALSE);
	nomfic=theApp.m_Chemin+"\\SOURCE\\dico.TXT";
	if((fp_Lib=fopen(nomfic,"rt"))==NULL) return(FALSE);
	
	while(fgets(txt,200,fp_Dico)!=NULL) if(txt[0]!=';') break;
	m_NbCritere=atoi(txt);
	m_LibCritere.SetSize(m_NbCritere);
	m_NbClasse.SetSize(m_NbCritere);
	m_LibClasse=new CStringArray[m_NbCritere];
	for(x=0;x<m_NbCritere;x++)
	{
		while(fgets(txt,200,fp_Dico)!=NULL) if(txt[0]!=';') break;
		m_NbClasse[x]=(short)atoi(txt);

		while(fgets(txt,200,fp_Lib)!=NULL) if(txt[0]!=';') break;
		txt[strlen(txt)-1]=0;
		m_LibCritere[x]=txt;

		for(int c=0;c<m_NbClasse[x];c++)
		{
			while(fgets(txt,200,fp_Lib)!=NULL) if(txt[0]!=';') break;
			txt[strlen(txt)-1]=0;
			m_LibClasse[x].Add(txt);
			
		}
		fgets(txt,200,fp_Lib);
	}
	fclose(fp_Dico);
	fclose(fp_Lib);
	return(TRUE);
}


BOOL CCible::ChargerIndexCib()
{
	FILE	*fp_Index;
	short	NbCibles=0;
	short	x=0;
	struct	IndexCible *Temp=NULL;

	m_NumCible.RemoveAll();
	m_LibCible.RemoveAll();
	CString txt;
	txt=theApp.m_Chemin+"\\CIBLE\\CIBLES.IDX";
	if((fp_Index=fopen(txt,"rb"))==NULL)
	{
		m_NextNumCible=1;
		return(TRUE);
	}
	short version,nbcible;
	fread(&version,sizeof(version),1,fp_Index);	// la version
	fread(&nbcible,sizeof(nbcible),1,fp_Index);	// le nombre de cibles
	fread(&m_NextNumCible,sizeof(m_NextNumCible),1,fp_Index); 	// le numero de cible suivant
	for(x=0;x<nbcible;x++)
	{
		WORD num;
		char lib[MAXLIBELLE+1];
		fread(&num,sizeof(short),1,fp_Index); // Numero de la cible
		fread(lib,sizeof(char),MAXLIBELLE+1,fp_Index); // Nom de la cible
		m_NumCible.Add(num);
		m_LibCible.Add(lib);
	}
	fclose(fp_Index);
	return(TRUE);
}

BOOL CCible::MemoriserIndexCib()
{
	FILE	*fp_Index;
	short	NbCibles=m_NumCible.GetSize();
	ASSERT(m_NumCible.GetSize()==m_LibCible.GetSize());

	CString txt;
	txt=theApp.m_Chemin+"\\CIBLE\\CIBLES.IDX";
	if(NbCibles==0) // if no cibles remove old IDX
	{
		remove(txt);
		return(TRUE);
	}

	if((fp_Index=fopen(txt,"wb"))==NULL)
	{
		AfxMessageBox("Unable to create index cibles");
		return(FALSE);
	}
	short version=1;
	fwrite(&version,sizeof(short),1,fp_Index);	// la version
	fwrite(&NbCibles,sizeof(short),1,fp_Index);	// le nombre de cibles
	fwrite(&m_NextNumCible,sizeof(short),1,fp_Index); 	// le numero de cible suivant
	
	for(int c=0;c<NbCibles;c++)
	{
		WORD num=m_NumCible[c];
		char lib[MAXLIBELLE+1];
		strcpy(lib,m_LibCible[c]);
		fwrite(&num,sizeof(short),1,fp_Index); // Numero de la cible
		fwrite(lib,sizeof(char)*MAXLIBELLE+1,1,fp_Index); // Nom de la cible
	}
	fclose(fp_Index);
	return(TRUE);
}

BOOL CCible::DeleteCible(short NumCible)
{
	struct IndexCible *Temp=NULL;
	struct IndexCible *Temp2=NULL;

	CString txt;
	for(int c=0;c<m_NumCible.GetSize();c++)
	{
		if(m_NumCible[c]==NumCible)
		{
			txt.Format("%s\\CIBLE\\CIB%05d.CIB",theApp.m_Chemin,NumCible);
			remove(txt);
			m_NumCible.RemoveAt(c);
			m_LibCible.RemoveAt(c);
			MemoriserIndexCib();
			return(TRUE);
		}
	}
	return(FALSE);
}

BOOL CCible::RenommerCible(short NumCible,LPCSTR Nom)
{
	struct IndexCible *Temp=NULL;

	for(int c=0;c<m_NumCible.GetSize();c++)
	{
		if(m_NumCible[c]==NumCible)
		{
			m_LibCible[c]=Nom;
			MemoriserIndexCib();
			return(TRUE);
		}
	}
	return(TRUE);
}

BOOL CCible::ChargerCible(TabSegment & Segment, short NumCible)
{
	FILE		*fp_Cible=NULL;
	short		VerCible,NbSegments;
	short		x,y,z;

	Segment.RemoveAll();
	CString txt;
	txt.Format("%s\\CIBLE\\CIB%05d.CIB",theApp.m_Chemin,NumCible);
	if((fp_Cible=fopen(txt,"rb"))==NULL)
	{
		AfxMessageBox("cannot read cible");
		return(FALSE);
	}
	fread(&VerCible,sizeof(short),1,fp_Cible);
	fread(&NbSegments,sizeof(short),1,fp_Cible);
	for(x=0;x<NbSegments;x++)
	{
		SEGMENT S;
		short NbCritere;
		fread(&NbCritere,sizeof(short),1,fp_Cible);
		for(y=0;y<NbCritere;y++)
		{
			CRITERE C;
			short nrcritere;
			fread(&nrcritere,sizeof(short),1,fp_Cible);
			C.NrCritere=nrcritere;
			short nbclasses;
			fread(&nbclasses,sizeof(short),1,fp_Cible);
			for(z=0;z<m_NbClasse[nrcritere];z++)C.Poids.Add(0);
			for(z=0;z<nbclasses;z++)
			{
				short classe,poids;
				fread(&classe,sizeof(short),1,fp_Cible);
				fread(&poids,sizeof(short),1,fp_Cible);
				C.Poids[classe]=(BYTE)poids;
			}
			S.Critere.Add(C);
		}
		Segment.Add(S);
	}
	fclose(fp_Cible);
	return(TRUE);
}


BOOL CCible::MemoriserCible(TabSegment & Segment,short NumCible)
{
	
	short VerCible=1;
	CString txt;
	txt.Format("%s\\CIBLE\\CIB%05d.CIB",theApp.m_Chemin,NumCible);
	FILE	*fp_Cible=NULL;
	if((fp_Cible=fopen(txt,"wb"))==NULL)
	{
		AfxMessageBox("cannot create cible");
		return(FALSE);
	}
	fwrite(&VerCible,sizeof(short),1,fp_Cible);
	short NbSegment=Segment.GetSize();
	fwrite(&NbSegment,sizeof(short),1,fp_Cible);
	for(int s=0;s<NbSegment;s++)
	{
		short NbCritere=Segment[s].Critere.GetSize();
		fwrite(&NbCritere,sizeof(short),1,fp_Cible);
		for(int c=0;c<NbCritere;c++)
		{
			short NrCritere=Segment[s].Critere[c].NrCritere;
			fwrite(&NrCritere,sizeof(short),1,fp_Cible);
			short NbClasses=0;
			for(short x=0;x<Segment[s].Critere[c].Poids.GetSize();x++)
			{
				if(Segment[s].Critere[c].Poids[x])NbClasses++;
			}
			fwrite(&NbClasses,sizeof(short),1,fp_Cible);
			for(x=0;x<Segment[s].Critere[c].Poids.GetSize();x++)
			{
				if(Segment[s].Critere[c].Poids[x])
				{
					fwrite(&x,sizeof(short),1,fp_Cible);
					short Poids=(short)Segment[s].Critere[c].Poids[x];
					fwrite(&Poids,sizeof(short),1,fp_Cible);
				}
			}
		}
	}
	fclose(fp_Cible);
	MemoriserIndexCib();
	return(TRUE);
}
// Vecteur à libérer au niveau de l'appellant
BYTE* CCible::GetNewVecteurCritere(int nrcritere) 
{
	FILE *fp_ind=NULL;
	char			tampon[1000];

	int NbIndiv=theApp.NbIndiv;
	BYTE * Classes=new BYTE [NbIndiv];

	sprintf(tampon,"%s\\SOURCE\\VAGUE.CRI",theApp.m_Chemin);	// nom du fichier des criteres
	if (NULL==(fp_ind=fopen(tampon,"rb"))) return NULL;	// ouvrir le fichier des criteres

	long pos=(long)nrcritere*(long)NbIndiv;	// trouve position index
	fseek(fp_ind,pos,SEEK_SET);	// seek position
	fread(Classes,NbIndiv,1,fp_ind);	// par section bien sur
	fclose(fp_ind);
	return Classes;
}
void CCible::CalculCible(TabSegment & Segment,double * Vecteur)
{
	char			tampon[1000];
	FILE 			*fp_ind=NULL;

//	if(Segment[0].Critere.GetSize()==0)return;
	sprintf(tampon,"%s\\SOURCE\\VAGUE.CRI",theApp.m_Chemin);	// nom du fichier des criteres
	if (NULL==(fp_ind=fopen(tampon,"rb"))) return;	// ouvrir le fichier des criteres

	int NbIndiv=theApp.NbIndiv;
	double * CoeffEx = theApp.CoeffEx;
	BYTE * sel=new BYTE [NbIndiv];
	BYTE * totsel=new BYTE [NbIndiv];
	BYTE * Classes=new BYTE [NbIndiv];
	memset(totsel,0,NbIndiv);
	
	double poptot=0;

	for(int ind=0;ind<NbIndiv;ind++)
	{
		poptot+=CoeffEx[ind];
	}
	
	// Boucler sur les groupes
	for (int seg=0; seg<Segment.GetSize(); seg++)
	{
		memset(sel,1,NbIndiv);
		for(int cri=0;cri<Segment[seg].Critere.GetSize();cri++)
		{
			int Critere=Segment[seg].Critere[cri].NrCritere;
			long pos=(long)Critere*(long)NbIndiv;	// trouve position index
			fseek(fp_ind,pos,SEEK_SET);	// seek position
			fread(Classes,NbIndiv,1,fp_ind);	// par section bien sur
			for(int ind=0;ind<NbIndiv;ind++)
			{
				if(Segment[seg].Critere[cri].Poids[Classes[ind]]==0)
				{
					//TRACE("individus éliminés:%d\n",ind);
					sel[ind]=0;
				}
			}
		}

		double pop=0;
		int nbcas=0;
		for(int ind=0;ind<NbIndiv;ind++)
		{
			if(sel[ind])
			{
				totsel[ind]=1;
				pop+=CoeffEx[ind];
				nbcas++;
			}
		}
		Segment[seg].PopPhy=pop;
		Segment[seg].PopPon=pop;
		Segment[seg].NbCas=nbcas;
		Segment[seg].PctUni=100.0*pop/poptot;
	}

	double pop=0;
	int nbcas=0;
	for(ind=0;ind<NbIndiv;ind++)
	{
		if(totsel[ind])
		{
			pop+=CoeffEx[ind];
			nbcas++;
		}
	}
	m_PopPhy=m_PopPon=pop;
	m_NbCas=nbcas;
	m_PctUni=100.0*pop/poptot;

	if(Vecteur)
	{
		for(ind=0;ind<NbIndiv;ind++)
		{
			if(totsel[ind])
			{
				Vecteur[ind]=CoeffEx[ind];
			}
			else 
			{
				Vecteur[ind]=0;
			}
		}
	}

	delete [] sel;
	delete [] totsel;
	delete [] Classes;
	
	fclose(fp_ind);
}



BOOL CCible::GetVectCible(VECTCIBLE *Vecteur)
{
	TabSegment Segment;
	ChargerCible(Segment,Vecteur->Cible);

	if(Vecteur->Vecteur==NULL)
		Vecteur->Vecteur=new double [theApp.NbIndiv];
	CalculCible(Segment,Vecteur->Vecteur);
	Vecteur->PopPhy=m_PopPhy;
	Vecteur->PopPon=m_PopPon;
	Vecteur->NbCas=m_NbCas;
	Vecteur->PctUni=m_PctUni;


	return(TRUE);
}



void CCible::Init()
{
	ChargerDico();
	ChargerIndexCib();
}

int CCible::GetIndexFromNumero(int Numero)
{
	for(int i=0;i<m_NumCible.GetSize();i++)
		if(m_NumCible[i]==Numero)return i;
	return -1;
}
