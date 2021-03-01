// RepItemsBuddyDlg.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "RepItemsBuddy.h"
#include "RepItemsBuddyDlg.h"
#include "DlgProxy.h"
#include ".\repitemsbuddydlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// boîte de dialogue CRepItemsBuddyDlg




IMPLEMENT_DYNAMIC(CRepItemsBuddyDlg, CDialog);
CRepItemsBuddyDlg::CRepItemsBuddyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRepItemsBuddyDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pAutoProxy = NULL;
}
CRepItemsBuddyDlg::~CRepItemsBuddyDlg()
{
	// S'il existe un proxy Automation pour cette boîte de dialogue, définir
	//  la valeur NULL pour son pointeur de retour vers cette boîte de dialogue, afin qu'il sache
	//  que celle-ci a été supprimée.
	if (m_pAutoProxy != NULL)
		m_pAutoProxy->m_pDialog = NULL;
}

void CRepItemsBuddyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CRepItemsBuddyDlg, CDialog)
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_DEBUG_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// gestionnaires de messages pour CRepItemsBuddyDlg

BOOL CRepItemsBuddyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Définir l'icône de cette boîte de dialogue. L'infrastructure effectue cela automatiquement
	//  lorsque la fenêtre principale de l'application n'est pas une boîte de dialogue
	SetIcon(m_hIcon, TRUE);			// Définir une grande icône
	SetIcon(m_hIcon, FALSE);		// Définir une petite icône

	// TODO : ajoutez ici une initialisation supplémentaire
	
	return TRUE;  // retourne TRUE, sauf si vous avez défini le focus sur un contrôle
}

// Si vous ajoutez un bouton Réduire à votre boîte de dialogue, vous devez utiliser le code ci-dessous
//  pour dessiner l'icône. Pour les applications MFC utilisant le modèle Document/Vue,
//  cela est fait automatiquement par l'infrastructure.

void CRepItemsBuddyDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // contexte de périphérique pour la peinture

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Centrer l'icône dans le rectangle client
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Dessiner l'icône
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// Le système appelle cette fonction pour obtenir le curseur à afficher lorsque l'utilisateur fait glisser
//  la fenêtre réduite.
HCURSOR CRepItemsBuddyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// Les serveurs Automation ne doivent pas quitter lorsqu'un utilisateur ferme l'interface utilisateur
//  alors qu'un contrôleur est encore placé sur l'un de ses objets. Ces
//  gestionnaires de messages s'assurent que 
//  l'interface utilisateur est masquée si le proxy est en cours d'utilisation, mais que la boîte de dialogue demeure s'il est
//  fermé.

void CRepItemsBuddyDlg::OnClose() 
{
	if (CanExit())
		CDialog::OnClose();
}

void CRepItemsBuddyDlg::OnOK() 
{
	if (CanExit())
		CDialog::OnOK();
}

void CRepItemsBuddyDlg::OnCancel() 
{
	if (CanExit())
		CDialog::OnCancel();
}

BOOL CRepItemsBuddyDlg::CanExit()
{
	// Si l'objet proxy est encore présent, le contrôleur
	//  Automation reste placé sur cette application. Conserver
	//  la boîte de dialogue, mais masquer l'interface utilisateur associée.
	if (m_pAutoProxy != NULL)
	{
		ShowWindow(SW_HIDE);
		return FALSE;
	}

	return TRUE;
}


void CRepItemsBuddyDlg::OnBnClickedButton1()
{
	if(m_pAutoProxy)
		m_pAutoProxy->ShowRepItem();
	else
	{
		m_pAutoProxy = new CRepItemsBuddyProxy();
		BOOL bret = m_pAutoProxy->LoadTerrain(37, 338);
		CString gg;
		gg = m_pAutoProxy->GetListPoids();
		m_pAutoProxy->SetPoids(44);
		m_pAutoProxy->ShowRepItem();
		//bret = m_pAutoProxy->LoadTerrain(31,91);
		//m_pAutoProxy->SetPoids(1);
		//m_pAutoProxy->ShowRepItem();
		delete m_pAutoProxy;
		m_pAutoProxy = NULL;

	}
}
