// DlgMessage.cpp : implementation file
//

#include "stdafx.h"
#include "DlgMessage.h"


// CDlgMessage dialog

IMPLEMENT_DYNAMIC(CDlgMessage, CDialog)

CDlgMessage::CDlgMessage(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMessage::IDD, pParent)
{
	// Init liste des messages
	m_Ligne_Message.Reset();
}

CDlgMessage::~CDlgMessage()
{
}

void CDlgMessage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LST_MESSAGE,	m_Lst_Message);
	DDX_Control(pDX, IDOK,				m_Btn_Ok);
	DDX_Control(pDX, IDCANCEL,			m_Btn_Cancel);
	DDX_Control(pDX, IDC_QU_MESSAGE,	m_TxtQuMessage);
}


BEGIN_MESSAGE_MAP(CDlgMessage, CDialog)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////
// Affichage du titre boite message 
void CDlgMessage::SetTitre(CString TxtTitre)
{
	m_Titre = TxtTitre;
}

///////////////////////////////////////////////////////////////////////
// Affiche la question finale
void CDlgMessage::SetTexteQuestion(CString TxtQuestion)
{
	m_Question = TxtQuestion;
}

///////////////////////////////////////////////////////////////////////
// Ajout message dans la liste
void CDlgMessage::AddMessage(CString Message)
{
	m_Ligne_Message.AddTail() = Message;
}

BOOL CDlgMessage::OnInitDialog()
{
	CDialog::OnInitDialog();

	this->CenterWindow();

	// Initialise titre
	this->SetWindowText(m_Titre);

	// Ajoute les infos messages
	m_Lst_Message.ResetContent();

	// Ajoute toutes les lignes d'info
	for (m_Ligne_Message.MoveFirst(); m_Ligne_Message.IsValid(); m_Ligne_Message.MoveNext())
	{
		/* A FAIRE >Distinguer les lignes (Titre en gras par exemple)
		// OUI, BONNE IDEE : peut être en ajoutant une fonction AddTitle qui ajoute
		// une ligne comme AddMessage mais dans un style (template XListbox) différent
		if (Index == 0)
			m_Lst_Message.SetItemHeight(Index, 20);
		*/
		
		m_Lst_Message.AddString(m_Ligne_Message.GetItem());
	}

	// Ajoute la question
	m_TxtQuMessage.SetWindowText(m_Question);

	return TRUE;
}
