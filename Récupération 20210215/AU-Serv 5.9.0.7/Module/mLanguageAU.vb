' <summary>
' Auto generated mLanguageAU class from C:\InstallShield 2019 Projects\AU-Serv 5.8.9.7.190722 (RadioPlus)\Module\mLanguageAU.xls
' </summary>
Public Module mLanguageAU
    Dim Languages(4) As String
    Dim MessageMap(4)() As String

    Public Sub SetCurrentLanguage(ByVal current As Integer)
        CurrentLanguage = current
    End Sub

    Public Function GetString(ByVal language As Integer, ByVal id As Integer) As String
        return MessageMap(language)(id)
    End Function

    Public Function GetString(ByVal id As Integer) As String
        return MessageMap(CurrentLanguage)(id)
    End Function

    Dim CurrentLanguage As Integer

    Dim Message_en(77) As String
    Dim Message_fr(77) As String
    Dim Message_it(77) As String
    Dim Message_pl(77) As String

    ''''''''''''''''''''''''''''''''''''''''''''''''
    ' Message ID definition
    Public Const MSG_EXIT_APPLICATION As Integer = 0
    Public Const MSG_PLEASE_WAIT As Integer = 1
    Public Const MSG_CONNECTING As Integer = 2
    Public Const MSG_CONNECTED As Integer = 3
    Public Const MSG_UPDATE_NUMBER As Integer = 4
    Public Const MSG_KEY_INVALID As Integer = 5
    Public Const MSG_SERIAL_INCORRECT As Integer = 6
    Public Const MSG_PATH_NOT_FOUND As Integer = 7
    Public Const MSG_DOWNLOAD_FULL As Integer = 8
    Public Const MSG_INTIALIZE As Integer = 9
    Public Const MSG_FOUND_UPDATE As Integer = 10
    Public Const MSG_NOT_UPDATE As Integer = 11
    Public Const MSG_LIST_UPDATE As Integer = 12
    Public Const MSG_DECONNECTING As Integer = 13
    Public Const MSG_DECONNECTED As Integer = 14
    Public Const BUTTON_STOP As Integer = 15
    Public Const BUTTON_EXIT As Integer = 16
    Public Const BUTTON_UPDATE As Integer = 17
    Public Const BUTTON_LAUNCH As Integer = 18
    Public Const MSG_IN_PROGRESS As Integer = 19
    Public Const MSG_DOWNLOAD As Integer = 20
    Public Const MSG_COPY_UPDATE_NUMBER As Integer = 21
    Public Const MSG_LAUNCH_APPLICATION_IN_PROGRESS As Integer = 22
    Public Const MSG_STOP_UPDATE As Integer = 23
    Public Const MSG_DOWNLOAD_AND_INSTALL As Integer = 24
    Public Const MSG_INSTALL As Integer = 25
    Public Const MSG_INSTALL_FULL As Integer = 26
    Public Const MSG_WAIT_INSTALL As Integer = 27
    Public Const MSG_UPDATE_SYSTEM As Integer = 28
    Public Const MSG_DIRECTORY_AND_INSTALL As Integer = 29
    Public Const MSG_APPLICATION_LAUNCHED_1 As Integer = 30
    Public Const MSG_APPLICATION_LAUNCHED_2 As Integer = 31
    Public Const MSD_APPLICATION_OPEN As Integer = 32
    Public Const MSG_PRODUCTCODE_MISSING As Integer = 33
    Public Const MSG_SERIAL_MISSING As Integer = 34
    Public Const MSG_LICENSE_EXPIRED As Integer = 35
    Public Const MSG_LICENSE_BLOCKED As Integer = 36
    Public Const MSG_PRODUCTCODE_INVALID As Integer = 37
    Public Const MSG_INVALID_PARAMETERS As Integer = 38
    Public Const MSG_ACCES_INVALID As Integer = 39
    Public Const MSG_ACCES_MISSING As Integer = 40
    Public Const MSG_ACCES_NO_SYNCHRO As Integer = 41
    Public Const MSG_NOT_INTERNET As Integer = 42
    Public Const BUTTON_STOP_PROCESS As Integer = 43
    Public Const BUTTON_IGNORE As Integer = 44
    Public Const MSG_PROCESS_WARNING As Integer = 45
    Public Const MSG_CONTINUE As Integer = 46
    Public Const BUTTON_YES As Integer = 47
    Public Const BUTTON_NO As Integer = 48
    Public Const MSG_PROCESS_CRITICAL As Integer = 49
    Public Const MSG_INPUT_PRODUCT_CODE As Integer = 50
    Public Const MSG_NUMBER_PRODUCT_CODE As Integer = 51
    Public Const MSG_COMPANY_NAME As Integer = 52
    Public Const MSG_INVALID_COMPANY_NAME As Integer = 53
    Public Const MSC_INCORRECT_PARAMETER As Integer = 54
    Public Const MSC_INITIALIZING As Integer = 55
    Public Const MSG_ECHEC_INTIALIZE As Integer = 56
    Public Const MSG_REMOTE_DESKTOP As Integer = 57
    Public Const MSG_RETURN_LICENSE As Integer = 58
    Public Const MSG_RETURN_LICENSE_ERROR As Integer = 59
    Public Const MSG_LICENSE_RETURNED As Integer = 60
    Public Const MSG_PRODUCT_CODE_INVALID As Integer = 61
    Public Const MSG_FAIL_CONNECT_INTERNET As Integer = 62
    Public Const MSG_ACCES_PATH_IS_DENIED As Integer = 63
    Public Const MSG_NO_UPDATE As Integer = 64
    Public Const MSG_FILE_IS_READONLY As Integer = 65
    Public Const MSG_INITIALIZING_ERROR As Integer = 66
    Public Const MSG_AVAILABLE_FREESPACE As Integer = 67
    Public Const MSG_CHANGE_SETTINGS As Integer = 68
    Public Const MSG_RESET_APP As Integer = 69
    Public Const MSG_ASK_REMOTE_DESKTOP As Integer = 70
    Public Const MSG_TOO_MANY_UPDATES As Integer = 71
    Public Const MSG_TOO_MANY_PATCH As Integer = 72
    Public Const MSG_UPDATE_SYSTEM_UPDATE As Integer = 73
    Public Const MSG_NO_LAUNCH As Integer = 74
    Public Const MSG_LICENSE_NOT_RECOGNIZED As Integer = 75
    Public Const MSG_INVALID_CONFIG As Integer = 76

    ''''''''''''''''''''''''''''''''''''''''''''''''
    ' Initialization of the module
    Sub New()

        Languages(0) = "en"
        MessageMap(0) =  Message_en
        Languages(1) = "fr"
        MessageMap(1) =  Message_fr
        Languages(2) = "it"
        MessageMap(2) =  Message_it
        Languages(3) = "pl"
        MessageMap(3) =  Message_pl

        Message_en(0) = "Do you really want to quit the application?"
        Message_en(1) = "Please wait �"
        Message_en(2) = "Connection �"
        Message_en(3) = "Online."
        Message_en(4) = "Update number"
        Message_en(5) = "The key is invalid, please contact your supplier. "
        Message_en(6) = "The serial number is invalid, please contact your supplier. "
        Message_en(7) = "Path not found!"
        Message_en(8) = "Total download progress :"
        Message_en(9) = "Initializing �"
        Message_en(10) = "Update search in progress �"
        Message_en(11) = "No available update."
        Message_en(12) = "Available update list:"
        Message_en(13) = "Logout �"
        Message_en(14) = "Offline."
        Message_en(15) = "Cancel    "
        Message_en(16) = "Exit       "
        Message_en(17) = "Update    "
        Message_en(18) = "Skip        updates    "
        Message_en(19) = "in progress �"
        Message_en(20) = "Download"
        Message_en(21) = "Copy update number"
        Message_en(22) = "Launching the application in progress �"
        Message_en(23) = "Do you really want to cancel the download?"
        Message_en(24) = "The following items will be downloaded and installed on this computer:"
        Message_en(25) = "Installation"
        Message_en(26) = "Total installation progress:"
        Message_en(27) = "Number of remaining updates:"
        Message_en(28) = "A new version of the update program has been installed. Please, restart the application  to continue�"
        Message_en(29) = "The following items will be installed on this computer:"
        Message_en(30) = "The application"
        Message_en(31) = "is in use."
        Message_en(32) = "Application already open"
        Message_en(33) = "The product code is missing, please contact your supplier."
        Message_en(34) = "The serial number is missing, please contact your supplier."
        Message_en(35) = "The license has expired, please contact your supplier."
        Message_en(36) = "The license has been invalidated, please contact your supplier."
        Message_en(37) = "The product code is invalid, please contact your supplier."
        Message_en(38) = "The parameters are invalid, please contact your supplier."
        Message_en(39) = "Invalid access file "
        Message_en(40) = "Missing access file "
        Message_en(41) = "Synchronization error"
        Message_en(42) = "No internet connection"
        Message_en(43) = "End Process"
        Message_en(44) = "Ignore"
        Message_en(45) = "Warning, the following process is being used in the application directory."
        Message_en(46) = "Do you really want to continue?"
        Message_en(47) = "Yes"
        Message_en(48) = "No"
        Message_en(49) = "Are you sure you want to close the following process?"
        Message_en(50) = "The product code is missing, please contact your supplier or thank you to enter it below."
        Message_en(51) = "Product code:"
        Message_en(52) = "Company Name:"
        Message_en(53) = "The company name #COMPANY# is invalid, please contact your supplier."
        Message_en(54) = "Incorrect parameter."
        Message_en(55) = "Initializing �"
        Message_en(56) = "Initialization failed."
        Message_en(57) = "Launching QuickSupport�"
        Message_en(58) = "Are you sure you return the license application?"
        Message_en(59) = "Problem: License not returned."
        Message_en(60) = "License returned."
        Message_en(61) = "Product Code Invalid."
        Message_en(62) = "Failed to connect to internet"
        Message_en(63) = "Acces to the path %FOLDER% is denied."
        Message_en(64) = "Are you really sure that you don't want to make the updates?"
        Message_en(65) = "Acces to the file %FILE% is denied."
        Message_en(66) = "Failed to initialize the application."
        Message_en(67) = "Insufficient disk space. (Free space: %FREESPACE%)"
        Message_en(68) = "Do you want to change the settings?"
        Message_en(69) = "Are you sure you want to reset the application?"
        Message_en(70) = "Do you want to execute the quick support?"
        Message_en(71) = "There are %COUNT% updates with the same id No. %INDEX%, please contact your supplier. "
        Message_en(72) = "There are %COUNT% updates with the same name '%NAME%', please contact your supplier. "
        Message_en(73) = "An update of the update system is available, do you want to do it?"
        Message_en(74) = "Please contact your supplier."
        Message_en(75) = "license not recognized, please contact your supplier."
        Message_en(76) = "Invalid configuration, please contact your supplier."

        Message_fr(0) = "Etes-vous s�r de vouloir quitter l'application ?"
        Message_fr(1) = "Veuillez patienter, s'il vous plait."
        Message_fr(2) = "Connexion ..."
        Message_fr(3) = "Connecter."
        Message_fr(4) = "Mise � jour n�"
        Message_fr(5) = "La cl� n'est pas valide, veuillez contacter votre fournisseur."
        Message_fr(6) = "Le num�ro de s�rie n'est pas valide, veuillez contacter votre fournisseur."
        Message_fr(7) = "Chemin non trouv� !"
        Message_fr(8) = "Progression totale du t�l�chargement:"
        Message_fr(9) = "Initialisation..."
        Message_fr(10) = "Recherche de mises � jour en cours..."
        Message_fr(11) = "Aucune mise � jour disponible."
        Message_fr(12) = "Liste des mises � jour disponibles :"
        Message_fr(13) = "D�connection ..."
        Message_fr(14) = "D�connecter."
        Message_fr(15) = "Annuler  "
        Message_fr(16) = "Fermer   "
        Message_fr(17) = "Mise � jour"
        Message_fr(18) = "Lancer     l'application"
        Message_fr(19) = "en cours ..."
        Message_fr(20) = "T�l�chargement"
        Message_fr(21) = "Copie de la mise � jour n�"
        Message_fr(22) = "Lancement de l'application en cours..."
        Message_fr(23) = "Etes-vous s�r de vouloir annuler le t�l�chargement ?"
        Message_fr(24) = "Les �l�ments suivants vont �tre t�l�charg�s et install�s sur cet ordinateur :"
        Message_fr(25) = "Installation"
        Message_fr(26) = "Progression totale de l'installation :"
        Message_fr(27) = "Nombre de mises � jour restantes :"
        Message_fr(28) = "Une nouvelle version du syst�me de mise � jour a �t� install�e, vous devez red�marrer l'application pour poursuivre les mises � jour."
        Message_fr(29) = "Les �l�ments suivants vont �tre intall�s sur cet ordinateur :"
        Message_fr(30) = "L'application"
        Message_fr(31) = "est en cours d'utilisation."
        Message_fr(32) = "Application d�j� ouverte"
        Message_fr(33) = "Le code produit est manquant, veuillez contacter votre fournisseur."
        Message_fr(34) = "Le num�ro de serie est manquant, veuillez contacter votre fournisseur."
        Message_fr(35) = "La licence a expir�, veuillez contacter votre fournisseur."
        Message_fr(36) = "La licence a �t� invalid�e, veuillez contacter votre fournisseur."
        Message_fr(37) = "Le code produit est invalide, veuillez contacter votre fournisseur."
        Message_fr(38) = "Les param�tres sont invalides, veuillez contacter votre fournisseur."
        Message_fr(39) = "Fichier d�acc�s invalide"
        Message_fr(40) = "Fichier d�acc�s manquant"
        Message_fr(41) = "Erreur de synchronisation"
        Message_fr(42) = "Pas de connexion internet"
        Message_fr(43) = "Arr�ter le(s) processus"
        Message_fr(44) = "Ignorer"
        Message_fr(45) = "Attention, les processus suivants sont en cours d'utilisation dans le r�pertoire de l'application."
        Message_fr(46) = "Etes-vous s�r de vouloir continuer?"
        Message_fr(47) = "Oui"
        Message_fr(48) = "Non"
        Message_fr(49) = "�tes-vous s�r de vouloir fermer les processus suivants?"
        Message_fr(50) = "Le code produit est manquant, veuillez contacter votre fournisseur ou merci de le saisir ci-dessous."
        Message_fr(51) = "Code produit :"
        Message_fr(52) = "Nom de la soci�t� :"
        Message_fr(53) = "Le nom de la soci�t� #COMPANY# est invalide, veuillez contacter votre fournisseur."
        Message_fr(54) = "Param�tre incorrect."
        Message_fr(55) = "Initialisation en cours�"
        Message_fr(56) = "Echec de l'initialisation."
        Message_fr(57) = "Lancement du support rapide�"
        Message_fr(58) = "Etes-vous s�r de vouloir retourner la licence de l'application?"
        Message_fr(59) = "Probl�me : licence non retourn�e."
        Message_fr(60) = "Licence retourn�e."
        Message_fr(61) = "Code de produit non valide."
        Message_fr(62) = "Impossible de se connecter � Internet"
        Message_fr(63) = "L'acc�s au chemin %FOLDER% est refus�."
        Message_fr(64) = "Etes-vous s�r de ne pas vouloir faire les mises � jour?"
        Message_fr(65) = "L'acc�s au fichier %FILE% est refus�."
        Message_fr(66) = "Impossible d'initialiser l'application."
        Message_fr(67) = "Espace disque insuffisant. (Espace libres: %FREESPACE%)"
        Message_fr(68) = "Voulez-vous modifier les param�tres?"
        Message_fr(69) = "Etes-vous s�r de vouloir r�initialiser l'application?"
        Message_fr(70) = "Voulez-vous ex�cuter le support rapide?"
        Message_fr(71) = "Il y a %COUNT% de mises � jour avec le m�me id n�%INDEX%, veuillez contacter votre fournisseur."
        Message_fr(72) = "Il y a %COUNT% de mises � jour avec le m�me nom '%NAME%', veuillez contacter votre fournisseur."
        Message_fr(73) = "Une mise � jour du syst�me de mise � jour est disponible, voulez-vous l'effectuer ?"
        Message_fr(74) = "Veuillez contacter votre fournisseur."
        Message_fr(75) = "Licence non reconnue, veuillez contacter votre fournisseur."
        Message_fr(76) = "Configuration invalide, veuillez contacter votre fournisseur."

        Message_it(0) = "Vuoi davvero per chiudere l'applicazione?"
        Message_it(1) = "Attendere prego �"
        Message_it(2) = "Connessione �"
        Message_it(3) = "On-line."
        Message_it(4) = "Aggiornare il numero"
        Message_it(5) = "La chiave non � valida, si prega di contattare il proprio fornitore."
        Message_it(6) = "Il numero di serie non � valido, si prega di contattare il proprio fornitore."
        Message_it(7) = "Percorso non trovato!"
        Message_it(8) = "Totale avanzamento del download:"
        Message_it(9) = "Inizializzazione �"
        Message_it(10) = "Cerca Aggiornamento in corso �"
        Message_it(11) = "Nessun aggiornamento disponibile."
        Message_it(12) = "Elenco degli aggiornamenti disponibili:"
        Message_it(13) = "Logout �"
        Message_it(14) = "Offline."
        Message_it(15) = "Annulla   "
        Message_it(16) = "Exit       "
        Message_it(17) = "Aggiornamento"
        Message_it(18) = "Skip       aggiornamenti"
        Message_it(19) = "in corso �"
        Message_it(20) = "Scaricare"
        Message_it(21) = "Copia numero aggiornamento"
        Message_it(22) = "Lanciare l'applicazione in corso �"
        Message_it(23) = "Vuoi veramente annullare il download?"
        Message_it(24) = "Le seguenti voci verranno scaricati e installati su questo computer:"
        Message_it(25) = "Installazione"
        Message_it(26) = "Stato di avanzamento dell'installazione Totale:"
        Message_it(27) = "Numero di aggiornamenti rimanenti:"
        Message_it(28) = "Una nuova versione del programma di aggiornamento � stato installato. Per favore, riavviare l'applicazione per continuare �"
        Message_it(29) = "Le seguenti voci verranno installati su questo computer:"
        Message_it(30) = "L'applicazione"
        Message_it(31) = "� in uso."
        Message_it(32) = "Applicazione gi� aperta"
        Message_it(33) = "Il codice prodotto � mancante, si prega di contattare il proprio fornitore."
        Message_it(34) = "Il numero di serie non � presente, si prega di contattare il proprio fornitore."
        Message_it(35) = "La licenza � scaduta, si prega di contattare il proprio fornitore."
        Message_it(36) = "La licenza � stata invalidata, si prega di contattare il proprio fornitore."
        Message_it(37) = "Il codice prodotto � valido, si prega di contattare il proprio fornitore."
        Message_it(38) = "I parametri non sono validi, si prega di contattare il proprio fornitore."
        Message_it(39) = "File non valido di accesso"
        Message_it(40) = "Manca l'accesso ai file"
        Message_it(41) = "Errore di sincronizzazione"
        Message_it(42) = "Nessuna connessione internet"
        Message_it(43) = "BUTTON_STOP_PROCESS"
        Message_it(44) = "BUTTON_IGNORE"
        Message_it(45) = "MSG_PROCESS_WARNING"
        Message_it(46) = "MSG_CONTINUE"
        Message_it(47) = "BUTTON_YES"
        Message_it(48) = "BUTTON_NO"
        Message_it(49) = "MSG_PROCESS_CRITICAL"
        Message_it(50) = "MSG_INPUT_PRODUCT_CODE"
        Message_it(51) = "MSG_NUMBER_PRODUCT_CODE"
        Message_it(52) = "MSG_COMPANY_NAME"
        Message_it(53) = "MSG_INVALID_COMPANY_NAME"
        Message_it(54) = "MSC_INCORRECT_PARAMETER"
        Message_it(55) = "MSC_INITIALIZING"
        Message_it(56) = "MSG_ECHEC_INTIALIZE"
        Message_it(57) = "MSG_REMOTE_DESKTOP"
        Message_it(58) = "MSG_RETURN_LICENSE"
        Message_it(59) = "MSG_RETURN_LICENSE_ERROR"
        Message_it(60) = "MSG_LICENSE_RETURNED"
        Message_it(61) = "MSG_PRODUCT_CODE_INVALID"
        Message_it(62) = "MSG_FAIL_CONNECT_INTERNET"
        Message_it(63) = "MSG_ACCES_PATH_IS_DENIED"
        Message_it(64) = "MSG_NO_UPDATE"
        Message_it(65) = "MSG_FILE_IS_READONLY"
        Message_it(66) = "MSC_INITIALIZING_ERROR"
        Message_it(67) = "MSG_AVAILABLE_FREESPACE"
        Message_it(68) = "MSG_CHANGE_SETTINGS"
        Message_it(69) = "MSG_RESET_APP"
        Message_it(70) = "MSG_ASK_REMOTE_DESKTOP"
        Message_it(71) = "MSG_TOO_MANY_UPDATES"
        Message_it(72) = "MSG_TOO_MANY_PATCH"
        Message_it(73) = "MSG_UPDATE_SYSTEM_CONFIRM"
        Message_it(74) = "MSG_NO_LAUNCH"
        Message_it(75) = "MSG_LICENSE_NOT_RECOGNIZED"
        Message_it(76) = "MSG_INVALID_CONFIG"

        Message_pl(0) = "Czy na pewno chcesz zamknac aplikacje?"
        Message_pl(1) = "Prosze czekac �"
        Message_pl(2) = "Polaczenie �"
        Message_pl(3) = "Online."
        Message_pl(4) = "Numer aktualizacji"
        Message_pl(5) = "Klucz jest nieprawidlowy, nalezy skontaktowac sie z dostawca."
        Message_pl(6) = "Numer seryjny jest nieprawidlowy, nalezy skontaktowac sie z dostawca."
        Message_pl(7) = "Nie mozna odnalezc sciezki!"
        Message_pl(8) = "Razem postepu pobierania:"
        Message_pl(9) = "Inicjowanie �"
        Message_pl(10) = "Szukaj w trakcie aktualizacji �"
        Message_pl(11) = "Brak dostepnych aktualizacji."
        Message_pl(12) = "Dostepne liscie aktualizacji:"
        Message_pl(13) = "Wyloguj sie �"
        Message_pl(14) = "Offline."
        Message_pl(15) = "Anuluj   "
        Message_pl(16) = "Wyjdz     "
        Message_pl(17) = "Uaktualnic"
        Message_pl(18) = "Aktualizacje      Przejdz"
        Message_pl(19) = "w toku �"
        Message_pl(20) = "Pobrac"
        Message_pl(21) = "Numer aktualizacji Kopiowania"
        Message_pl(22) = "Uruchomienie aplikacji w toku �"
        Message_pl(23) = "Czy na pewno chcesz zrezygnowac z pobierania?"
        Message_pl(24) = "Nastepujace elementy zostana pobrane i zainstalowane na tym komputerze:"
        Message_pl(25) = "Instalacja"
        Message_pl(26) = "Razem postepu instalacji:"
        Message_pl(27) = "Liczba pozostalych aktualizacji:"
        Message_pl(28) = "Nowa wersja programu aktualizacja zostala zainstalowana. Prosze ponownie uruchomic aplikacje, aby kontynuowac �"
        Message_pl(29) = "Nastepujace elementy zostana zainstalowane na tym komputerze:"
        Message_pl(30) = "Stosowania"
        Message_pl(31) = "jest w uzyciu."
        Message_pl(32) = "Wniosek juz otwarty"
        Message_pl(33) = "Kod produktu brakuje, skontaktuj sie ze sprzedawca."
        Message_pl(34) = "Numer seryjny jest niedostepny, skontaktuj sie ze sprzedawca."
        Message_pl(35) = "Licencja wygasla, skontaktuj sie ze sprzedawca."
        Message_pl(36) = "Pozwolenie zostalo uniewaznione, skontaktuj sie ze sprzedawca."
        Message_pl(37) = "Kod produktu jest nieprawidlowy, nalezy skontaktowac sie z dostawca."
        Message_pl(38) = "Parametry sa nieprawidlowe, nalezy skontaktowac sie z dostawca."
        Message_pl(39) = "Nieprawidlowy�dostep do plik�w"
        Message_pl(40) = "Brak�dostepu do plik�w"
        Message_pl(41) = "Blad synchronizacji"
        Message_pl(42) = "Nie do internetu"
        Message_pl(43) = "BUTTON_STOP_PROCESS"
        Message_pl(44) = "BUTTON_IGNORE"
        Message_pl(45) = "MSG_PROCESS_WARNING"
        Message_pl(46) = "MSG_CONTINUE"
        Message_pl(47) = "BUTTON_YES"
        Message_pl(48) = "BUTTON_NO"
        Message_pl(49) = "MSG_PROCESS_CRITICAL"
        Message_pl(50) = "MSG_INPUT_PRODUCT_CODE"
        Message_pl(51) = "MSG_NUMBER_PRODUCT_CODE"
        Message_pl(52) = "MSG_COMPANY_NAME"
        Message_pl(53) = "MSG_INVALID_COMPANY_NAME"
        Message_pl(54) = "MSC_INCORRECT_PARAMETER"
        Message_pl(55) = "MSC_INITIALIZING"
        Message_pl(56) = "MSG_ECHEC_INTIALIZE"
        Message_pl(57) = "MSG_REMOTE_DESKTOP"
        Message_pl(58) = "MSG_RETURN_LICENSE"
        Message_pl(59) = "MSG_RETURN_LICENSE_ERROR"
        Message_pl(60) = "MSG_LICENSE_RETURNED"
        Message_pl(61) = "MSG_PRODUCT_CODE_INVALID"
        Message_pl(62) = "MSG_FAIL_CONNECT_INTERNET"
        Message_pl(63) = "MSG_ACCES_PATH_IS_DENIED"
        Message_pl(64) = "MSG_NO_UPDATE"
        Message_pl(65) = "MSG_FILE_IS_READONLY"
        Message_pl(66) = "MSC_INITIALIZING_ERROR"
        Message_pl(67) = "MSG_AVAILABLE_FREESPACE"
        Message_pl(68) = "MSG_CHANGE_SETTINGS"
        Message_pl(69) = "MSG_RESET_APP"
        Message_pl(70) = "MSG_ASK_REMOTE_DESKTOP"
        Message_pl(71) = "MSG_TOO_MANY_UPDATES"
        Message_pl(72) = "MSG_TOO_MANY_PATCH"
        Message_pl(73) = "MSG_UPDATE_SYSTEM_CONFIRM"
        Message_pl(74) = "MSG_NO_LAUNCH"
        Message_pl(75) = "MSG_LICENSE_NOT_RECOGNIZED"
        Message_pl(76) = "MSG_INVALID_CONFIG"


    End Sub

End Module
