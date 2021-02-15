Option Strict On
Option Explicit On

Imports System.IO
Imports System.Net
Imports IWshRuntimeLibrary

Friend Class MultiUserEDI

    Inherits System.Windows.Forms.Form

    Public Declare Function GetAsyncKeyState Lib "user32.dll" (ByVal vKey As Integer) As Integer

    Public WithEvents Connection As CConnection

    Const BEFORE As Integer = -1 'Inserts the line before nLineNumber. 
    Const AFTER As Integer = 1 'Inserts the line after nLineNumber. 
    Const APPEND As Integer = 2 'Appends szInsertLine to the line indicated by nLineNumber. 
    Const REMPLACE As Integer = 0 'Replaces the line indicated by nLineNumber with szInsertLine. 
    Const CONTINU As Integer = 1 'Retrieves the next occurrence (if any) of the search string. 
    Const RESTART As Integer = 0 'Retrieves the first instance of the search string. 
    Const END_OF_FILE As Integer = -4 ' The end of file was reached without finding the search string. 
    Const FILE_NOT_FOUND As Integer = -2 ' InstallShield was unable to find the file in szFileName. 
    Const FILE_LINE_LENGTH As Integer = -3 ' The line exceeds the maximum length allowed. 
    Const OTHER_FAILURE As Integer = -1 ' An unspecified error has occurred. 
    Const FILE_RD_ONLY As Integer = -5 ' Indicates that the file is write-protected. 
    Const LINE_NUMBER As Integer = -7 ' Indicates that one of the line numbers you specified is less than zero, or the line number does not exist in the file. 
    Const OUT_OF_DISK_SPACE As Integer = -6 ' Indicates that there is insufficient space on the disk drive to complete the specified operation. 

    Const INDEX_COMPANY_NAME As Short = 0
    Const INDEX_PATH_USERS As Short = 1
    Const INDEX_FOLDER_EXTERNE As Short = 2
    Const INDEX_CODE_EDI As Short = 3
    Const INDEX_FOLDER_NOMENCLATURE As Short = 4
    Const INDEX_USER_EMAIL As Short = 5
    Const INDEX_RAISON_NEW As Short = 6
    Const INDEX_SOCIETE_MBS As Short = 7
    Const INDEX_CONSOLIDATION As Short = 8
    Const INDEX_AFFINAGE As Short = 9
    Const INDEX_INTF_MBS As Short = 10

    Const INDEX_LAST As Short = 10
    Const NUMBER_COMPANY_MAX As Short = 12

    Const NUMBER_EINTERLOC_MAX As Short = 29

    Public bUseShellExecute As Boolean
    Public bUseProcessExecute As Boolean
    Public TableUserExist As Boolean = True

    Public AppPath As String

    Dim DefaultReadingEDI As String

    Dim PATH_USERS As String
    Dim USER_EMAIL As String
    Dim PATH_FILE_TABLE_USER As String
    Dim FOLDER_EXTERNE As String
    Dim LAST_COMPANY As String
    Dim COMPANY_NAME As String
    Dim EMAIL_VERIF As String = Nothing
    Dim COMPANY_NUMBER As Integer = 0
    Dim N_CLIENT As String = Nothing
    Dim USER_TABLE As String = Nothing

    ReadOnly TableUtilisateur(NUMBER_COMPANY_MAX, INDEX_LAST) As String

    ReadOnly ListUnivers As ArrayList = New ArrayList

    ReadOnly AppVersion As String = FileVersionInfo.GetVersionInfo(Application.ExecutablePath).FileVersion

    Private Sub Update_MultiUser()

        Dim MultiUserOld As String = ParsePath(Application.ExecutablePath, PATH_FILENAME_ONLY) & ".old"

        If IO.File.Exists(MultiUserOld) Then

            PleaseWait.Show()
            System.Windows.Forms.Application.DoEvents()

            Dim nvBoucle As Integer = 0

            Do

                Try
                    IO.File.Delete(MultiUserOld)
                    PleaseWait.Hide()
                    Exit Do
                Catch ex As Exception

                End Try

                Threading.Thread.Sleep(100)
                System.Windows.Forms.Application.DoEvents()

                nvBoucle += 1

                If nvBoucle > 100 Then
                    PleaseWait.Hide()
                    Exit Sub
                End If

            Loop

        End If

    End Sub

    Private Sub ButtonQuitter_Click()
        Me.Close()
    End Sub

    Private Shared Function BaliseConsoData(ByVal p As String) As Boolean
        If p.Contains("<p name=" + Chr(34) + "consoData") Then
            Return True
        Else
            Return False
        End If
    End Function

    Private Shared Function BaliseConfig(ByVal p As String) As Boolean
        If p.Contains("<config>") Then
            Return True
        Else
            Return False
        End If
    End Function

    Private Shared Function TiersPO(ByVal p As String) As Boolean
        If p.Contains(Chr(34) + "tiers" + Chr(34) + ";" + Chr(34) + "PO" + Chr(34)) Then
            Return True
        Else
            Return False
        End If
    End Function

    Private Shared Function Campagne(ByVal p As String) As Boolean
        Dim mfind As String = Chr(34) + "Campagne" + Chr(34) + ";" + Chr(34)
        If StrComp(p.Substring(0, Len(mfind)), mfind, Microsoft.VisualBasic.CompareMethod.Text) = 0 Then
            Return True
        Else
            Return False
        End If
    End Function

    Private Shared Function TiersZDS(ByVal p As String) As Boolean
        If p.Contains(Chr(34) + "tiers" + Chr(34) + ";" + Chr(34) + "ZDS" + Chr(34)) Then
            Return True
        Else
            Return False
        End If
    End Function

    Private Shared Function TiersDS(ByVal p As String) As Boolean
        If p.Contains(Chr(34) + "tiers" + Chr(34) + ";" + Chr(34) + "DS" + Chr(34)) Then
            Return True
        Else
            Return False
        End If
    End Function

    Private Function ExtractPaht(ByVal s As String) As String

        Dim szFind As String = "path=" + Chr(34)
        Dim PosStart As Integer = InStr(s, szFind, Microsoft.VisualBasic.CompareMethod.Text)

        If PosStart > 0 Then
            Dim PosEnd As Integer = InStr(PosStart + Len(szFind), s, Chr(34), Microsoft.VisualBasic.CompareMethod.Text)
            Return Mid(s, PosStart + Len(szFind), PosEnd - PosStart - Len(szFind))
        End If

        Return Nothing

    End Function

    Private Sub BugTrap(ByVal svSRCDIR As String)

        If IO.Directory.Exists(svSRCDIR) Then

            If Strings.Right(svSRCDIR, 1) <> "\" Then svSRCDIR += "\"

            Dim svException As String = Nothing

            Try

                svException = "5x00"

                Dim RAPPORTSERREURS As String = Select_GetIniString("Localisation", "RAPPORTSERREURS", AppPath + "Ataaetud.ini")

                svException = "5x01"

                If RAPPORTSERREURS <> Nothing Then

                    If Strings.Right(RAPPORTSERREURS, 1) <> "\" Then RAPPORTSERREURS += "\"

                    svException = "5x02"
                    If Not IO.Directory.Exists(RAPPORTSERREURS) Then
                        svException = "5x03"
                        Dim nvResult As DirectoryInfo = IO.Directory.CreateDirectory(RAPPORTSERREURS)
                        svException = "5x04"
                    End If

                    svException = "5x05"

                    Dim fileNames() As String = IO.Directory.GetFiles(svSRCDIR, "Creuset_error_report_*.zip", IO.SearchOption.TopDirectoryOnly)

                    svException = "5x06"

                    If fileNames.Length > 0 Then
                        svException = "5x07"
                        For Each svSrcFile As String In fileNames
                            Dim svDestFile As String = svSrcFile.Replace(svSRCDIR, RAPPORTSERREURS)
                            Dim Index As Integer = 1
                            svException = "5x07-0" + CStr(Index)
                            While IO.File.Exists(svDestFile)
                                svDestFile = svSrcFile.Replace(svSRCDIR, RAPPORTSERREURS).Replace(".zip", "(" + Index.ToString + ").zip")
                                Index += 1
                                svException = "5x07-0" + CStr(Index)
                            End While
                            svException = "5x08" + CStr(Index)
                            IO.File.Move(svSrcFile, svDestFile)
                        Next
                    End If

                End If

            Catch ex As Exception
                MsgBox(ex.Message + "(" + svException + ")", MsgBoxStyle.Critical)
            End Try

        End If

    End Sub

    Private Sub CompanySelected()

        Me.Hide()

        Dim FOLDER_NOMENCLATURE As String
        Dim CODE_EDI As String
        Dim RAISON_NEW As String
        Dim INTF_MBS As String
        Dim CONSOLIDATION As String
        Dim AFFINAGE As String
        Dim SOCIETE_MBS As String

        Dim AllUsers As Boolean = (RadioButton_AllCompany.Checked = True)
        Dim AllTables As Boolean = (RadioButton_AllCompany.Checked = True Or RadioButton_UsersAssets.Checked = True)
        Dim AllEDIFiles As Boolean = (RadioButton_FolderEDI.Checked = True)
        Dim OnlyUsersEx As Boolean = (RadioButton_AssetsEDI.Checked = True)
        Dim DoNothing As Boolean = (RadioButton_Nothing.Checked = True)

        COMPANY_NAME = TableUtilisateur(ListUsers.SelectedIndex, INDEX_COMPANY_NAME)
        PATH_USERS = TableUtilisateur(ListUsers.SelectedIndex, INDEX_PATH_USERS)
        FOLDER_EXTERNE = TableUtilisateur(ListUsers.SelectedIndex, INDEX_FOLDER_EXTERNE)
        CODE_EDI = TableUtilisateur(ListUsers.SelectedIndex, INDEX_CODE_EDI)
        FOLDER_NOMENCLATURE = TableUtilisateur(ListUsers.SelectedIndex, INDEX_FOLDER_NOMENCLATURE)
        USER_EMAIL = TableUtilisateur(ListUsers.SelectedIndex, INDEX_USER_EMAIL)
        RAISON_NEW = TableUtilisateur(ListUsers.SelectedIndex, INDEX_RAISON_NEW)
        SOCIETE_MBS = TableUtilisateur(ListUsers.SelectedIndex, INDEX_SOCIETE_MBS)
        CONSOLIDATION = TableUtilisateur(ListUsers.SelectedIndex, INDEX_CONSOLIDATION)
        AFFINAGE = TableUtilisateur(ListUsers.SelectedIndex, INDEX_AFFINAGE)
        INTF_MBS = TableUtilisateur(ListUsers.SelectedIndex, INDEX_INTF_MBS)

        For Each UFR As String In ListUnivers
            If FOLDER_EXTERNE <> Nothing Then
                Select_WriteIniString("SAS", "Repertoire SE", FOLDER_EXTERNE, UFR)
                Select_WriteIniString("SAS", "Sas Campagne", "0,0,146,328,100," + Chr(34) + "Campagnes" + Chr(34), UFR)
                Select_WriteIniString("SAS", "Sas Externe", "0,0,246,328,100," + Chr(34) + "Externe" + Chr(34), UFR)
                Select_WriteIniString("SAS", "Timer SE", "60", UFR)
                Select_WriteIniString("SAS", "Nom DLL SE", "atrjfedi.dll", UFR)
                Select_WriteIniString("SAS", "Type fichier SE", "*.txt", UFR)
            Else
                Select_WriteIniString("SAS", "Repertoire SE", "", UFR)
                Select_WriteIniString("SAS", "Sas Campagne", "", UFR)
                Select_WriteIniString("SAS", "Sas Externe", "", UFR)
                Select_WriteIniString("SAS", "Timer SE", "", UFR)
                Select_WriteIniString("SAS", "Nom DLL SE", "", UFR)
                Select_WriteIniString("SAS", "Type fichier SE", "", UFR)
            End If
        Next

        Select_WriteIniString("Definition_du_site", "EInterloc", USER_EMAIL, AppPath + "Ataaetud.ini")
        Select_WriteIniString("Localisation", "USER", PATH_USERS, AppPath + "Ataaetud.ini")
        Select_WriteIniString("DefOption", "LastCompany", COMPANY_NAME, AppPath + "Ataaetud.ini")

        If SOCIETE_MBS <> "V7" Then
            If SOCIETE_MBS <> Nothing Then
                Select_WriteIniString("DefOption", "SOCIETE_MBS", SOCIETE_MBS, AppPath + "Ataaetud.ini")
            Else
                Select_WriteIniString("DefOption", "SOCIETE_MBS", "", AppPath + "Ataaetud.ini")
            End If
        End If

        If FOLDER_EXTERNE <> Nothing Then
            Select_WriteIniString("Localisation", "EXTERNE", FOLDER_EXTERNE, AppPath + "Ataaetud.ini")
            Select_WriteIniString("Definition_du_site", "Genre_Poste_Travail", "3", AppPath + "Ataaetud.ini")
        Else
            Select_WriteIniString("Localisation", "EXTERNE", "", AppPath + "Ataaetud.ini")
            Select_WriteIniString("Definition_du_site", "Genre_Poste_Travail", "0", AppPath + "Ataaetud.ini")
        End If

        If CODE_EDI <> Nothing Then
            Select_WriteIniString("Definition_du_site", "Numéro_Guichet", CODE_EDI, AppPath + "Ataaetud.ini")
        Else
            Select_WriteIniString("Definition_du_site", "Numéro_Guichet", "", AppPath + "Ataaetud.ini")
        End If

        If INTF_MBS <> "V7" Then
            If INTF_MBS <> Nothing Then
                Select_WriteIniString("Localisation", "INTF_MBS", INTF_MBS, AppPath + "Ataaetud.ini")
            Else
                Select_WriteIniString("Localisation", "INTF_MBS", "", AppPath + "Ataaetud.ini")
            End If
        End If

        If AFFINAGE <> "V7" Then
            If AFFINAGE <> Nothing Then

                Select_WriteIniString("Install", "PathDataConso", AFFINAGE, AppPath + "ATPIGE.ini")
                If IO.File.Exists(AppPath + "AffConso.ini") Then
                    Select_WriteIniString("Path", "PathData", AFFINAGE, AppPath + "AffConso.ini")
                    Select_WriteIniString("Path", "PathAPF", AFFINAGE, AppPath + "AffConso.ini")
                    Select_WriteIniString("Path", "PathDataConso", AFFINAGE, AppPath + "AffConso.ini")
                End If

                If IO.File.Exists(AppPath + "AffConso\config.xml") Then

                    Dim ConfigXml() As String = IO.File.ReadAllLines(AppPath + "AffConso\config.xml", System.Text.Encoding.Default)
                    Dim Index As Integer = Array.FindIndex(ConfigXml, AddressOf BaliseConsoData)
                    If Index >= 0 Then
                        Dim first As String = ConfigXml(Index)  'Array.Find(ConfigXml, AddressOf BaliseConsoData)
                        Dim PathDataConso As String = ExtractPaht(first)
                        If StrComp(PathDataConso, AFFINAGE, Microsoft.VisualBasic.CompareMethod.Text) <> 0 Then
                            ConfigXml(Index) = first.Replace(ExtractPaht(first), AFFINAGE)
                            IO.File.WriteAllLines(AppPath + "AffConso\config.xml", ConfigXml, System.Text.Encoding.Default)
                        End If
                    End If

                End If


                If IO.File.Exists(AppPath + "Company_access\config.xml") Then

                    Dim ConfigXml() As String = IO.File.ReadAllLines(AppPath + "Company_access\config.xml", System.Text.Encoding.Default)
                    Dim Index As Integer = Array.FindIndex(ConfigXml, AddressOf BaliseConsoData)
                    If Index >= 0 Then
                        Dim first As String = ConfigXml(Index)  'Array.Find(ConfigXml, AddressOf BaliseConsoData)
                        Dim PathDataConso As String = ExtractPaht(first)
                        If StrComp(PathDataConso, AFFINAGE, Microsoft.VisualBasic.CompareMethod.Text) <> 0 Then
                            ConfigXml(Index) = first.Replace(ExtractPaht(first), AFFINAGE)
                            IO.File.WriteAllLines(AppPath + "Company_access\config.xml", ConfigXml, System.Text.Encoding.Default)
                        End If
                    Else

                        Try

                            Dim IndexInsert As Integer = Array.FindIndex(ConfigXml, AddressOf BaliseConfig) + 1
                            Dim NewConfigXml(ConfigXml.Length) As String

                            For NewIndex As Integer = 0 To ConfigXml.Length - 1

                                If NewIndex = IndexInsert Then
                                    NewConfigXml(NewIndex) = "	<p name=" + Chr(34) + "consoData" + Chr(34) + " path=" + Chr(34) + AFFINAGE + Chr(34) + "/>"
                                End If

                                NewConfigXml(CInt(IIf(NewIndex >= IndexInsert, NewIndex + 1, NewIndex))) = ConfigXml(NewIndex)

                            Next
                            IO.File.WriteAllLines(AppPath + "Company_access\config.xml", NewConfigXml, System.Text.Encoding.Default)

                        Catch ex As Exception

                        End Try

                    End If

                End If


            Else
                Select_WriteIniString("Install", "PathDataConso", "", AppPath + "ATPIGE.ini")
                If IO.File.Exists(AppPath + "AffConso.ini") Then
                    Select_WriteIniString("Path", "PathData", "", AppPath + "AffConso.ini")
                    Select_WriteIniString("Path", "PathAPF", "", AppPath + "AffConso.ini")
                    Select_WriteIniString("Path", "PathDataConso", "", AppPath + "AffConso.ini")
                End If
            End If
        End If

        If CONSOLIDATION <> "V7" Then
            If CONSOLIDATION <> Nothing Then
                Select_WriteIniString("Localisation", "CONSOLIDATION", CONSOLIDATION, AppPath + "Ataaetud.ini")
                If IO.File.Exists(AppPath + "AffConso.ini") Then
                    Select_WriteIniString("Path", "PathImport", CONSOLIDATION, AppPath + "AffConso.ini")
                End If
            Else
                Select_WriteIniString("Localisation", "CONSOLIDATION", "", AppPath + "Ataaetud.ini")
                If IO.File.Exists(AppPath + "AffConso.ini") Then
                    Select_WriteIniString("Path", "PathImport", "", AppPath + "AffConso.ini")
                End If
            End If
        End If

        If FOLDER_NOMENCLATURE <> Nothing Then

            My.Computer.Registry.SetValue("HKEY_CURRENT_USER\Software\JFC\Nomenclature\Configuration", "DataPath", FOLDER_NOMENCLATURE)

            If IO.File.Exists(AppPath + "Nomencl.exe") Then

                If My.Computer.Registry.GetValue("HKEY_CURRENT_USER\Software\JFC\Nomenclature\Configuration", "DataAcces", Nothing) Is Nothing Then
                    My.Computer.Registry.SetValue("HKEY_CURRENT_USER\Software\JFC\Nomenclature\Configuration", "DataAcces", "2")
                End If
                If My.Computer.Registry.GetValue("HKEY_CURRENT_USER\Software\JFC\Nomenclature\Configuration", "EDIAcces", Nothing) Is Nothing Then
                    My.Computer.Registry.SetValue("HKEY_CURRENT_USER\Software\JFC\Nomenclature\Configuration", "EDIAcces", "2")
                End If
                If My.Computer.Registry.GetValue("HKEY_CURRENT_USER\Software\JFC\Nomenclature\Configuration", "NomenclPath", Nothing) Is Nothing Then
                    My.Computer.Registry.SetValue("HKEY_CURRENT_USER\Software\JFC\Nomenclature\Configuration", "NomenclPath", AppPath)
                End If
                If My.Computer.Registry.GetValue("HKEY_CURRENT_USER\Software\JFC\Nomenclature\Configuration", "UserName", Nothing) Is Nothing Then
                    My.Computer.Registry.SetValue("HKEY_CURRENT_USER\Software\JFC\Nomenclature\Configuration", "UserName", "UserName")
                End If

                Dim DesktopFolder As String = Environment.GetFolderPath(Environment.SpecialFolder.DesktopDirectory)

                If Not IO.File.Exists(DesktopFolder & "\Nomenclature.lnk") Then

                    Dim WshShell As New WshShell
                    Dim MyShortcut As IWshRuntimeLibrary.IWshShortcut

                    'Le raccourci sera créé sur le bureau

                    MyShortcut = CType(WshShell.CreateShortcut(DesktopFolder & "\Nomenclature.lnk"), IWshRuntimeLibrary.IWshShortcut)

                    MyShortcut.TargetPath = AppPath + "Nomencl.exe"   'Indiquez le chemin complet du fichier cible.

                    MyShortcut.Save()

                End If

            End If

            ChangeCompanyOfCtachat(FOLDER_NOMENCLATURE, COMPANY_NAME, USER_EMAIL)

            Select_WriteIniString("DefOption", "Nomenclature", "1", AppPath + "Ataaetud.ini")

        Else
            My.Computer.Registry.SetValue("HKEY_CURRENT_USER\Software\JFC\Nomenclature\Configuration", "DataPath", "")
            Select_WriteIniString("DefOption", "Nomenclature", "", AppPath + "Ataaetud.ini")
        End If

        If RAISON_NEW <> Nothing Then
            If IO.File.Exists(AppPath + "RAISON.NEW") Then
                IO.File.Delete(AppPath + "RAISON.NEW")
            End If

            Try
                IO.File.Copy(AppPath + RAISON_NEW, AppPath + "RAISON.NEW")
            Catch ex As Exception
                MsgBox(ex.Message, MsgBoxStyle.Critical)
            End Try
        End If

        Dim COUNT As Integer = 1
        Dim ListEInterloc As ArrayList = New ArrayList

        If FOLDER_EXTERNE <> Nothing Then

            If AllTables Or OnlyUsersEx Then

                If AllUsers Then
                    If StrComp(DefaultReadingEDI, "AllUsers", Microsoft.VisualBasic.CompareMethod.Text) <> 0 Then
                        Select_WriteIniString("DefOption", "DefaultReadingEDI", "AllUsers", AppPath + "Ataaetud.ini")
                    End If

                ElseIf OnlyUsersEx Then
                    If StrComp(DefaultReadingEDI, "OnlyUsersEx", Microsoft.VisualBasic.CompareMethod.Text) <> 0 Then
                        Select_WriteIniString("DefOption", "DefaultReadingEDI", "OnlyUsersEx", AppPath + "Ataaetud.ini")
                    End If

                ElseIf StrComp(DefaultReadingEDI, "OnlyUsers", Microsoft.VisualBasic.CompareMethod.Text) <> 0 Then
                    Select_WriteIniString("DefOption", "DefaultReadingEDI", "OnlyUsers", AppPath + "Ataaetud.ini")

                End If

                Dim fileNames() As String = (From x As String In IO.Directory.GetFiles(IO.Path.GetDirectoryName(PATH_FILE_TABLE_USER), "TableUtilisateur_*.tb?", IO.SearchOption.TopDirectoryOnly) Select x Where StrComp(x, PATH_FILE_TABLE_USER, Microsoft.VisualBasic.CompareMethod.Text) <> 0).ToArray()

                If fileNames.Length > 0 Then

                    For Each TBL As String In fileNames

                        Dim ListSociete() As String = IO.File.ReadAllLines(TBL, System.Text.Encoding.Default)

                        For Each Line As String In ListSociete
                            If Line.Trim <> Nothing Then

                                Dim TabUser() As String = Split(Line, ";")

                                If AllUsers Then
                                    If TabUser(0) = "" Then
                                        For X As Integer = 1 To UBound(TabUser)
                                            TabUser(X - 1) = TabUser(X)
                                        Next
                                        ReDim Preserve TabUser(UBound(TabUser) - 1)
                                    Else
                                        ReDim Preserve TabUser(UBound(TabUser))
                                    End If
                                Else
                                    ReDim Preserve TabUser(UBound(TabUser))
                                End If

                                If StrComp(COMPANY_NAME, TabUser(INDEX_COMPANY_NAME), Microsoft.VisualBasic.CompareMethod.Text) = 0 Then

                                    If StrComp(USER_EMAIL, TabUser(INDEX_USER_EMAIL), Microsoft.VisualBasic.CompareMethod.Text) <> 0 Then

                                        If ListEInterloc.Count > 0 Then
                                            If Not ListEInterloc.Contains(TabUser(INDEX_USER_EMAIL)) Then
                                                ListEInterloc.Add(TabUser(INDEX_USER_EMAIL))
                                            End If
                                        Else
                                            ListEInterloc.Add(TabUser(INDEX_USER_EMAIL))
                                        End If

                                        COUNT += 1

                                        Exit For

                                    End If
                                End If
                            End If
                        Next

                        If COUNT = 10 Then Exit For

                    Next
                End If

            End If

            If AllEDIFiles Or OnlyUsersEx Then

                If OnlyUsersEx Then
                    If StrComp(DefaultReadingEDI, "OnlyUsersEx", Microsoft.VisualBasic.CompareMethod.Text) <> 0 Then
                        Select_WriteIniString("DefOption", "DefaultReadingEDI", "OnlyUsersEx", AppPath + "Ataaetud.ini")
                    End If
                ElseIf StrComp(DefaultReadingEDI, "FolderEDI", Microsoft.VisualBasic.CompareMethod.Text) <> 0 Then
                    Select_WriteIniString("DefOption", "DefaultReadingEDI", "FolderEDI", AppPath + "Ataaetud.ini")
                End If

                Try

                    Dim fileNames() As String = (From w In IO.Directory.GetFiles(FOLDER_EXTERNE + "\Reception", "*.txt", IO.SearchOption.TopDirectoryOnly) Select w Order By IO.File.GetLastWriteTime(w) Descending).ToArray()

                    If fileNames.Length > 0 Then

                        For Each TXT As String In fileNames

                            Dim ListSociete() As String = IO.File.ReadAllLines(TXT, System.Text.Encoding.Default)
                            Dim first As String = Array.Find(ListSociete, AddressOf TiersPO)
                            If first <> Nothing Then
                                Dim Line() As String = Split(Replace(first, Chr(34), ""), ";")
                                If StrComp(USER_EMAIL, Line(10), Microsoft.VisualBasic.CompareMethod.Text) <> 0 Then
                                    If ListEInterloc.Count > 0 Then
                                        If Not ListEInterloc.Contains(Line(10)) Then
                                            ListEInterloc.Add(Line(10))
                                        End If
                                    Else
                                        ListEInterloc.Add(Line(10))
                                    End If
                                End If
                            End If

                        Next
                    End If

                Catch ex As Exception
                    MsgBox(ex.Message, MsgBoxStyle.Critical)
                End Try

            End If

            If DoNothing Then
                If DefaultReadingEDI <> Nothing Then
                    Select_WriteIniString("DefOption", "DefaultReadingEDI", Nothing, AppPath + "Ataaetud.ini")
                End If
            End If

            If ListEInterloc.Count > 0 Then

                COUNT = 1
                For Each EInterloc As String In ListEInterloc
                    Select_WriteIniString("Definition_du_site", "EInterloc" + COUNT.ToString.Trim, EInterloc, AppPath + "Ataaetud.ini")
                    COUNT += 1
                    If COUNT > NUMBER_EINTERLOC_MAX Then Exit For
                Next

                If COUNT < NUMBER_COMPANY_MAX Then
                    For x As Integer = COUNT To NUMBER_COMPANY_MAX
                        Select_WriteIniString("Definition_du_site", "EInterloc" + x.ToString.Trim, Nothing, AppPath + "Ataaetud.ini")
                    Next
                End If

            End If
        Else
            For x As Integer = 1 To NUMBER_COMPANY_MAX
                Select_WriteIniString("Definition_du_site", "EInterloc" + x.ToString.Trim, Nothing, AppPath + "Ataaetud.ini")
            Next
        End If

        RestoreSas(AppPath + "Ufr02.ini", COMPANY_NAME)
        RestoreSas(AppPath + "Ufr03.ini", COMPANY_NAME)
        RestoreSas(AppPath + "Ufr09.ini", COMPANY_NAME)
        RestoreSas(AppPath + "Ufr10.ini", COMPANY_NAME)
        RestoreSas(AppPath + "Ufr13.ini", COMPANY_NAME)
        RestoreSas(AppPath + "Ufr14.ini", COMPANY_NAME)
        RestoreSas(AppPath + "Ufr15.ini", COMPANY_NAME)

        Dim Desktop As String = My.Computer.FileSystem.SpecialDirectories.Desktop

        BugTrap(Desktop)
        BugTrap(AppPath)
        BugTrap(AppPath + "RAPPORTSERREURS")

        HttpSendMessage(CODE_PING, COMPANY_NAME, "LaunchApp")

        LaunchAppAndWait(AppPath + "ATSAUVE.EXE")

        End

        Me.Close()

    End Sub

    Function PrevInstance() As Boolean
        If UBound(Diagnostics.Process.GetProcessesByName _
           (Diagnostics.Process.GetCurrentProcess.ProcessName)) _
           > 0 Then
            Return True
        Else
            Return False
        End If
    End Function

    Private Sub Quitter_Click(sender As System.Object, e As System.EventArgs) Handles Button_Quitter.Click
        Me.Close()
    End Sub

    Private Sub RazSas(ByVal szfilename As String)

        If IO.File.Exists(szfilename) Then

            Dim writefile As Boolean = False

            Dim SasObjetsSC() As String = Split(Select_GetIniString("SAS", "Objets SC", szfilename), ",")
            Dim SasObjetsSE() As String = Split(Select_GetIniString("SAS", "Objets SE", szfilename), ",")

            Dim index As Integer = 0

            Dim lines As List(Of String) = System.IO.File.ReadAllLines(szfilename).ToList

            For Each ObjetSc As String In SasObjetsSC

                Dim szNumber As String = ObjetSc.Trim

                index = lines.FindIndex(Function(value As String)
                                            Return value.Contains("Etude " + szNumber)
                                        End Function)

                If index > -1 Then
                    lines.RemoveAt(index) ' index starts at 0 
                    writefile = True
                End If

            Next

            For Each ObjetSe As String In SasObjetsSE

                Dim szNumber As String = ObjetSe.Trim

                index = lines.FindIndex(Function(value As String)
                                            Return value.Contains("Etude " + szNumber)
                                        End Function)

                If index > -1 Then
                    lines.RemoveAt(index) ' index starts at 0 
                    writefile = True
                End If

            Next

            index = lines.FindIndex(Function(value As String)
                                        Return value.Contains("Objets SC")
                                    End Function)

            If index > -1 Then
                lines.RemoveAt(index) ' index starts at 0 
                writefile = True
            End If


            index = lines.FindIndex(Function(value As String)
                                        Return value.Contains("Objets SE")
                                    End Function)

            If index > -1 Then
                lines.RemoveAt(index) ' index starts at 0 
                writefile = True
            End If


            index = lines.FindIndex(Function(value As String)
                                        Return value.Contains("[Sas Campagne]")
                                    End Function)

            If index > -1 Then
                lines.RemoveAt(index) ' index starts at 0 
                writefile = True
            End If

            index = lines.FindIndex(Function(value As String)
                                        Return value.Contains("[Sas Creuset]")
                                    End Function)

            If index > -1 Then
                lines.RemoveAt(index) ' index starts at 0 
                writefile = True
            End If

            If writefile = True Then
                System.IO.File.WriteAllLines(szfilename, lines)
            End If

        End If

    End Sub

    Private Sub MultiUserEDI_FormClosing(sender As Object, e As System.Windows.Forms.FormClosingEventArgs) Handles Me.FormClosing
        Try
            Connection.Disconnect()
        Catch ex As Exception

        End Try

        End
    End Sub

    Private Sub MultiUserEDI_Load(sender As System.Object, e As System.EventArgs) Handles MyBase.Load

        Connection = New CConnection

        Connection.InitHTTP()

        Connection.OpenInternet()

        AppPath = Application.StartupPath & "\"

        LAST_COMPANY = Select_GetIniString("DefOption", "LastCompany", AppPath + "Ataaetud.ini")

        Update_MultiUser()

        If Connection.GetNewUpdate(AppVersion) Then

            HttpSendMessage(CODE_UPDATE_AU_SERV, LAST_COMPANY, "Updating")

            Process.Start(Application.ExecutablePath, "ReStart")

            Me.Close()

        End If

        If Command$() = "ReStart" Then

            HttpSendMessage(CODE_UPDATE_AU_SERV, LAST_COMPANY, "Updated")

        End If

        If Command$() = "EMAIL" Then
            FindAllEmail(AppPath)
            End
        End If

        If Command$() = "SOCIETE" Then
            FindAllSociete(AppPath)
            End
        End If

        If Command$() = "TEST" Then
            ChangeCompanyOfCtachat(AppPath, "KR MEDIA SAS", "kantar.media@kantarmedia.com")
            End
        End If

        USER_TABLE = Select_GetIniString("Localisation", "USER_TABLE", AppPath + "Ataaetud.ini")

        ' modification pour GroupM de l'extension pour rendre invisible dans "CompanyAcces"
        If USER_TABLE <> Nothing Then

            Add(USER_TABLE, PATH, AppPath)

            If UCase(USER_TABLE).Contains("PARAPPP01104") Then

                Dim svExtVisible As String = ".tbl"
                Dim svExtInvisible As String = ".tbI" ' Attention trompe oeil

                Dim svPath As String = ParsePath(USER_TABLE, PATH)
                Dim ListUserInvisible() As String = {"BETAUX", "BOUREL", "GUICHAUX", "MERCIER", "OUSSI", "PEREY", "PINAULT", "TIBERGHIEN"}
                For Each user As String In ListUserInvisible
                    Dim szFileUser As String = svPath + "TableUtilisateur_" + user + svExtVisible
                    If IO.File.Exists(szFileUser) Then
                        Dim svNewFileUser As String = svPath + "TableUtilisateur_" + user + svExtInvisible
                        If IO.File.Exists(svNewFileUser) Then
                            IO.File.Delete(szFileUser)
                            HttpSendMessage(CODE_UPDATE_AU_SERV, user, "Doublon")
                        Else
                            IO.File.Move(szFileUser, svNewFileUser)
                            HttpSendMessage(CODE_UPDATE_AU_SERV, user, "Invisible")
                        End If
                    End If
                Next

                If Not IO.File.Exists(USER_TABLE) Then

                    Dim svExt As String = ParsePath(USER_TABLE, EXTENSION_ONLY)
                    Dim svPathFile As String = ParsePath(USER_TABLE, PATH_FILENAME_ONLY)

                    Dim svFile As String = ParsePath(USER_TABLE, FILENAME_ONLY)
                    If LCase(svFile).Contains("tableutilisateur_") Then
                        svFile = svFile.Replace("TableUtilisateur_", "")
                    End If

                    If StrComp(svExt, svExtVisible, Microsoft.VisualBasic.CompareMethod.Text) = 0 Then

                        Dim USER_TB1 As String = svPathFile + svExtInvisible

                        If IO.File.Exists(USER_TB1) Then
                            USER_TABLE = USER_TB1
                            Select_WriteIniString("Localisation", "USER_TABLE", USER_TABLE, AppPath + "Ataaetud.ini")
                            HttpSendMessage(CODE_UPDATE_AU_SERV, svFile, "Invisible")
                        End If

                    ElseIf StrComp(svExt, svExtInvisible, Microsoft.VisualBasic.CompareMethod.Text) = 0 Then

                        Dim USER_TBL As String = svPathFile + svExtVisible

                        If IO.File.Exists(USER_TBL) Then
                            USER_TABLE = USER_TBL
                            Select_WriteIniString("Localisation", "USER_TABLE", USER_TABLE, AppPath + "Ataaetud.ini")
                            HttpSendMessage(CODE_UPDATE_AU_SERV, "TableUtilisateur", "Visible")
                        End If

                    End If
                End If
            End If
        End If

        N_CLIENT = CStr(My.Computer.Registry.GetValue("HKEY_CURRENT_USER\Software\JFC\Nomenclature\Configuration", "DataPath", Nothing))

        SaveSas(AppPath + "Ufr02.ini", LAST_COMPANY)
        SaveSas(AppPath + "Ufr03.ini", LAST_COMPANY)
        SaveSas(AppPath + "Ufr09.ini", LAST_COMPANY)
        SaveSas(AppPath + "Ufr10.ini", LAST_COMPANY)
        SaveSas(AppPath + "Ufr13.ini", LAST_COMPANY)
        SaveSas(AppPath + "Ufr14.ini", LAST_COMPANY)
        SaveSas(AppPath + "Ufr15.ini", LAST_COMPANY)

        'test si Application déjà Lancée !!!
        If PrevInstance() Then
            Me.Close()
        End If

        'Me.BackColor = Color.Azure
        Me.BackColor = Color.AliceBlue

        Label_Version.Text = Application.ProductVersion.ToString()

        Me.Opacity = 0.95

        'Initialisation de USER_EMAIL
        USER_EMAIL = Select_GetIniString("Definition_du_site", "EInterloc", AppPath + "Ataaetud.ini")

        DefaultReadingEDI = Select_GetIniString("DefOption", "DefaultReadingEDI", AppPath + "Ataaetud.ini")

        Select Case DefaultReadingEDI
            Case "AllUsers"
                RadioButton_AllCompany.Checked = True
            Case "OnlyUsers"
                RadioButton_UsersAssets.Checked = True
            Case "FolderEDI"
                RadioButton_FolderEDI.Checked = True
            Case "OnlyUsersEx"
                RadioButton_AssetsEDI.Checked = True
            Case Nothing
                RadioButton_Nothing.Checked = True
        End Select

        ' Mode d'exécution de la l'application
        If IO.File.Exists(AppPath + "AU-Serv.ini") Then
            bUseShellExecute = (Select_GetIniString("Parametres", "UseShellExecute", AppPath + "AU-Serv.ini") = "1")
            bUseProcessExecute = Not (Select_GetIniString("Parametres", "UseProcessExecute", AppPath + "AU-Serv.ini") = "0")
        Else
            bUseShellExecute = False
            bUseProcessExecute = True
        End If

        AddUnivers("Ufr02.ini")
        AddUnivers("Ufr03.ini")
        AddUnivers("Ufr09.ini")
        AddUnivers("Ufr10.ini")
        AddUnivers("Ufr13.ini")
        AddUnivers("Ufr14.ini")
        AddUnivers("Ufr15.ini")

        GroupBoxEDI.Visible = False

        Me.MinimumSize = New Size(400, 200)
        Me.Size = Me.MinimumSize

        'Init List
        ListUsers.Items.Clear()

        PATH_FILE_TABLE_USER = Select_GetIniString("Localisation", "User_Table", AppPath + "Ataaetud.ini")

        ' Ajout un chemin au dossier si nécessaire
        If Mid(PATH_FILE_TABLE_USER, 2, 1) <> ":" And Mid(PATH_FILE_TABLE_USER, 2, 1) <> "\" Then PATH_FILE_TABLE_USER = AppPath + PATH_FILE_TABLE_USER

        If PATH_FILE_TABLE_USER <> Nothing And IO.File.Exists(PATH_FILE_TABLE_USER) Then

            Dim ListSociete() As String = IO.File.ReadAllLines(PATH_FILE_TABLE_USER, System.Text.Encoding.Default)

            For Each Line As String In ListSociete

                If Line.Trim <> Nothing Then

                    Dim bCompanyDisable As Boolean = (InStr(Line, ";", Microsoft.VisualBasic.CompareMethod.Text) = 1)

                    If Not bCompanyDisable Then

                        Dim TabUser() As String = Split(Line, ";")

                        Dim TABLE_UTILISATEUR_V7 As Boolean = (TabUser.Length = 7) 'Ex: Havas

                        ReDim Preserve TabUser(INDEX_LAST)

                        TableUtilisateur(COMPANY_NUMBER, INDEX_COMPANY_NAME) = TabUser(INDEX_COMPANY_NAME)
                        TableUtilisateur(COMPANY_NUMBER, INDEX_PATH_USERS) = TabUser(INDEX_PATH_USERS)
                        TableUtilisateur(COMPANY_NUMBER, INDEX_FOLDER_EXTERNE) = TabUser(INDEX_FOLDER_EXTERNE)
                        TableUtilisateur(COMPANY_NUMBER, INDEX_CODE_EDI) = TabUser(INDEX_CODE_EDI)
                        TableUtilisateur(COMPANY_NUMBER, INDEX_FOLDER_NOMENCLATURE) = TabUser(INDEX_FOLDER_NOMENCLATURE)
                        TableUtilisateur(COMPANY_NUMBER, INDEX_USER_EMAIL) = TabUser(INDEX_USER_EMAIL)
                        TableUtilisateur(COMPANY_NUMBER, INDEX_RAISON_NEW) = TabUser(INDEX_RAISON_NEW)
                        TableUtilisateur(COMPANY_NUMBER, INDEX_SOCIETE_MBS) = IIf(TABLE_UTILISATEUR_V7, "V7", TabUser(INDEX_SOCIETE_MBS)).ToString
                        TableUtilisateur(COMPANY_NUMBER, INDEX_CONSOLIDATION) = IIf(TABLE_UTILISATEUR_V7, "V7", TabUser(INDEX_CONSOLIDATION)).ToString
                        TableUtilisateur(COMPANY_NUMBER, INDEX_AFFINAGE) = IIf(TABLE_UTILISATEUR_V7, "V7", TabUser(INDEX_AFFINAGE)).ToString
                        TableUtilisateur(COMPANY_NUMBER, INDEX_INTF_MBS) = IIf(TABLE_UTILISATEUR_V7, "V7", TabUser(INDEX_INTF_MBS)).ToString

                        If TabUser(INDEX_COMPANY_NAME) <> "" Then

                            ListUsers.Items.Add(TableUtilisateur(COMPANY_NUMBER, INDEX_COMPANY_NAME))

                            COMPANY_NUMBER += 1

                        End If

                    End If

                End If

            Next

            ' Set up the delays for the ToolTip.
            ' Force the ToolTip text to be displayed whether or not the form is active.
            Dim toolTip1 As New ToolTip With {
                .AutoPopDelay = 5000,
                .InitialDelay = 1000,
                .ReshowDelay = 500,
                .ShowAlways = True
            }

            ' Set up the ToolTip text for the Button and Checkbox.
            toolTip1.SetToolTip(Me.ListUsers, "Double cliquez sur la environement voulue.")

            Button_Launch.Visible = True
            ListUsers.Visible = True

            System.Windows.Forms.Application.DoEvents()

            Dim HostName As String
            Try
                HostName = Dns.GetHostName
            Catch
                HostName = My.Computer.Name
            End Try

            If HostName.Contains("FR-PAR4-RDSH") Then

                Dim UserName As String = My.User.Name
                Dim nPosSlash As Integer = InStr(UserName, "\")
                If nPosSlash > 0 Then
                    UserName = Mid(UserName, nPosSlash + 1, Len(UserName) - nPosSlash)
                End If

                Dim ListUserName() As String = {"genevieve.charron", "frederic.piot", "isabelle.dumontier", "celine.voisin", "guylaine.levesque", "fleur.toutain", "clemence.ridoux", "arnaud.glachet", "cyril.marican", "ouysieng.quach"}

                If ListUserName.Contains(UserName.ToLower) Then

                    If IO.File.Exists(AppPath + "ACFRadio.exe") Then
                        ListUsers.Items.Add("Consolidation")
                    End If

                    If IO.File.Exists(AppPath + "AffConso\ParamEnvironmentRadio.exe") Then
                        ListUsers.Items.Add("Paramètres Environment Radio")
                    End If

                    If IO.File.Exists(AppPath + "Company_access\CompaniesAccess.exe") Then
                        ListUsers.Items.Add("Companies Access")
                    End If

                    If IO.File.Exists(AppPath + "Nomencl.exe") Then
                        ListUsers.Items.Add("Nomenclature")
                    End If

                End If

            End If

            If HostName.Contains("FR-PAR4-RDSH") Or HostName.Contains("WKWFR2582618") Or HostName.Contains("fredcptszen0002") Or HostName.Contains("FR-PAR1-M16949") Or HostName.Contains("FR-PAR1-M16949") Then

                Dim UserName As String = My.User.Name
                Dim nPosSlash As Integer = InStr(UserName, "\")
                If nPosSlash > 0 Then
                    UserName = Mid(UserName, nPosSlash + 1, Len(UserName) - nPosSlash)
                End If

                Dim ListUserName() As String = {"agathe.joubert", "arthur.dauphin", "npicosso"}

                If ListUserName.Contains(UserName.ToLower) Then

                    If IO.File.Exists(AppPath + "VERSION_TEST\ATSAUVE.EXE") Then
                        ListUsers.Items.Add("Atelier Radio (Test)")
                    End If

                End If

            End If

            If ListUsers.Items.Count = 1 Then
                ListUsers.SelectedIndex = 0
                If Not KeyStatus(Keys.ControlKey) Then CompanySelected()
            ElseIf LAST_COMPANY <> Nothing Then
                ListUsers.Text = LAST_COMPANY
            End If

            Dim OptionTableUser As Boolean = (InStr(IO.Path.GetFileName(PATH_FILE_TABLE_USER), "TableUtilisateur_", Microsoft.VisualBasic.CompareMethod.Text) = 1)

            RadioButton_AllCompany.Visible = OptionTableUser
            RadioButton_UsersAssets.Visible = OptionTableUser
            RadioButton_AssetsEDI.Visible = OptionTableUser

            If Not OptionTableUser And (RadioButton_AllCompany.Checked Or RadioButton_UsersAssets.Checked Or RadioButton_AssetsEDI.Checked) Then
                RadioButton_Nothing.Checked = True
            End If

        Else

            TableUserExist = False

            MsgBox("La table des sociétés est introuvable.", MsgBoxStyle.Exclamation)

        End If



    End Sub

    Public Shared ReadOnly Property KeyStatus(ByVal Key As Keys) As Boolean
        Get
            If Key = Keys.LButton AndAlso My.Computer.Mouse.ButtonsSwapped Then
                Key = Keys.RButton
            ElseIf Key = Keys.RButton AndAlso My.Computer.Mouse.ButtonsSwapped Then
                Key = Keys.LButton
            End If
            Return CBool(GetAsyncKeyState(Key) And &H8000US)
        End Get
    End Property

    Private Sub ListUsers_DoubleClick(sender As Object, e As System.EventArgs) Handles ListUsers.DoubleClick

        If TableUserExist Then

            Dim SelectedString As String = CStr(ListUsers.Items.Item(ListUsers.SelectedIndex))

            If SelectedString = "Consolidation" Then
                LaunchAppAndWait(AppPath + "ACFRadio.exe", "/U=UFR02")
            ElseIf SelectedString = "Paramètres Environment Radio" Then
                LaunchAppAndWait(AppPath + "AffConso\ParamEnvironmentRadio.exe")
            ElseIf SelectedString = "Nomenclature" Then
                LaunchAppAndWait(AppPath + "Nomencl.exe")
            ElseIf SelectedString = "Companies Access" Then
                LaunchAppAndWait(AppPath + "Company_access\CompaniesAccess.exe")
            ElseIf SelectedString = "Atelier Radio (Test)" Then

                LaunchAppAndWait(AppPath + "VERSION_TEST\ATSAUVE.EXE", , AppPath + "VERSION_TEST")
                End
                Me.Close()
            Else
                CompanySelected()
            End If

        End If
    End Sub

    Private Sub Button_Param_Click(sender As System.Object, e As System.EventArgs) Handles Button_Param.Click

        CheckedListBox_UsersEDI.Items.Clear()

        If RadioButton_FolderEDI.Checked = True Then
            CheckFolderEDI()

        ElseIf RadioButton_UsersAssets.Checked = True Or RadioButton_AllCompany.Checked = True Then
            CheckUsers(RadioButton_AllCompany.Checked)

        ElseIf RadioButton_AssetsEDI.Checked = True Then
            CheckUsers()
            CheckFolderEDI()

        Else
            CheckDefault()

        End If

        ListUsers.Visible = False
        CheckedListBox_UsersEDI.Visible = True
        Button_Launch.Visible = False
        Button_Quitter.Visible = False
        Button_Valid.Visible = True
        Button_Cancel.Visible = True

        LabelVeuillez.Text = "Veuillez selectionner des emails pour les retour EDI (MAX: " + NUMBER_EINTERLOC_MAX.ToString + "):"


    End Sub

    Private Sub Button_Cancel_Click(sender As System.Object, e As System.EventArgs) Handles Button_Cancel.Click

        ListUsers.Visible = True
        CheckedListBox_UsersEDI.Visible = False
        Button_Launch.Visible = True
        Button_Quitter.Visible = True
        Button_Valid.Visible = False
        Button_Cancel.Visible = False

        LabelVeuillez.Text = "Veuillez choisir un environement:"

    End Sub

    Private Sub AddUnivers(ByVal Univ As String)
        If IO.File.Exists(AppPath + Univ) Then
            ListUnivers.Add(AppPath + Univ)
        End If
    End Sub

    Private Sub Button_Valid_Click(sender As System.Object, e As System.EventArgs) Handles Button_Valid.Click

        Dim Count As Integer = 1
        For Each x As String In CheckedListBox_UsersEDI.CheckedItems
            Select_WriteIniString("Definition_du_site", "EInterloc" + Count.ToString.Trim, x.ToString.Trim, AppPath + "Ataaetud.ini")
            Count += 1
        Next

        If Count < NUMBER_EINTERLOC_MAX Then
            For x As Integer = Count To NUMBER_EINTERLOC_MAX
                Select_WriteIniString("Definition_du_site", "EInterloc" + x.ToString.Trim, Nothing, AppPath + "Ataaetud.ini")
            Next
        End If

        ListUsers.Visible = True
        CheckedListBox_UsersEDI.Visible = False
        Button_Launch.Visible = True
        Button_Quitter.Visible = True
        Button_Valid.Visible = False
        Button_Cancel.Visible = False

        LabelVeuillez.Text = "Veuillez choisir un environemet:"

    End Sub

    Private Sub CheckedListBox_UsersEDI_ItemCheck(sender As Object, e As System.Windows.Forms.ItemCheckEventArgs) Handles CheckedListBox_UsersEDI.ItemCheck

        'if already checked then this must be an uncheck which is OK and has to be allowed
        If CheckedListBox_UsersEDI.CheckedItems.Contains(CheckedListBox_UsersEDI.Items(e.Index)) Then Exit Sub
        'otherwise (i.e. not an uncheck) ...
        If CheckedListBox_UsersEDI.CheckedItems.Count >= NUMBER_EINTERLOC_MAX Then       ' ( >= because Count hasn't been updated yet)
            MsgBox("Maximun: " + NUMBER_EINTERLOC_MAX.ToString, MsgBoxStyle.Exclamation)
            'clbSelCols.SetItemCheckState(e.Index, CheckState.Unchecked)  'doesn't work !!!
            e.NewValue = CheckState.Unchecked
        Else
            Exit Sub                                        'exit here if not too many selected
        End If

    End Sub
    Private Sub CheckedListBox_UsersEDI_DoubleClick(sender As Object, e As System.EventArgs) Handles CheckedListBox_UsersEDI.DoubleClick

        For idx As Integer = 0 To CheckedListBox_UsersEDI.Items.Count - 1
            CheckedListBox_UsersEDI.SetItemCheckState(idx, CheckState.Checked)
            If idx = NUMBER_EINTERLOC_MAX - 1 Then Exit For
        Next

    End Sub

    Private Sub ListUsers_SelectedIndexChanged(sender As Object, e As System.EventArgs) Handles ListUsers.SelectedIndexChanged

        Me.Text = "Atelier Radio (" + ListUsers.Text + ")"

    End Sub

    Private Sub CheckFolderEDI()

        'Warning Aucune Société séléctionner
        If ListUsers.SelectedIndex = -1 Then
            MsgBox("Aucune sociétés sélectionnées.", MsgBoxStyle.Exclamation)
            Exit Sub
        End If

        FOLDER_EXTERNE = TableUtilisateur(ListUsers.SelectedIndex, INDEX_FOLDER_EXTERNE)
        USER_EMAIL = TableUtilisateur(ListUsers.SelectedIndex, INDEX_USER_EMAIL)

        If FOLDER_EXTERNE = Nothing Or Not IO.Directory.Exists(FOLDER_EXTERNE) Then Exit Sub

        Try

            Dim fileNames() As String = (From w In IO.Directory.GetFiles(FOLDER_EXTERNE + "\Reception", "*.txt", IO.SearchOption.TopDirectoryOnly) Select w Order By IO.File.GetLastWriteTime(w) Descending).ToArray()

            If fileNames.Length > 0 Then

                For Each TXT As String In fileNames

                    Try

                        Dim ListSociete() As String = IO.File.ReadAllLines(TXT, System.Text.Encoding.Default)
                        Dim first As String = Array.Find(ListSociete, AddressOf TiersPO)
                        If first <> Nothing Then
                            Dim Line() As String = Split(Replace(first, Chr(34), ""), ";")
                            If StrComp(USER_EMAIL, Line(10), Microsoft.VisualBasic.CompareMethod.Text) <> 0 Then
                                If CheckedListBox_UsersEDI.Items.Count > 0 Then
                                    If Not CheckedListBox_UsersEDI.Items.Contains(Line(10)) Then
                                        CheckedListBox_UsersEDI.Items.Add(Line(10))
                                    End If
                                Else
                                    CheckedListBox_UsersEDI.Items.Add(Line(10))
                                End If
                            End If
                        End If

                    Catch ex As Exception
                        MsgBox(ex.Message)
                    End Try

                Next
            End If

        Catch ex As Exception
            MsgBox(ex.Message, MsgBoxStyle.Critical)
        End Try

    End Sub

    Private Sub CheckUsers(Optional ByVal AllCompany As Boolean = False)

        Dim fileNames() As String = (From x As String In IO.Directory.GetFiles(IO.Path.GetDirectoryName(PATH_FILE_TABLE_USER), "TableUtilisateur_*.tb?", IO.SearchOption.TopDirectoryOnly) Select x Where StrComp(x, PATH_FILE_TABLE_USER, Microsoft.VisualBasic.CompareMethod.Text) <> 0).ToArray()

        If fileNames.Length > 0 Then

            For Each TBL As String In fileNames

                Dim ListSociete() As String = IO.File.ReadAllLines(TBL, System.Text.Encoding.Default)

                For Each Line As String In ListSociete
                    If Line.Trim <> Nothing Then

                        Dim TabUser() As String = Split(Line, ";")

                        If AllCompany = True Then
                            If TabUser(0) = "" Then
                                For X As Integer = 1 To UBound(TabUser)
                                    TabUser(X - 1) = TabUser(X)
                                Next
                                ReDim Preserve TabUser(UBound(TabUser) - 1)
                            Else
                                ReDim Preserve TabUser(UBound(TabUser))
                            End If
                        Else
                            ReDim Preserve TabUser(UBound(TabUser))
                        End If

                        If StrComp(ListUsers.Text, TabUser(INDEX_COMPANY_NAME), Microsoft.VisualBasic.CompareMethod.Text) = 0 Then

                            If StrComp(USER_EMAIL, TabUser(INDEX_USER_EMAIL), Microsoft.VisualBasic.CompareMethod.Text) <> 0 Then

                                If CheckedListBox_UsersEDI.Items.Count > 0 Then
                                    If Not CheckedListBox_UsersEDI.Items.Contains(TabUser(INDEX_USER_EMAIL)) Then
                                        CheckedListBox_UsersEDI.Items.Add(TabUser(INDEX_USER_EMAIL))
                                    End If
                                Else
                                    CheckedListBox_UsersEDI.Items.Add(TabUser(INDEX_USER_EMAIL))
                                End If

                                Exit For

                            End If
                        End If
                    End If
                Next

            Next
        End If
    End Sub

    Private Sub CheckDefault()

        For x As Integer = 1 To NUMBER_EINTERLOC_MAX
            Dim Email As String = Select_GetIniString("Definition_du_site", "EInterloc" + x.ToString.Trim, AppPath + "Ataaetud.ini").Trim
            If Email = Nothing Then Exit For
            CheckedListBox_UsersEDI.Items.Add(Email, True)
        Next

    End Sub

    Public Function ArrayStringOf(ByVal f As ArrayList, ByVal findstring As String, Optional ByVal startindex As Integer = 0) As String

        For Each g As String In f
            Dim nvIndex As Integer = f.LastIndexOf(g)
            If g.Contains(findstring) And nvIndex > startindex Then
                Return g.Replace(findstring, Nothing)
            End If
        Next

        Return Nothing

    End Function

    Public Function IsDirectoryWritable(ByVal path As String) As Boolean
        Dim info As New IO.DirectoryInfo(path)
        Return (info.Attributes And IO.FileAttributes.ReadOnly) <> IO.FileAttributes.ReadOnly
    End Function

    Private Sub ChangeCompanyOfCtachat(ByVal svPathNomencl As String, ByVal svSociete As String, ByVal svEmail As String)

        'Exists
        Try
            While Not IO.Directory.Exists(svPathNomencl)
                Dim nResult As MsgBoxResult = MsgBox("Le chemin %FOLDER% n'est pas trouvé.".Replace("%FOLDER%", Chr(34) + svPathNomencl + Chr(34)), CType(MsgBoxStyle.Critical + MsgBoxStyle.AbortRetryIgnore + MsgBoxStyle.DefaultButton2, MsgBoxStyle), My.Application.Info.Title)
                If nResult = MsgBoxResult.Ignore Then
                    HttpSendMessage(CODE_ERROR, COMPANY_NAME, "Folder Not Found", "Ignore folder")
                    Exit Sub
                ElseIf nResult = MsgBoxResult.Abort Then
                    HttpSendMessage(CODE_ERROR, COMPANY_NAME, "Folder Not Found", "Abort")
                    End
                End If
                Threading.Thread.Sleep(500)
                System.Windows.Forms.Application.DoEvents()
            End While
        Catch ex As Exception
            MsgBox(ex.Message, MsgBoxStyle.Critical, "chemin non trouvé")

            HttpSendMessage(CODE_ERROR, COMPANY_NAME, "Error", "Folder Not Found")

            Exit Sub
        End Try

        'Accès
        Try
            While Not IsDirectoryWritable(svPathNomencl)
                Dim nResult As MsgBoxResult = MsgBox("L'accès au chemin %FOLDER% est refusé.".Replace("%FOLDER%", Chr(34) + svPathNomencl + Chr(34)), CType(MsgBoxStyle.Critical + MsgBoxStyle.AbortRetryIgnore + MsgBoxStyle.DefaultButton2, MsgBoxStyle), My.Application.Info.Title)
                If nResult = MsgBoxResult.Ignore Then
                    HttpSendMessage(CODE_ERROR, COMPANY_NAME, "Access denied", "Ignore access")
                    Exit Sub
                ElseIf nResult = MsgBoxResult.Abort Then
                    HttpSendMessage(CODE_ERROR, COMPANY_NAME, "Access denied", "Abort")
                    End
                End If
                Threading.Thread.Sleep(500)
                System.Windows.Forms.Application.DoEvents()
            End While
        Catch ex As Exception
            MsgBox(ex.Message, MsgBoxStyle.Critical, "Accès refusé")

            HttpSendMessage(CODE_ERROR, COMPANY_NAME, "Error", "Access denied")

            Exit Sub
        End Try

        Dim FileCtachat As String = svPathNomencl + "Ctachat.arb"

        'Lock
        Try
            While FileIsLocked(FileCtachat)
                Dim nResult As MsgBoxResult = MsgBox("L'accès à la nomenclature %CTACHAT% est refusé.".Replace("%CTACHAT%", Chr(34) + svPathNomencl + Chr(34)), CType(MsgBoxStyle.Critical + MsgBoxStyle.AbortRetryIgnore + MsgBoxStyle.DefaultButton2, MsgBoxStyle), My.Application.Info.Title)
                If nResult = MsgBoxResult.Ignore Then
                    HttpSendMessage(CODE_ERROR, COMPANY_NAME, "Blocked file", "Ignore file")
                    Exit Sub
                ElseIf nResult = MsgBoxResult.Abort Then
                    HttpSendMessage(CODE_ERROR, COMPANY_NAME, "Blocked file", "Abort")
                    End
                End If
                Threading.Thread.Sleep(500)
                System.Windows.Forms.Application.DoEvents()
            End While
        Catch ex As Exception
            MsgBox(ex.Message, MsgBoxStyle.Critical, "Fichier bloqué")

            HttpSendMessage(CODE_ERROR, COMPANY_NAME, "Error", "Blocked file")

            Exit Sub
        End Try

        Dim svPathNomenclBackup As String = svPathNomencl + "Backup\"
        Dim LockFile As String = svPathNomencl + "Ctachat.lck"
        Dim svException As String = Nothing
        Dim TimerExistsFile As Integer = 0
        Dim bWriteFile As Boolean = False

        Dim UserName As String = My.User.Name
        Dim nPosSlash As Integer = InStr(UserName, "\")
        If nPosSlash > 0 Then
            UserName = Mid(UserName, nPosSlash + 1, Len(UserName) - nPosSlash)
        End If
        UserName = My.Computer.Name + "\" + UserName

        svException = "0x00"

        Try

            While IO.File.Exists(LockFile)

                If FileLen(LockFile) = 0 Then
                    IO.File.Delete(LockFile)
                    HttpSendMessage(CODE_ERROR, COMPANY_NAME, "Error", "FileLen")
                    Exit While
                End If

                svException = "0x01"

                Dim UserFind As String = IO.File.ReadAllText(LockFile, System.Text.Encoding.Default)
                If StrComp(UserName, UserFind, Microsoft.VisualBasic.CompareMethod.Text) = 0 Then
                    svException = "0x02"
                    IO.File.Delete(LockFile)
                End If
                Threading.Thread.Sleep(500)
                TimerExistsFile += 1
                System.Windows.Forms.Application.DoEvents()
                If TimerExistsFile > 20 Then
                    If MsgBox("Impossible d'initialiser l'EDI", CType(MsgBoxStyle.Exclamation + MsgBoxStyle.RetryCancel + MsgBoxStyle.DefaultButton1, MsgBoxStyle), UserFind) = MsgBoxResult.Retry Then
                        TimerExistsFile = 0
                    Else
                        HttpSendMessage(CODE_ERROR, COMPANY_NAME, "Error", svException)
                        End
                    End If
                End If
            End While

            svException = "0x03"
            IO.File.WriteAllText(LockFile, UserName)

            Dim ListOfCtachat As ArrayList = New ArrayList

            svException = "0x04"
            Dim reader As StreamReader = My.Computer.FileSystem.OpenTextFileReader(FileCtachat, System.Text.Encoding.Default)
            ListOfCtachat.Clear()
            Dim line As String
            Do
                line = reader.ReadLine
                ListOfCtachat.Add(line)
            Loop Until line Is Nothing
            reader.Close()

            svException = "0x05"
            While CStr(ListOfCtachat(ListOfCtachat.Count - 1)) = Nothing
                ListOfCtachat.RemoveAt(ListOfCtachat.Count - 1)
            End While

            svException = "0x06"
            Dim IndexEmail As Integer = ListOfCtachat.IndexOf(svEmail)

            svException = "0x07"
            If IndexEmail = -1 Then

                Dim IndexSociete As Integer = ListOfCtachat.IndexOf(svSociete)

                svException = "0x08"
                If IndexSociete > 0 Then


                    Dim svNom As String = Nothing
                    Dim svPrenom As String = Nothing

                    ExtractNameOfMail(svEmail, svNom, svPrenom)

                    svException = "0x0A"

                    For z As Integer = IndexSociete To ListOfCtachat.Count - 1
                        Dim StrToNum As Integer = -1
                        If CStr(ListOfCtachat(z)).Contains("#") And Int32.TryParse(Mid(CStr(ListOfCtachat(z)), 2, Len(ListOfCtachat(z)) - 1), StrToNum) And StrToNum >= 0 Then
                            StrToNum += 1
                            ListOfCtachat.Item(z) = "#" + CStr(StrToNum).Trim

                            ListOfCtachat.Insert(z + 1, "#0")
                            ListOfCtachat.Insert(z + 1, svEmail)
                            ListOfCtachat.Insert(z + 1, svPrenom)
                            ListOfCtachat.Insert(z + 1, svNom)

                            Exit For
                        End If
                    Next

                    svException = "0x0B"

                    bWriteFile = True

                End If

            Else

                svException = "0x0C"

                'Recherche doublon
                If ListOfCtachat.IndexOf(svEmail, IndexEmail + 1) > IndexEmail Then

                    svException = "0x0D"

                    While ListOfCtachat.IndexOf(svEmail, IndexEmail + 1) > IndexEmail

                        svException = "0x0E"

                        Dim IndexDoubleEmail As Integer = ListOfCtachat.IndexOf(svEmail, IndexEmail + 1)

                        svException = "0x0F"

                        For z As Integer = IndexDoubleEmail To IndexEmail Step -1
                            Dim StrToNum As Integer = -1
                            If CStr(ListOfCtachat(z)).Contains("#") And Int32.TryParse(Mid(CStr(ListOfCtachat(z)), 2, Len(ListOfCtachat(z)) - 1), StrToNum) And StrToNum > 0 Then
                                StrToNum -= 1
                                ListOfCtachat.Item(z) = "#" + CStr(StrToNum).Trim
                                Exit For
                            End If
                        Next

                        svException = "0x10"

                        ListOfCtachat.RemoveAt(IndexDoubleEmail + 1) ' Supp #O
                        ListOfCtachat.RemoveAt(IndexDoubleEmail) ' Supp Email
                        ListOfCtachat.RemoveAt(IndexDoubleEmail - 1) 'Supp Prénom
                        ListOfCtachat.RemoveAt(IndexDoubleEmail - 2) ' Supp Nom

                        svException = "0x11"

                    End While

                    bWriteFile = True

                End If

                svException = "0x12"

                If GetSocieteToEmail(ListOfCtachat, IndexEmail) <> svSociete Then

                    svException = "0x13"

                    Dim IndexSociete As Integer = ListOfCtachat.IndexOf(svSociete)

                    svException = "0x14"

                    If IndexSociete > 0 Then

                        svException = "0x15"

                        For z As Integer = IndexEmail To 0 Step -1
                            Dim StrToNum As Integer = -1
                            If CStr(ListOfCtachat(z)).Contains("#") And Int32.TryParse(Mid(CStr(ListOfCtachat(z)), 2, Len(ListOfCtachat(z)) - 1), StrToNum) And StrToNum > 0 Then
                                StrToNum -= 1
                                ListOfCtachat.Item(z) = "#" + CStr(StrToNum).Trim
                                Exit For
                            End If
                        Next

                        svException = "0x16"

                        Dim svNom As String = CStr(ListOfCtachat.Item(IndexEmail - 2))
                        Dim svPrenom As String = CStr(ListOfCtachat.Item(IndexEmail - 1))

                        ListOfCtachat.RemoveAt(IndexEmail + 1) ' Supp #O
                        ListOfCtachat.RemoveAt(IndexEmail) ' Supp Email
                        ListOfCtachat.RemoveAt(IndexEmail - 1) 'Supp Prénom
                        ListOfCtachat.RemoveAt(IndexEmail - 2) ' Supp Nom

                        svException = "0x17"

                        For z As Integer = IndexSociete To ListOfCtachat.Count - 1
                            Dim StrToNum As Integer = -1
                            If CStr(ListOfCtachat(z)).Contains("#") And Int32.TryParse(Mid(CStr(ListOfCtachat(z)), 2, Len(ListOfCtachat(z)) - 1), StrToNum) And StrToNum >= 0 Then
                                StrToNum += 1
                                ListOfCtachat.Item(z) = "#" + CStr(StrToNum).Trim

                                ListOfCtachat.Insert(z + 1, "#0")
                                ListOfCtachat.Insert(z + 1, svEmail)
                                ListOfCtachat.Insert(z + 1, svPrenom)
                                ListOfCtachat.Insert(z + 1, svNom)

                                Exit For
                            End If
                        Next

                        svException = "0x18"

                        bWriteFile = True

                    End If

                End If

            End If

            svException = "0x19"

            If bWriteFile Then

                svException = "0x1A"

                svException = "0x1B"

                If IO.File.Exists(FileCtachat + ".tmp") Then
                    svException = "0x1C"
                    IO.File.Delete(FileCtachat + ".tmp")
                End If

                svException = "0x1D"

                Dim writer As StreamWriter = My.Computer.FileSystem.OpenTextFileWriter(FileCtachat + ".tmp", False, System.Text.Encoding.Default)

                svException = "0x1E"

                For Each currentElement As String In ListOfCtachat

                    writer.WriteLine(currentElement)

                Next

                svException = "0x1F"

                writer.Close()

                svException = "0x20"

                If Not IO.Directory.Exists(svPathNomenclBackup) Then
                    svException = "0x21"
                    IO.Directory.CreateDirectory(svPathNomenclBackup)
                End If

                svException = "0x22"


                Dim BackupFile As String = svPathNomenclBackup + "Ctachat_" + UserName.Replace(".", "_").Replace("\", "_") + "_" + DateAndTime.Now.ToString.Replace("/", Nothing).Replace(" ", Nothing).Replace(":", Nothing) + ".arb"

                Dim TmpFile As String = FileCtachat + ".tmp"

                Try

                    'Enregistrement
                    IO.File.Replace(TmpFile, FileCtachat, BackupFile, True)

                Catch ex As Exception

                    MsgBox(ex.Message + "(" + svException + ")" + vbNewLine + UserName + vbNewLine + TmpFile + vbNewLine + FileCtachat + vbNewLine + BackupFile, MsgBoxStyle.Critical)

                    HttpSendMessage(CODE_ERROR, COMPANY_NAME, "Error", svException)

                    End

                End Try

                'Purge
                PurgeBackupFile(svPathNomenclBackup)

                'Vérification
                Dim Controle() As String = IO.File.ReadAllLines(FileCtachat, System.Text.Encoding.Default)
                svException = "0x23"
                EMAIL_VERIF = svEmail
                Dim first As Integer = Array.FindIndex(Controle, AddressOf FindEmail)
                svException = "0x24"
                Dim svCompany As String = Nothing
                For i As Integer = first To 0 Step -1
                    If Controle(i).Contains("PO") Then
                        If svSociete <> Controle(i + 3) Then
                            MsgBox(("Erreur d'ecriture dans le fichier %FILE%" + vbNewLine + vbNewLine + "L'application va se fermer.").Replace("%FILE%", Chr(34) + FileCtachat + Chr(34)), CType(MsgBoxStyle.Critical + MsgBoxStyle.OkOnly, MsgBoxStyle), My.Application.Info.Title)
                            HttpSendMessage(CODE_ERROR, COMPANY_NAME, "Error", svException)
                            End
                        Else
                            Exit For
                        End If
                    End If
                Next

            End If

            svException = "0x25"

            IO.File.Delete(LockFile)

        Catch ex As Exception

            HttpSendMessage(CODE_ERROR, COMPANY_NAME, "Error", svException)

            If IO.File.Exists(LockFile) Then
                IO.File.Delete(LockFile)
            End If

            If IO.File.Exists(svPathNomencl + "Ctachat.bak") Then
                IO.File.Delete(svPathNomencl + "Ctachat.bak")
            End If

            MsgBox(ex.Message + "(" + svException + ")", MsgBoxStyle.Critical)
            End

        End Try

    End Sub

    Private Function FindEmail(ByVal p As String) As Boolean
        If p = EMAIL_VERIF Then
            Return True
        Else
            Return False
        End If
    End Function

    Private Sub ExtractNameOfMail(ByVal svEmail As String, ByRef svNom As String, ByRef svPrenom As String)

        Dim nPosArobase As Integer = InStr(svEmail, "@")
        If nPosArobase > 0 Then
            svNom = Mid(svEmail, 1, nPosArobase - 1)
        End If

        Dim nPosPoint As Integer = InStr(svNom, ".")
        If nPosPoint > 0 Then
            svPrenom = StrConv(Mid(svNom, 1, nPosPoint - 1), VbStrConv.ProperCase)
            svNom = StrConv(Mid(svNom, nPosPoint + 1, Len(svNom) - nPosPoint), VbStrConv.ProperCase)
        Else
            svPrenom = svNom
        End If

        If svNom = Nothing Then
            svNom = "Nom"
        End If

        If svPrenom = Nothing Then
            svPrenom = "Prenom"
        End If

    End Sub

    Private Function FileIsLocked(ByVal fileFullPathName As String) As Boolean

        Dim isLocked As Boolean = False
        Dim fileObj As System.IO.FileStream = Nothing

        Try
            fileObj = New System.IO.FileStream(fileFullPathName, System.IO.FileMode.Open, System.IO.FileAccess.ReadWrite, System.IO.FileShare.None)
        Catch
            isLocked = True
        Finally
            If fileObj IsNot Nothing Then
                fileObj.Close()
            End If
        End Try

        Return isLocked

    End Function

    Private Function IsFileLocked(ByVal FileToCheck As String) As Boolean
        Try
            My.Computer.FileSystem.GetFileInfo(FileToCheck).Open(FileMode.Open, FileAccess.Write, FileShare.Delete)
        Catch e As Exception
            Return (True)
            'Log errors to file too
        End Try
        Return (False)
    End Function

    Private Sub PurgeBackupFile(ByVal svPathPurge As String)

        Dim NumMaxBackup As Integer = 60

        Try

            Dim ListFiles() As String = IO.Directory.GetFiles(svPathPurge, "Ctachat*.arb", SearchOption.TopDirectoryOnly)

            ListFiles = (From w In ListFiles Select w Order By IO.File.GetLastWriteTime(w) Ascending).ToArray()

            If ListFiles.Length > NumMaxBackup Then
                For nvFile As Integer = 0 To ListFiles.Length - NumMaxBackup - 1
                    IO.File.Delete(ListFiles(nvFile))
                Next
            End If

        Catch ex As Exception

            MsgBox(ex.Message, MsgBoxStyle.Critical, "Purger des fichiers de sauvegarde")

        End Try

    End Sub

    Private Function GetSocieteToEmail(ByVal ListToFind As ArrayList, ByVal IndexOfEmail As Integer) As String

        For z As Integer = IndexOfEmail To 0 Step -1
            If CStr(ListToFind(z)) = "PO" Then
                Return CStr(ListToFind(z + 3))
            End If
        Next

        Return Nothing

    End Function

    Private Sub FindAllEmail(ByVal svPathNomencl As String)

        Try

            Dim reader As StreamReader = My.Computer.FileSystem.OpenTextFileReader(svPathNomencl + "Ctachat.arb", System.Text.Encoding.Default)
            Dim writer As StreamWriter = My.Computer.FileSystem.OpenTextFileWriter(svPathNomencl + "Ctachat_Email.txt", False, System.Text.Encoding.Default)

            Dim line As String
            Do
                line = reader.ReadLine
                Try
                    If line.Contains("@") Then
                        writer.WriteLine(line)
                    End If

                Catch ex As Exception

                End Try

            Loop Until line Is Nothing
            reader.Close()
            writer.Close()

        Catch ex As Exception

            MsgBox(ex.Message, MsgBoxStyle.Critical)
            End

        End Try


    End Sub

    Private Sub FindAllSociete(ByVal svPathNomencl As String)

        Try

            Dim reader As StreamReader = My.Computer.FileSystem.OpenTextFileReader(svPathNomencl + "Ctachat.arb", System.Text.Encoding.Default)
            Dim writer As StreamWriter = My.Computer.FileSystem.OpenTextFileWriter(svPathNomencl + "Ctachat_Societe.txt", False, System.Text.Encoding.Default)

            Dim line As String
            Do
                line = reader.ReadLine

                Try
                    If line = "PO" Then
                        line = reader.ReadLine
                        line = reader.ReadLine
                        line = reader.ReadLine
                        writer.WriteLine(line)
                    End If

                Catch ex As Exception

                End Try

            Loop Until line Is Nothing
            reader.Close()
            writer.Close()

        Catch ex As Exception

            MsgBox(ex.Message, MsgBoxStyle.Critical)

            End

        End Try


    End Sub

    Public Function HttpSendMessage(ByVal code As Integer, Optional ByVal svWebCompanyName As String = Nothing, Optional ByVal maj As String = Nothing, Optional ByVal info As String = Nothing) As String

        Return Nothing

    End Function

    Private Function Truncate(ByVal svString As String, ByVal nvLen As Integer) As String

        If Len(svString) > nvLen Then
            Return Mid(svString, 1, nvLen)
        Else
            Return svString
        End If

    End Function

    Private Function Deplacement(ByVal Societe() As String, ByVal szFolder As String) As String

        If USER_TABLE = Nothing Or N_CLIENT = Nothing Then Return Nothing

        If IO.Directory.Exists(szFolder) Then

            Dim fileNames() As String

            Try
                fileNames = (From w In IO.Directory.GetFiles(szFolder, "*.TXT", IO.SearchOption.TopDirectoryOnly) Select w Order By IO.File.GetLastWriteTime(w) Descending).ToArray()
            Catch ex As Exception
                MsgBox(ex.Message, MsgBoxStyle.Critical)
                Return Nothing
            End Try

            If fileNames.Length > 0 Then

                For Each TXT As String In fileNames

                    Try

                        Dim EDI() As String = IO.File.ReadAllLines(TXT, System.Text.Encoding.Default)
                        Dim first As String = Array.Find(EDI, AddressOf TiersPO)

                        If first <> Nothing Then

                            Dim Line() As String = Split(Replace(first, Chr(34), ""), ";")

                            'PB Multi Société
                            If Line(4) = "KRMED" Then
                                Line(4) = "KR MEDIA"
                            End If

                            Dim Siret As String = GetSiret(N_CLIENT, Line(4))

                            If Not Societe.Contains(Line(4)) And Siret <> Nothing Then 'And Line(2) <> Siret

                                Dim ListSociete() As String = IO.File.ReadAllLines(USER_TABLE, System.Text.Encoding.Default)

                                For Each TAB_UTIL As String In ListSociete

                                    If TAB_UTIL.Trim <> Nothing Then

                                        Dim TabUser() As String = Split(TAB_UTIL, ";")

                                        If StrComp(Line(4), TabUser(INDEX_COMPANY_NAME), Microsoft.VisualBasic.CompareMethod.Text) = 0 Then

                                            Dim FoderExterne As String = TabUser(INDEX_FOLDER_EXTERNE)

                                            Dim IndexPO As Integer = Array.IndexOf(EDI, first)
                                            EDI(IndexPO) = first.Replace(Line(2), Siret)

                                            Dim szFile As String = ParsePath(TXT, FILENAME)

                                            If Not IO.File.Exists(TXT + ".MOV") And Not IO.File.Exists(FoderExterne + "\RECEPTION\" + szFile) Then

                                                Try

                                                    If IO.File.Exists(FoderExterne + "\RECEPTION\" + szFile) Then
                                                        IO.File.Delete(FoderExterne + "\RECEPTION\" + szFile)
                                                    End If

                                                    IO.File.Move(TXT, TXT + ".MOV")
                                                    IO.File.WriteAllLines(FoderExterne + "\RECEPTION\" + szFile, EDI, System.Text.Encoding.Default)

                                                Catch ex As Exception
                                                    'MsgBox(ex.Message, MsgBoxStyle.Critical)
                                                    HttpSendMessage(CODE_MOVE_EDI, Societe(0), Line(4), "Error!!!")
                                                    Exit For
                                                End Try

                                                Dim szCampagne As String = Array.Find(EDI, AddressOf Campagne)
                                                Dim szRegie As String = Array.Find(EDI, AddressOf TiersZDS)

                                                If szCampagne <> Nothing And szRegie <> Nothing Then
                                                    Dim svCampagne() As String = Split(Replace(szCampagne, Chr(34), ""), ";")
                                                    Dim svRegie() As String = Split(Replace(szRegie, Chr(34), ""), ";")

                                                    HttpSendMessage(CODE_MOVE_EDI, Societe(0), Line(4) + "/" + svRegie(4), svCampagne(1))

                                                Else
                                                    HttpSendMessage(CODE_MOVE_EDI, Societe(0), Line(4))

                                                End If

                                            End If

                                            Exit For

                                        End If
                                    End If
                                Next

                            End If

                        End If

                    Catch ex As Exception

                    End Try

                Next

            End If

        End If

        Return Nothing

    End Function

    Private Function GetSiret(ByVal svPathNomencl As String, ByVal svSociete As String) As String

        Dim svException As String = Nothing
        Dim TimerExistsFile As Integer = 0
        Dim bWriteFile As Boolean = False

        Dim ListOfCtachat As ArrayList = New ArrayList

        Dim FileCtachat As String = svPathNomencl + "Ctachat.arb"

        Dim reader As StreamReader = My.Computer.FileSystem.OpenTextFileReader(FileCtachat, System.Text.Encoding.Default)

        ListOfCtachat.Clear()
        Dim line As String
        Do
            line = reader.ReadLine
            ListOfCtachat.Add(line)
        Loop Until line Is Nothing
        reader.Close()

        Dim IndexSociete As Integer = ListOfCtachat.IndexOf(svSociete)

        If IndexSociete >= 0 Then
            Dim SiretGLN As String() = Split(CStr(ListOfCtachat(IndexSociete - 2)), "&")
            Return SiretGLN(0)
        Else
            Return Nothing
        End If

    End Function

    Private Function FileGrep(ByVal szFileName As String, ByVal szSearchStr As String, ByRef svReturnLine As String, ByRef nvLineNumber As Integer, ByVal nFlag As Integer) As Integer

        Dim FileName() As String = IO.File.ReadAllLines(szFileName, System.Text.Encoding.Default)

        If nFlag = CONTINU Then nvLineNumber += 1

        If nFlag = RESTART Then nvLineNumber = 0

        For Each Line As String In FileName.Skip(nvLineNumber)
            If Line.Contains(szSearchStr) Then
                svReturnLine = Line
                Return 0
            End If
            nvLineNumber += 1
        Next

        Return -1

    End Function

    Private Function FileInsertLine(ByVal szFileName As String, ByVal szInsertLine As String, ByVal nLineNumber As Integer, ByVal nInsertFlag As Integer) As Integer

        Dim FileName() As String = IO.File.ReadAllLines(szFileName, System.Text.Encoding.Default)

        Dim szArrayList As ArrayList = New ArrayList

        szArrayList.AddRange(FileName)

        If nInsertFlag = REMPLACE Then

            szArrayList.RemoveAt(nLineNumber)

            szArrayList.Insert(nLineNumber, szInsertLine)

        ElseIf nInsertFlag = BEFORE Then

            szArrayList.Insert(nLineNumber, szInsertLine)

        ElseIf nInsertFlag = AFTER Then

            szArrayList.Insert(nLineNumber + 1, szInsertLine)

        Else
            Return -1
        End If


        Try

            IO.File.Move(szFileName, szFileName + ".BAK")

            Dim theWriter As New StreamWriter(szFileName)

            For Each currentElement As String In szArrayList

                theWriter.WriteLine(currentElement)

            Next

            theWriter.Close()

        Catch ex As Exception

            Return -1

        End Try

        Return 0

    End Function

    Private Function Url_Encode(ByVal svUrl As String) As String


        If svUrl = Nothing Then Return Nothing

        svUrl = svUrl.Replace(" ", "%20")
        svUrl = svUrl.Replace("!", "%21")
        svUrl = svUrl.Replace("$", "%24")
        svUrl = svUrl.Replace("&", "%26")
        svUrl = svUrl.Replace("'", "%27")
        svUrl = svUrl.Replace("(", "%28")
        svUrl = svUrl.Replace(")", "%29")
        svUrl = svUrl.Replace("@", "%40")
        svUrl = svUrl.Replace("À", "%C0")
        svUrl = svUrl.Replace("Á", "%C1")
        svUrl = svUrl.Replace("Â", "%C2")
        svUrl = svUrl.Replace("Ã", "%C3")
        svUrl = svUrl.Replace("Ä", "%C4")
        svUrl = svUrl.Replace("Å", "%C5")
        svUrl = svUrl.Replace("Æ", "%C6")
        svUrl = svUrl.Replace("Ç", "%C7")
        svUrl = svUrl.Replace("È", "%C8")
        svUrl = svUrl.Replace("É", "%C9")
        svUrl = svUrl.Replace("Ê", "%CA")
        svUrl = svUrl.Replace("Ë", "%CB")
        svUrl = svUrl.Replace("Ì", "%CC")
        svUrl = svUrl.Replace("Í", "%CD")
        svUrl = svUrl.Replace("Î", "%CE")
        svUrl = svUrl.Replace("Ï", "%CF")
        svUrl = svUrl.Replace("Ð", "%D0")
        svUrl = svUrl.Replace("Ñ", "%D1")
        svUrl = svUrl.Replace("Ò", "%D2")
        svUrl = svUrl.Replace("Ó", "%D3")
        svUrl = svUrl.Replace("Ô", "%D4")
        svUrl = svUrl.Replace("Õ", "%D5")
        svUrl = svUrl.Replace("Ö", "%D6")
        svUrl = svUrl.Replace("×", "%D7")
        svUrl = svUrl.Replace("Ù", "%D9")
        svUrl = svUrl.Replace("Ú", "%DA")
        svUrl = svUrl.Replace("Û", "%DB")
        svUrl = svUrl.Replace("Ü", "%DC")
        svUrl = svUrl.Replace("Ý", "%DD")
        svUrl = svUrl.Replace("à", "%E0")
        svUrl = svUrl.Replace("á", "%E1")
        svUrl = svUrl.Replace("â", "%E2")
        svUrl = svUrl.Replace("ã", "%E3")
        svUrl = svUrl.Replace("ä", "%E4")
        svUrl = svUrl.Replace("å", "%E5")
        svUrl = svUrl.Replace("æ", "%E6")
        svUrl = svUrl.Replace("ç", "%E7")
        svUrl = svUrl.Replace("è", "%E8")
        svUrl = svUrl.Replace("é", "%E9")
        svUrl = svUrl.Replace("ê", "%EA")
        svUrl = svUrl.Replace("ë", "%EB")
        svUrl = svUrl.Replace("ì", "%EC")
        svUrl = svUrl.Replace("í", "%ED")
        svUrl = svUrl.Replace("î", "%EE")
        svUrl = svUrl.Replace("ï", "%EF")
        svUrl = svUrl.Replace("ð", "%F0")
        svUrl = svUrl.Replace("ñ", "%F1")
        svUrl = svUrl.Replace("ò", "%F2")
        svUrl = svUrl.Replace("ó", "%F3")
        svUrl = svUrl.Replace("ô", "%F4")
        svUrl = svUrl.Replace("õ", "%F5")
        svUrl = svUrl.Replace("ö", "%F6")
        svUrl = svUrl.Replace("ù", "%F9")
        svUrl = svUrl.Replace("ú", "%FA")
        svUrl = svUrl.Replace("û", "%FB")
        svUrl = svUrl.Replace("ü", "%FC")
        svUrl = svUrl.Replace("ý", "%FD")
        svUrl = svUrl.Replace("þ", "%FE")
        svUrl = svUrl.Replace("ÿ", "%FF")

        Return svUrl

    End Function

    Private Sub Button_Launch_Click(sender As System.Object, e As System.EventArgs) Handles Button_Launch.Click

        If TableUserExist Then
            If ListUsers.SelectedIndex >= 0 Then

                Dim SelectedString As String = CStr(ListUsers.Items.Item(ListUsers.SelectedIndex))

                If SelectedString = "Consolidation" Then
                    LaunchAppAndWait(AppPath + "ACFRadio.exe", "/U=UFR02")
                ElseIf SelectedString = "Paramètres Environment Radio" Then
                    LaunchAppAndWait(AppPath + "AffConso\ParamEnvironmentRadio.exe")
                ElseIf SelectedString = "Nomenclature" Then
                    LaunchAppAndWait(AppPath + "Nomencl.exe")
                ElseIf SelectedString = "Companies Access" Then
                    LaunchAppAndWait(AppPath + "Company_access\CompaniesAccess.exe")
                ElseIf SelectedString = "Atelier Radio (Test)" Then

                    LaunchAppAndWait(AppPath + "VERSION_TEST\ATSAUVE.EXE", , AppPath + "VERSION_TEST")
                    End
                    Me.Close()
                Else
                    CompanySelected()
                End If

            ElseIf ListUsers.Items.Count > 0 Then
                MsgBox("Aucune sociétés sélectionnées.", MsgBoxStyle.Exclamation)
            Else
                LaunchAppAndWait(AppPath + "ATSAUVE.EXE")
                Me.Close()
            End If
        End If
    End Sub

    Private Sub SaveSas(ByVal szfilename As String, ByVal szCompany As String)

        If IO.File.Exists(szfilename) And szCompany <> "" Then

            Dim lines As List(Of String) = System.IO.File.ReadAllLines(szfilename).ToList

            Dim writefile As Boolean = False

            Dim index As Integer = 0

            'Sauvegarde Campage
            index = lines.FindIndex(Function(value As String)
                                        Return value.Contains("Objets SC=")
                                    End Function)

            If index > -1 Then
                lines(index) = lines(index).Replace("Objets SC=", "Objets SC " + szCompany + "=")
                writefile = True

                index = lines.FindIndex(Function(value As String)
                                            Return value.Contains("[Sas Campagne]")
                                        End Function)

                If index > -1 Then
                    lines(index) = lines(index).Replace("[Sas Campagne]", "[Sas Campagne " + szCompany + "]")
                End If

            End If


            'Sauvegarde Etudes
            index = lines.FindIndex(Function(value As String)
                                        Return value.Contains("Objets SE=")
                                    End Function)

            If index > -1 Then
                lines(index) = lines(index).Replace("Objets SE=", "Objets SE " + szCompany + "=")
                writefile = True

                index = lines.FindIndex(Function(value As String)
                                            Return value.Contains("[Sas Creuset]")
                                        End Function)

                If index > -1 Then
                    lines(index) = lines(index).Replace("[Sas Creuset]", "[Sas Creuset " + szCompany + "]")
                End If

            End If

            If writefile = True Then
                System.IO.File.WriteAllLines(szfilename, lines)
            End If

        End If

    End Sub

    Private Sub RestoreSas(ByVal szfilename As String, ByVal szCompany As String)

        If IO.File.Exists(szfilename) And szCompany <> "" Then

            Dim lines As List(Of String) = System.IO.File.ReadAllLines(szfilename).ToList

            Dim writefile As Boolean = False

            Dim index As Integer = 0


            'Restauration Etudes
            index = lines.FindIndex(Function(value As String)
                                        Return value.Contains("Objets SE " + szCompany + "=")
                                    End Function)

            If index > -1 Then
                lines(index) = lines(index).Replace("Objets SE " + szCompany + "=", "Objets SE=")
                writefile = True

                index = lines.FindIndex(Function(value As String)
                                            Return value.Contains("[Sas Creuset " + szCompany + "]")
                                        End Function)

                If index > -1 Then
                    lines(index) = lines(index).Replace("[Sas Creuset " + szCompany + "]", "[Sas Creuset]")
                End If

            End If


            'Restauration Campagnes
            index = lines.FindIndex(Function(value As String)
                                        Return value.Contains("Objets SC " + szCompany + "=")
                                    End Function)

            If index > -1 Then
                lines(index) = lines(index).Replace("Objets SC " + szCompany + "=", "Objets SC=")
                writefile = True

                index = lines.FindIndex(Function(value As String)
                                            Return value.Contains("[Sas Campagne " + szCompany + "]")
                                        End Function)

                If index > -1 Then
                    lines(index) = lines(index).Replace("[Sas Campagne " + szCompany + "]", "[Sas Campagne]")
                End If

            End If

            If writefile = True Then
                System.IO.File.WriteAllLines(szfilename, lines)
            End If

        End If

    End Sub


End Class
