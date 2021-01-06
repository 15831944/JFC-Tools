//
// Fichier: JSRCDataExo.cpp
//

// on inclut les définitions nécessaires
#include "stdafx.h"
#include "JSRCDataExo.h"

///////////////////////////////////////////////
// l'unique instance des sources data exogènes
JSRCDataExo JSRCDataExo::m_Instance;

////////////////////
// Constructeur
JSRCDataExo::JSRCDataExo()
{
	// on ne fait rien
}

JSRCDataExo::JSRCDataExo(const JSRCDataExo & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'opérateur pour recopier les éléments
JSRCDataExo & JSRCDataExo::operator =(const JSRCDataExo & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////////////////////////
// la fonction pour récupérer l'instance des sources exogènes
const JSRCDataExo & JSRCDataExo::GetInstance()
{
	// on initialise l'indicateur
	static JBool m_Load = false;

	// Init Map Ident Grandeurs (pour vérifiant non redondance identifiant grandeur)
	JSRCDataExo::m_Instance.m_MapIdGrandeur.Reset();

	// on teste si on doit faire le chargement
	if (!m_Load)
	{
		// Récupère le chemin des fichiers exogènes
		CString CurrentDir = AfxGetApp()->GetProfileString("Data", "SourcesPath", 0);
		CurrentDir += "\\DataExo";
		SetCurrentDirectory(CurrentDir);

		// Init Liste des noms data exogène
		JSRCDataExo::m_Instance.m_ListNomDataExo.Reset();

		// Init Map Data Exogène
		JSRCDataExo::m_Instance.m_MapDataExo.Reset();

		// Puis on traite tous les fichiers csv appartenant au répertoire données exogènes
		CFileFind finder;

		// Créé le chemin de recherche avec extension csv
		CurrentDir += _T("\\*.csv");

		// Commence la recherche des fichiers data exogène
		BOOL OkFind = finder.FindFile(CurrentDir);

		// Boucle sur tous kes fichiers exogènes (extension csv)
		while (OkFind)
		{
			// Recherche fichier
			OkFind = finder.FindNextFile();

			// Passe les . et ..
			if (finder.IsDots())
				continue;

			// Ignore les ss-directory
			if (!finder.IsDirectory())
			{
				// Nom du fichier trouvé
				CString NomFile =finder.GetFileName(); 

				// Récupère nom du fichier complet
				CString FichDataExo = finder.GetFilePath();

				// Traite le fichier
				CString TxtErr;
				if (!JSRCDataExo::m_Instance.ChargeFichExo(FichDataExo, TxtErr))
				{
					CString Message;
					Message = "Fichier " + NomFile;
					Message += " incorrectement chargé !!!... " + TxtErr;
					AfxMessageBox(Message, MB_ICONEXCLAMATION); 
					// throw JInternalError::GetInstance(); 
				}
			}
		}

		// Fin de la recherche
		finder.Close();

		// Chargement correct
		m_Load = true;

		// Remet le répertoire courant
		SetCurrentDirectory(AfxGetApp()->GetProfileString("Data", "SourcesPath", 0));
	}

	// on renvoie l'instance
	return (m_Instance);
}

//////////////////////////////////////////////////////////////////////
// Chargement d'un fichier exogène
JBool JSRCDataExo::ChargeFichExo(CString FichDataExo, CString &TxtErr)
{
	// Définitions locales
	CString path;
	CStdioFile File;
	int Pos;

	// Init Texte erreur
	TxtErr = "";

	// Ouverture du fichier data exogène
	if (File.Open(FichDataExo, CFile::modeRead | CFile::typeText ))
	{
		// Lecture date fichier
		CString Data;
		if(!File.ReadString(Data))
		{
			// Manque l'info date fichier exogène
			TxtErr = "Manque date fichier"; 
			return false;
			// throw JInternalError::GetInstance(); 
		}
		
		// Lecture version
		if(File.ReadString(Data))
		{
			// Recherche séparateur
			Pos = Data.Find(";");
			if (Pos <= 0)
			{
				// Manque l'info version fichier exogène
				TxtErr = "Manque version fichier"; 
				return false;
				// throw JInternalError::GetInstance(); 
			}
			else if (Data.Left(Pos) != "Version")
			{
				// Manque l'info version fichier exogène
				TxtErr = "Manque version fichier"; 
				return false;
				// throw JInternalError::GetInstance(); 
			}
		}

		// Ajoute le nom de la donnée exogène à la liste
		CString LabelDataExo	= "";
		CString AbrLabelDataExo = "";
		if(File.ReadString(Data))
		{
			// Voir si libellé abrégé
			Pos = Data.Find(";");
			if (Pos > 0)
			{
				// Récupère libellé court et libellé complet
				AbrLabelDataExo = Data.Mid(Pos+1);
				AbrLabelDataExo.Remove(';');
				LabelDataExo    = Data.Left(Pos);
			}
			else
			{
				// Pas de libellé court, par défaut le même
				LabelDataExo    = Data;
				AbrLabelDataExo = LabelDataExo;
			}

			// Ajoute info libellé complet à la liste
			// JSRCDataExo::m_Instance.m_ListNomDataExo.AddTail() = LabelDataExo; 
		}
		else
		{
			// Manque le nom du fichier exogène
			TxtErr = "Manque nom fichier exogène"; 
			return false;
			// throw JInternalError::GetInstance(); 
		}

		// Récupère min et max des valeurs pour cette data exo
		JFlt64 MinVal, MaxVal;
		if(File.ReadString(Data))
		{
			Pos = Data.Find(";");
			if (Pos >= 0 && Data.Left(Pos) == "MinMax")
			{
				// Min et max à tester pour les valeurs qui vont suivre
				Data.Remove(' ');
				Data = Data.Mid(Pos+1);
				Pos = Data.Find(";");
				if (Pos >= 0)
				{
					MinVal = atof(Data.Left(Pos));
					
					MaxVal = atof(Data.Mid(Pos+1));
				}
				else
				{
					// Manque l'info min et max valeurs exogènes
					TxtErr =  "Manque valeur min max"; 
					return false;
					// throw JInternalError::GetInstance(); 
				}
			}
			else
			{
				// Manque l'info min et max valeurs exogènes
				TxtErr = "Manque valeur min max"; 
				return false;
				//throw JInternalError::GetInstance(); 
			}
		}
		else
		{
			// Manque l'info min et max valeurs exogènes
			TxtErr = "Manque valeur min max"; 
			return false;
			//throw JInternalError::GetInstance(); 
		}

		// Déclare élmts data exogène pour ajout map
		CDataExo &DataExo = JSRCDataExo::m_Instance.m_MapDataExo.Add(LabelDataExo);

		// Ajoute le libellé abrégé
		DataExo.m_AbrLabelDataExo = AbrLabelDataExo;

		// Lecture nb décimales pour les valeurs exogènes
		if(File.ReadString(Data))
		{
			Pos = Data.Find(";");
			if (Pos >= 0 && Data.Left(Pos) == "NbDec")
			{
				// Récupère info nb décimales
				DataExo.m_NbDecExo = atoi(Data.Mid(Pos+1));
			}	
			else
			{
				// Manque l'info nb décimales
				TxtErr = "Manque info nb décimales"; 
				return false;
				//throw JInternalError::GetInstance(); 
			}
		}
		else
		{
			// Manque l'info nb décimales
			TxtErr = "Manque info nb décimales"; 
			return false;
			// throw JInternalError::GetInstance(); 
		}
		
		// Lecture type de valeurs attendues
		if(File.ReadString(Data))
		{
			Pos = Data.Find(";");
			if (Pos >= 0 && Data.Left(Pos) == "Type")
			{
				// Récupère type valeur 
				DataExo.m_TypeDataExo = atoi(Data.Mid(Pos+1));
			}	
			else
			{
				// Manque l'info type valeur
				TxtErr = "Manque type de valeur exogène"; 
				return false;
				// throw JInternalError::GetInstance(); 
			}
		}
		else
		{
			// Manque l'info type valeur
			TxtErr = "Manque type de valeur exogène"; 
			return false;
			// throw JInternalError::GetInstance(); 
		}

		// Lecture ident début grandeur pour ces datas exogènes
		if(File.ReadString(Data))
		{
			Pos = Data.Find(";");
			if (Pos >= 0 && Data.Left(Pos) == "IdVal")
			{
				// Récupère type valeur 
				DataExo.m_IdDepGrandeur = atoi(Data.Mid(Pos+1));

				// Vérifie si data non déjà utilisé et compris entre 100 et 999
				if (DataExo.m_IdDepGrandeur < 100 || DataExo.m_IdDepGrandeur >= 1000)
				{
					TxtErr = "Une valeur IdVal n'est pas dans les bornes identificateurs grandeur (soit >=100 et < 1000)"; 
					return false;
				}
				
				// Vérifie non déjà utilisé
				JSRCDataExo::m_Instance.m_MapIdGrandeur.MoveTo(DataExo.m_IdDepGrandeur);
				if (!m_MapIdGrandeur.IsValid())
				{
					// Ajout au map pour vérif suivante
					JSRCDataExo::m_Instance.m_MapIdGrandeur.Add(DataExo.m_IdDepGrandeur);
				}
				else
				{
					TxtErr = "Valeur identifiant grandeur déjà utilisée"; 
					return false;
				}

			}	
			else
			{
				// Manque l'info ident début grandeur exogène
				TxtErr = "Manque ident début grandeur exogène"; 
				return false;
				// throw JInternalError::GetInstance(); 
			}
		}
		else
		{
			// Manque l'info ident début grandeur exogène
			TxtErr = "Manque ident début grandeur exogène"; 
			return false;
			// throw JInternalError::GetInstance(); 
		}
		
		// Charge les valeurs exogènes par id support
		while (File.ReadString(Data))
		{
			// Lecture séparateur csv
			Pos = Data.Find(";");

			if (Pos >= 0)
			{
				
				// Libellé support (non sauvegardé, juste là pour clarté fichier)
				CString lblSupport;
				lblSupport = Data = Data.Mid(Pos+1);

				// Lecture id support
				Pos = Data.Find(";");
				if (Pos >= 0)
				{
					// Sauvegarde id support
					JInt32 IdSupport = atoi(Data.Left(Pos));

					// Test validité support
					if (IdSupport <= 0)
					{
						// Valeur associé au support non valide
						TxtErr = "Valeur associé au support dépasse les bornes min et max";
						return false;
						// throw JInternalError::GetInstance(); 
					}								
					else
					{
						// Puis lecture valeur associé
						Data = Data.Mid(Pos+1);
						Data.Remove(' ');
						JFlt64 ValSupp = atoi(Data);

						// Vérifie valeur incluse dans les bornes min et max
						if (ValSupp < MinVal || ValSupp > MaxVal)
						{
							// Valeur associé au support non valide
							TxtErr = "Valeur associé au support dépasse les bornes min et max";
							return false;
							// throw JInternalError::GetInstance(); 
						}
						else
						{
							// Stockage id support et valeur associé
							DataExo.m_MapValExo.MoveTo(IdSupport);
							if(!DataExo.m_MapValExo.IsValid())
								DataExo.m_MapValExo.Add(IdSupport) = ValSupp; 
							else
							{
								TxtErr.Format("Valeur en doublon pour %s", lblSupport);
								return false;
							}

						}
					}
				}
				else
				{
					// Manque l'info ident support
					TxtErr = "Manque information identification support"; 
					return false;
					// throw JInternalError::GetInstance(); 
				}
			}
			else
			{
				// Manque l'info libellé support
				TxtErr = "Manque information libellé support"; 
				return false;
				// throw JInternalError::GetInstance(); 
			}
			
		}

		// ferme le fichier
		File.Close();

		// Ajoute info libellé complet à la liste
		JSRCDataExo::m_Instance.m_ListNomDataExo.AddTail() = LabelDataExo; 

		// Tout s'est bien passé
		return true;
	}

	// Mauvais chargement ou fichier inexistant
	return false;
}

///////////////////
// le destructeur
JSRCDataExo::~JSRCDataExo()
{
	// on ne fait rien
}

/////////////////////////////////////////////////////////////////////////////
// Récupèration de la liste des noms date exogènes disponibles
const JList <CString> & JSRCDataExo::GetListNomDataExo() const
{
	// Renvoi liste des libellés data exogènes
	return (m_Instance.m_ListNomDataExo);
}

/////////////////////////////////////////////////////////////////////////////
// Récupération valeur d'un titre pour une data exo
JBool JSRCDataExo::GetValeur(CString NomDataExo, JUnt32 IdSupport, JFlt64 &Valeur) const
{
	// On se positionne via le libellé recherché
	JSRCDataExo::m_Instance.m_MapDataExo.MoveTo(NomDataExo);

	// Test si cette data exogène existe et est bien chargé
	if (m_MapDataExo.IsValid())
	{
		// Puis on récupère les infos pour cette data
		CDataExo &DataExo = JSRCDataExo::m_Instance.m_MapDataExo.GetItem();

		// Et la valeur associée à cet ident support
		DataExo.m_MapValExo.MoveTo(IdSupport);

		// Test si id support bien renseigné pour cette data exogène
		if (DataExo.m_MapValExo.IsValid())
		{
			// Récupère la valeur
			Valeur = DataExo.m_MapValExo.GetItem(); 
			return true;
		}
	}

	// Instance non chargé ou data exogène non trouvée
	return false;
}

/////////////////////////////////////////////////////////////////////////////
// Récupération type et nb décimal des valeurs inclues dans une data exo
JBool JSRCDataExo::GetTypeValeur(CString NomDataExo, JUnt32 &Type, JUnt32 &NbDec) const
{
	// On se positionne via le libellé recherché
	JSRCDataExo::m_Instance.m_MapDataExo.MoveTo(NomDataExo);

	// Test si cette data exogène existe et est bien chargé
	if (m_MapDataExo.IsValid())
	{
		// Puis on récupère les infos data exogène
		CDataExo &DataExo = JSRCDataExo::m_Instance.m_MapDataExo.GetItem();

		// Puis le type de valeur attendue pour cette data
		Type = DataExo.m_TypeDataExo;

		// Puis le nombre de décimal attendue pour les valeurs de cette data
		NbDec = DataExo.m_NbDecExo;
		
		// Aucun problème
		return true;
	}

	// Instance non chargé ou data exogène non trouvée
	return false;
}

////////////////////////////////////////////////////////////////////
// Récupération 1er Id Grandeur pour chaque data exe
JBool JSRCDataExo::GetIdDepGrandeur(CString NomDataExo, JUnt32 &IdDepGrandeur) const
{
	// On se positionne via le libellé recherché
	JSRCDataExo::m_Instance.m_MapDataExo.MoveTo(NomDataExo);

	// Test si cette data exogène existe et est bien chargé
	if (m_MapDataExo.IsValid())
	{
		// Puis on récupère les infos data exogène
		CDataExo &DataExo = JSRCDataExo::m_Instance.m_MapDataExo.GetItem();

		// Enfin l'id garndeur de départ
		IdDepGrandeur = DataExo.m_IdDepGrandeur;

		// Aucun problème
		return true;
	}

	// Instance non chargé ou data exogène non trouvée
	return false;
}

//////////////////////////////////////////////////////////////////////
// Récupération libellé abrégé data exogène
JBool JSRCDataExo::GetAbrLib(CString NomDataExo, CString &AbrNomDataExo) const
{
	// On se positionne via le libellé recherché
	JSRCDataExo::m_Instance.m_MapDataExo.MoveTo(NomDataExo);

	// Test si cette data exogène existe et est bien chargé
	if (m_MapDataExo.IsValid())
	{
		// Puis on récupère les infos data exogène
		CDataExo &DataExo = JSRCDataExo::m_Instance.m_MapDataExo.GetItem();

		// Enfin le lilellé abrégé de la data exogène
		AbrNomDataExo = DataExo.m_AbrLabelDataExo;

		// Aucun problème
		return true;
	}

	// Instance non chargé ou data exogène non trouvée
	return false;
}

// Récupération libellé via ident grandeur
CString JSRCDataExo:: GetLib(JUnt32 IdGrandeur) const
{
	// Recherche parmi tous les libellés data exogène
	for (JSRCDataExo::m_Instance.m_ListNomDataExo.MoveFirst();
		 JSRCDataExo::m_Instance.m_ListNomDataExo.IsValid();
		 JSRCDataExo::m_Instance.m_ListNomDataExo.MoveNext())
	{
		// Récupère le libellé data
		CString &NomDataExo = JSRCDataExo::m_Instance.m_ListNomDataExo.GetItem();

		// On se positionne via le libellé recherché
		JSRCDataExo::m_Instance.m_MapDataExo.MoveTo(NomDataExo);

		// Test si cette data exogène existe et est bien chargé
		if (m_MapDataExo.IsValid())
		{
			// Puis on récupère les infos data exogène
			CDataExo &DataExo = JSRCDataExo::m_Instance.m_MapDataExo.GetItem();

			// Test si ident trouvé
			if (DataExo.m_IdDepGrandeur == IdGrandeur)
				return NomDataExo;
		}
	}

	// Pas de libellé trouvé
	return "";
}