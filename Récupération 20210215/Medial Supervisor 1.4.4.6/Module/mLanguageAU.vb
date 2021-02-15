' <summary>
' Auto generated mLanguageAU class from C:\$BACKUP.JFC\Medial Supervisor\Module\mLanguageAU.xls
' </summary>
Public Module mLanguageAU
    ReadOnly Languages(4) As String
    ReadOnly MessageMap(4)() As String

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

    ReadOnly Message_en(43) As String
    ReadOnly Message_fr(43) As String
    ReadOnly Message_it(43) As String
    ReadOnly Message_pl(43) As String

    ''''''''''''''''''''''''''''''''''''''''''''''''
    ' Message ID definition
    Public Const MSG_UPDATE_NUMBER As Integer = 0
    Public Const MSG_KEY_INVALID As Integer = 1
    Public Const MSG_SERIAL_INCORRECT As Integer = 2
    Public Const MSG_PATH_NOT_FOUND As Integer = 3
    Public Const MSG_NOT_UPDATE As Integer = 4
    Public Const MSG_DOWNLOAD As Integer = 5
    Public Const MSG_COPY_UPDATE_NUMBER As Integer = 6
    Public Const MSG_PRODUCTCODE_MISSING As Integer = 7
    Public Const MSG_SERIAL_MISSING As Integer = 8
    Public Const MSG_LICENSE_EXPIRED As Integer = 9
    Public Const MSG_LICENSE_BLOCKED As Integer = 10
    Public Const MSG_PRODUCTCODE_INVALID As Integer = 11
    Public Const MSG_INVALID_PARAMETERS As Integer = 12
    Public Const MSG_SERVICE_NO_START As Integer = 13
    Public Const MSG_SERVICE_START As Integer = 14
    Public Const MSG_SERVICE_STOP As Integer = 15
    Public Const MSG_NO_SERVICE As Integer = 16
    Public Const MSG_COMPANY As Integer = 17
    Public Const MSG_NOT_REPLY As Integer = 18
    Public Const MSG_INFO_UPDATE As Integer = 19
    Public Const MSG_CONNECTION_FAIL As Integer = 20
    Public Const MSG_UPDATE_NAME As Integer = 21
    Public Const MSG_UPDATE_NOT_FOUND As Integer = 22
    Public Const MSG_ERROR_READING_FILE_ACCES As Integer = 23
    Public Const MSG_UPDATE_COMPLETE As Integer = 24
    Public Const MSG_DOWNLOAD_COMPLETE As Integer = 25
    Public Const MSG_COPY_COMPLETE As Integer = 26
    Public Const MSG_UPDATE_FAIL As Integer = 27
    Public Const MSG_CODE_ERROR As Integer = 28
    Public Const MSG_SERVICE_INFORMATION As Integer = 29
    Public Const MSG_SERVICE_RESTART As Integer = 30
    Public Const MSG_SERVICE_RESTARTED As Integer = 31
    Public Const MSG_DEPENDENCE_NOT_STARTED As Integer = 32
    Public Const MSG_SERVICE_NO_RESTART As Integer = 33
    Public Const MSG_SERVICE_EXCEPTION As Integer = 34
    Public Const MSG_SERVICE_IS_NOT_STARTED As Integer = 35
    Public Const MSG_SERVICE_IS_STARTED As Integer = 36
    Public Const MSG_SERVICE_NOT_STOP As Integer = 37
    Public Const MSG_SERVICE_IS_STOPPED As Integer = 38
    Public Const MSG_SERVICE_LOGONUSER As Integer = 39
    Public Const MSG_ACCES_INVALID As Integer = 40
    Public Const MSG_ACCES_MISSING As Integer = 41
    Public Const MSG_ACCES_NO_SYNCHRO As Integer = 42

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

        Message_en(0) = "Update number "
        Message_en(1) = "The key is invalid, please contact your supplier. "
        Message_en(2) = "The serial number is invalid, please contact your supplier. "
        Message_en(3) = "Path not found!"
        Message_en(4) = "No available update."
        Message_en(5) = "Download"
        Message_en(6) = "Copy update number"
        Message_en(7) = "The product code is missing, please contact your supplier."
        Message_en(8) = "The serial number is missing, please contact your supplier."
        Message_en(9) = "The license has expired, please contact your supplier."
        Message_en(10) = "The license has been invalidated, please contact your supplier."
        Message_en(11) = "The product code is invalid, please contact your supplier."
        Message_en(12) = "The parameters are invalid, please contact your supplier."
        Message_en(13) = "The service did not start properly."
        Message_en(14) = "The service has started successfully."
        Message_en(15) = "The service was stopped successfully."
        Message_en(16) = "No services found"
        Message_en(17) = "Company: "
        Message_en(18) = "Do not reply"
        Message_en(19) = "Information updated: "
        Message_en(20) = "Connection failed"
        Message_en(21) = "Name update: "
        Message_en(22) = "Update not found: "
        Message_en(23) = "Error reading file acces"
        Message_en(24) = "File update completed."
        Message_en(25) = "File download completed."
        Message_en(26) = "Copy file completed"
        Message_en(27) = "Update failed"
        Message_en(28) = "Error code:"
        Message_en(29) = "Information service"
        Message_en(30) = "The service has restarted successfully."
        Message_en(31) = "The service is restarted."
        Message_en(32) = "Dependence not started."
        Message_en(33) = "The service did not restart properly."
        Message_en(34) = "Exception"
        Message_en(35) = "The service is not started"
        Message_en(36) = "The service is started"
        Message_en(37) = "The service does not stop."
        Message_en(38) = "The service is stopped"
        Message_en(39) = "User Login"
        Message_en(40) = "Invalid access file"
        Message_en(41) = "Missing access file"
        Message_en(42) = "Synchronization error"

        Message_fr(0) = "Mise à jour n° "
        Message_fr(1) = "La clé n'est pas valide, veuillez contacter votre fournisseur."
        Message_fr(2) = "Le numéro de série n'est pas valide, veuillez contacter votre fournisseur."
        Message_fr(3) = "Chemin non trouvé !"
        Message_fr(4) = "Aucune mises à jour disponibles."
        Message_fr(5) = "Téléchargement"
        Message_fr(6) = "Copie de la mise à jour n°"
        Message_fr(7) = "Le code produit est manquant, veuillez contacter votre fournisseur."
        Message_fr(8) = "le numéro de serie est manquant, veuillez contacter votre fournisseur."
        Message_fr(9) = "La licence a expiré, veuillez contacter votre fournisseur."
        Message_fr(10) = "La licence a été invalidé, veuillez contacter votre fournisseur."
        Message_fr(11) = "Le code produit est invalide, veuillez contacter votre fournisseur."
        Message_fr(12) = "Les paramètres sont invalides, veuillez contacter votre fournisseur."
        Message_fr(13) = "Le service n'a pas démarré correctement."
        Message_fr(14) = "Le service a démarré avec succès."
        Message_fr(15) = "Le service a été arrêté."
        Message_fr(16) = "Aucun service trouvé"
        Message_fr(17) = "Société: "
        Message_fr(18) = "Ne pas répondre"
        Message_fr(19) = "Informations mise à jour: "
        Message_fr(20) = "La connexion a échoué"
        Message_fr(21) = "Nom de la mise à jour: "
        Message_fr(22) = "Mise à jour introuvable: "
        Message_fr(23) = "Erreur de lecture du fichier d'accès"
        Message_fr(24) = "Exécution du fichier terminé."
        Message_fr(25) = "Téléchargement du fichier terminé."
        Message_fr(26) = "Copie de fichier terminé"
        Message_fr(27) = "La mise à jour à échoué"
        Message_fr(28) = "Code erreur:"
        Message_fr(29) = "Information service"
        Message_fr(30) = "Le service a redémarré avec succès."
        Message_fr(31) = "Le service va redémarré."
        Message_fr(32) = "Dépendance non démaré."
        Message_fr(33) = "Le service n'a pas redémarré correctement."
        Message_fr(34) = "Exception"
        Message_fr(35) = "Le service n'est pas démarré"
        Message_fr(36) = "Le service est démarré"
        Message_fr(37) = "Le service ne s'arrête pas."
        Message_fr(38) = "Le service est arrété"
        Message_fr(39) = "Connexion de l'utilisateur"
        Message_fr(40) = "Fichier d’accès invalide"
        Message_fr(41) = "Fichier d’accès manquant"
        Message_fr(42) = "Erreur de synchronisation"

        Message_it(0) = "Aggiornare il numero"
        Message_it(1) = "La chiave non è valida, si prega di contattare il proprio fornitore."
        Message_it(2) = "Il numero di serie non è valido, si prega di contattare il proprio fornitore."
        Message_it(3) = "Percorso non trovato!"
        Message_it(4) = "Nessun aggiornamento disponibile."
        Message_it(5) = "Scaricare"
        Message_it(6) = "Copia numero aggiornamento"
        Message_it(7) = "Il codice prodotto è mancante, si prega di contattare il proprio fornitore."
        Message_it(8) = "Il numero di serie non è presente, si prega di contattare il proprio fornitore."
        Message_it(9) = "La licenza è scaduta, si prega di contattare il proprio fornitore."
        Message_it(10) = "La licenza è stata invalidata, si prega di contattare il proprio fornitore."
        Message_it(11) = "Il codice prodotto è valido, si prega di contattare il proprio fornitore."
        Message_it(12) = "I parametri non sono validi, si prega di contattare il proprio fornitore."
        Message_it(13) = ""
        Message_it(14) = ""
        Message_it(15) = ""
        Message_it(16) = ""
        Message_it(17) = ""
        Message_it(18) = ""
        Message_it(19) = ""
        Message_it(20) = ""
        Message_it(21) = ""
        Message_it(22) = ""
        Message_it(23) = ""
        Message_it(24) = ""
        Message_it(25) = ""
        Message_it(26) = ""
        Message_it(27) = ""
        Message_it(28) = ""
        Message_it(29) = ""
        Message_it(30) = ""
        Message_it(31) = ""
        Message_it(32) = ""
        Message_it(33) = ""
        Message_it(34) = ""
        Message_it(35) = ""
        Message_it(36) = ""
        Message_it(37) = ""
        Message_it(38) = ""
        Message_it(39) = ""
        Message_it(40) = ""
        Message_it(41) = ""
        Message_it(42) = ""

        Message_pl(0) = "Numer aktualizacji"
        Message_pl(1) = "Klucz jest nieprawidlowy, nalezy skontaktowac sie z dostawca."
        Message_pl(2) = "Numer seryjny jest nieprawidlowy, nalezy skontaktowac sie z dostawca."
        Message_pl(3) = "Nie mozna odnalezc sciezki!"
        Message_pl(4) = "Brak dostepnych aktualizacji."
        Message_pl(5) = "Pobrac"
        Message_pl(6) = "Numer aktualizacji Kopiowania"
        Message_pl(7) = "Kod produktu brakuje, skontaktuj sie ze sprzedawca."
        Message_pl(8) = "Numer seryjny jest niedostepny, skontaktuj sie ze sprzedawca."
        Message_pl(9) = "Licencja wygasla, skontaktuj sie ze sprzedawca."
        Message_pl(10) = "Pozwolenie zostalo uniewaznione, skontaktuj sie ze sprzedawca."
        Message_pl(11) = "Kod produktu jest nieprawidlowy, nalezy skontaktowac sie z dostawca."
        Message_pl(12) = "Parametry sa nieprawidlowe, nalezy skontaktowac sie z dostawca."
        Message_pl(13) = ""
        Message_pl(14) = ""
        Message_pl(15) = ""
        Message_pl(16) = ""
        Message_pl(17) = ""
        Message_pl(18) = ""
        Message_pl(19) = ""
        Message_pl(20) = ""
        Message_pl(21) = ""
        Message_pl(22) = ""
        Message_pl(23) = ""
        Message_pl(24) = ""
        Message_pl(25) = ""
        Message_pl(26) = ""
        Message_pl(27) = ""
        Message_pl(28) = ""
        Message_pl(29) = ""
        Message_pl(30) = ""
        Message_pl(31) = ""
        Message_pl(32) = ""
        Message_pl(33) = ""
        Message_pl(34) = ""
        Message_pl(35) = ""
        Message_pl(36) = ""
        Message_pl(37) = ""
        Message_pl(38) = ""
        Message_pl(39) = ""
        Message_pl(40) = ""
        Message_pl(41) = ""
        Message_pl(42) = ""


    End Sub

End Module
