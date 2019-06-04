Option Strict On
Option Explicit On

Imports VB = Microsoft.VisualBasic
Imports System.Net

Imports System
Imports System.Management

Imports System.Collections.Generic
Imports System.Linq
Imports System.Text
Imports System.Runtime.InteropServices
Imports System.Security.Principal
Imports System.ComponentModel

Friend Class AUService

    Inherits System.Windows.Forms.Form

    Public EndOfFile As String = "#END#" ' Version => 5.8.5.0
    Public EndOfFileOld As String = "##" ' Version <= 5.8.4.1
    Public EndOfFilePlantage As String = "###" ' Plantage sur les versions <= 5.8.4.1.

    'Fonction spécifique
    Public AdmUser As String = "[ADM]"
    Public Skip As String = "[SKIP]"

    'Program
    Public DisplayNameReStart As String = "ReStart"
    Public DisplayNameZipLib As String = "ICSharpCode.SharpZipLib"
    Public QuickSupport As String = "TeamViewerQS_fr-idcah9cn6w.exe"

    'CommandLine
    Dim CommandLine_ReStartAutoUpdate As String = "RESTART_AUTO_UPDATE"
    Dim CommandLine_ReStartProg As String = "ReStart"
    Dim CommandLine_InitializeInstall As String = "InitializeInstall"

    Public WithEvents Connection As cConnection
    Public WithEvents lb As New ListBoxEX

    Public HostEntry As String
    Public UserName As String

    Public AUServiceIni, AppPath, svPatch, svCodeAppli, svFileMaj, svCompanyName, svSerie, svWebCompanyName, ExecuteFile, ExcludeSubDir As String
    Public bUseShellExecute, bUseProcessExecute, bQuickSupport As Boolean
    Public bSendRequest As Boolean = True
    Dim svNameAppli, svRemoteDir, AUServPath, ExecuteCmdLine, svSerial, svLocalDir, HostName, svPatchCopy As String
    Dim ExecuteWait, KillProcess, Ping, ForceId, ForceWait, bOnlyDownload, bButtonExecuteVisible, ForceUpdate, StopedService As Boolean
    Dim tot_taille, taille_enc, fic_taille As Long
    Dim nWaitClose As Integer

    Public bAdmUser As Boolean

    Dim FileAcces As String

    Public bAutomaticUpdate As Boolean = False
    Public bReturnLicence As Boolean = True
    Dim idx_maj_encours As Short = 0
    Dim idx_maj_max As Short
    Dim svId As String = ""
    Dim bAutomatic As Boolean = True
    Dim bRemovePatch, bCheckProcess As Boolean
    Dim bLaunchUpdateAuto As Boolean = False
    Dim svFilter As String = ""
    Dim svUpdate As String = ""
    Dim nvStartUpdate As Integer = 0
    Dim AutorisationUpdate As Boolean = True
    Dim AutorisationLaunchApp As Boolean = True
    Dim bCopyBeforeExecute As Boolean = False
    Dim FreeSpace As Double = 0
    Dim svSerialFound As String = ""
    Dim HostEntryName As String = ""

    Structure FILE_UPDATE
        Dim numMaj0 As String
        Dim fic_source1 As String
        Dim taille2 As String
        Dim CheckSum3 As String
        Dim infoMaj4 As String
        Dim fic_destination5 As String
        Dim reload As Boolean
        Dim restart As Boolean
    End Structure

    Dim File() As FILE_UPDATE

    Private Sub AUService_FormClosing(ByVal sender As Object, ByVal e As System.Windows.Forms.FormClosingEventArgs) Handles Me.FormClosing

        System.Windows.Forms.Application.DoEvents()

        If Me.Visible = True Then

            If MsgBox(mLanguageAU.GetString(MSG_EXIT_APPLICATION), MsgBoxStyle.Exclamation Or MsgBoxStyle.YesNo Or MsgBoxStyle.DefaultButton2, My.Application.Info.Title) = MsgBoxResult.Yes Then

                Connection.Disconnect()

                Me.Visible = False

                End

            Else
                e.Cancel = True
            End If
        Else
            End
        End If

    End Sub

    Public Function IsProcessRunning(ByVal name As String) As Boolean
        For Each clsProcess As Process In Process.GetProcesses()
            If clsProcess.ProcessName.StartsWith(name) Then
                Return True
            End If
        Next
        Return False
    End Function

    Private Sub AUService_Load(ByVal eventSender As System.Object, ByVal eventArgs As System.EventArgs) Handles MyBase.Load

        Me.Visible = False
        Me.KeyPreview = True

        'Controle si une instance existe déjà 
        If Diagnostics.Process.GetProcessesByName(Diagnostics.Process.GetCurrentProcess.ProcessName).Length > 1 And Command$() <> CommandLine_ReStartProg And Command$() <> CommandLine_ReStartAutoUpdate Then
            Dim NumberOfProcess As Integer = 0
            Dim PathProcessCurrent As String = Diagnostics.Process.GetCurrentProcess.MainModule.FileName ' Chemin complet de l'application
            Dim NameProcessCurrent As String = Diagnostics.Process.GetCurrentProcess.ProcessName.ToUpper ' Nom application
            'Dim NameProcessCurrent As String = Diagnostics.Process.GetCurrentProcess.MainModule.ModuleName 'Nom de l'exe

            Dim Prc() As Process = Process.GetProcesses() 'on récupère la liste des processus de Windows: ATTENTION ce sont des tableaux
            For x As Integer = 0 To UBound(Prc) ' on fait toute la liste dans une boucle
                'Dim pName As String = Prc(x).ProcessName.ToUpper(New System.Globalization.CultureInfo("fr-FR", False))
                Dim pName As String = Prc(x).ProcessName.ToUpper(New System.Globalization.CultureInfo("en-US", False))
                If pName = NameProcessCurrent Then ' Cette application nous embete ?

                    Dim pPathProcess As String = Nothing

                    'Access is denied (Multi session)
                    Try
                        pPathProcess = Prc(x).MainModule.FileName
                    Catch ex As Exception
                        'MsgBox(ex.Message, MsgBoxStyle.Critical)
                    End Try

                    If pPathProcess = PathProcessCurrent Then NumberOfProcess += 1
                    If NumberOfProcess > 1 Then
                        Dim szMesssage As String = mLanguageAU.GetString(MSD_APPLICATION_OPEN) + vbNewLine + vbNewLine + mLanguageAU.GetString(MSG_CONTINUE)
                        If MsgBox(szMesssage, CType(MsgBoxStyle.Exclamation + MsgBoxStyle.YesNo + MsgBoxStyle.DefaultButton2, MsgBoxStyle), My.Application.Info.Title) = MsgBoxResult.No Then
                            'ShowWindow(CInt(Prc(x).Handle), SW_SHOW)
                            Me.Close()
                        Else
                            bAutomatic = False
                        End If
                    End If
                End If
            Next
        End If

        'Version <= 5.8.5.2
        Try
            If IO.File.Exists(Application.ExecutablePath.Replace(".exe", ".old")) Then IO.File.Delete((Application.ExecutablePath.Replace(".exe", ".old")))
        Catch ex As Exception

        End Try

        Connection = New cConnection

        'CRTL + F12
        If KeyStatus(Keys.F12) AndAlso KeyStatus(Keys.ControlKey) Then
            'If KeyStatus(Keys.ControlKey) And KeyStatus(Keys.ShiftKey) And KeyStatus(Keys.O) Then

            Dim nvResult = DialogNetWork.ShowDialog()
            If nvResult = Windows.Forms.DialogResult.OK Then
                Me.Visible = False
                RestartProg()
            ElseIf nvResult = Windows.Forms.DialogResult.Abort Then
                Application.Exit()
            End If
        End If

        Init_AUService()

        If Command$() = CommandLine_ReStartProg Then

            HttpSendMessage(CODE_UPDATE_AU_SERV, , , "Updated", FileVersionInfo.GetVersionInfo(Application.ExecutablePath).FileVersion, Application.ProductName)

        End If

        If IO.File.Exists(svLocalDir + "Info.txt") Then
            HttpSendMessage(CODE_PING)
        End If

        'PB Cancel Button
        If bCheckProcess Then
            DialogProcessKill.ShowDialog()
            Dim nvResult = DialogProcessKill.DialogResult
            If nvResult = Windows.Forms.DialogResult.No Or nvResult = Windows.Forms.DialogResult.Cancel Then
                Application.Exit()
            ElseIf nvResult = Windows.Forms.DialogResult.Yes Then
                AutorisationUpdate = False
                bAutomatic = False
            End If
        End If

        If Not IsDirectoryWritable(svLocalDir) Then
            MsgBox(mLanguageAU.GetString(MSG_ACCES_PATH_IS_DENIED).Replace("%FOLDER%", Chr(34) + svLocalDir + Chr(34)), MsgBoxStyle.Critical, My.Application.Info.Title)
            AutorisationUpdate = False
            bAutomatic = False
        End If

        If Not IsDirectoryWritable(AppPath) Then
            MsgBox(mLanguageAU.GetString(MSG_ACCES_PATH_IS_DENIED).Replace("%FOLDER%", Chr(34) + AppPath + Chr(34)), MsgBoxStyle.Critical, My.Application.Info.Title)
            AutorisationLaunchApp = False
            AutorisationUpdate = False
            bAutomatic = False
        End If

        If IsFileReadOnly(AUServiceIni) Then
            MsgBox(mLanguageAU.GetString(MSG_FILE_IS_READONLY).Replace("%FILE%", Chr(34) + AUServiceIni + Chr(34)), MsgBoxStyle.Critical, My.Application.Info.Title)
            AutorisationLaunchApp = False
            AutorisationUpdate = False
            bAutomatic = False
        End If

        If svPatchCopy <> "" Then
            If Not IsDirectoryWritable(svPatchCopy) Then
                MsgBox(mLanguageAU.GetString(MSG_ACCES_PATH_IS_DENIED).Replace("%FOLDER%", Chr(34) + svPatchCopy + Chr(34)), MsgBoxStyle.Critical, My.Application.Info.Title)
                AutorisationUpdate = False
                bAutomatic = False
            End If
        End If

        'Check Espace disque dur libre
        Dim AvailableFreeSpace As Double = My.Computer.FileSystem.GetDriveInfo(AUServPath).AvailableFreeSpace

        If FreeSpace > 0 Then
            If (FreeSpace * 1048576) > AvailableFreeSpace Then ' en Mo
                MsgBox(mLanguageAU.GetString(MSG_AVAILABLE_FREESPACE).Replace("%FREESPACE%", ConvertOctets(AvailableFreeSpace)), MsgBoxStyle.Critical, My.Application.Info.Title)
                'AutorisationLaunchApp = False
                AutorisationUpdate = False
                bAutomatic = False
            End If
        End If

        Label_Information.Text = mLanguageAU.GetString(MSG_DOWNLOAD_AND_INSTALL)

        Button_Execute.Text = mLanguageAU.GetString(BUTTON_LAUNCH)
        Button_Suivant.Text = mLanguageAU.GetString(BUTTON_UPDATE)
        Button_Fermer.Text = mLanguageAU.GetString(BUTTON_EXIT)

        'TEST fait sur la longueur du test du bouton "Mise à jour"
        Dim LenButton As Integer = Button_Suivant.Text.Length
        If LenButton > 11 Then
            Button_Suivant.Font = New System.Drawing.Font(Button_Suivant.Font.FontFamily, Button_Execute.Font.Size, System.Drawing.FontStyle.Bold)
            If LenButton > 12 Then
                Button_Suivant.Text = Button_Suivant.Text.Remove(12, LenButton - 12) + "."
            End If
        End If

        Label_Name_Maj.Visible = False
        Button_Suivant.Enabled = False
        Button_Execute.Enabled = False
        Button_Fermer.Enabled = False

        Label_Version.Text = Application.ProductVersion.ToString()

        System.Windows.Forms.Application.DoEvents()

        Me.Show()
        Me.Visible = True

        If KeyStatus(Keys.ControlKey) Then
            bAutomatic = False
        End If

        Button_Execute.Visible = bButtonExecuteVisible

        System.Windows.Forms.Application.DoEvents()

        Create_ListBoxEx()

        Dim nvGetError As Integer = 0

        InitializeInstall()
        nvGetError = Refresh_ListUpdateWeb()

        'Gestion erreur
        If nvGetError = 0 Then

        ElseIf nvGetError = -1 Then 'ACCES_MISSING
            bAutomatic = False
        ElseIf nvGetError = -2 Then 'ACCES_INVALID
            bAutomatic = False
        ElseIf nvGetError = -3 Then 'ACCES_NO_SYNCHRO

        ElseIf nvGetError = -4 Then 'NOT_INTERNET

        ElseIf idx_maj_max < 0 Then 'NOT_UPDATE

        Else 'LIST_UPDATE

        End If

        'Settings.ComboAllUpdates.Visible = (nvGetError = -3)

        LaunchQuickSupport(True)

        If StopedService Then

            Button_Execute.Enabled = False
            Button_Fermer.Enabled = False
            Button_Suivant.Enabled = False

            MsgBox(mLanguageAU.GetString(MSG_NO_LAUNCH), MsgBoxStyle.Critical, My.Application.Info.Title)

            HttpSendMessage(CODE_EXCEPTION, , , "Stoped Service", FileVersionInfo.GetVersionInfo(Application.ExecutablePath).FileVersion, Application.ProductName)

            Me.Visible = False
            Me.Close()
        End If

        Me.Activate()

        If bAutomatic And idx_maj_encours - 1 = idx_maj_max And AutorisationLaunchApp Then
            WaitTime(3)
            Excute_Application()
        Else
            Label_Name_Maj.Visible = True

            'idx_maj_encours - 1 = idx_maj_max

            If ForceUpdate Then
                Button_Execute.Enabled = AutorisationLaunchApp And idx_maj_encours - 1 = idx_maj_max
            Else
                Button_Execute.Enabled = AutorisationLaunchApp
            End If

            Button_Fermer.Enabled = True
            If bLaunchUpdateAuto And AutorisationUpdate Then
                UpdateApp()
            End If
        End If

        If bAutomaticUpdate And AutorisationUpdate Then
            UpdateApp()
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

    Private Sub lb_DoubleClick(ByVal sender As Object, ByVal e As System.EventArgs) Handles lb.DoubleClick
        If lb.SelectedIndex >= 0 Then
            Dim it As ListBoxEX.ItemEX
            it = lb.Items(lb.SelectedIndex)
            MessageBox.Show(it.User, it.Text, MessageBoxButtons.OK, MessageBoxIcon.Exclamation)
        End If
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



    Private Function Internet_Update() As Integer

        Dim fic_source As String
        Dim fic_destination As String
        Dim TabAcces() As String
        Dim ColAcces() As String
        Dim nIndex As Integer
        Dim nPos As Integer
        Dim svResult As String

        'Funtion return:
        '- 1 Pas de fichier d'accès
        '- 2 Fichier d’accès erroné
        '- 3 Problème de Synchronisation
        '- 4 Pas de connexion internet


        'Connection à internet
        Label_Etat_Maj.Text = mLanguageAU.GetString(MSG_CONNECTING)
        System.Windows.Forms.Application.DoEvents()

        If Not Connection.DelegateOpenInternet Then
            Label_Etat_Maj.Text = mLanguageAU.GetString(MSG_DECONNECTED)
            Connection.Disconnect()
            idx_maj_max = -1
            Return -4 ' Pas de connexion internet
            'Exit Function
        End If

        Label_Etat_Maj.Text = mLanguageAU.GetString(MSG_CONNECTED)
        System.Windows.Forms.Application.DoEvents()

        tot_taille = 0

        If FileAcces = Nothing Then
            Connection.RemoteFile = svFileMaj
            FileAcces = Connection.DelegateGetFileToString()
        End If

        System.Windows.Forms.Application.DoEvents()

        If FileAcces = "" Then
            bAutomatic = False
            idx_maj_max = -1
            Return -1 ' Pas de fichier d'accès
            'Exit Function
        End If

        If InStr(FileAcces, EndOfFile) = 0 And InStr(FileAcces, EndOfFileOld) = 0 And InStr(FileAcces, EndOfFilePlantage) = 0 Then
            idx_maj_max = -1
            Return -2 ' Fichier d’accès erroné
            'Exit Function
        End If

        '************** A MODIFIER POUR LES RUSSES *****************

        TabAcces = Split(FileAcces, vbNewLine)
        Dim TabListAcces As ArrayList = New ArrayList
        Dim bStartUpdate As Boolean = False
        Dim bStartFilter As Boolean = False

        'svFilter = "Wave"
        'svUpdate = "Version1337e.exe"

        For Each s In TabAcces

            If s = EndOfFile Or s = EndOfFileOld Or s = EndOfFilePlantage Or s = Nothing Then Exit For
            Dim sPathInfo() As String = Split(s, ",")

            Settings.ComboAllUpdates.Items.Add(sPathInfo(1))

            If Not bStartUpdate Then
                If svPatch <> "" Then
                    If sPathInfo(1) = svPatch Then
                        If svFilter <> "" Or svUpdate <> "" Then
                            nvStartUpdate = CInt(sPathInfo(0))
                        End If
                        bStartUpdate = True
                    End If
                Else
                    bStartUpdate = True
                End If
            End If

            If Not bStartFilter Then
                If svUpdate <> "" Then
                    If sPathInfo(1) = svUpdate Then
                        bStartFilter = True
                    End If
                Else
                    bStartFilter = True
                End If
            End If

            If (InStr(sPathInfo(1), svFilter, CompareMethod.Text) = 1 And bStartFilter And svFilter <> "") Or bStartUpdate Then
                If sPathInfo(1) <> svPatch And sPathInfo(1) <> svUpdate Then
                    TabListAcces.Add(s)
                End If
            End If
        Next

        If svPatch <> "" And Not bStartUpdate Then
            bAutomatic = False
            idx_maj_max = -1
            TabListAcces.Clear()
            Return -3
        End If

        TabListAcces.Add(EndOfFile)

        Dim TabIndex As Integer = 0
        ReDim TabAcces(TabListAcces.Count)
        For Each s In TabListAcces
            TabAcces(TabIndex) = s.ToString
            TabIndex = TabIndex + 1
        Next

        System.Windows.Forms.Application.DoEvents()

        ReDim File(0)

        On Error Resume Next

        For Each fic_source In TabAcces

            If fic_source = EndOfFile Or fic_source = EndOfFileOld Or fic_source = EndOfFilePlantage Then
                nIndex = nIndex - 1
                Exit For
            End If

            If ((fic_source.Contains(AdmUser) And bAdmUser = True) Or Not fic_source.Contains(AdmUser)) And Not fic_source.Contains(Skip) Then

                If fic_source.Contains(AdmUser) Then
                    fic_source = fic_source.Replace(AdmUser, "")
                End If

                If fic_source.Contains(Skip) Then
                    fic_source = fic_source.Replace(Skip, "")
                End If

                System.Windows.Forms.Application.DoEvents()

                ReDim ColAcces(4)
                ColAcces = Split(fic_source, ",", 5)

                System.Windows.Forms.Application.DoEvents()

                File(nIndex).numMaj0 = ColAcces(0) 'numMaj
                File(nIndex).fic_source1 = ColAcces(1) 'fic_source
                File(nIndex).taille2 = ColAcces(2) 'taille
                File(nIndex).CheckSum3 = ColAcces(3) 'CheckSum

                System.Windows.Forms.Application.DoEvents()

                ' *********************************************
                ' * Retour à la ligne si ligne supp à 70 char *
                ' *********************************************
                Dim nvSplitLine As Integer = 70
                Dim nPosSpace As Integer = 0
                Dim nvLenString As Integer = 0
                Dim nvCountLine As Integer = 0
                Dim TabString() As String = Split(ColAcces(4), "\r\n")
                Dim nvNumLine As Integer = TabString.Length - 1
                ReDim Preserve TabString(nvNumLine)

                While nvNumLine >= nvCountLine

                    System.Windows.Forms.Application.DoEvents()

                    nvLenString = Len(TabString(nvCountLine))
                    If nvLenString > nvSplitLine Then
                        nPosSpace = InStrRev(TabString(nvCountLine), " ", nvSplitLine, CompareMethod.Text)
                        TabString(nvCountLine) = Mid(TabString(nvCountLine), 1, nPosSpace - 1) + "\r\n" + Mid(TabString(nvCountLine), nPosSpace + 1, nvLenString - nPosSpace)
                        ColAcces(4) = Join(TabString, "\r\n")
                        TabString = Split(ColAcces(4), "\r\n")
                        nvNumLine = TabString.Length - 1
                        ReDim Preserve TabString(nvNumLine)
                    End If
                    nvCountLine = nvCountLine + 1
                End While
                ' *********************************************

                System.Windows.Forms.Application.DoEvents()

                ' Ajout le 24/01/2013
                Dim nPosCrLf As Integer = InStrRev(ColAcces(4), "\r\n") - 1
                If nPosCrLf = (ColAcces(4).Length - 4) Then
                    ColAcces(4) = Mid(ColAcces(4), 1, nPosCrLf)
                End If

                File(nIndex).infoMaj4 = Replace(ColAcces(4), "\r\n", vbNewLine, , , CompareMethod.Text) 'ColAcces(4) 'infoMaj
                File(nIndex).fic_destination5 = svCodeAppli & VB.Right(("0000" + Trim(ColAcces(0))), 4) & "_" & ColAcces(1) 'fic_destination

                System.Windows.Forms.Application.DoEvents()


                '**************  COMPRENDS PAS !!! ************************
                'Replace(ColAcces(4), "\r\n", vbNewLine, , , CompareMethod.Text)

                tot_taille = CLng(tot_taille + Val(ColAcces(2)))

                'Modif limite info
                Dim a() As String = Split(File(nIndex).infoMaj4, vbNewLine)
                If a.Length > 11 Then
                    ReDim Preserve a(7) 'Limite 11
                    a(7) = "etc..."
                    File(nIndex).infoMaj4 = Join(a, vbNewLine)
                End If

                If (CBool(IIf(svRemoteDir <> Nothing, IO.File.Exists(svRemoteDir + File(nIndex).fic_destination5), False)) Or IO.File.Exists(svLocalDir + File(nIndex).fic_destination5)) Then
                    lb.Add(File(nIndex).infoMaj4, mLanguageAU.GetString(MSG_UPDATE_NUMBER) & " " & File(nIndex).numMaj0, 4)
                Else
                    lb.Add(File(nIndex).infoMaj4, mLanguageAU.GetString(MSG_UPDATE_NUMBER) & " " & File(nIndex).numMaj0, 5)
                End If

                nIndex = nIndex + 1

                ReDim Preserve File(nIndex)

            End If

            System.Windows.Forms.Application.DoEvents()

        Next

        On Error GoTo 0

        idx_maj_max = CShort(nIndex)

        Return 0

    End Function

    Public Function GetAppPath(ByVal EndWithBackSlach As Boolean) As String
        Dim p As String
        ' Recupere le chemin complet avec le nom de l'executable
        p = Application.ExecutablePath

        'Attention l'objet application n'existe que sur les projets Win Forms sinon, il faut remplacer 
        'p = Application.ExecutablePath
        'par
        ' p = System.Reflection.Assembly.GetExecutingAssembly.Location.ToString()

        ' Extrait la partie repertoire du chemin complet
        If (EndWithBackSlach) Then
            Return p.Substring(0, p.LastIndexOf("\") + 1)
        Else
            Return p.Substring(0, p.LastIndexOf("\"))
        End If
    End Function

    Public Function GetAppPath() As String
        Return (GetAppPath(True))
    End Function

    Private Sub Init_AUService()

        ' Définition du dossier de démarrage
        AppPath = Application.StartupPath & "\"
        'AppPath = "C:\GRM\ARTRADIO\"

        ' Définition d'un chemin de fichier pour d'autre extension
        AUServPath = ParsePath(Application.ExecutablePath, PATH_FILENAME_ONLY)

        ' Fichier INI de paramétrages
        AUServiceIni = AUServPath & ".ini"
        If IO.File.Exists(AUServiceIni) = False Then Me.Close()

        'Ajout de la version dans l'ini
        Dim AppBuild As String = Select_GetIniString("Parametres", "AppBuild", AUServiceIni)
        Dim AppBuildCurrent As String = My.Application.Info.Version.ToString
        If AppBuild <> AppBuildCurrent Then
            AppBuild = AppBuildCurrent
            Select_WriteIniString("Parametres", "AppBuild", AppBuild, AUServiceIni)
        End If

        Dim AppVersion As String = Select_GetIniString("Parametres", "AppVersion", AUServiceIni)
        Dim AppVersionCurrent As String = FileVersionInfo.GetVersionInfo(Application.ExecutablePath).FileVersion
        If AppVersion <> AppVersionCurrent Then
            AppVersion = AppVersionCurrent
            Select_WriteIniString("Parametres", "AppVersion", AppVersion, AUServiceIni)
        End If

        ' Choix de la langue
        Dim svLanguage As String = UCase(Select_GetIniString("Parametres", "Language", AUServiceIni))
        If svLanguage = "EN" Then
            SetCurrentLanguage(0)
        ElseIf svLanguage = "FR" Then
            SetCurrentLanguage(1)
        ElseIf svLanguage = "IT" Then
            SetCurrentLanguage(2)
        ElseIf svLanguage = "PL" Then
            SetCurrentLanguage(3)
        Else 'Default FR
            SetCurrentLanguage(1)
        End If

        ' Définition des dossiers 
        svPatch = Select_GetIniString("Parametres", "Patch", AUServiceIni)
        svRemoteDir = Select_GetIniString("Parametres", "RemoteDir", AUServiceIni)
        svLocalDir = Select_GetIniString("Parametres", "LocalDir", AUServiceIni)
        svPatchCopy = Select_GetIniString("Parametres", "PatchCopy", AUServiceIni)
        bRemovePatch = Not (Select_GetIniString$("Parametres", "RemovePatch", AUServiceIni$) = "0")

        Dim svLogUpdate As String = Select_GetIniString("Parametres", "LogUpdate", AUServiceIni).Trim
        If svLogUpdate = Nothing Then
            Select_WriteIniString("Parametres", "LogUpdate", "1", AUServiceIni)
        End If

        Dim svCheckProcess As String = Select_GetIniString("Parametres", "CheckProcess", AUServiceIni).Trim
        If svCheckProcess = Nothing Then
            Select_WriteIniString("Parametres", "CheckProcess", "1", AUServiceIni)
            bCheckProcess = True
        Else
            bCheckProcess = (svCheckProcess = "1")
        End If

        ExcludeSubDir = Select_GetIniString("Parametres", "ExcludeSubDir", AUServiceIni).Trim
        If ExcludeSubDir <> Nothing Then
            ExcludeSubDir = AppPath + ExcludeSubDir
        End If

        'Admin user
        bAdmUser = (Select_GetIniString$("Parametres", "AdmUser", AUServiceIni$) = "1")

        ' Code de l'application
        svCodeAppli = Trim(Select_GetIniString("Parametres", "CodeAppli", AUServiceIni))
        svNameAppli = Trim(Select_GetIniString("Parametres", "NameAppli", AUServiceIni))

        ' N'envoie pas de message sur le serveur de JFC
        bSendRequest = Not (Select_GetIniString("Parametres", "SendRequest", AUServiceIni) = "0")

        'Modif QuickSupport du 09/10/2018
        bQuickSupport = (StrComp(Select_GetIniString("Parametres", "QuickSupport", AUServiceIni), "Disable", CompareMethod.Text) <> 0)

        ' N'exécute pas les mise à jour
        bOnlyDownload = (Select_GetIniString$("Parametres", "OnlyDownload", AUServiceIni) = "1")

        ' Durée d'attente avant fermeture de la boite de dialogue au lancement de l'application
        Dim sWaitClose = Trim(Select_GetIniString("Parametres", "WaitClose", AUServiceIni))
        If sWaitClose = "" Then
            nWaitClose = 4
        Else
            nWaitClose = CInt(sWaitClose)
        End If

        ' Définition de l'application à lancer avec option d'attente
        ExecuteFile = Select_GetIniString("Parametres", "Execute", AUServiceIni)

        ' Mode d'exécution de la l'application
        bUseShellExecute = (Select_GetIniString("Parametres", "UseShellExecute", AUServiceIni) = "1")
        bUseProcessExecute = Not (Select_GetIniString("Parametres", "UseProcessExecute", AUServiceIni) = "0")

        ' Option WAIT au lancement de l'application
        If ExecuteFile <> Nothing Then

            Dim svExecuteWait() As String = Split(ExecuteFile, ",")

            If svExecuteWait.Count = 2 Then
                ExecuteWait = (Strings.StrComp(svExecuteWait(1).Trim, "WAIT", CompareMethod.Text) = 0)
                ExecuteFile = svExecuteWait(0).Trim
            End If

            Dim svExecuteCmd() As String = Split(ExecuteFile.Trim, ".exe ")
            If svExecuteCmd.Count = 2 Then
                ExecuteCmdLine = svExecuteCmd(1).Trim
                ExecuteFile = svExecuteCmd(0) + ".exe"
            End If

            ExecuteFile = Add(ExecuteFile, PATH, AppPath)

        Else
            ExecuteWait = False
            ExecuteCmdLine = ""
        End If

        ActiveExe(ExecuteFile, False)

        ' Bouton de lancement de l'application visible si l'application existe
        bButtonExecuteVisible = IO.File.Exists(ExecuteFile)

        ' Lance l'application automatiquement après les mises à jour
        If bAutomatic = True Then
            bAutomatic = bButtonExecuteVisible And Not (Select_GetIniString$("Parametres", "Automatic", AUServiceIni) = "0")
        End If

        ' Lance les mises à jour automatiquement
        bAutomaticUpdate = (Select_GetIniString$("Parametres", "AutomaticUpdate", AUServiceIni) = "1")

        ' -------------------------------------------------
        ' Exemple d'utilisation dans un projet :
        ' -------------------------------------------------
        'Dim OS As String = My.Computer.Info.OSFullName
        'Dim NameComputer As String = My.Computer.Name
        'Dim PlatformOS As String = My.Computer.Info.OSPlatform
        'Dim VersionOS As String = My.Computer.Info.OSVersion
        'Dim NameUser As String = My.User.Name

        HostEntry = Trim(Select_GetIniString("Parametres", "Name", AUServiceIni))

        Try
            HostEntryName = Dns.GetHostEntry(Dns.GetHostName).HostName
        Catch ex As Exception
            HostEntryName = My.Computer.Name
        End Try

        ' Nom long de la machine
        If HostEntry = "" Then
            HostEntry = HostEntryName
        End If

        ' Nom court de la machine
        If HostName = "" Then
            Try
                HostName = Dns.GetHostName
            Catch ex As Exception
                HostName = My.Computer.Name
            End Try
        End If

        ' Nom de la société qui sert au téléchargement des MAJ avec conversion pour respecter le code HTTP
        svCompanyName = Trim(Select_GetIniString("Parametres", "Company", AUServiceIni))
        svWebCompanyName = Url_Encode(svCompanyName)

        ' Nom de la fenêtre
        Me.Text = "AutoUpdate Services - " & svNameAppli & " - " & svCompanyName

        ' Calcul et récupération des clés de contrôle
        svSerial = Select_GetIniString("Parametres", "Serial", AUServiceIni)
        Dim CurrentMacAddress As String = Nothing
        If svSerial.Length >= 12 Then CurrentMacAddress = VB.Right(svSerial, 12)

        Dim svCle As String = Select_GetIniString("Parametres", "Cle", AUServiceIni)
        Dim svSerialVirtual As String = Select_GetIniString("Parametres", "SerialVirtual", AUServiceIni)
        Dim svSerialGroup As String = Select_GetIniString("Parametres", "SerialGroup", AUServiceIni)
        MacAddress.bVirtual = (svSerialVirtual <> "") Or (svSerialGroup <> "")
        MacAddress.bDebug = (Select_GetIniString("Parametres", "DebugNetwork", AUServiceIni) = "1")

        Dim GetMacAddress As String = MacAddress.GetValue(CurrentMacAddress)
        Dim GetHddSerial As String = HddSerial()
        svSerialFound = GetHddSerial & GetMacAddress

        'Modif du 21/09/2016
        If CurrentMacAddress = "000000000000" And CurrentMacAddress <> GetMacAddress Then
            If InStr(svSerial, GetHddSerial, CompareMethod.Text) = 1 Then
                Select_WriteIniString("Parametres", "Serial", svSerialFound, AUServiceIni)
                svSerial = svSerialFound
            End If
        End If

        Dim svCryptage As String = AppendString(Crypter(svCompanyName, "sha1"))
        Dim svCleFound As String = AppendString(Crypter(svCompanyName, "sha512"))
        svSerie = Select_GetIniString("Parametres", "ProductCode", AUServiceIni)

        svFilter = Select_GetIniString("Parametres", "Filter", AUServiceIni)
        svUpdate = Select_GetIniString("Parametres", "Update", AUServiceIni)

        bCopyBeforeExecute = (Select_GetIniString("Parametres", "CopyBeforeExecute", AUServiceIni).Trim = "1")

        ' Initialize Install
        If Command$() = CommandLine_InitializeInstall Then
            Select_WriteIniString("Parametres", "Cle", svCleFound, AUServiceIni)
            Select_WriteIniString("Parametres", "Serial", svSerialFound, AUServiceIni)
            Me.Close()
        ElseIf UCase(Command$()) = CommandLine_ReStartAutoUpdate Then
            bLaunchUpdateAuto = True
        End If

        ' Création du fichier d'accès
        svFileMaj = svWebCompanyName & "." & svCryptage

        ' Définition du Proxy
        Dim svProxyLogin As String = ""
        Dim svProxyPassword As String = ""
        Dim svProxy As String = Select_GetIniString("Parametres", "Proxy", AUServiceIni)
        If svProxy <> "" Then
            Dim nPosProxy As Short = CShort(InStr(svProxy, "@"))
            If nPosProxy > 0 Then
                Dim nPosPwd As Short = CShort(InStr(svProxy, ":"))
                svProxyLogin = Mid(svProxy, 1, nPosPwd - 1)
                svProxyPassword = Mid(svProxy, nPosPwd + 1, nPosProxy - nPosPwd - 1)
                svProxy = Mid(svProxy, nPosProxy + 1, Len(svProxy) - nPosProxy)
            End If
            svProxy = "http=http://" & svProxy
        End If

        ' Définition de la taille du buffer
        Dim nBuffer As Integer = CInt(Val(Select_GetIniString("Parametres", "Buffer", AUServiceIni)))
        If nBuffer = 0 Then nBuffer = 8

        ' Définition des paramètres de connexion
        With Connection
            .Proxy = svProxy
            .ProxyLog = svProxyLogin
            .ProxyPwd = svProxyPassword
            .DirectConnect = (Select_GetIniString("Parametres", "DirectConnect", AUServiceIni) = "1")
            .InternetBuffer = nBuffer
            .DebugMode = (Select_GetIniString("Parametres", "DebugMode", AUServiceIni) = "1")
            .InternetFlagsMask = CInt(Val(Select_GetIniString("Parametres", "InternetFlagsMask", AUServiceIni)))
            .RemoteDir = Connection.GetRemoteDir
        End With

        ' Ajout un chemin au dossier si nécessaire
        If Mid(svLocalDir, 2, 1) <> ":" And Mid(svLocalDir, 2, 1) <> "\" Then svLocalDir = AppPath + svLocalDir

        ' Ajout de backslah a la fin du dossier si nécessaire
        svLocalDir = Add(svLocalDir, BACKSLASH)

        'Update AU-Serv
        If Connection.GetNewUpdate(AppVersion, (Select_GetIniString("Parametres", "TestUpdate", AUServiceIni) = "1")) Then

            RestartProg()

        End If

        'Id User
        Try
            UserName = My.User.Name
            Dim nPosSlash As Integer = InStr(UserName, "\")
            If nPosSlash > 0 Then
                UserName = Mid(UserName, nPosSlash + 1, Len(UserName) - nPosSlash)
            End If
        Catch ex As Exception
            svId = Select_GetIniString("Parametres", "Id", AUServiceIni)
            If svId <> "" Then
                UserName = svId
            End If
        End Try

        ' Définition du serveur de mise à jour
        Dim svServerName As String = Select_GetIniString("Parametres", "ServerName", AUServiceIni)
        If svServerName <> Nothing Then Connection.ServerName = svServerName

        If MacAddress.bVirtual Then
            Select_WriteIniString("Parametres", "Id", UserName, AUServiceIni)
            HostEntry = HostName + "." + UserName
        End If

        ' Crée "LocalDir"
        If Not IO.Directory.Exists(svLocalDir) Then
            If VB.Left(svLocalDir, 2) <> "\\" Then
                IO.Directory.CreateDirectory(svLocalDir)
            End If
            If Not IO.Directory.Exists(svLocalDir) Then
                MsgBox(mLanguageAU.GetString(MSG_PATH_NOT_FOUND) & vbNewLine & vbNewLine & svLocalDir, MsgBoxStyle.Critical And MsgBoxStyle.SystemModal, My.Application.Info.Title)
                Me.Close()
            End If
        End If

        'Demande Product Code si manquant
        If svSerie = "" And svSerialVirtual = "" And svSerialGroup = "" Then

DlgProductCode:
            DialogProductCode.ShowDialog()
            If DialogProductCode.DialogResult = Windows.Forms.DialogResult.Cancel Then
                Select_WriteIniString("Parametres", "Serial", "", AUServiceIni)
                If svSerial = "" Then
                    svSerial = svSerialFound
                End If
                HttpSendMessage(CODE_SERIAL_MISSING)

                Me.Close()

            Else

                svSerie = DialogProductCode.svSerie

                Dim svCompanyNameInstall As String = Connection.GetAutorisationInstall(svSerialFound, svSerie)

                If svCompanyName = "" Or svCompanyNameInstall <> svCompanyName Then

                    Try
                        Dim nvCodeAutorisation As Integer = CInt(svCompanyNameInstall)

                        If svCompanyName = "" Then

                            'DialogProductCode.TextProductCode.BackColor = Color.Red

                            If nvCodeAutorisation = -7 Then
                                ' PB CConnection internet
                                If MsgBox(mLanguageAU.GetString(MSG_FAIL_CONNECT_INTERNET) + " (Exception: " + Str(nvCodeAutorisation).Trim + ")" + vbNewLine + vbNewLine + mLanguageAU.GetString(MSG_CHANGE_SETTINGS), MsgBoxStyle.Critical Or MsgBoxStyle.YesNo Or MsgBoxStyle.DefaultButton2, My.Application.Info.Title & " : " & svSerie) = MsgBoxResult.Yes Then

                                    If SettingsDialog() = Windows.Forms.DialogResult.Cancel Then
                                        Me.Close()
                                    End If

                                End If
                            End If

                            MsgBox(mLanguageAU.GetString(MSG_PRODUCTCODE_INVALID) + " (Exception: " + Str(nvCodeAutorisation).Trim + ")", MsgBoxStyle.Critical, My.Application.Info.Title & " : " & svSerie)

                            GoTo DlgProductCode

                        End If


                    Catch ex As Exception

                        svCompanyName = svCompanyNameInstall

                        svCryptage = AppendString(Crypter(svCompanyName, "sha1"))
                        svCleFound = AppendString(Crypter(svCompanyName, "sha512"))

                        Select_WriteIniString("Parametres", "Cle", svCleFound, AUServiceIni)
                        Select_WriteIniString("Parametres", "Company", svCompanyName, AUServiceIni)
                        Select_WriteIniString("Parametres", "ProductCode", svSerie, AUServiceIni)
                        Select_WriteIniString("Parametres", "Serial", svSerialFound, AUServiceIni)

                        svSerial = svSerialFound

                        svWebCompanyName = Url_Encode(svCompanyName)
                        svFileMaj = svWebCompanyName & "." & svCryptage

                        ' Nom de la fenêtre
                        Me.Text = "AutoUpdate Services - " & svNameAppli & " - " & svCompanyName

                        HttpSendMessage(CODE_INSTALLATION, , svSerie, "Install")

                    End Try

                Else

                    Select_WriteIniString("Parametres", "ProductCode", svSerie, AUServiceIni)
                    'Select_WriteIniString("Parametres", "Serial", svSerialFound, AUServiceIni)

                    HttpSendMessage(CODE_INSTALLATION, , svSerie, "Install")


                End If

                svCle = svCleFound

            End If

        ElseIf svSerie = "" Then
            MsgBox(mLanguageAU.GetString(MSG_PRODUCTCODE_MISSING), MsgBoxStyle.Critical, My.Application.Info.Title & " : " & svSerialFound)
            HttpSendMessage(CODE_SERIAL_MISSING)
            Me.Close()

        Else
            svSerie = svSerie.Replace("-", "")

        End If

        'Simul Install
        'Dim svSociete As String = Connection.GetSociete(svSerialFound, svSerie, svCompanyName)

        Dim svSerialChecksum As String = ""
        Dim nvCode As Integer = 0

        If svSerialVirtual <> "" Then
            nvCode = Connection.GetAutorisation(svSerialVirtual, svSerie, svWebCompanyName)
            svSerialChecksum = svSerialVirtual
            bReturnLicence = False
        ElseIf svSerialGroup <> "" Then
            nvCode = Connection.GetAutorisation(svSerialGroup, svSerie, svWebCompanyName)
            svSerialChecksum = svSerialGroup
            bReturnLicence = False
        Else
            nvCode = Connection.GetAutorisation(svSerialFound, svSerie, svWebCompanyName)
            svSerialChecksum = svSerialFound
            bReturnLicence = True
        End If

        'Checksum
        Dim ChecksumToday As String = Crypter(Date.Now.ToShortDateString + svSerialChecksum, "sha1")
        Dim Checksum As String = Select_GetIniString("Parametres", "Checksum", AUServiceIni)

        'ChecksumValidate(Checksum, svSerialChecksum) 'Pour test

        If nvCode = 1 Then

            If svSerialGroup <> "" And svSerial <> "" And svSerial <> svSerialFound Then
                MsgBox(mLanguageAU.GetString(MSG_INVALID_PARAMETERS) + " (Exception: " + Str(nvCode).Trim + ")", MsgBoxStyle.Critical, My.Application.Info.Title & " : " & svSerie)
                HttpSendMessage(CODE_INVALID_PARAMETERS, , svSerie, "Parameter")
                Select_WriteIniString("Parametres", "Serial", "", AUServiceIni)
                Me.Close()
            End If

            If svSerial = "" Or svSerialVirtual <> "" Then

                svSerial = svSerialFound

                If svSerial = "" Then
                    HttpSendMessage(CODE_INITAILIZE_INSTALL, , svSerie, "Install")
                End If

                Select_WriteIniString("Parametres", "Serial", svSerial, AUServiceIni)
                If Select_GetIniString("Parametres", "Serial", AUServiceIni) <> svSerial Then
                    MsgBox(mLanguageAU.GetString(MSG_INITIALIZING_ERROR) + " (Exception: " + Str(nvCode).Trim + ")", MsgBoxStyle.Critical, My.Application.Info.Title)
                    Me.Close()
                End If

            End If

            If Checksum <> ChecksumToday Then
                Select_WriteIniString("Parametres", "Checksum", ChecksumToday, AUServiceIni)
            End If

        ElseIf nvCode < 0 Then ' ce n’est pas bon + Connection internet Ok

            If nvCode = -1 Then ' le numéro de série et le hardware code ne correspondent pas


                ' Modif du 22/01/2013
                ' Clé déjà utilisé
                If svCle = "" Then
                    Select_WriteIniString("Parametres", "ProductCode", "", AUServiceIni)
                    svSerie = ""
                    GoTo DlgProductCode
                End If

                Select_WriteIniString("Parametres", "Serial", "", AUServiceIni)
                MsgBox(mLanguageAU.GetString(MSG_INVALID_PARAMETERS) + " (Exception: " + Str(nvCode).Trim + ")", MsgBoxStyle.Critical, My.Application.Info.Title & " : " & svSerie)

                If svSerial = "" Then
                    svSerial = svSerialFound
                End If
                HttpSendMessage(CODE_INVALID_PARAMETERS, , svSerie, "Parameter")
                Me.Close()

            ElseIf nvCode = -2 Then 'poste bloqué
                Select_WriteIniString("Parametres", "Serial", "", AUServiceIni)
                MsgBox(mLanguageAU.GetString(MSG_LICENSE_BLOCKED) + " (Exception: " + Str(nvCode).Trim + ")", MsgBoxStyle.Critical, My.Application.Info.Title & " : " & svSerie)
                If svSerial = "" Then
                    svSerial = svSerialFound
                End If
                HttpSendMessage(CODE_LICENSE_LOCKED, , svSerie, "Lock")
                Me.Close()

            ElseIf nvCode = -3 Then 'poste supprimé
                Select_WriteIniString("Parametres", "Cle", "", AUServiceIni)
                Select_WriteIniString("Parametres", "Serial", "", AUServiceIni)
                Select_WriteIniString("Parametres", "ProductCode", "", AUServiceIni)
                MsgBox(mLanguageAU.GetString(MSG_LICENSE_EXPIRED) + " (Exception: " + Str(nvCode).Trim + ")", MsgBoxStyle.Critical, My.Application.Info.Title & " : " & svSerie)
                If svSerial = "" Then
                    svSerial = svSerialFound
                End If
                HttpSendMessage(CODE_LICENSE_EXPIRED, , svSerie, "Expire")
                Me.Close()

            ElseIf nvCode = -4 Then 'le numéro de série n’existe pas
                Select_WriteIniString("Parametres", "Serial", "", AUServiceIni)
                Select_WriteIniString("Parametres", "ProductCode", "", AUServiceIni)
                MsgBox(mLanguageAU.GetString(MSG_PRODUCTCODE_INVALID) + " (Exception: " + Str(nvCode).Trim + ")", MsgBoxStyle.Critical, My.Application.Info.Title & " : " & svSerie)
                If svSerial = "" Then
                    svSerial = svSerialFound
                End If
                HttpSendMessage(CODE_INVALID_SERIAL, , svSerie, "Unknown")
                Me.Close()

            ElseIf nvCode = -5 Then 'Reset Hardware
                If svCle = "" Then
                    svCle = svCleFound
                    Select_WriteIniString("Parametres", "Cle", svCle, AUServiceIni)

                    If Select_GetIniString("Parametres", "Cle", AUServiceIni) <> svCle Then
                        MsgBox(mLanguageAU.GetString(MSG_INITIALIZING_ERROR) + " (Exception: " + Str(nvCode).Trim + ")", MsgBoxStyle.Critical, My.Application.Info.Title)
                        Me.Close()
                    End If

                End If
                Select_WriteIniString("Parametres", "Serial", svSerialFound, AUServiceIni)

                If Select_GetIniString("Parametres", "Serial", AUServiceIni) <> svSerialFound Then
                    MsgBox(mLanguageAU.GetString(MSG_INITIALIZING_ERROR) + " (Exception: " + Str(nvCode).Trim + ")", MsgBoxStyle.Critical, My.Application.Info.Title)
                    Me.Close()
                End If

                If Checksum <> ChecksumToday Then
                    Select_WriteIniString("Parametres", "Checksum", ChecksumToday, AUServiceIni)
                End If

                svSerial = svSerialFound
                HttpSendMessage(CODE_RESET_SERIAL, , svSerie, "Reset")
                'Select_WriteIniString("Parametres", "Cle", svCleFound, AUServiceIni)

            ElseIf nvCode = -6 Then 'Incorrect CompanyName/ProductCode
                Select_WriteIniString("Parametres", "ProductCode", "", AUServiceIni)
                Select_WriteIniString("Parametres", "Serial", "", AUServiceIni)

                Dim svMsg As String = mLanguageAU.GetString(MSG_INVALID_COMPANY_NAME) + " (Exception: " + Str(nvCode).Trim + ")"
                svMsg = svMsg.Replace("#COMPANY#", Chr(34) + svCompanyName + Chr(34))
                MsgBox(svMsg, MsgBoxStyle.Critical, My.Application.Info.Title & " : " & svSerie)
                If svSerial = "" Then
                    svSerial = svSerialFound
                End If
                HttpSendMessage(CODE_INCORRECT_LICENCE_COMPANY, , svSerie, "Incorrect")
                Me.Close()

            ElseIf nvCode = -7 Then 'Incorrect parameter
                Select_WriteIniString("Parametres", "Serial", "", AUServiceIni)
                MsgBox(mLanguageAU.GetString(MSC_INCORRECT_PARAMETER) + " (Exception: " + Str(nvCode).Trim + ")", MsgBoxStyle.Critical, My.Application.Info.Title & " : " & svSerie)
                HttpSendMessage(CODE_INCORRECT_PARAMETERS, , svSerie, "Incorrect")
                Me.Close()

            End If

        ElseIf nvCode = 0 Then

            If Not ChecksumValidate(Checksum, svSerialChecksum) Then

                ' PB CConnection internet
                If MsgBox(mLanguageAU.GetString(MSG_FAIL_CONNECT_INTERNET) + " (Exception: " + Str(nvCode).Trim + ")" + vbNewLine + vbNewLine + mLanguageAU.GetString(MSG_CHANGE_SETTINGS), MsgBoxStyle.Critical Or MsgBoxStyle.YesNo Or MsgBoxStyle.DefaultButton2, My.Application.Info.Title & " : " & svSerie) = MsgBoxResult.Yes Then

                    If SettingsDialog() = Windows.Forms.DialogResult.Cancel Then
                        Me.Close()
                    End If

                Else
                    Me.Close()
                End If

            End If

        End If

        ' Vérification de la clé et du numéro de série
        If svSerialFound <> svSerial Then
            MsgBox(mLanguageAU.GetString(MSG_SERIAL_INCORRECT), MsgBoxStyle.Critical, My.Application.Info.Title & " : " & svSerial)
            svSerial = svSerialFound
            HttpSendMessage(CODE_SERIAL_INCORRECT, , svSerie, "Incorrect")
            Me.Close()
        ElseIf svCleFound <> svCle Then ' And svCle <> "" Then
            MsgBox(mLanguageAU.GetString(MSG_KEY_INVALID), MsgBoxStyle.Critical, My.Application.Info.Title & " : " & svCle)
            HttpSendMessage(CODE_KEY_INVALID, , svCle, "key")
            Me.Close()
        End If

        'Validation de Product Code
        If Select_GetIniString("Parametres", "ProductCode", AUServiceIni) = "" Then
            Select_WriteIniString("Parametres", "ProductCode", svSerie, AUServiceIni)

            If Select_GetIniString("Parametres", "ProductCode", AUServiceIni) <> svSerie Then
                MsgBox(mLanguageAU.GetString(MSG_INITIALIZING_ERROR) + " (ProductCode)", MsgBoxStyle.Critical, My.Application.Info.Title)
                Me.Close()
            End If

            'Validation de la Cle
            If Select_GetIniString("Parametres", "Cle", AUServiceIni) = "" Then
                Select_WriteIniString("Parametres", "Cle", svCleFound, AUServiceIni)

                If Select_GetIniString("Parametres", "Cle", AUServiceIni) <> svCleFound Then
                    MsgBox(mLanguageAU.GetString(MSG_INITIALIZING_ERROR) + " (Cle)", MsgBoxStyle.Critical, My.Application.Info.Title)
                    Me.Close()
                End If

            End If
        End If

        Try
            FreeSpace = CInt(Select_GetIniString("Parametres", "FreeSpace", AUServiceIni))
        Catch ex As Exception
            FreeSpace = 0
        End Try

        If MacAddress.bVirtual Then
            If Connection.GetLisenseAutorisation(svCompanyName, HostEntry) = False Then
                HttpSendMessage(CODE_LICENSE_NOT_RECOGNIZED, , CStr(IIf(HostEntry.Contains(UserName), "", UserName)))
                Select_WriteIniString("Parametres", "Serial", "", AUServiceIni)
                Select_WriteIniString("Parametres", "Checksum", "", AUServiceIni)
                MsgBox(mLanguageAU.GetString(MSG_LICENSE_NOT_RECOGNIZED), MsgBoxStyle.Critical, My.Application.Info.Title & " : " & HostEntry)
                Me.Close()
            End If
        End If

        ' Ajout de backslah a la fin du dossier si nécessaire
        If svRemoteDir.Contains("/") Then
            svRemoteDir = Nothing
        ElseIf svRemoteDir <> Nothing Then

            svRemoteDir = Add(svRemoteDir, BACKSLASH)

            ' Crée "RemoteDir"
            If Not IO.Directory.Exists(svRemoteDir) Then
                If VB.Left(svRemoteDir, 2) <> "\\" Then
                    IO.Directory.CreateDirectory(svRemoteDir)
                End If
                If Not IO.Directory.Exists(svRemoteDir) Then
                    MsgBox(mLanguageAU.GetString(MSG_PATH_NOT_FOUND) & vbNewLine & vbNewLine & svRemoteDir, MsgBoxStyle.Critical And MsgBoxStyle.SystemModal, My.Application.Info.Title)
                    Me.Close()
                End If
            End If

        End If


        If svPatchCopy <> "" Then

            ' Ajout de backslah a la fin du dossier si nécessaire
            svPatchCopy = Add(svPatchCopy, BACKSLASH)


            ' Crée "PatchCopy"
            If Not IO.Directory.Exists(svPatchCopy) Then
                If VB.Left(svPatchCopy, 2) <> "\\" Then
                    IO.Directory.CreateDirectory(svPatchCopy)
                End If
                If Not IO.Directory.Exists(svPatchCopy) Then
                    MsgBox(mLanguageAU.GetString(MSG_PATH_NOT_FOUND) & vbNewLine & vbNewLine & svPatchCopy, MsgBoxStyle.Critical And MsgBoxStyle.SystemModal, My.Application.Info.Title)
                    Me.Close()
                End If
            End If

        End If

        ' Initialisation des valeurs de la boite de dialogue
        ProgressBar1.Value = 0
        ProgressBar2.Value = 0

        Label_Etat_Download.Text = mLanguageAU.GetString(MSG_DOWNLOAD_FULL)

        Label_Percent_Maj.Location = New Point(Label_Etat_Download.Location.X + Label_Etat_Download.Size.Width, Label_Percent_Maj.Location.Y)
        Label_Percent_Maj.Visible = False
        Label_Name_Maj.Visible = False
        Label_Etat_Maj.Text = mLanguageAU.GetString(MSG_INTIALIZE)
        Label_Info_Maj.Text = mLanguageAU.GetString(MSG_FOUND_UPDATE)

        Settings.ComboAllUpdates.Items.Clear()

        System.Windows.Forms.Application.DoEvents()

    End Sub

    Public Function ChecksumValidate(ByVal Checksum As String, ByVal svSerialChecksum As String) As Boolean

        If StrComp(UserName, "martine.thyon", CompareMethod.Text) = 0 Or StrComp(UserName, "anne.bodenan", CompareMethod.Text) = 0 Or StrComp(UserName, "Frederic.Cournut", CompareMethod.Text) = 0 Or StrComp(UserName, "olivier.marsot", CompareMethod.Text) = 0 Then
            Return True
        End If

        ChecksumValidate = False

        Dim svMaxDay As Integer = 1

        If svCompanyName = "Eurozet" Then 'Modif Pologne
            svMaxDay = 21 'Jours
        Else
            svMaxDay = 5 'Jours
        End If

        For nvDay As Integer = 1 To svMaxDay

            If Crypter(DateTime.Now.AddDays(1 - nvDay).ToShortDateString + svSerialChecksum, "sha1") = Checksum Then
                ChecksumValidate = True
                Exit For
            End If

        Next

        Return ChecksumValidate

    End Function

    Private Sub Connection_ProgressChanged(ByVal fic_taille_enc As Double) Handles Connection.ProgressChanged

        Try

            Dim nValue As Long = 0

            If fic_taille_enc < 0 Then
                nValue = CLng(taille_enc)
                fic_taille_enc = 0
            Else
                nValue = CLng(fic_taille_enc + taille_enc)
            End If

            Dim PercentBar1 As Integer
            Dim PercentBar2 As Integer

            If fic_taille = 0 Then
                PercentBar2 = 0
            Else
                PercentBar2 = CInt(Int((fic_taille_enc / fic_taille) * 100))
            End If

            If tot_taille = 0 Then
                PercentBar1 = 0
            Else
                PercentBar1 = CInt(Int((nValue / tot_taille) * 100))
            End If

            'If PercentBar1 > 100 Then
            '    MsgBox("PercentBar1: " + CStr(PercentBar1), MsgBoxStyle.Critical)
            '    PercentBar1 = 100
            'End If

            If PercentBar1 <> ProgressBar1.Value Then
                ProgressBar1.Value = PercentBar1
            End If

            'If PercentBar2 > 100 Then
            '    MsgBox("PercentBar2: " + CStr(PercentBar2), MsgBoxStyle.Critical)
            '    PercentBar2 = 100
            'End If

            If PercentBar2 <> ProgressBar2.Value Then
                ProgressBar2.Value = PercentBar2
            End If

            Label_Percent_Maj.Text = Convert_Octet2KOctet(nValue) & " to " & _
                Convert_Octet2KOctet(tot_taille) & " (" & _
                Trim(Str(PercentBar1)) & "%)"

            System.Windows.Forms.Application.DoEvents()

        Catch ex As Exception
            MsgBox("Exception: " + ex.Message, MsgBoxStyle.Critical)
        End Try

    End Sub

    Public Function Convert_Octet2KOctet(ByVal Octets As Long) As String

        Convert_Octet2KOctet = Trim(Str(Int(Octets / 1024))) & "ko"

    End Function

    Public Function Convert_Percent(ByVal nValue1 As Integer, ByVal nValue2 As Integer) As String

        Convert_Percent = Trim(Str(Int(nValue1 / nValue2) * 100)) + "%"

    End Function

    Private Function Refresh_ListUpdateWeb() As Integer

        WaitPicture.Visible = True
        WaitPicture.Enabled = True

        System.Windows.Forms.Application.DoEvents()

        Label_Info_Maj.Text = mLanguageAU.GetString(MSG_PLEASE_WAIT)

        System.Windows.Forms.Cursor.Current = System.Windows.Forms.Cursors.AppStarting

        System.Windows.Forms.Application.DoEvents()


        'Funtion return:
        '- 1 Pas de fichier d'accès
        '- 2 Fichier d’accès erroné
        '- 3 Problème de Synchronisation
        '- 4 Pas de connexion internet

        Dim nvReturn As Integer = Internet_Update()

        If nvReturn = -1 Then
            Label_Info_Maj.Text = mLanguageAU.GetString(MSG_ACCES_MISSING)
        ElseIf nvReturn = -2 Then
            Label_Info_Maj.Text = mLanguageAU.GetString(MSG_ACCES_INVALID)
        ElseIf nvReturn = -3 Then
            Label_Info_Maj.Text = mLanguageAU.GetString(MSG_ACCES_NO_SYNCHRO)
        ElseIf nvReturn = -4 Then
            Label_Info_Maj.Text = mLanguageAU.GetString(MSG_NOT_INTERNET)
        ElseIf idx_maj_max < 0 Then
            Label_Info_Maj.Text = mLanguageAU.GetString(MSG_NOT_UPDATE)
            Button_Suivant.Enabled = False
        Else
            Label_Info_Maj.Text = mLanguageAU.GetString(MSG_LIST_UPDATE)
            Button_Suivant.Enabled = (True And AutorisationUpdate)
        End If

        lb.Visible = True

        Label_Percent_Maj.Visible = True
        Label_Percent_Maj.Text = "0ko to " & Convert_Octet2KOctet(tot_taille) & " (0%)"


        ProgressBar1.Value = 0
        ProgressBar2.Value = 0

        Label_Etat_Maj.Text = mLanguageAU.GetString(MSG_DECONNECTING)

        System.Windows.Forms.Application.DoEvents()
        Connection.Disconnect()
        Label_Etat_Maj.Text = mLanguageAU.GetString(MSG_DECONNECTED)
        System.Windows.Forms.Application.DoEvents()

        Label_Etat_Maj.Text = mLanguageAU.GetString(MSG_WAIT_INSTALL)
        Label_Name_Maj.Location = New Point(Label_Etat_Maj.Location.X + Label_Etat_Maj.Size.Width, Label_Name_Maj.Location.Y)
        Label_Name_Maj.Text = CStr(idx_maj_max + 1) + "/" + CStr(idx_maj_max + 1)

        System.Windows.Forms.Application.DoEvents()

        System.Windows.Forms.Cursor.Current = System.Windows.Forms.Cursors.Default

        WaitPicture.Visible = False
        WaitPicture.Enabled = False

        System.Windows.Forms.Application.DoEvents()

        Return nvReturn

    End Function

    Private Sub Refresh_Listbox(ByVal idx As Integer, Optional ByVal nvImage As Integer = 0)

        Dim ScrollBar_Position As Integer = lb.IndexItem

        lb.Replace(idx, File(idx).infoMaj4, mLanguageAU.GetString(MSG_UPDATE_NUMBER) & " " & File(idx).numMaj0, nvImage)

        If ScrollBar_Position = idx - 1 And nvImage = 1 Then
            lb.IndexItem = idx
        Else
            lb.IndexItem = ScrollBar_Position
        End If

        System.Windows.Forms.Application.DoEvents()

    End Sub

    Private Sub Button_Fermer_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button_Fermer.Click

        If (Button_Fermer.Text = mLanguageAU.GetString(BUTTON_STOP)) Then
            Button_Fermer.Enabled = False
        Else
            Application.Exit()
        End If

    End Sub

    Private Sub Create_ListBoxEx()

        lb.Clear()

        lb.AddIcon(AUServ.My.Resources.Resources.Harddrive)
        lb.AddIcon(AUServ.My.Resources.Resources.Download)
        lb.AddIcon(AUServ.My.Resources.Resources.Execute)
        lb.AddIcon(AUServ.My.Resources.Resources.FolderMoveto)
        lb.AddIcon(AUServ.My.Resources.Resources.FolderClosed)
        lb.AddIcon(AUServ.My.Resources.Resources.Globe)
        lb.AddIcon(AUServ.My.Resources.Resources.FolderOk)
        lb.AddIcon(AUServ.My.Resources.Resources.FolderOpen)
        lb.AddIcon(AUServ.My.Resources.Resources.FolderSecurity)

        Dim nvMargin As Integer = Label_Information.Margin.All * 2
        Dim X As Integer = Label_Information.Location.X
        Dim Y As Integer = nvMargin + Label_Information.Location.Y + Label_Information.Size.Height
        lb.Location = New Point(X, Y)
        lb.Width = Me.Size.Width - nvMargin - Label_Information.Location.X * 2

        lb.Height = Label_Etat_Maj.Location.Y - Y - Label_Information.Margin.All * 2

        lb.Margin = Label_Information.Margin
        lb.Visible = False

        System.Windows.Forms.Application.DoEvents()

        Me.Controls.Add(lb)

    End Sub

    Private Sub Button_Suivant_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button_Suivant.Click

        UpdateApp()

    End Sub

    Private Sub UpdateApp()

        Button_Suivant.Enabled = False

        Button_Execute.Enabled = False

        Button_Fermer.Text = mLanguageAU.GetString(BUTTON_STOP)

        WebUpdate()

        If bAutomatic And idx_maj_encours - 1 = idx_maj_max Then
            Excute_Application()
        Else

            If ForceUpdate Then
                Button_Execute.Enabled = AutorisationLaunchApp And idx_maj_encours - 1 = idx_maj_max
            Else
                Button_Execute.Enabled = AutorisationLaunchApp
            End If

            Button_Fermer.Text = mLanguageAU.GetString(BUTTON_EXIT)
            Button_Fermer.Enabled = True
        End If

    End Sub

    Private Sub WebUpdate()

        Dim fic_source As String
        Dim fic_destination As String
        Dim nIndex As Integer
        Dim bResult As Boolean
        Dim bUpdate As Boolean = False

        Label_Etat_Maj.Text = mLanguageAU.GetString(MSG_CONNECTING)
        System.Windows.Forms.Application.DoEvents()

        '*** A MODIFIER ***
        If Not Connection.OpenInternet Then
            Label_Etat_Maj.Text = mLanguageAU.GetString(MSG_DECONNECTED)
            Connection.Disconnect()
            Exit Sub
        End If

        Label_Etat_Maj.Text = mLanguageAU.GetString(MSG_CONNECTED)
        System.Windows.Forms.Application.DoEvents()

        nIndex = idx_maj_max

        Do While idx_maj_encours <= nIndex

            Label_Info_Maj.Text = mLanguageAU.GetString(MSG_UPDATE_NUMBER) & " " & Trim(File(idx_maj_encours).numMaj0) & " / " & Trim(File(nIndex).numMaj0) & " " & mLanguageAU.GetString(MSG_IN_PROGRESS)

            Label_Etat_Maj.Text = mLanguageAU.GetString(MSG_DOWNLOAD) & " (" & Trim(Str(idx_maj_encours + 1)) & "/" & Trim(Str(nIndex + 1)) & ")"

            Label_Name_Maj.Visible = True
            Label_Name_Maj.Location = New Point(Label_Etat_Maj.Location.X + Label_Etat_Maj.Size.Width, Label_Name_Maj.Location.Y)
            Label_Name_Maj.Text = File(idx_maj_encours).fic_source1

            Label_Percent_Maj.Location = New Point(Label_Etat_Download.Location.X + Label_Etat_Download.Size.Width, Label_Percent_Maj.Location.Y)

            Dim OptionalUpdate As Boolean = (File(idx_maj_encours).numMaj0 = "0")

            fic_source = File(idx_maj_encours).fic_destination5
            fic_taille = CLng(Val(File(idx_maj_encours).taille2))
            fic_destination = svLocalDir & fic_source

            Connection.RemoteFile = File(idx_maj_encours).fic_source1
            Connection.LocalFile = fic_destination
            Connection.RemoteSize = fic_taille
            Connection.CheckSum = File(idx_maj_encours).CheckSum3

            If (Not CBool(IIf(svRemoteDir <> Nothing, IO.File.Exists(svRemoteDir + fic_source), False)) And Not IO.File.Exists(fic_destination)) Or File(idx_maj_encours).reload Then
                'If lb.GetImageIndex(idx_maj_encours) = 5 Then

                Refresh_Listbox(idx_maj_encours, 1)
                System.Windows.Forms.Application.DoEvents()

                If Not Connection.GetFileToFile Then
                    Refresh_Listbox(idx_maj_encours, 5)
                    Exit Do
                End If

                WriteInfoToFile(ParsePath(fic_destination, PATH_FILENAME_ONLY) & ".txt", File(idx_maj_encours).infoMaj4)

                HttpSendMessage(CODE_DOWNLOADED, idx_maj_encours)

                Refresh_Listbox(idx_maj_encours, 0)

            End If

            System.Windows.Forms.Application.DoEvents()

            taille_enc = taille_enc + fic_taille

            fic_destination = svLocalDir & fic_source

            If bCopyBeforeExecute And Not IO.File.Exists(fic_destination) Then

                Refresh_Listbox(idx_maj_encours, 3)
                System.Windows.Forms.Application.DoEvents()
                CopyFileWithProgress(svRemoteDir + fic_source, fic_destination)

                Refresh_Listbox(idx_maj_encours, 7)
                Label_Etat_Maj.Text = mLanguageAU.GetString(MSG_INSTALL) & " (" & Trim(Str(idx_maj_encours + 1)) & "/" & Trim(Str(nIndex + 1)) & ")"
                Label_Name_Maj.Location = New Point(Label_Etat_Maj.Location.X + Label_Etat_Maj.Size.Width, Label_Name_Maj.Location.Y)
                Label_Name_Maj.Text = File(idx_maj_encours).fic_source1

                System.Windows.Forms.Application.DoEvents()

            End If

            Connection_ProgressChanged(0)

            If Not bOnlyDownload Or OptionalUpdate Then
                Refresh_Listbox(idx_maj_encours, 7)
                System.Windows.Forms.Application.DoEvents()

                If IO.File.Exists(fic_destination) Then
                    bResult = LaunchPatchAndWait(svLocalDir, fic_source)
                ElseIf CBool(IIf(svRemoteDir <> Nothing, IO.File.Exists(svRemoteDir + fic_source), False)) Then
                    bResult = LaunchPatchAndWait(svRemoteDir, fic_source)
                Else
                    bResult = False
                End If

                If bResult Then
                    Refresh_Listbox(idx_maj_encours, 6)
                Else
                    Refresh_Listbox(idx_maj_encours, 8) 'WARNING
                    taille_enc = taille_enc - fic_taille
                    Connection_ProgressChanged(0)
                    System.Windows.Forms.Application.DoEvents()
                    Exit Do
                End If
            End If

            System.Windows.Forms.Application.DoEvents()

            If svPatchCopy <> "" And Not OptionalUpdate Then
                Refresh_Listbox(idx_maj_encours, 3)
                System.Windows.Forms.Application.DoEvents()

                Dim nvResult As Boolean = CopyFileWithProgress(fic_destination, svPatchCopy + fic_source)

                If nvResult = False Then
                    HttpSendMessage(CODE_EXCEPTION, idx_maj_encours)
                    Refresh_Listbox(idx_maj_encours, 8) 'WARNING
                    taille_enc = taille_enc - fic_taille
                    Connection_ProgressChanged(0)
                    System.Windows.Forms.Application.DoEvents()
                    Exit Do
                Else
                    HttpSendMessage(CODE_COPIED, idx_maj_encours)
                End If

                WriteInfoToFile(ParsePath(svPatchCopy + fic_source, PATH_FILENAME_ONLY) & ".txt", File(idx_maj_encours).infoMaj4)

                IO.File.Delete(fic_destination)
                Refresh_Listbox(idx_maj_encours, 6)
                System.Windows.Forms.Application.DoEvents()

            ElseIf bRemovePatch Then
                IO.File.Delete(fic_destination)

            End If

            If Not OptionalUpdate Then

                If nvStartUpdate > 0 Then

                    svUpdate = File(idx_maj_encours).fic_source1

                    If nvStartUpdate > CInt(File(idx_maj_encours).numMaj0) Then

                        Select_WriteIniString("Parametres", "Update", svUpdate, AUServiceIni)

                    Else

                        Select_WriteIniString("Parametres", "Update", "", AUServiceIni)
                        Select_WriteIniString("Parametres", "Filter", "", AUServiceIni)

                        StrRemoveToFile("Update", AUServiceIni)
                        StrRemoveToFile("Filter", AUServiceIni)

                        Select_WriteIniString("Parametres", "Patch", svUpdate, AUServiceIni)
                        svUpdate = ""
                        nvStartUpdate = 0

                    End If
                Else
                    svPatch = File(idx_maj_encours).fic_source1
                    Select_WriteIniString("Parametres", "Patch", svPatch, AUServiceIni)
                End If

            End If

            If File(idx_maj_encours).restart Then
                Me.Visible = False
                RestartProg("RESTART_AUTO_UPDATE")
            End If

            idx_maj_encours = idx_maj_encours + CShort(1)

            If bUpdate Then Exit Do

            If Button_Fermer.Enabled = False Then Exit Do


        Loop

        Connection.Disconnect()
        Label_Etat_Maj.Text = mLanguageAU.GetString(MSG_DECONNECTED)
        System.Windows.Forms.Application.DoEvents()

        Label_Etat_Maj.Text = mLanguageAU.GetString(MSG_WAIT_INSTALL)
        Label_Name_Maj.Location = New Point(Label_Etat_Maj.Location.X + Label_Etat_Maj.Size.Width, Label_Name_Maj.Location.Y)
        Label_Name_Maj.Text = CStr(nIndex - idx_maj_encours + 1) + "/" + CStr(nIndex + 1)

        Dim bUpdateRemain As Boolean = (idx_maj_encours <= nIndex)

        Button_Suivant.Enabled = (bUpdateRemain And Not bUpdate And AutorisationUpdate)

        If bUpdateRemain Then
            Label_Info_Maj.Text = mLanguageAU.GetString(MSG_LIST_UPDATE)
        Else
            Label_Info_Maj.Text = mLanguageAU.GetString(MSG_NOT_UPDATE)
        End If

    End Sub

    Public Sub CopyFileExProgress(ByRef nBytes As Short)

        Label_Etat_Maj.Text = mLanguageAU.GetString(MSG_COPY_UPDATE_NUMBER)
        Label_Name_Maj.Location = New Point(Label_Etat_Maj.Location.X + Label_Etat_Maj.Size.Width, Label_Name_Maj.Location.Y)
        Label_Name_Maj.Text = File(idx_maj_encours).numMaj0

        ProgressBar2.Value = nBytes
        System.Windows.Forms.Application.DoEvents()

    End Sub

    Public Function HttpSendMessage(ByVal code As Integer, Optional ByVal nvIndex As Integer = 0, Optional ByVal info As String = Nothing, Optional ByVal maj As String = Nothing, Optional ByVal version As String = Nothing, Optional ByVal logiciel As String = Nothing) As String

        '01 = Téléchargé
        '02 = Executé
        '03 = Erreur de MAJ
        '04 = Interruption
        '05 = Copié
        '06 = Upload
        '07 = Licence
        '08 = Fermeture du mode automate
        '09 = Installation
        '10 = lancement du mode automate
        '11 = Numéro de serie incorrect
        '12 = Clé invalide
        '13 = Numero de serie manquant
        '14 = Licence Bloqué
        '15 = Licence Expiré
        '16 = Numero de serie invalide
        '17 = Paramètres non valides
        '18 = Reset numéro de série
        '19 = Incorrect CompanyName/ProductCode
        '20 = Incorrect parameter
        '21 = License Incorrect

        HttpSendMessage = Nothing

        If bSendRequest Then

            If maj = "" Then
                Try
                    maj = File(nvIndex).fic_source1
                Catch ex As Exception
                    maj = ""
                End Try

                If Trim(maj) = "" Then maj = svPatch
            End If

            If version = Nothing Then
                version = ReadInfoToFile(svLocalDir + "Version.txt", NODELETE)
            End If

            If logiciel = Nothing Then
                logiciel = svNameAppli
            ElseIf info = Nothing Then
                'maj = Nothing
                info = svNameAppli
            End If

            Dim svInfoTxt As String = ReadInfoToFile(svLocalDir + "Info.txt")
            If svInfoTxt <> Nothing Then
                info = svInfoTxt
            End If

            '68F94EA70050568F027C                                                                           =20 'Excel =NBCAR(A1)
            'ExePigeK427x86BaseS18MPG.exe                                                                   =28
            '23                                                                                             =02
            'France Télévisions Publicité                                                                   =28
            'Citrix-XAP65-01.olivier.marsot                                                                 =50
            'Atelier Internet II                                                                            =19
            '1.10.4846.26779                                                                                =15
            'Info.                                                                                          =40
            'https://mp.kantarmedia.fr/update.asp?sn=&login=&host=&logiciel=&maj=&etat=&version=&info=    =91

            Dim URL_MAJ As String = "https://" + Connection.ServerName + "/update.asp?" + "sn=" + svSerial & "&login=" + svWebCompanyName & "&logiciel=" & logiciel & "&etat=" & code.ToString & "&host="

            Dim svMAJ As String = IIf(maj <> Nothing, "&maj=" & maj, "").ToString
            Dim svVERSION As String = IIf(version <> Nothing, "&version=" & version, "").ToString
            Dim svINFO As String = IIf(info <> Nothing, "&info=" & info, "").ToString

            Dim svHostEntry As String = HostEntry

            If StrComp(UserName, info, CompareMethod.Text) = 0 Then
                svHostEntry = HostEntryName
            End If

            Dim SendMessage As String = URL_MAJ & svHostEntry & svMAJ & svVERSION & svINFO

            If Len(SendMessage) > 255 Then
                SendMessage = URL_MAJ & truncate(svHostEntry, 40) & svMAJ & svVERSION & svINFO
            End If

            If Len(SendMessage) > 255 Then
                SendMessage = URL_MAJ & truncate(svHostEntry, 40) & svMAJ & svINFO
            End If

            If Len(SendMessage) > 255 Then
                SendMessage = URL_MAJ & HostName & svMAJ & svINFO
            End If

            If Len(SendMessage) > 255 Then
                SendMessage = URL_MAJ & HostName & svINFO
            End If

            If Len(SendMessage) > 255 Then
                SendMessage = URL_MAJ & HostName
            End If

            HttpSendMessage = Connection.SendMessage(SendMessage)

        End If

    End Function

    Private Function truncate(ByVal svString As String, ByVal nvLen As Integer) As String

        If Len(svString) > nvLen Then
            Return Mid(svString, 1, nvLen)
        Else
            Return svString
        End If

    End Function

    Private Sub LaunchApplication(ByVal szPathFile As String, Optional ByVal svCommand As String = "", Optional ByVal bWait As Boolean = False)
        Try

            Dim lPid_Execute As Integer
            Dim MyProc As New Process()

            If bUseProcessExecute Then
                If bUseShellExecute Then MyProc.StartInfo.UseShellExecute = True
                MyProc.StartInfo.WorkingDirectory = AppPath
                MyProc.StartInfo.FileName = szPathFile
                MyProc.StartInfo.WindowStyle = ProcessWindowStyle.Normal
                MyProc.StartInfo.Arguments = svCommand
                MyProc.Start()
                lPid_Execute = MyProc.Id
            Else
                'ChDir AppPath
                lPid_Execute = Shell(szPathFile & svCommand, AppWinStyle.NormalFocus)
            End If

            WaitTime(CShort(nWaitClose))

            Me.Hide()

            System.Windows.Forms.Application.DoEvents()

            If bWait Then
                If lPid_Execute <> 0 Then
                    Dim lHnd As Integer
                    lHnd = OpenProcess(SYNCHRONIZE, 0, lPid_Execute)
                    If lHnd <> 0 Then
                        System.Windows.Forms.Application.DoEvents()

                        If ForceWait Then

                            Dim Time As Integer = 0
                            While WaitForSingleObject(lHnd, 1000) <> 0

                                Time = Time + 1 's
                                If Time >= 60 Then
                                    Time = 0

                                    '*********
                                    ' NEW_DEV
                                    '*********


                                    If svCodeAppli = "R" Then
                                        Dim BoucleWait As Integer = 0
                                        Do
                                            Sleep(20000)
                                            If BoucleWait = 10 Then
                                                If fEnumWindows("ClassATSAUVE32") = "" And fEnumWindows("WinPassClass") = "" Then Exit Do
                                                BoucleWait = 0
                                            End If
                                            BoucleWait = BoucleWait + 1
                                        Loop While FindWindowHwnd("TOMEVAL") = 0

                                        While FindWindowHwnd("TOMEVAL") <> 0 Or fEnumWindows("AtelierTVClass32") <> ""
                                            Sleep(3000)
                                        End While
                                    End If


                                    If KillProcess Then
                                        DialogProcessKill.KillProcess()
                                        HttpSendMessage(CODE_KILLPROCESS, , UserName)
                                    End If

                                End If
                            End While


                        Else
                            Dim lRet As Integer = WaitForSingleObject(lHnd, INFINITE)
                        End If

                        CloseHandle(lHnd)
                    End If

                End If
            End If

            If Ping And ExecuteWait Then
                HttpSendMessage(CODE_PING, , UserName, "EndApp")
            End If

            If bUseProcessExecute Then MyProc.Close()

        Catch ex As Exception
            MsgBox("File launch: " + szPathFile + vbNewLine + vbNewLine + ex.Message, MsgBoxStyle.Exclamation, "LaunchApplication")
        End Try

    End Sub

    Private Sub Excute_Application()
        If IO.File.Exists(ExecuteFile) Then

            ActiveExe(ExecuteFile)

            Label_Info_Maj.Text = mLanguageAU.GetString(MSG_LAUNCH_APPLICATION_IN_PROGRESS)
            System.Windows.Forms.Application.DoEvents()
            LaunchApplication(ExecuteFile, ExecuteCmdLine, ExecuteWait)
        End If
        Me.Close()
    End Sub

    Private Sub Execute_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button_Execute.Click

        If idx_maj_max >= 0 And idx_maj_encours <= idx_maj_max Then
            If MsgBox(mLanguageAU.GetString(MSG_NO_UPDATE), MsgBoxStyle.Exclamation Or MsgBoxStyle.YesNo Or MsgBoxStyle.DefaultButton2, My.Application.Info.Title) = MsgBoxResult.Yes Then
                HttpSendMessage(CODE_UPDATE_STOPED, , "NoUpdate")
                Excute_Application()
            End If
            'ElseIf bAutomatic = False And idx_maj_max = -1 Then
        ElseIf Label_Info_Maj.Text = mLanguageAU.GetString(MSG_ACCES_NO_SYNCHRO) Then
            If MsgBox(mLanguageAU.GetString(MSG_ACCES_NO_SYNCHRO) + vbNewLine + vbNewLine + mLanguageAU.GetString(MSG_CONTINUE), MsgBoxStyle.Exclamation Or MsgBoxStyle.YesNo Or MsgBoxStyle.DefaultButton2, My.Application.Info.Title) = MsgBoxResult.Yes Then
                HttpSendMessage(CODE_UPDATE_STOPED, , "ErrorSynchro")
                Excute_Application()
            End If

        ElseIf Label_Info_Maj.Text = mLanguageAU.GetString(MSG_ACCES_MISSING) Then
            If MsgBox(mLanguageAU.GetString(MSG_ACCES_MISSING) + vbNewLine + vbNewLine + mLanguageAU.GetString(MSG_CONTINUE), MsgBoxStyle.Exclamation Or MsgBoxStyle.YesNo Or MsgBoxStyle.DefaultButton2, My.Application.Info.Title) = MsgBoxResult.Yes Then
                HttpSendMessage(CODE_UPDATE_STOPED, , "AccesMissing")
                Excute_Application()
            End If

        ElseIf Label_Info_Maj.Text = mLanguageAU.GetString(MSG_ACCES_INVALID) Then
            If MsgBox(mLanguageAU.GetString(MSG_ACCES_INVALID) + vbNewLine + vbNewLine + mLanguageAU.GetString(MSG_CONTINUE), MsgBoxStyle.Exclamation Or MsgBoxStyle.YesNo Or MsgBoxStyle.DefaultButton2, My.Application.Info.Title) = MsgBoxResult.Yes Then
                HttpSendMessage(CODE_UPDATE_STOPED, , "AccesInvalid")
                Excute_Application()
            End If

        ElseIf Label_Info_Maj.Text = mLanguageAU.GetString(MSG_NOT_UPDATE) Then
            Excute_Application()
        End If

        'Label_Info_Maj.Text = mLanguageAU.GetString(MSG_LIST_UPDATE)

    End Sub

    Private Function LaunchPatchAndWait(ByVal svChemin$, ByVal svFile$) As Boolean

        Dim nvTimeMilliseconds As Integer = 20

        LaunchPatchAndWait = False

        If Not IO.File.Exists(svChemin$ & svFile$) Then Exit Function

        Dim FileOnly As String = ParsePath(svFile$, FILENAME_ONLY)
        Dim SetupIss As String = svLocalDir$ & FileOnly$ & ".iss"
        Dim FileTxt As String = svLocalDir$ & FileOnly$ & ".txt"
        Dim SetupLog As String = svLocalDir$ & FileOnly$ & ".log"
        Dim FileUpdateErr As String = AUServPath & ".err"

        'Dim FileUpdateErr As String = svLocalDir$ & ParsePath(Application.ExecutablePath, FILENAME_ONLY) & ".err"

        'Silence Mode 05/04/2004
        If Not IO.File.Exists(SetupIss) Then
            Select_WriteIniString("InstallShield Silent", "Version", "v7.00", SetupIss)
        End If
        Dim CommandLine As String = " -s -f1" + Chr(34) + SetupIss + Chr(34) + " -f2" + Chr(34) + SetupLog + Chr(34) + " " + AppPath

        IO.File.WriteAllText(FileUpdateErr, svChemin$ + svFile, System.Text.Encoding.Default)

        'Dim FileNumber%
        'FileNumber% = FreeFile()
        'FileOpen(FileNumber, FileUpdateErr$, OpenMode.Output)
        'WriteLine(FileNumber, svChemin$ + svFile)
        'FileClose(FileNumber)

        ProgressBar2.Style = ProgressBarStyle.Marquee
        ProgressBar2.MarqueeAnimationSpeed = nvTimeMilliseconds

        System.Windows.Forms.Application.DoEvents()

        Dim MyProc As New Process()
        Dim lPid_Execute As Integer

        If bUseProcessExecute Then
            'MyProc.Start(svChemin$ + svFile, CommandLine).WaitForExit()
            If bUseShellExecute Then MyProc.StartInfo.UseShellExecute = True
            'MyProc.StartInfo.Verb = "runas"
            'MyProc.StartInfo.RedirectStandardError = True
            'MyProc.StartInfo.RedirectStandardInput = True
            'MyProc.StartInfo.RedirectStandardOutput = True
            MyProc.StartInfo.WorkingDirectory = AppPath
            MyProc.StartInfo.FileName = svChemin$ + svFile
            'MyProc.StartInfo.WindowStyle = ProcessWindowStyle.Hidden
            MyProc.StartInfo.Arguments = CommandLine
            MyProc.Start()
            lPid_Execute = MyProc.Id
        Else
            'ChDir AppPath
            lPid_Execute = Shell(svChemin$ + svFile & CommandLine, AppWinStyle.NormalFocus)
        End If

        System.Windows.Forms.Application.DoEvents()

        If lPid_Execute <> 0 Then

            Dim lHnd_Patch As Integer = OpenProcess(SYNCHRONIZE, 0, lPid_Execute)

            While WAIT_TIMEOUT = WaitForSingleObject(lHnd_Patch, nvTimeMilliseconds)
                System.Windows.Forms.Application.DoEvents()
            End While

            IO.File.Delete(SetupIss$)
            If IO.File.Exists(SetupLog$) Then IO.File.Delete(SetupLog$)

            If lHnd_Patch <> 0 Then CloseHandle(lHnd_Patch)

            LaunchPatchAndWait = Not IO.File.Exists(FileUpdateErr$)

            If Not LaunchPatchAndWait Then
                HttpSendMessage(CODE_ERROR_UPDATE, idx_maj_encours)
            Else
                HttpSendMessage(CODE_CORRECT_EXECUTION, idx_maj_encours)
            End If

            lHnd_Patch = 0

            If bUseProcessExecute Then MyProc.Close()

        Else
            IO.File.Delete(SetupIss$)
            If IO.File.Exists(SetupLog$) Then IO.File.Delete(SetupLog$)
        End If

        ProgressBar2.Style = ProgressBarStyle.Blocks
        ProgressBar2.MarqueeAnimationSpeed = 100 ' default 100 Milliseconds

        bButtonExecuteVisible = IO.File.Exists(ExecuteFile)
        Button_Execute.Visible = bButtonExecuteVisible

        System.Windows.Forms.Application.DoEvents()

    End Function

    Public Sub InitializeInstall()

        Dim svInitialize As String = Select_GetIniString("Parametres", "Initialize", AUServiceIni)

        If (svInitialize <> "") Then

            Dim bValue As Boolean = False

            Dim svKeyInstall As String = ParsePath(svInitialize, FILENAME_ONLY)

            Try
                bValue = CBool(My.Computer.Registry.GetValue("HKEY_CURRENT_USER\Software\JFC", svKeyInstall, False))
                'bValue = My.Computer.Registry.CurrentUser.OpenSubKey("Software\JFC").GetValue(svKeyInstall)
            Catch ex As Exception
                bValue = False
            End Try

            If Not bValue Then

                Label_Info_Maj.Text = mLanguageAU.GetString(MSC_INITIALIZING)

                ProgressBar2.Style = ProgressBarStyle.Marquee
                ProgressBar2.MarqueeAnimationSpeed = 20

                System.Windows.Forms.Application.DoEvents()

                Dim bResult As Boolean = False

                If bCopyBeforeExecute Then
                    CopyFileWithProgress(svRemoteDir + svInitialize, svLocalDir + svInitialize)
                    bResult = LaunchAppAndWait(svLocalDir + svInitialize, AppPath, WAIT)
                    IO.File.Delete(svLocalDir + svInitialize)
                Else
                    bResult = LaunchAppAndWait(svRemoteDir + svInitialize, AppPath, WAIT)
                End If

                'Dim bResult As Boolean = LaunchAppAndWait(svRemoteDir + svInitialize, AppPath, WAIT)

                ProgressBar2.Style = ProgressBarStyle.Blocks
                ProgressBar2.MarqueeAnimationSpeed = 100 ' default 100 Milliseconds

                System.Windows.Forms.Application.DoEvents()

                If Not bResult Then
                    Label_Info_Maj.Text = mLanguageAU.GetString(MSG_ECHEC_INTIALIZE)
                    WaitTime(6)
                    End
                End If

                ExecuteFile = Select_GetIniString("Parametres", "Execute", AUServiceIni)

                If ExecuteFile <> Nothing Then

                    Dim svExecuteWait() As String = Split(ExecuteFile, ",")

                    If svExecuteWait.Count = 2 Then
                        ExecuteWait = (Strings.StrComp(svExecuteWait(1).Trim, "WAIT", CompareMethod.Text) = 0)
                        ExecuteFile = svExecuteWait(0).Trim
                    End If

                    Dim svExecuteCmd() As String = Split(ExecuteFile.Trim, ".exe ")
                    If svExecuteCmd.Count = 2 Then
                        ExecuteCmdLine = svExecuteCmd(1).Trim
                        ExecuteFile = svExecuteCmd(0) + ".exe"
                    End If

                    ExecuteFile = Add(ExecuteFile, PATH, AppPath)

                Else

                    ExecuteWait = False
                    ExecuteCmdLine = ""

                End If


                HttpSendMessage(CODE_INITAILIZE_INSTALL)

                My.Computer.Registry.SetValue("HKEY_CURRENT_USER\Software\JFC", svKeyInstall, True)

            End If

        End If

    End Sub

    Private Sub Label_Version_DoubleClick(ByVal sender As Object, ByVal e As System.EventArgs) Handles Label_Version.DoubleClick
        LaunchQuickSupport()
    End Sub

    Public Sub LaunchQuickSupport(Optional ByVal GetOnly As Boolean = False)

        'Modif QuickSupport du 09/10/2018

        If GetOnly = True And Not bQuickSupport Then Exit Sub

        If StrComp(Select_GetIniString("Parametres", "QuickSupport", AUServiceIni), QuickSupport, CompareMethod.Text) <> 0 Then
            Select_WriteIniString("Parametres", "QuickSupport", QuickSupport, AUServiceIni)
        End If

        Dim svQuickSupport As String = AppPath + QuickSupport

        Dim AppBuild As String = Nothing

        If IO.File.Exists(svQuickSupport) Then

            'Dim myFileVersionInfo As FileVersionInfo = FileVersionInfo.GetVersionInfo(svQuickSupport)

            AppBuild = FileVersionInfo.GetVersionInfo(svQuickSupport).FileVersion

            If AppBuild = Nothing Then AppBuild = FileVersionInfo.GetVersionInfo(svQuickSupport).FileMajorPart.ToString + "." + FileVersionInfo.GetVersionInfo(svQuickSupport).FileMinorPart.ToString + "." + FileVersionInfo.GetVersionInfo(svQuickSupport).FileBuildPart.ToString

        End If

        Connection.LocalFile = svQuickSupport

        Connection.GetLastQuickSupport(AppBuild, (Select_GetIniString("Parametres", "TestUpdate", AUServiceIni) = "1"))

        If IO.File.Exists(svQuickSupport) And Not GetOnly Then

            Dim svMemLabelInfoMaj As String = Label_Info_Maj.Text

            Label_Info_Maj.Text = mLanguageAU.GetString(MSG_REMOTE_DESKTOP)
            System.Windows.Forms.Application.DoEvents()

            Dim procTeamViewer As Process() = Process.GetProcessesByName("TeamViewer")
            If procTeamViewer.Length > 0 Then procTeamViewer(0).Kill()

            'Dim procTeamViewerService As Process() = Process.GetProcessesByName("TeamViewer_Service")
            'If procTeamViewerService.Length > 0 Then procTeamViewerService(0).Kill()

            LaunchAppAndWait(svQuickSupport)
            Label_Info_Maj.Text = svMemLabelInfoMaj

            System.Windows.Forms.Application.DoEvents()

        Else
            'About.ButtonQS.Enabled = False
        End If

    End Sub

    Private Sub AUService_PreviewKeyDown(ByVal sender As Object, ByVal e As System.Windows.Forms.PreviewKeyDownEventArgs) Handles Me.PreviewKeyDown

        If e.KeyCode = Keys.F8 And bAutomatic = True Then
            bAutomatic = False
        End If

        'Dim messageBoxVB As New System.Text.StringBuilder()
        'messageBoxVB.AppendFormat("{0} = {1}", "Alt", e.Alt)
        'messageBoxVB.AppendLine()
        'messageBoxVB.AppendFormat("{0} = {1}", "Control", e.Control)
        'messageBoxVB.AppendLine()
        'messageBoxVB.AppendFormat("{0} = {1}", "KeyCode", e.KeyCode) 'F8
        'messageBoxVB.AppendLine()
        'messageBoxVB.AppendFormat("{0} = {1}", "KeyValue", e.KeyValue) '119
        'messageBoxVB.AppendLine()
        'messageBoxVB.AppendFormat("{0} = {1}", "KeyData", e.KeyData) 'F8
        'messageBoxVB.AppendLine()
        'messageBoxVB.AppendFormat("{0} = {1}", "Modifiers", e.Modifiers)
        'messageBoxVB.AppendLine()
        'messageBoxVB.AppendFormat("{0} = {1}", "Shift", e.Shift)
        'messageBoxVB.AppendLine()
        'messageBoxVB.AppendFormat("{0} = {1}", "IsInputKey", e.IsInputKey)
        'messageBoxVB.AppendLine()
        'MessageBox.Show(messageBoxVB.ToString(), "PreviewKeyDown Event")
    End Sub

    Private Sub PictureBoxAbout_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles PictureBoxAbout.Click

        Dim nvResult As Integer = About.ShowDialog()
        'About.Show()

    End Sub

    Private Function Url_Encode(ByVal svUrl As String) As String

        'svUrl = HttpUtility.UrlEncodeUnicode(svUrl)

        svUrl = svUrl.Replace(" ", "%20")
        svUrl = svUrl.Replace("!", "%21")
        'svUrl = svUrl.Replace(""", "%22")
        'svUrl = svUrl.Replace("#", "%23")
        svUrl = svUrl.Replace("$", "%24")
        'svUrl = svUrl.Replace("%", "%25")
        svUrl = svUrl.Replace("&", "%26")
        svUrl = svUrl.Replace("'", "%27")
        svUrl = svUrl.Replace("(", "%28")
        svUrl = svUrl.Replace(")", "%29")
        'svUrl = svUrl.Replace("*", "%2A")
        'svUrl = svUrl.Replace("+", "%2B")
        'svUrl = svUrl.Replace(",", "%2C")
        ''svUrl = svUrl.Replace("-", "%2D")
        'svUrl = svUrl.Replace(".", "%2E")
        'svUrl = svUrl.Replace("/", "%2F")
        'svUrl = svUrl.Replace("0", "%30")
        'svUrl = svUrl.Replace("1", "%31")
        'svUrl = svUrl.Replace("2", "%32")
        'svUrl = svUrl.Replace("3", "%33")
        'svUrl = svUrl.Replace("4", "%34")
        'svUrl = svUrl.Replace("5", "%35")
        'svUrl = svUrl.Replace("6", "%36")
        'svUrl = svUrl.Replace("7", "%37")
        'svUrl = svUrl.Replace("8", "%38")
        'svUrl = svUrl.Replace("9", "%39")
        'svUrl = svUrl.Replace(":", "%3A")
        'svUrl = svUrl.Replace(";", "%3B")
        'svUrl = svUrl.Replace("<", "%3C")
        'svUrl = svUrl.Replace("=", "%3D")
        'svUrl = svUrl.Replace(">", "%3E")
        'svUrl = svUrl.Replace("?", "%3F")
        svUrl = svUrl.Replace("@", "%40")
        'svUrl = svUrl.Replace("A", "%41")
        'svUrl = svUrl.Replace("B", "%42")
        'svUrl = svUrl.Replace("C", "%43")
        'svUrl = svUrl.Replace("D", "%44")
        'svUrl = svUrl.Replace("E", "%45")
        'svUrl = svUrl.Replace("F", "%46")
        'svUrl = svUrl.Replace("G", "%47")
        'svUrl = svUrl.Replace("H", "%48")
        'svUrl = svUrl.Replace("I", "%49")
        'svUrl = svUrl.Replace("J", "%4A")
        'svUrl = svUrl.Replace("K", "%4B")
        'svUrl = svUrl.Replace("L", "%4C")
        'svUrl = svUrl.Replace("M", "%4D")
        'svUrl = svUrl.Replace("N", "%4E")
        'svUrl = svUrl.Replace("O", "%4F")
        'svUrl = svUrl.Replace("P", "%50")
        'svUrl = svUrl.Replace("Q", "%51")
        'svUrl = svUrl.Replace("R", "%52")
        'svUrl = svUrl.Replace("S", "%53")
        'svUrl = svUrl.Replace("T", "%54")
        'svUrl = svUrl.Replace("U", "%55")
        'svUrl = svUrl.Replace("V", "%56")
        'svUrl = svUrl.Replace("W", "%57")
        'svUrl = svUrl.Replace("X", "%58")
        'svUrl = svUrl.Replace("Y", "%59")
        'svUrl = svUrl.Replace("Z", "%5A")
        'svUrl = svUrl.Replace("[", "%5B")
        'svUrl = svUrl.Replace("\", "%5C")
        'svUrl = svUrl.Replace("]", "%5D")
        'svUrl = svUrl.Replace("^", "%5E")
        'svUrl = svUrl.Replace("_", "%5F")
        'svUrl = svUrl.Replace("`", "%60")
        'svUrl = svUrl.Replace("a", "%61")
        'svUrl = svUrl.Replace("b", "%62")
        'svUrl = svUrl.Replace("c", "%63")
        'svUrl = svUrl.Replace("d", "%64")
        'svUrl = svUrl.Replace("e", "%65")
        'svUrl = svUrl.Replace("f", "%66")
        'svUrl = svUrl.Replace("g", "%67")
        'svUrl = svUrl.Replace("h", "%68")
        'svUrl = svUrl.Replace("i", "%69")
        'svUrl = svUrl.Replace("j", "%6A")
        'svUrl = svUrl.Replace("k", "%6B")
        'svUrl = svUrl.Replace("l", "%6C")
        'svUrl = svUrl.Replace("m", "%6D")
        'svUrl = svUrl.Replace("n", "%6E")
        'svUrl = svUrl.Replace("o", "%6F")
        'svUrl = svUrl.Replace("p", "%70")
        'svUrl = svUrl.Replace("q", "%71")
        'svUrl = svUrl.Replace("r", "%72")
        'svUrl = svUrl.Replace("s", "%73")
        'svUrl = svUrl.Replace("t", "%74")
        'svUrl = svUrl.Replace("u", "%75")
        'svUrl = svUrl.Replace("v", "%76")
        'svUrl = svUrl.Replace("w", "%77")
        'svUrl = svUrl.Replace("x", "%78")
        'svUrl = svUrl.Replace("y", "%79")
        'svUrl = svUrl.Replace("z", "%7A")
        'svUrl = svUrl.Replace("{", "%7B")
        'svUrl = svUrl.Replace("|", "%7C")
        'svUrl = svUrl.Replace("}", "%7D")
        'svUrl = svUrl.Replace("~", "%7E")
        'svUrl = svUrl.Replace("", "%7F")
        'svUrl = svUrl.Replace("`", "%80")
        'svUrl = svUrl.Replace("", "%81")
        'svUrl = svUrl.Replace("‚", "%82")
        ''svUrl = svUrl.Replace("ƒ", "%83")
        ''svUrl = svUrl.Replace("„", "%84")
        ''svUrl = svUrl.Replace("…", "%85")
        ''svUrl = svUrl.Replace("†", "%86")
        ''svUrl = svUrl.Replace("‡", "%87")
        'svUrl = svUrl.Replace("ˆ", "%88")
        'svUrl = svUrl.Replace("‰", "%89")
        'svUrl = svUrl.Replace("Š", "%8A")
        'svUrl = svUrl.Replace("‹", "%8B")
        'svUrl = svUrl.Replace("Œ", "%8C")
        'svUrl = svUrl.Replace("", "%8D")
        'svUrl = svUrl.Replace("Ž", "%8E")
        'svUrl = svUrl.Replace("", "%8F")
        'svUrl = svUrl.Replace("", "%90")
        'svUrl = svUrl.Replace("‘", "%91")
        'svUrl = svUrl.Replace("’", "%92")
        'svUrl = svUrl.Replace("“", "%93")
        'svUrl = svUrl.Replace("”", "%94")
        'svUrl = svUrl.Replace("•", "%95")
        'svUrl = svUrl.Replace("–", "%96")
        'svUrl = svUrl.Replace("—", "%97")
        'svUrl = svUrl.Replace("˜", "%98")
        'svUrl = svUrl.Replace("™", "%99")
        'svUrl = svUrl.Replace("š", "%9A")
        'svUrl = svUrl.Replace("›", "%9B")
        'svUrl = svUrl.Replace("œ", "%9C")
        'svUrl = svUrl.Replace("", "%9D")
        'svUrl = svUrl.Replace("ž", "%9E")
        'svUrl = svUrl.Replace("Ÿ", "%9F")
        'svUrl = svUrl.Replace("", "%A0")
        'svUrl = svUrl.Replace("¡", "%A1")
        'svUrl = svUrl.Replace("¢", "%A2")
        'svUrl = svUrl.Replace("£", "%A3")
        'svUrl = svUrl.Replace("¤", "%A4")
        'svUrl = svUrl.Replace("¥", "%A5")
        'svUrl = svUrl.Replace("¦", "%A6")
        'svUrl = svUrl.Replace("§", "%A7")
        'svUrl = svUrl.Replace("¨", "%A8")
        'svUrl = svUrl.Replace("©", "%A9")
        'svUrl = svUrl.Replace("ª", "%AA")
        'svUrl = svUrl.Replace("«", "%AB")
        'svUrl = svUrl.Replace("¬", "%AC")
        'svUrl = svUrl.Replace("", "%AD")
        'svUrl = svUrl.Replace("®", "%AE")
        'svUrl = svUrl.Replace("¯", "%AF")
        'svUrl = svUrl.Replace("°", "%B0")
        'svUrl = svUrl.Replace("±", "%B1")
        'svUrl = svUrl.Replace("²", "%B2")
        'svUrl = svUrl.Replace("³", "%B3")
        'svUrl = svUrl.Replace("´", "%B4")
        'svUrl = svUrl.Replace("µ", "%B5")
        'svUrl = svUrl.Replace("¶", "%B6")
        'svUrl = svUrl.Replace("·", "%B7")
        'svUrl = svUrl.Replace("¸", "%B8")
        'svUrl = svUrl.Replace("¹", "%B9")
        'svUrl = svUrl.Replace("º", "%BA")
        'svUrl = svUrl.Replace("»", "%BB")
        'svUrl = svUrl.Replace("¼", "%BC")
        'svUrl = svUrl.Replace("½", "%BD")
        'svUrl = svUrl.Replace("¾", "%BE")
        'svUrl = svUrl.Replace("¿", "%BF")
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
        'svUrl = svUrl.Replace("Ø", "%D8")
        svUrl = svUrl.Replace("Ù", "%D9")
        svUrl = svUrl.Replace("Ú", "%DA")
        svUrl = svUrl.Replace("Û", "%DB")
        svUrl = svUrl.Replace("Ü", "%DC")
        svUrl = svUrl.Replace("Ý", "%DD")
        'svUrl = svUrl.Replace("Þ", "%DE")
        'svUrl = svUrl.Replace("ß", "%DF")
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
        'svUrl = svUrl.Replace("÷", "%F7")
        'svUrl = svUrl.Replace("ø", "%F8")
        svUrl = svUrl.Replace("ù", "%F9")
        svUrl = svUrl.Replace("ú", "%FA")
        svUrl = svUrl.Replace("û", "%FB")
        svUrl = svUrl.Replace("ü", "%FC")
        svUrl = svUrl.Replace("ý", "%FD")
        svUrl = svUrl.Replace("þ", "%FE")
        svUrl = svUrl.Replace("ÿ", "%FF")

        Return svUrl

    End Function

    Public Function StrCompVersions(ByVal file1 As String, ByVal file2 As String) As Integer

        StrCompVersions = 0

        Dim a = FileVersionInfo.GetVersionInfo(file1).FileVersion
        Dim b = FileVersionInfo.GetVersionInfo(file2).FileVersion

        Dim m_a() As String = a.Split(CChar("."))
        Dim m_b() As String = b.Split(CChar("."))

        For Index = 0 To m_a.Length - 1
            If Val(m_a(Index)) > Val(m_b(Index)) Then
                StrCompVersions = 1
                Exit For
            ElseIf Val(m_a(Index)) < Val(m_b(Index)) Then
                StrCompVersions = -1
                Exit For
            End If
        Next

    End Function

    Private Function SettingsDialog() As Integer

        Dim nvResult As Integer = Settings.ShowDialog()

        If nvResult = Windows.Forms.DialogResult.OK Then

            System.Windows.Forms.Application.DoEvents()

            idx_maj_encours = 0
            taille_enc = 0

            Init_AUService()

            Create_ListBoxEx()

            Label_Information.Text = mLanguageAU.GetString(MSG_DOWNLOAD_AND_INSTALL)
            InitializeInstall()
            Refresh_ListUpdateWeb()

            Label_Name_Maj.Visible = True

            'idx_maj_encours - 1 = idx_maj_max 'Toutes les MAJ sont OK

            If ForceUpdate Then

                If Not Button_Execute.Enabled And idx_maj_encours - 1 = idx_maj_max Then
                    Button_Execute.Enabled = True

                ElseIf Button_Execute.Enabled And idx_maj_encours - 1 < idx_maj_max Then
                    Button_Execute.Enabled = False

                End If

            End If

            '    nvResult = 0

            'ElseIf nvResult = Windows.Forms.DialogResult.Cancel Then
            '    nvResult = 0

            'ElseIf nvResult = Windows.Forms.DialogResult.Ignore Then
            '    nvResult = 0

        End If

        Return nvResult

    End Function

    Private Sub PictureSettings_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles PictureSettings.Click

        SettingsDialog()

    End Sub

    Private Sub RestartProg(Optional ByVal cmdline As String = Nothing)

        If IO.File.Exists(AppPath + DisplayNameReStart + ".exe") Then

            Dim MyProc As New Process()

            MyProc.StartInfo.WorkingDirectory = AppPath
            MyProc.StartInfo.FileName = DisplayNameReStart + ".exe"
            MyProc.StartInfo.UseShellExecute = False
            MyProc.StartInfo.CreateNoWindow = True
            MyProc.StartInfo.WindowStyle = ProcessWindowStyle.Hidden
            'MyProc.StartInfo.Arguments = Application.ExecutablePath
            MyProc.StartInfo.Arguments = CStr(IIf(cmdline <> Nothing, cmdline, Application.ExecutablePath))
            MyProc.Start()
            'MyProc.WaitForExit();

            'Process.Start(AppPath + DisplayNameReStart + ".exe", Application.ExecutablePath)

            Me.Close()

        Else

            HttpSendMessage(CODE_UPDATE_AU_SERV, , "File Not Found", , , DisplayNameReStart)

        End If

    End Sub

    Private Function FindComputerUserName(ByVal s As String) As Boolean

        'If s.ToLower() = (My.Computer.Name + "." + UserName).ToLower() Then
        'End If

        If StrComp(s, My.Computer.Name + "." + UserName, CompareMethod.Text) = 0 Then
            Return True
        Else
            Return False
        End If

    End Function

End Class


