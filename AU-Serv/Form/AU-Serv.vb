Option Strict On
Option Explicit On

Imports System.IO
Imports System.Net

Friend Class AUService

    Inherits System.Windows.Forms.Form

    Public EndOfFile As String = "##"

    Public AdmUser As String = "[ADM]"
    Public Skip As String = "[SKIP]"
    Public OnlyHostName As String = "[HOSTNAME]"

    Public DisplayNameReStart As String = "ReStart"
    Public QuickSupport As String = "TeamViewerQS_fr-idcah9cn6w.exe"

    ReadOnly CommandLine_ReStartAutoUpdate As String = "RESTART_AUTO_UPDATE"
    ReadOnly CommandLine_ReStartProg As String = "ReStart"
    ReadOnly CommandLine_InitializeInstall As String = "InitializeInstall"

    Public WithEvents Connection As CConnection
    Public WithEvents Lb As New ListBoxEX

    Public HostEntry As String
    Public UserName As String

    Public AUServiceIni, AppPath, svPatch, svCodeAppli, svFileMaj, svCompanyName, svSerie, svWebCompanyName, ExecuteFile, ExcludeSubDir As String
    Public bUseShellExecute, bUseProcessExecute, bQuickSupport As Boolean
    Public bSendRequest As Boolean = True
    Dim svNameAppli, svRemoteDir, AUServPath, ExecuteCmdLine, svSerial, svLocalDir, HostName, svPatchCopy As String
    Private ExecuteWait As Boolean
    Private ReadOnly KillProcess As Boolean
    Private Ping As Boolean
    Private ReadOnly ForceId As Boolean
    Private ReadOnly ForceWait As Boolean
    Private bOnlyDownload As Boolean
    Private bButtonExecuteVisible As Boolean
    Private ReadOnly ForceUpdate As Boolean
    Private ReadOnly StopedService As Boolean
    Dim tot_taille, taille_enc, fic_taille As Long
    Dim nWaitClose As Integer

    Public bAdmUser As Boolean

    Dim FileAcces As String
    Dim addUpdate As String

    ReadOnly IgnoreCRC As Boolean = False
    Public bForceSynchonize As Boolean = False
    Public bSynchonize As Boolean = False
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
    ReadOnly IgnoreSerial As Boolean = False
    Dim svSerialFound As String = ""
    Dim HostEntryName As String = ""
    Public LogUpdate As Boolean = False

    Public ErrorDeletePatchs As List(Of String) = New List(Of String)()

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

        Label_Version.Text = Application.ProductVersion.ToString()

        If Diagnostics.Process.GetProcessesByName(Diagnostics.Process.GetCurrentProcess.ProcessName).Length > 1 And Command$() <> CommandLine_ReStartProg And Command$() <> CommandLine_ReStartAutoUpdate Then
            Dim NumberOfProcess As Integer = 0
            Dim PathProcessCurrent As String = Diagnostics.Process.GetCurrentProcess.MainModule.FileName
            Dim NameProcessCurrent As String = Diagnostics.Process.GetCurrentProcess.ProcessName.ToUpper
            Dim Prc() As Process = Process.GetProcesses()
            For x As Integer = 0 To UBound(Prc)
                Dim pName As String = Prc(x).ProcessName.ToUpper(New System.Globalization.CultureInfo("en-US", False))
                If pName = NameProcessCurrent Then

                    Dim pPathProcess As String = Nothing
                    Try
                        pPathProcess = Prc(x).MainModule.FileName
                    Catch ex As Exception
                        'MsgBox(ex.Message, MsgBoxStyle.Critical)
                    End Try

                    If pPathProcess = PathProcessCurrent Then NumberOfProcess += 1
                    If NumberOfProcess > 1 Then
                        Dim szMesssage As String = mLanguageAU.GetString(MSD_APPLICATION_OPEN) + vbNewLine + vbNewLine + mLanguageAU.GetString(MSG_CONTINUE)
                        If MsgBox(szMesssage, CType(MsgBoxStyle.Exclamation + MsgBoxStyle.YesNo + MsgBoxStyle.DefaultButton2, MsgBoxStyle), My.Application.Info.Title) = MsgBoxResult.No Then
                            Me.Close()
                        Else
                            bAutomatic = False
                        End If
                    End If
                End If
            Next
        End If

        ''''''''''''''''
        '''  Execution du ExcuteBefore
        '''
        ''' 
        AUServiceIni = IO.Path.ChangeExtension(Application.ExecutablePath, ".ini")
        Dim executeBefore As String = Select_GetIniString("Parametres", "ExecuteBefore", AUServiceIni)

        If executeBefore <> "" Then
            Dim MyProcBefore As New Process()
            MyProcBefore.StartInfo.WorkingDirectory = executeBefore.Substring(0, executeBefore.LastIndexOf("\") + 1)
            MyProcBefore.StartInfo.FileName = executeBefore
            MyProcBefore.Start()
        End If

        Connection = New CConnection

        Init_AUService()

        If Command$() = CommandLine_ReStartProg Then

            HttpSendMessage(CODE_UPDATE_AU_SERV, , , "Updated", FileVersionInfo.GetVersionInfo(Application.ExecutablePath).FileVersion, Application.ProductName)

        End If

        If IO.File.Exists(svLocalDir + "Info.txt") Then
            HttpSendMessage(CODE_PING)
        End If

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

        Dim AvailableFreeSpace As Double = My.Computer.FileSystem.GetDriveInfo(AUServPath).AvailableFreeSpace

        If FreeSpace > 0 Then
            If (FreeSpace * 1048576) > AvailableFreeSpace Then
                MsgBox(mLanguageAU.GetString(MSG_AVAILABLE_FREESPACE).Replace("%FREESPACE%", ConvertOctets(AvailableFreeSpace)), MsgBoxStyle.Critical, My.Application.Info.Title)
                AutorisationUpdate = False
                bAutomatic = False
            End If
        End If

        Label_Information.Text = mLanguageAU.GetString(MSG_DOWNLOAD_AND_INSTALL)

        Button_Execute.Text = mLanguageAU.GetString(BUTTON_LAUNCH)
        Button_Suivant.Text = mLanguageAU.GetString(BUTTON_UPDATE)
        Button_Fermer.Text = mLanguageAU.GetString(BUTTON_EXIT)

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

        If nvGetError = 0 Then

        ElseIf nvGetError = -1 Then
            bAutomatic = False
        ElseIf nvGetError = -2 Then
            bAutomatic = False
        ElseIf nvGetError = -3 Then

        ElseIf nvGetError = -4 Then

        ElseIf nvGetError = -5 Then
            bAutomatic = False
        ElseIf nvGetError = -6 Then

        ElseIf idx_maj_max < 0 Then

        Else

        End If

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

    Private Sub Lb_DoubleClick(ByVal sender As Object, ByVal e As System.EventArgs) Handles Lb.DoubleClick
        If Lb.SelectedIndex >= 0 Then
            Dim it As ListBoxEX.ItemEX
            it = Lb.Items(Lb.SelectedIndex)
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

    Public Function ArrayStringOfv2(ByVal f As ArrayList, ByVal findstring As String, Optional ByVal startindex As Integer = 0) As String

        For Each g As String In f
            Dim nvIndex As Integer = f.LastIndexOf(g)
            If g.Contains(findstring) And nvIndex > startindex Then
                Dim szReturn As String = g.Replace(findstring, Nothing)
                Return CStr(IIf(szReturn.Trim = Nothing, "ALL", szReturn))
            End If
        Next

        Return Nothing

    End Function

    Public Function ArrayBoolOf(ByVal f As ArrayList, ByVal findstring As String, Optional ByVal startindex As Integer = 0) As Boolean

        For Each g As String In f
            Dim nvIndex As Integer = f.LastIndexOf(g)
            If g.Contains(findstring) And nvIndex > startindex Then
                Return True
            End If
        Next

        Return False

    End Function

    Public Function CheckApp(Optional ByVal Wait As Boolean = False) As String

        Ping = False

        Dim svAddUpdate As String = Nothing

        If FileAcces = Nothing Or Wait = True Then
            FileAcces = Connection.GetFileAcces
        End If

        Return svAddUpdate

    End Function

    Private Function Internet_Update() As Integer

        Dim fic_source As String
        Dim fic_destination As String
        Dim TabAcces() As String
        Dim ColAcces() As String
        Dim nIndex As Integer
        Dim nPos As Integer
        Dim svResult As String

        If Not CheckCRC_Config() Then
            bAutomatic = False
            idx_maj_max = -1
            Return -6
        End If

        Label_Etat_Maj.Text = mLanguageAU.GetString(MSG_CONNECTING)
        System.Windows.Forms.Application.DoEvents()

        If Not Connection.DelegateOpenInternet Then
            Label_Etat_Maj.Text = mLanguageAU.GetString(MSG_DECONNECTED)
            Connection.Disconnect()
            idx_maj_max = -1
            Return -4
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
            Return -1
        End If

        If InStr(FileAcces, EndOfFile) = 0 Then
            idx_maj_max = -1
            Return -2
        End If

        TabAcces = Split(FileAcces, vbNewLine)
        If CheckCRC_Acces(TabAcces) = False Then
            idx_maj_max = -1
            Return -5
        End If

        Dim TabListAcces As ArrayList = New ArrayList
        Dim bStartUpdate As Boolean = False
        Dim bStartFilter As Boolean = False

        For Each s In TabAcces

            If s = EndOfFile Or s = Nothing Then Exit For
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

        If addUpdate <> Nothing Then TabListAcces.Insert(0, addUpdate)

        TabListAcces.Add(EndOfFile)

        Dim TabIndex As Integer = 0
        ReDim TabAcces(TabListAcces.Count)
        For Each s In TabListAcces
            TabAcces(TabIndex) = s.ToString
            TabIndex += 1
        Next

        System.Windows.Forms.Application.DoEvents()

        ReDim File(0)

        On Error Resume Next

        For Each fic_source In TabAcces

            If fic_source = EndOfFile Then
                nIndex -= 1
                Exit For
            End If

            Dim ByHostEntry As Boolean
            If fic_source.Contains(OnlyHostName) Then
                Dim nPosHostNameFound As Integer = InStr(fic_source, OnlyHostName, CompareMethod.Text)
                Dim szHostNameFound As String = Mid(fic_source, nPosHostNameFound + OnlyHostName.Length, fic_source.Length - nPosHostNameFound - OnlyHostName.Length + 1).Trim
                ByHostEntry = (StrComp(HostEntry, szHostNameFound, CompareMethod.Text) = 0)
                fic_source = Mid(fic_source, 1, nPosHostNameFound - 1)
            Else
                ByHostEntry = True
            End If

            If ((fic_source.Contains(AdmUser) And bAdmUser = True) Or Not fic_source.Contains(AdmUser)) And Not fic_source.Contains(Skip) And ByHostEntry Then

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

                File(nIndex).numMaj0 = ColAcces(0)
                File(nIndex).fic_source1 = ColAcces(1)
                File(nIndex).taille2 = ColAcces(2)
                File(nIndex).CheckSum3 = ColAcces(3)

                System.Windows.Forms.Application.DoEvents()

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
                    nvCountLine += 1
                End While

                System.Windows.Forms.Application.DoEvents()

                Dim nPosCrLf As Integer = InStrRev(ColAcces(4), "\r\n") - 1
                If nPosCrLf = (ColAcces(4).Length - 4) Then
                    ColAcces(4) = Mid(ColAcces(4), 1, nPosCrLf)
                End If

                File(nIndex).infoMaj4 = Replace(ColAcces(4), "\r\n", vbNewLine, , , CompareMethod.Text)
                File(nIndex).fic_destination5 = svCodeAppli & Strings.Right(("0000" + Trim(ColAcces(0))), 4) & "_" & ColAcces(1)

                System.Windows.Forms.Application.DoEvents()

                tot_taille = CLng(tot_taille + Val(ColAcces(2)))

                Dim a() As String = Split(File(nIndex).infoMaj4, vbNewLine)
                If a.Length > 11 Then
                    ReDim Preserve a(7)
                    a(7) = "etc..."
                    File(nIndex).infoMaj4 = Join(a, vbNewLine)
                End If

                If (CBool(IIf(svRemoteDir <> Nothing, IO.File.Exists(svRemoteDir + File(nIndex).fic_destination5), False)) Or IO.File.Exists(svLocalDir + File(nIndex).fic_destination5)) Then
                    Lb.Add(File(nIndex).infoMaj4, mLanguageAU.GetString(MSG_UPDATE_NUMBER) & " " & File(nIndex).numMaj0, 4)
                Else
                    Lb.Add(File(nIndex).infoMaj4, mLanguageAU.GetString(MSG_UPDATE_NUMBER) & " " & File(nIndex).numMaj0, 5)
                End If

                nIndex += 1

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
        p = Application.ExecutablePath
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

        AppPath = Application.StartupPath & "\"
        AUServPath = IO.Path.GetDirectoryName(Application.ExecutablePath)
        AUServiceIni = IO.Path.ChangeExtension(Application.ExecutablePath, ".ini")
        If IO.File.Exists(AUServiceIni) = False Then Me.Close()

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

        Dim svLanguage As String = UCase(Select_GetIniString("Parametres", "Language", AUServiceIni))
        If svLanguage = "EN" Then
            SetCurrentLanguage(0)
        ElseIf svLanguage = "FR" Then
            SetCurrentLanguage(1)
        ElseIf svLanguage = "IT" Then
            SetCurrentLanguage(2)
        ElseIf svLanguage = "PL" Then
            SetCurrentLanguage(3)
        Else
            SetCurrentLanguage(1)
        End If

        svPatch = Select_GetIniString("Parametres", "Patch", AUServiceIni)
        svRemoteDir = Select_GetIniString("Parametres", "RemoteDir", AUServiceIni)
        svLocalDir = Select_GetIniString("Parametres", "LocalDir", AUServiceIni)
        svPatchCopy = Select_GetIniString("Parametres", "PatchCopy", AUServiceIni)
        bRemovePatch = Not (Select_GetIniString$("Parametres", "RemovePatch", AUServiceIni$) = "0")

        Dim svLogUpdate As String = Select_GetIniString("Parametres", "LogUpdate", AUServiceIni).Trim
        If svLogUpdate = Nothing Then
            svLogUpdate = "1"
            Select_WriteIniString("Parametres", "LogUpdate", svLogUpdate, AUServiceIni)
        End If
        LogUpdate = (svLogUpdate = "1")

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

        bAdmUser = (Select_GetIniString$("Parametres", "AdmUser", AUServiceIni$) = "1")

        svCodeAppli = Trim(Select_GetIniString("Parametres", "CodeAppli", AUServiceIni))
        svNameAppli = Trim(Select_GetIniString("Parametres", "NameAppli", AUServiceIni))

        bSendRequest = Not (Select_GetIniString("Parametres", "SendRequest", AUServiceIni) = "0")

        bQuickSupport = (StrComp(Select_GetIniString("Parametres", "QuickSupport", AUServiceIni), "Disable", CompareMethod.Text) <> 0)

        bOnlyDownload = (Select_GetIniString("Parametres", "OnlyDownload", AUServiceIni) = "1")

        Dim sWaitClose = Trim(Select_GetIniString("Parametres", "WaitClose", AUServiceIni))
        If sWaitClose = "" Then
            nWaitClose = 4
        Else
            nWaitClose = CInt(sWaitClose)
        End If

        ExecuteFile = Select_GetIniString("Parametres", "Execute", AUServiceIni)

        bUseShellExecute = (Select_GetIniString("Parametres", "UseShellExecute", AUServiceIni) = "1")
        bUseProcessExecute = Not (Select_GetIniString("Parametres", "UseProcessExecute", AUServiceIni) = "0")

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

        bButtonExecuteVisible = IO.File.Exists(ExecuteFile)

        If bAutomatic = True Then
            bAutomatic = bButtonExecuteVisible And Not (Select_GetIniString$("Parametres", "Automatic", AUServiceIni) = "0")
        End If

        bAutomaticUpdate = (Select_GetIniString$("Parametres", "AutomaticUpdate", AUServiceIni) = "1")

        HostEntry = Trim(Select_GetIniString("Parametres", "Name", AUServiceIni))

        Try
            HostEntryName = Dns.GetHostEntry(Dns.GetHostName).HostName
        Catch ex As Exception
            HostEntryName = My.Computer.Name
        End Try

        If HostEntry = "" Then
            HostEntry = HostEntryName
        End If

        If HostName = "" Then
            Try
                HostName = Dns.GetHostName
            Catch ex As Exception
                HostName = My.Computer.Name
            End Try
        End If

        svCompanyName = Trim(Select_GetIniString("Parametres", "Company", AUServiceIni))
        svWebCompanyName = Url_Encode(svCompanyName)

        Me.Text = "AutoUpdate Services - " & svNameAppli & " - " & svCompanyName

        svSerial = Select_GetIniString("Parametres", "Serial", AUServiceIni)
        Dim CurrentMacAddress As String = Nothing
        If svSerial.Length >= 12 Then CurrentMacAddress = Strings.Right(svSerial, 12)

        Dim svCle As String = Select_GetIniString("Parametres", "Cle", AUServiceIni)
        Dim svSerialVirtual As String = Select_GetIniString("Parametres", "SerialVirtual", AUServiceIni)
        Dim svSerialGroup As String = Select_GetIniString("Parametres", "SerialGroup", AUServiceIni)
        MacAddress.bVirtual = (svSerialVirtual <> "") Or (svSerialGroup <> "")
        MacAddress.bDebug = (Select_GetIniString("Parametres", "DebugNetwork", AUServiceIni) = "1")

        Dim GetMacAddress As String = MacAddress.GetValue(CurrentMacAddress)
        Dim GetHddSerial As String = HddSerial()
        svSerialFound = GetHddSerial & GetMacAddress

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

        If Command$() = CommandLine_InitializeInstall Then
            Select_WriteIniString("Parametres", "Cle", svCleFound, AUServiceIni)
            Select_WriteIniString("Parametres", "Serial", svSerialFound, AUServiceIni)
            Me.Close()
        ElseIf UCase(Command$()) = CommandLine_ReStartAutoUpdate Then
            bLaunchUpdateAuto = True
        End If

        svFileMaj = svWebCompanyName & "." & svCryptage

        Dim svProxyLogin As String = ""
        Dim svProxyPassword As String = ""
        Dim svProxy As String = Select_GetIniString("Parametres", "Proxy", AUServiceIni)

        If svProxy <> "" Then
            Dim nPoshttp As Integer = InStr(svProxy, "://")
            Dim svEntetehttp As String = Nothing

            If nPoshttp > 0 Then
                nPoshttp += 3
                svEntetehttp = Mid(svProxy, 1, nPoshttp - 1)
                svProxy = Mid(svProxy, nPoshttp, Len(svProxy) - nPoshttp + 1)
            End If

            Dim nPosProxy As Short = CShort(InStr(svProxy, "@"))
            If nPosProxy > 0 Then
                Dim nPosPwd As Short = CShort(InStr(svProxy, ":"))
                svProxyLogin = Mid(svProxy, 1, nPosPwd - 1)
                svProxyPassword = Mid(svProxy, nPosPwd + 1, nPosProxy - nPosPwd - 1)
                svProxy = Mid(svProxy, nPosProxy + 1, Len(svProxy) - nPosProxy)
            End If
            svProxy = svEntetehttp + svProxy
        End If

        Dim nBuffer As Integer = CInt(Val(Select_GetIniString("Parametres", "Buffer", AUServiceIni)))
        If nBuffer = 0 Then nBuffer = 8

        With Connection
            .Proxy = svProxy
            .ProxyLog = svProxyLogin
            .ProxyPwd = svProxyPassword
            .DirectConnect = (Select_GetIniString("Parametres", "DirectConnect", AUServiceIni) = "1")
            .InternetBuffer = nBuffer
            .DebugMode = (Select_GetIniString("Parametres", "DebugMode", AUServiceIni) = "1")
            .InternetFlagsMask = CLng(Val(Select_GetIniString("Parametres", "InternetFlagsMask", AUServiceIni)))
            .RemoteDir = Connection.GetRemoteDir
        End With


        '''''
        ''' Recherche du ProductCode si il a été supprimé
        '''''

        If svSerie = "" Then

            ' je vais rechercher sur le serveur le ProductCode qui correspond au Serial
            If svSerialGroup <> "" Then
                svSerie = Connection.GetProductCode(svSerialGroup)
            ElseIf svSerialVirtual <> "" Then
                svSerie = Connection.GetProductCode(svSerialVirtual)
            ElseIf svSerial <> "" Then
                svSerie = Connection.GetProductCode(svSerial)
            Else
                Select_WriteIniString("Parametres", "Serial", svSerialFound, AUServiceIni)
                svSerie = Connection.GetProductCode(svSerialFound)
            End If

            Select_WriteIniString("Parametres", "ProductCode", svSerie, AUServiceIni)

        End If


        If Mid(svLocalDir, 2, 1) <> ":" And Mid(svLocalDir, 2, 1) <> "\" Then svLocalDir = AppPath + svLocalDir

        svLocalDir = Add(svLocalDir, BACKSLASH)

        Dim svServerName As String = Select_GetIniString("Parametres", "ServerName", AUServiceIni)
        If svServerName <> Nothing Then
            svServerName = CStr(IIf(svServerName.IndexOf("http") = 0, svServerName, "https://" & svServerName))
            Connection.ServerName = svServerName
        End If

        If Not IO.Directory.Exists(svLocalDir) Then
            If Strings.Left(svLocalDir, 2) <> "\\" Then
                IO.Directory.CreateDirectory(svLocalDir)
            End If
            If Not IO.Directory.Exists(svLocalDir) Then
                MsgBox(mLanguageAU.GetString(MSG_PATH_NOT_FOUND) & vbNewLine & vbNewLine & svLocalDir, MsgBoxStyle.Critical And MsgBoxStyle.SystemModal, My.Application.Info.Title)
                Me.Close()
            End If
        End If

        If Connection.InitializeUpdate(AppVersion, (Select_GetIniString("Parametres", "TestUpdate", AUServiceIni) = "1")) Then
            Me.Visible = False
            RestartProg()
        End If

        addUpdate = CheckApp()

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

        If MacAddress.bVirtual Then
            Select_WriteIniString("Parametres", "Id", UserName, AUServiceIni)
            HostEntry = HostName + "." + UserName
        End If

        If svSerie = "" And svSerialVirtual = "" And svSerialGroup = "" Then

DlgProductCode:
            DialogProductCode.ShowDialog()
            If DialogProductCode.DialogResult = Windows.Forms.DialogResult.Cancel Then
                Select_WriteIniString("Parametres", "Serial", "", AUServiceIni)
                If svSerial = "" Then
                    svSerial = svSerialFound
                End If
                HttpSendMessage(CODE_SERIAL_MISSING)
                Me.Visible = False
                Me.Close()

            Else

                svSerie = DialogProductCode.svSerie

                Dim svCompanyNameInstall As String = Connection.GetAutorisationInstall(svSerialFound, svSerie)

                If svCompanyName = "" Or svCompanyNameInstall <> svCompanyName Then

                    Try
                        Dim nvCodeAutorisation As Integer = CInt(svCompanyNameInstall)

                        If svCompanyName = "" Then

                            If nvCodeAutorisation = -7 Then
                                If MsgBox(mLanguageAU.GetString(MSG_FAIL_CONNECT_INTERNET) + " (Exception: " + Str(nvCodeAutorisation).Trim + ")" + vbNewLine + vbNewLine + mLanguageAU.GetString(MSG_CHANGE_SETTINGS), MsgBoxStyle.Critical Or MsgBoxStyle.YesNo Or MsgBoxStyle.DefaultButton2, My.Application.Info.Title & " : " & svSerie) = MsgBoxResult.Yes Then

                                    If SettingsDialog() = Windows.Forms.DialogResult.Cancel Then
                                        Me.Visible = False
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

                        Me.Text = "AutoUpdate Services - " & svNameAppli & " - " & svCompanyName

                        HttpSendMessage(CODE_INSTALLATION, , svSerie, "Install")

                    End Try

                Else

                    Select_WriteIniString("Parametres", "ProductCode", svSerie, AUServiceIni)

                    HttpSendMessage(CODE_INSTALLATION, , svSerie, "Install")


                End If

                svCle = svCleFound

            End If

        ElseIf svSerie = "" Then
            MsgBox(mLanguageAU.GetString(MSG_PRODUCTCODE_MISSING), MsgBoxStyle.Critical, My.Application.Info.Title & " : " & svSerialFound)
            HttpSendMessage(CODE_SERIAL_MISSING)
            Me.Visible = False
            Me.Close()

        Else
            svSerie = svSerie.Replace("-", "")

        End If

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

        Dim ChecksumToday As String = Crypter(Date.Now.ToShortDateString + svSerialChecksum, "sha1")
        Dim Checksum As String = Select_GetIniString("Parametres", "Checksum", AUServiceIni)

        If nvCode = 1 Then

            If svSerialGroup <> "" And svSerial <> "" And svSerial <> svSerialFound Then
                MsgBox(mLanguageAU.GetString(MSG_INVALID_PARAMETERS) + " (Exception: " + Str(nvCode).Trim + ")", MsgBoxStyle.Critical, My.Application.Info.Title & " : " & svSerie)
                HttpSendMessage(CODE_INVALID_PARAMETERS, , svSerie, "Parameter")
                Select_WriteIniString("Parametres", "Serial", "", AUServiceIni)
                Me.Visible = False
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
                    Me.Visible = False
                    Me.Close()
                End If

            End If

            If Checksum <> ChecksumToday Then
                Select_WriteIniString("Parametres", "Checksum", ChecksumToday, AUServiceIni)
            End If

        ElseIf nvCode < 0 Then

            If nvCode = -1 Then

                If svCle = "" Then
                    Select_WriteIniString("Parametres", "ProductCode", "", AUServiceIni)
                    svSerie = ""
                    GoTo DlgProductCode
                End If

                If svSerial = "" Then
                    svSerial = svSerialFound
                End If

                If Not IgnoreSerial Then

                    HttpSendMessage(CODE_INVALID_PARAMETERS, , svSerie, "Parameter")
                    MsgBox(mLanguageAU.GetString(MSG_INVALID_PARAMETERS) + " (Exception: " + Str(nvCode).Trim + ")", MsgBoxStyle.Critical, My.Application.Info.Title & " : " & svSerie)

                    Select_WriteIniString("Parametres", "Serial", "", AUServiceIni)
                    Me.Visible = False
                    Me.Close()

                End If

            ElseIf nvCode = -2 Then
                Select_WriteIniString("Parametres", "Serial", "", AUServiceIni)
                MsgBox(mLanguageAU.GetString(MSG_LICENSE_BLOCKED) + " (Exception: " + Str(nvCode).Trim + ")", MsgBoxStyle.Critical, My.Application.Info.Title & " : " & svSerie)
                If svSerial = "" Then
                    svSerial = svSerialFound
                End If
                HttpSendMessage(CODE_LICENSE_LOCKED, , svSerie, "Lock")
                Me.Visible = False
                Me.Close()

            ElseIf nvCode = -3 Then
                Select_WriteIniString("Parametres", "Cle", "", AUServiceIni)
                Select_WriteIniString("Parametres", "Serial", "", AUServiceIni)
                Select_WriteIniString("Parametres", "ProductCode", "", AUServiceIni)
                MsgBox(mLanguageAU.GetString(MSG_LICENSE_EXPIRED) + " (Exception: " + Str(nvCode).Trim + ")", MsgBoxStyle.Critical, My.Application.Info.Title & " : " & svSerie)
                If svSerial = "" Then
                    svSerial = svSerialFound
                End If
                HttpSendMessage(CODE_LICENSE_EXPIRED, , svSerie, "Expire")
                Me.Visible = False
                Me.Close()

            ElseIf nvCode = -4 Then
                If Not IgnoreSerial Then
                    Select_WriteIniString("Parametres", "Serial", "", AUServiceIni)
                    Select_WriteIniString("Parametres", "ProductCode", "", AUServiceIni)
                    MsgBox(mLanguageAU.GetString(MSG_PRODUCTCODE_INVALID) + " (Exception: " + Str(nvCode).Trim + ")", MsgBoxStyle.Critical, My.Application.Info.Title & " : " & svSerie)
                    If svSerial = "" Then
                        svSerial = svSerialFound
                    End If
                    HttpSendMessage(CODE_INVALID_SERIAL, , svSerie, "Unknown")
                    Me.Visible = False
                    Me.Close()
                End If

            ElseIf nvCode = -5 Then
                If svCle = "" Then
                    svCle = svCleFound
                    Select_WriteIniString("Parametres", "Cle", svCle, AUServiceIni)

                    If Select_GetIniString("Parametres", "Cle", AUServiceIni) <> svCle Then
                        MsgBox(mLanguageAU.GetString(MSG_INITIALIZING_ERROR) + " (Exception: " + Str(nvCode).Trim + ")", MsgBoxStyle.Critical, My.Application.Info.Title)
                        Me.Visible = False
                        Me.Close()
                    End If

                End If
                Select_WriteIniString("Parametres", "Serial", svSerialFound, AUServiceIni)

                If Select_GetIniString("Parametres", "Serial", AUServiceIni) <> svSerialFound Then
                    MsgBox(mLanguageAU.GetString(MSG_INITIALIZING_ERROR) + " (Exception: " + Str(nvCode).Trim + ")", MsgBoxStyle.Critical, My.Application.Info.Title)
                    Me.Visible = False
                    Me.Close()
                End If

                If Checksum <> ChecksumToday Then
                    Select_WriteIniString("Parametres", "Checksum", ChecksumToday, AUServiceIni)
                End If

                svSerial = svSerialFound
                HttpSendMessage(CODE_RESET_SERIAL, , svSerie, "Reset")

            ElseIf nvCode = -6 Then
                Select_WriteIniString("Parametres", "ProductCode", "", AUServiceIni)
                Select_WriteIniString("Parametres", "Serial", "", AUServiceIni)

                Dim svMsg As String = mLanguageAU.GetString(MSG_INVALID_COMPANY_NAME) + " (Exception: " + Str(nvCode).Trim + ")"
                svMsg = svMsg.Replace("#COMPANY#", Chr(34) + svCompanyName + Chr(34))
                MsgBox(svMsg, MsgBoxStyle.Critical, My.Application.Info.Title & " : " & svSerie)
                If svSerial = "" Then
                    svSerial = svSerialFound
                End If
                HttpSendMessage(CODE_INCORRECT_LICENCE_COMPANY, , svSerie, "Incorrect")
                Me.Visible = False
                Me.Close()

            ElseIf nvCode = -7 Then
                Select_WriteIniString("Parametres", "Serial", "", AUServiceIni)
                MsgBox(mLanguageAU.GetString(MSC_INCORRECT_PARAMETER) + " (Exception: " + Str(nvCode).Trim + ")", MsgBoxStyle.Critical, My.Application.Info.Title & " : " & svSerie)
                HttpSendMessage(CODE_INCORRECT_PARAMETERS, , svSerie, "Incorrect")
                Me.Visible = False
                Me.Close()

            End If

        ElseIf nvCode = 0 Then

            If Not ChecksumValidate(Checksum, svSerialChecksum) Then

                If MsgBox(mLanguageAU.GetString(MSG_FAIL_CONNECT_INTERNET) + " (Exception: " + Str(nvCode).Trim + ")" + vbNewLine + vbNewLine + mLanguageAU.GetString(MSG_CHANGE_SETTINGS), MsgBoxStyle.Critical Or MsgBoxStyle.YesNo Or MsgBoxStyle.DefaultButton2, My.Application.Info.Title & " : " & svSerie) = MsgBoxResult.Yes Then

                    If SettingsDialog() = Windows.Forms.DialogResult.Cancel Then
                        Me.Visible = False
                        Me.Close()
                    End If

                Else
                    Me.Visible = False
                    Me.Close()
                End If

            End If

        End If

        If svSerialFound <> svSerial And IgnoreSerial = False Then
            MsgBox(mLanguageAU.GetString(MSG_SERIAL_INCORRECT), MsgBoxStyle.Critical, My.Application.Info.Title & " : " & svSerial)
            svSerial = svSerialFound
            HttpSendMessage(CODE_SERIAL_INCORRECT, , svSerie, "Incorrect")
            Me.Visible = False
            Me.Close()
        ElseIf svCleFound <> svCle Then
            MsgBox(mLanguageAU.GetString(MSG_KEY_INVALID), MsgBoxStyle.Critical, My.Application.Info.Title & " : " & svCle)
            HttpSendMessage(CODE_KEY_INVALID, , svCle, "key")
            Me.Visible = False
            Me.Close()
        End If

        If Select_GetIniString("Parametres", "ProductCode", AUServiceIni) = "" Then
            Select_WriteIniString("Parametres", "ProductCode", svSerie, AUServiceIni)

            If Select_GetIniString("Parametres", "ProductCode", AUServiceIni) <> svSerie Then
                MsgBox(mLanguageAU.GetString(MSG_INITIALIZING_ERROR) + " (ProductCode)", MsgBoxStyle.Critical, My.Application.Info.Title)
                Me.Visible = False
                Me.Close()
            End If

            If Select_GetIniString("Parametres", "Cle", AUServiceIni) = "" Then
                Select_WriteIniString("Parametres", "Cle", svCleFound, AUServiceIni)

                If Select_GetIniString("Parametres", "Cle", AUServiceIni) <> svCleFound Then
                    MsgBox(mLanguageAU.GetString(MSG_INITIALIZING_ERROR) + " (Cle)", MsgBoxStyle.Critical, My.Application.Info.Title)
                    Me.Visible = False
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
                Me.Visible = False
                Me.Close()
            End If
        End If

        If svRemoteDir.Contains("/") Then
            svRemoteDir = Nothing
        ElseIf svRemoteDir <> Nothing Then

            svRemoteDir = Add(svRemoteDir, BACKSLASH)

            If Not IO.Directory.Exists(svRemoteDir) Then
                If Strings.Left(svRemoteDir, 2) <> "\\" Then
                    IO.Directory.CreateDirectory(svRemoteDir)
                End If
                If Not IO.Directory.Exists(svRemoteDir) Then
                    MsgBox(mLanguageAU.GetString(MSG_PATH_NOT_FOUND) & vbNewLine & vbNewLine & svRemoteDir, MsgBoxStyle.Critical And MsgBoxStyle.SystemModal, My.Application.Info.Title)
                    Me.Visible = False
                    Me.Close()
                End If
            End If

        End If


        If svPatchCopy <> "" Then

            svPatchCopy = Add(svPatchCopy, BACKSLASH)

            If Not IO.Directory.Exists(svPatchCopy) Then
                If Strings.Left(svPatchCopy, 2) <> "\\" Then
                    IO.Directory.CreateDirectory(svPatchCopy)
                End If
                If Not IO.Directory.Exists(svPatchCopy) Then
                    MsgBox(mLanguageAU.GetString(MSG_PATH_NOT_FOUND) & vbNewLine & vbNewLine & svPatchCopy, MsgBoxStyle.Critical And MsgBoxStyle.SystemModal, My.Application.Info.Title)
                    Me.Visible = False
                    Me.Close()
                End If
            End If

        End If

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

        Dim svMaxDay As Integer

        If svCompanyName = "Eurozet" Then
            svMaxDay = 21
        Else
            svMaxDay = 5
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

            If PercentBar1 <> ProgressBar1.Value Then
                ProgressBar1.Value = PercentBar1
            End If

            If PercentBar2 <> ProgressBar2.Value Then
                ProgressBar2.Value = PercentBar2
            End If

            Label_Percent_Maj.Text = Convert_Octet2KOctet(nValue) & " to " &
                Convert_Octet2KOctet(tot_taille) & " (" &
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

        Dim nvReturn As Integer = Internet_Update()

        If nvReturn = -1 Then
            Label_Info_Maj.Text = mLanguageAU.GetString(MSG_ACCES_MISSING)
        ElseIf nvReturn = -2 Then
            Label_Info_Maj.Text = mLanguageAU.GetString(MSG_ACCES_INVALID)
        ElseIf nvReturn = -3 Then
            Label_Info_Maj.Text = mLanguageAU.GetString(MSG_ACCES_NO_SYNCHRO)
        ElseIf nvReturn = -4 Then
            Label_Info_Maj.Text = mLanguageAU.GetString(MSG_NOT_INTERNET)
        ElseIf nvReturn = -5 Then
            Label_Info_Maj.Text = mLanguageAU.GetString(MSG_ACCES_INVALID) + " (CRC)"
        ElseIf nvReturn = -6 Then
            Label_Info_Maj.Text = mLanguageAU.GetString(MSG_ACCES_NO_SYNCHRO) + " (CRC)"
        ElseIf idx_maj_max < 0 Then
            Label_Info_Maj.Text = mLanguageAU.GetString(MSG_NOT_UPDATE)
            Button_Suivant.Enabled = False
        Else
            Label_Info_Maj.Text = mLanguageAU.GetString(MSG_LIST_UPDATE)
            Button_Suivant.Enabled = (True And AutorisationUpdate)
        End If

        Lb.Visible = True

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

        Dim ScrollBar_Position As Integer = Lb.IndexItem

        Lb.Replace(idx, File(idx).infoMaj4, mLanguageAU.GetString(MSG_UPDATE_NUMBER) & " " & File(idx).numMaj0, nvImage)

        If ScrollBar_Position = idx - 1 And nvImage = 1 Then
            Lb.IndexItem = idx
        Else
            Lb.IndexItem = ScrollBar_Position
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

        Lb.Clear()

        Lb.AddIcon(AUServ.My.Resources.Resources.Harddrive)
        Lb.AddIcon(AUServ.My.Resources.Resources.Download)
        Lb.AddIcon(AUServ.My.Resources.Resources.Execute)
        Lb.AddIcon(AUServ.My.Resources.Resources.FolderMoveto)
        Lb.AddIcon(AUServ.My.Resources.Resources.FolderClosed)
        Lb.AddIcon(AUServ.My.Resources.Resources.Globe)
        Lb.AddIcon(AUServ.My.Resources.Resources.FolderOk)
        Lb.AddIcon(AUServ.My.Resources.Resources.FolderOpen)
        Lb.AddIcon(AUServ.My.Resources.Resources.FolderSecurity)

        Dim nvMargin As Integer = Label_Information.Margin.All * 2
        Dim X As Integer = Label_Information.Location.X
        Dim Y As Integer = nvMargin + Label_Information.Location.Y + Label_Information.Size.Height
        Lb.Location = New Point(X, Y)
        Lb.Width = Me.Size.Width - nvMargin - Label_Information.Location.X * 2

        Lb.Height = Label_Etat_Maj.Location.Y - Y - Label_Information.Margin.All * 2

        Lb.Margin = Label_Information.Margin
        Lb.Visible = False

        System.Windows.Forms.Application.DoEvents()

        Me.Controls.Add(Lb)

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

                Refresh_Listbox(idx_maj_encours, 1)
                System.Windows.Forms.Application.DoEvents()

                If Not Connection.GetFileToFile Then
                    Refresh_Listbox(idx_maj_encours, 5)
                    Exit Do
                End If

                WriteInfoToFile(IO.Path.ChangeExtension(fic_destination, ".txt"), File(idx_maj_encours).infoMaj4)

                HttpSendMessage(CODE_DOWNLOADED, idx_maj_encours)

                Refresh_Listbox(idx_maj_encours, 0)

            End If

            System.Windows.Forms.Application.DoEvents()

            taille_enc += fic_taille

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
                    Refresh_Listbox(idx_maj_encours, 8)
                    taille_enc -= fic_taille
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
                    Refresh_Listbox(idx_maj_encours, 8)
                    taille_enc -= fic_taille
                    Connection_ProgressChanged(0)
                    System.Windows.Forms.Application.DoEvents()
                    Exit Do
                Else
                    HttpSendMessage(CODE_COPIED, idx_maj_encours)
                End If

                WriteInfoToFile(IO.Path.ChangeExtension(IO.Path.Combine(svPatchCopy, fic_source), ".txt"), File(idx_maj_encours).infoMaj4)

                Try
                    If IO.File.Exists(fic_destination) Then
                        IO.File.Delete(fic_destination)
                    End If
                Catch ex As Exception
                    ErrorDeletePatchs.Add(fic_destination)
                End Try

                Refresh_Listbox(idx_maj_encours, 6)
                System.Windows.Forms.Application.DoEvents()

            ElseIf bRemovePatch Then
                Try
                    If IO.File.Exists(fic_destination) Then
                        IO.File.Delete(fic_destination)
                    End If
                Catch ex As Exception
                    ErrorDeletePatchs.Add(fic_destination)
                End Try
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
                        AddCRC_Config()

                        svUpdate = ""
                        nvStartUpdate = 0

                    End If
                Else
                    svPatch = File(idx_maj_encours).fic_source1
                    Select_WriteIniString("Parametres", "Patch", svPatch, AUServiceIni)
                    AddCRC_Config()
                End If

            End If

            If File(idx_maj_encours).restart Then
                Me.Visible = False
                RestartProg("RESTART_AUTO_UPDATE")
            End If

            idx_maj_encours += CShort(1)

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

        For Each patch As String In ErrorDeletePatchs
            Try
                If IO.File.Exists(patch) Then
                    IO.File.Delete(patch)
                End If
            Catch ex As Exception

            End Try
        Next

    End Sub

    Public Sub CopyFileExProgress(ByRef nBytes As Short)

        Label_Etat_Maj.Text = mLanguageAU.GetString(MSG_COPY_UPDATE_NUMBER)
        Label_Name_Maj.Location = New Point(Label_Etat_Maj.Location.X + Label_Etat_Maj.Size.Width, Label_Name_Maj.Location.Y)
        Label_Name_Maj.Text = File(idx_maj_encours).numMaj0

        ProgressBar2.Value = nBytes
        System.Windows.Forms.Application.DoEvents()

    End Sub

    Public Function HttpSendMessage(ByVal code As Integer, Optional ByVal nvIndex As Integer = 0, Optional ByVal info As String = Nothing, Optional ByVal maj As String = Nothing, Optional ByVal version As String = Nothing, Optional ByVal logiciel As String = Nothing) As String

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
                info = svNameAppli
            End If

            Dim svInfoTxt As String = ReadInfoToFile(svLocalDir + "Info.txt")
            If svInfoTxt <> Nothing Then
                info = svInfoTxt
            End If

            Dim URL_MAJ As String = Connection.ServerName + "/update.asp?" + "sn=" + svSerial & "&login=" + svWebCompanyName & "&logiciel=" & logiciel & "&etat=" & code.ToString & "&host="

            Dim svMAJ As String = IIf(maj <> Nothing, "&maj=" & maj, "").ToString
            Dim svVERSION As String = IIf(version <> Nothing, "&version=" & version, "").ToString
            Dim svINFO As String = IIf(info <> Nothing, "&info=" & info, "").ToString

            Dim svHostEntry As String = HostEntry

            If StrComp(UserName, info, CompareMethod.Text) = 0 Then
                svHostEntry = HostEntryName
            End If

            Dim SendMessage As String = URL_MAJ & svHostEntry & svMAJ & svVERSION & svINFO

            If Len(SendMessage) > 255 Then
                SendMessage = URL_MAJ & Truncate(svHostEntry, 40) & svMAJ & svVERSION & svINFO
            End If

            If Len(SendMessage) > 255 Then
                SendMessage = URL_MAJ & Truncate(svHostEntry, 40) & svMAJ & svINFO
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

    Private Function Truncate(ByVal svString As String, ByVal nvLen As Integer) As String

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

                MyProc.StartInfo.WorkingDirectory = IO.Path.GetDirectoryName(szPathFile)
                MyProc.StartInfo.FileName = szPathFile
                MyProc.StartInfo.WindowStyle = ProcessWindowStyle.Normal
                MyProc.StartInfo.Arguments = svCommand
                MyProc.Start()
                lPid_Execute = MyProc.Id
            Else
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

                                Time += 1
                                If Time >= 60 Then
                                    Time = 0

                                    CheckApp(True)

                                    If svCodeAppli = "R" Then
                                        Dim BoucleWait As Integer = 0
                                        Do
                                            Sleep(20000)
                                            If BoucleWait = 10 Then
                                                If FEnumWindows("ClassATSAUVE32") = "" And FEnumWindows("WinPassClass") = "" Then Exit Do
                                                BoucleWait = 0
                                            End If
                                            BoucleWait += 1
                                        Loop While FindWindowHwnd("TOMEVAL") = 0

                                        While FindWindowHwnd("TOMEVAL") <> 0 Or FEnumWindows("AtelierTVClass32") <> ""
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

        ElseIf Label_Info_Maj.Text.Contains(mLanguageAU.GetString(MSG_ACCES_NO_SYNCHRO)) Then
            If MsgBox(mLanguageAU.GetString(MSG_ACCES_NO_SYNCHRO) + vbNewLine + vbNewLine + mLanguageAU.GetString(MSG_CONTINUE), MsgBoxStyle.Exclamation Or MsgBoxStyle.YesNo Or MsgBoxStyle.DefaultButton2, My.Application.Info.Title) = MsgBoxResult.Yes Then
                HttpSendMessage(CODE_UPDATE_STOPED, , "ErrorSynchro")
                Excute_Application()
            End If

        ElseIf Label_Info_Maj.Text = mLanguageAU.GetString(MSG_ACCES_MISSING) Then
            If MsgBox(mLanguageAU.GetString(MSG_ACCES_MISSING) + vbNewLine + vbNewLine + mLanguageAU.GetString(MSG_CONTINUE), MsgBoxStyle.Exclamation Or MsgBoxStyle.YesNo Or MsgBoxStyle.DefaultButton2, My.Application.Info.Title) = MsgBoxResult.Yes Then
                HttpSendMessage(CODE_UPDATE_STOPED, , "AccesMissing")
                Excute_Application()
            End If

        ElseIf Label_Info_Maj.Text.Contains(mLanguageAU.GetString(MSG_ACCES_INVALID)) Then
            If MsgBox(mLanguageAU.GetString(MSG_ACCES_INVALID) + vbNewLine + vbNewLine + mLanguageAU.GetString(MSG_CONTINUE), MsgBoxStyle.Exclamation Or MsgBoxStyle.YesNo Or MsgBoxStyle.DefaultButton2, My.Application.Info.Title) = MsgBoxResult.Yes Then
                HttpSendMessage(CODE_UPDATE_STOPED, , "AccesInvalid")
                Excute_Application()
            End If

        ElseIf Label_Info_Maj.Text = mLanguageAU.GetString(MSG_NOT_UPDATE) Then
            Excute_Application()
        End If

    End Sub

    Private Function DeleteFileAndFolder(ByVal path As String) As Boolean

        For Each pFile As String In IO.Directory.GetFiles(path)
            IO.File.Delete(pFile)
        Next

        For Each pFolder As String In IO.Directory.GetDirectories(path)
            DeleteFileAndFolder(pFolder)
        Next

        IO.Directory.Delete(path, False)

    End Function

    Private Function LaunchPatchAndWait(ByVal svPath As String, ByVal svFile As String) As Boolean

        Dim nvTimeMilliseconds As Integer = 20

        LaunchPatchAndWait = False

        If Not IO.File.Exists(IO.Path.Combine(svPath, svFile)) Then Exit Function

        Dim SetupIss As String = IO.Path.ChangeExtension(IO.Path.Combine(svLocalDir, svFile), ".iss")
        Dim FileTxt As String = IO.Path.ChangeExtension(SetupIss, ".txt")
        Dim SetupLog As String = IO.Path.ChangeExtension(SetupIss, ".log")
        Dim FileUpdateErr As String = IO.Path.ChangeExtension(Application.ExecutablePath, ".err")

        Dim svDisk1Folder As String = svPath
        Dim svTempFolder As String = IO.Path.GetTempPath()
        Dim CommandLine As String = "/s /f1" + Chr(34) + SetupIss + Chr(34) + " /f2" + Chr(34) + SetupLog + Chr(34) + " " + AppPath

        IO.File.WriteAllText(FileUpdateErr, svDisk1Folder + svFile + " " + CommandLine, System.Text.Encoding.Default)

        If Not IO.File.Exists(SetupIss) Then
            Select_WriteIniString("InstallShield Silent", "Version", "v7.00", SetupIss)
        End If

        ProgressBar2.Style = ProgressBarStyle.Marquee
        ProgressBar2.MarqueeAnimationSpeed = nvTimeMilliseconds

        System.Windows.Forms.Application.DoEvents()

        Dim pathInstallShieldTmp As String = IO.Path.Combine(svDisk1Folder, "Tmp")

        Dim env_TMP As String = Environment.GetEnvironmentVariable("TMP")

        Environment.SetEnvironmentVariable("TMP", pathInstallShieldTmp)

        If IO.Directory.Exists(pathInstallShieldTmp) Then
            Try
                DeleteFileAndFolder(pathInstallShieldTmp)
            Catch ex As Exception
                ErrorDeletePatchs.Add(pathInstallShieldTmp)
            End Try
        End If

        Dim dirInfo As DirectoryInfo = IO.Directory.CreateDirectory(pathInstallShieldTmp)

        Dim MyProc As New Process()
        Dim lPid_Execute As Integer

        If bUseProcessExecute Then
            If bUseShellExecute Then MyProc.StartInfo.UseShellExecute = True
            MyProc.StartInfo.WorkingDirectory = svDisk1Folder
            MyProc.StartInfo.FileName = svDisk1Folder + svFile
            MyProc.StartInfo.Arguments = CommandLine
            MyProc.Start()
            lPid_Execute = MyProc.Id
        Else
            lPid_Execute = Shell(svDisk1Folder + svFile & " " & CommandLine, AppWinStyle.NormalFocus)
        End If

        System.Windows.Forms.Application.DoEvents()

        If lPid_Execute <> 0 Then

            Dim lHnd_Patch As Integer = OpenProcess(SYNCHRONIZE, 0, lPid_Execute)

            While WAIT_TIMEOUT = WaitForSingleObject(lHnd_Patch, nvTimeMilliseconds)
                System.Windows.Forms.Application.DoEvents()
            End While

            Try
                IO.File.Delete(SetupIss$)
            Catch ex As Exception

            End Try

            If IO.File.Exists(SetupLog$) Then IO.File.Delete(SetupLog$)

            If lHnd_Patch <> 0 Then CloseHandle(lHnd_Patch)

            LaunchPatchAndWait = Not IO.File.Exists(FileUpdateErr$)

            If Not LaunchPatchAndWait Then
                HttpSendMessage(CODE_ERROR_UPDATE, idx_maj_encours)
            Else
                HttpSendMessage(CODE_CORRECT_EXECUTION, idx_maj_encours)
            End If

            If bUseProcessExecute Then MyProc.Close()

        Else
            IO.File.Delete(SetupIss$)
            If IO.File.Exists(SetupLog$) Then IO.File.Delete(SetupLog$)
        End If

        Environment.SetEnvironmentVariable("TMP", env_TMP)

        ProgressBar2.Style = ProgressBarStyle.Blocks
        ProgressBar2.MarqueeAnimationSpeed = 100

        bButtonExecuteVisible = IO.File.Exists(ExecuteFile)
        Button_Execute.Visible = bButtonExecuteVisible

        System.Windows.Forms.Application.DoEvents()

    End Function

    Public Sub InitializeInstall()

        Dim svInitialize As String = Select_GetIniString("Parametres", "Initialize", AUServiceIni)

        If (svInitialize <> "") Then

            Dim bValue As Boolean = False

            Dim svKeyInstall As String = IO.Path.GetFileNameWithoutExtension(svInitialize)

            Try
                bValue = CBool(My.Computer.Registry.GetValue("HKEY_CURRENT_USER\Software\JFC", svKeyInstall, False))
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

                ProgressBar2.Style = ProgressBarStyle.Blocks
                ProgressBar2.MarqueeAnimationSpeed = 100

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

        If GetOnly = True And Not bQuickSupport Then Exit Sub

        If StrComp(Select_GetIniString("Parametres", "QuickSupport", AUServiceIni), QuickSupport, CompareMethod.Text) <> 0 Then
            Select_WriteIniString("Parametres", "QuickSupport", QuickSupport, AUServiceIni)
        End If

        Dim svQuickSupport As String = AppPath + QuickSupport

        Connection.GetFileToRootDir(svQuickSupport, (Select_GetIniString("Parametres", "TestUpdate", AUServiceIni) = "1"))

        If IO.File.Exists(svQuickSupport) And Not GetOnly Then

            Dim svMemLabelInfoMaj As String = Label_Info_Maj.Text

            Label_Info_Maj.Text = mLanguageAU.GetString(MSG_REMOTE_DESKTOP)
            System.Windows.Forms.Application.DoEvents()

            Dim procTeamViewer As Process() = Process.GetProcessesByName("TeamViewer")
            If procTeamViewer.Length > 0 Then procTeamViewer(0).Kill()

            LaunchAppAndWait(svQuickSupport)
            Label_Info_Maj.Text = svMemLabelInfoMaj

            System.Windows.Forms.Application.DoEvents()

        End If

    End Sub

    Private Sub AUService_PreviewKeyDown(ByVal sender As Object, ByVal e As System.Windows.Forms.PreviewKeyDownEventArgs) Handles Me.PreviewKeyDown

        If e.KeyCode = Keys.F8 And bAutomatic = True Then
            bAutomatic = False
        End If

    End Sub

    Private Sub PictureBoxAbout_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles PictureBoxAbout.Click

        Dim nvResult As Integer = About.ShowDialog()

    End Sub

    Private Function Url_Encode(ByVal svUrl As String) As String

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

            If ForceUpdate Then

                If Not Button_Execute.Enabled And idx_maj_encours - 1 = idx_maj_max Then
                    Button_Execute.Enabled = True

                ElseIf Button_Execute.Enabled And idx_maj_encours - 1 < idx_maj_max Then
                    Button_Execute.Enabled = False

                End If

            End If

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
            MyProc.StartInfo.Arguments = CStr(IIf(cmdline <> Nothing, cmdline, Application.ExecutablePath))
            MyProc.Start()
            Me.Close()

        Else

            HttpSendMessage(CODE_UPDATE_AU_SERV, , "File Not Found", , , DisplayNameReStart)

        End If

    End Sub

    Private Function FindComputerUserName(ByVal s As String) As Boolean

        If StrComp(s, My.Computer.Name + "." + UserName, CompareMethod.Text) = 0 Then
            Return True
        Else
            Return False
        End If

    End Function

    Private Function CheckCRC_Acces(ByVal acces() As String) As Boolean

        If IgnoreCRC Then Return True

        Dim myList As ArrayList = New ArrayList
        myList.Clear()
        myList.AddRange(acces)

        Dim svCRC As String = ArrayStringOf(myList, "%CRC%")

        If svCRC <> "" Then

            Dim index As Integer = myList.IndexOf("%CRC%" + svCRC)
            myList.RemoveAt(index)

            Dim NewCRC As String = AppendString(ArrayListToString(myList))

            If NewCRC = svCRC Then Return True

        End If

        Return False

    End Function

    Private Function ArrayListToString(ByVal list As ArrayList) As String

        Dim mylist As String = Nothing
        For Each u As String In list
            If mylist = Nothing Then
                mylist = u
            Else
                mylist = mylist + vbNewLine + u
            End If
        Next

        Return mylist

    End Function

    Public Sub AddCRC_Config(Optional SendMsg As Boolean = False)

        Dim svCRC As String = Select_GetIniString("Parametres", "CRC", AUServiceIni)

        Dim NewCRC As String = AppendString(svPatch + svSerie)

        If NewCRC <> svCRC Then

            Select_WriteIniString("Parametres", "CRC", NewCRC, AUServiceIni)

            If SendMsg Then HttpSendMessage(CODE_ADD_CRC_CONFIG_FILE, , "ResetCRC")

        End If

    End Sub

    Private Function CheckCRC_Config() As Boolean

        Dim svCRC As String = Select_GetIniString("Parametres", "CRC", AUServiceIni)

        If svCRC <> "" Then

            If AppendString(svPatch + svSerie) = svCRC Then Return True

        Else

            AddCRC_Config(True) : Return True

        End If

        HttpSendMessage(CODE_ERROR_CRC_CONFIG_FILE, , "ErrorCRC")

        Return False

    End Function

End Class


