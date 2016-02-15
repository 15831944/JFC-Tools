// FicTbl.cpp: implementation of the CFicTbl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FicTbl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFicTbl::CFicTbl()
{
	m_fFileOpened=0;
	m_BuffLine=new char[TAILLEBUFFLINE];
	m_NbField=0;
}

CFicTbl::~CFicTbl()
{
	delete [] m_BuffLine;
	if(m_NbField!=0)delete [] m_Pos;
}


short CFicTbl::OpenFile(LPCSTR aNomFic,char aMode)
{

	if(m_fFileOpened!=0){
		AfxMessageBox("OpenFile() sur un fichier déjà ouvert ???");
		return(1);
	}

	CString MsgErr;
	BOOL r;

	m_NomFic=aNomFic;
	switch(aMode){
		case 'r': 
			r=m_FicTbl.Open(aNomFic,CFile::modeRead | CFile::typeText);
		break;

		case 'w':
			r=m_FicTbl.Open(aNomFic,CFile::modeCreate | CFile::modeWrite | CFile::typeText);
		break;
	}

	if(!r){
		MsgErr.Format("Erreur à l'ouverture du fichier %s ",aNomFic);
		AfxMessageBox(MsgErr);
		return(1);
	}

	m_NroLigne=0;
	m_fFileOpened=1;
	return(0);
}

short CFicTbl::CloseFile()
{
	m_FicTbl.Close();
	m_fFileOpened=0;
	return(0);
}

short CFicTbl::FindStar()
{
	CString MsgErr;

	do{
		if(!m_FicTbl.ReadString(m_BuffLine,TAILLEBUFFLINE)){
			MsgErr.Format("Erreur à la recherche de * dans %s ",m_NomFic);
			AfxMessageBox(MsgErr);
			return(0);
		}
		m_NroLigne++;
	}while(m_BuffLine[0]!='*');

	SupprimeCARIAGE_LINE(m_BuffLine);
	return((short)atoi(&m_BuffLine[1]));
}

short CFicTbl::FindHash()
{
	CString MsgErr;

	do{
		if(!m_FicTbl.ReadString(m_BuffLine,TAILLEBUFFLINE)){
			MsgErr.Format("Erreur à la recherche de * dans %s ",m_NomFic);
			AfxMessageBox(MsgErr);
			return(0);
		}
		m_NroLigne++;
	}while(m_BuffLine[0]!='#');

	SupprimeCARIAGE_LINE(m_BuffLine);
//	return(1);
	return((short)atoi(&m_BuffLine[1]));
}



//Suppression des blancs de fin superflus
void CFicTbl::SupBlancsFin(LPSTR  ch)
{	
	short i;
	for (i=strlen(ch)-1; i>=0; i--)
		if (ch[i]!=' ') break;
	ch[i+1]=0;
}

//Suppression des caracteres<32 et 
void CFicTbl::SupprimeCARIAGE_LINE(LPSTR txt)
{	
	short i, l;
	l=strlen(txt)-1;
	for(i=l ; i>=0 ; i--){
		if (txt[i]<32){
		    txt[i]=0;
			return;
		}
	}
}


short CFicTbl::FormatteLine()
{	
	short i;

	SupprimeCARIAGE_LINE(m_BuffLine);
	// des 0 pour arreter les champs
	for(i=1 ; i<m_NbField ; i++)m_BuffLine[m_Pos[i]]=0;
	// on réduit les champs
	SupBlancsFin(m_BuffLine);	//1er champ
	for(i=1 ; i<m_NbField ; i++)			//les autres
		SupBlancsFin(&m_BuffLine[m_Pos[i]+1]);
	return(0);
}

short CFicTbl::SetLineFormat(short Nb0,short Nb1,short Nb2,short Nb3,short Nb4,short Nb5,
	short Nb6,short Nb7,short Nb8,short Nb9,short Nb10,short Nb11,short Nb12,short Nb13,
	short Nb14,short Nb15,short Nb16)
{	
	short x;
	
	if(m_NbField!=0) delete [] m_Pos;

	if(Nb0==0){
		AfxMessageBox("Erreur dans la commande CFicTbl::SetLineFormat");
		return(1);
	}
	if(Nb1==0)m_NbField=1;
	else if(Nb2==0)m_NbField=2;		else if(Nb3==0)m_NbField=3;
	else if(Nb4==0)m_NbField=4;		else if(Nb5==0)m_NbField=5;
	else if(Nb6==0)m_NbField=6;		else if(Nb7==0)m_NbField=7;
	else if(Nb8==0)m_NbField=8;		else if(Nb9==0)m_NbField=9;
	else if(Nb10==0)m_NbField=10;	else if(Nb11==0)m_NbField=11;
	else if(Nb12==0)m_NbField=12;	else if(Nb13==0)m_NbField=13;
	else if(Nb14==0)m_NbField=14;	else if(Nb15==0)m_NbField=15;
	else m_NbField=16;

	m_Pos=new short [m_NbField];
	m_Pos[0]=x=-1;
	if(m_NbField>1)m_Pos[1]=x+=Nb0+1;		if(m_NbField>2)m_Pos[2]=x+=Nb1+1;
	if(m_NbField>3)m_Pos[3]=x+=Nb2+1;		if(m_NbField>4)m_Pos[4]=x+=Nb3+1;
	if(m_NbField>5)m_Pos[5]=x+=Nb4+1;		if(m_NbField>6)m_Pos[6]=x+=Nb5+1;
	if(m_NbField>7)m_Pos[7]=x+=Nb6+1;		if(m_NbField>8)m_Pos[8]=x+=Nb7+1;
	if(m_NbField>9)m_Pos[9]=x+=Nb8+1;		if(m_NbField>10)m_Pos[10]=x+=Nb9+1;
	if(m_NbField>11)m_Pos[11]=x+=Nb10+1;	if(m_NbField>12)m_Pos[12]=x+=Nb11+1;
	if(m_NbField>13)m_Pos[13]=x+=Nb12+1;	if(m_NbField>14)m_Pos[14]=x+=Nb13+1;
	if(m_NbField>15)m_Pos[15]=x+=Nb14+1;	if(m_NbField>16)m_Pos[16]=x+=Nb15+1;
	if(m_NbField>17)m_Pos[17]=x+=Nb16+1;
	return(0);
}

short CFicTbl::ReadLine(void)
{	
	CString MsgErr;
	
	if(m_NbField==0){
		AfxMessageBox("SetFormatLine() indispensable dans fc CFicTbl::ReadLine");
		return(1);
	}
	memset(m_BuffLine, 0, TAILLEBUFFLINE);
	if(!m_FicTbl.ReadString(m_BuffLine,TAILLEBUFFLINE)){
		MsgErr.Format("Impossible de lire la ligne %d",m_NroLigne);
		AfxMessageBox(MsgErr);
		return(1);
	}
	m_NroLigne++;
	FormatteLine();
	return(0);
}


short CFicTbl::WriteStar(short aNbRec)
{	
	sprintf(m_BuffLine,"*%d\n", aNbRec);
	m_FicTbl.WriteString(m_BuffLine);
	return(0);
}

short CFicTbl::WriteHash(short aNbRec)
{	
	sprintf(m_BuffLine,"#\n");
	m_FicTbl.WriteString(m_BuffLine);
	return(0);
}

short CFicTbl::WriteLine(LPCSTR aLine)
{	
	CString MsgErr;
	sprintf(m_BuffLine,"%s\n",aLine);
	m_FicTbl.WriteString(m_BuffLine);
	return(0);

}

char CFicTbl::GetChar(short PosField)
{	
	return(m_BuffLine[1+m_Pos[PosField]]);
}

LPCSTR CFicTbl::GetCharPtr(short PosField)
{	
	return(&m_BuffLine[1+m_Pos[PosField]]);
}

short CFicTbl::GetShort(short PosField)
{	
	return((short)atoi(&m_BuffLine[1+m_Pos[PosField]]));
}

long CFicTbl::GetLong(short PosField)
{	
	return(atol(&m_BuffLine[1+m_Pos[PosField]]));
}
