Option Strict On
Option Explicit On

Public Class CreateUpdate

    Private ProjectFile As String
    Private MediaName As String
    Private FileTxtInfoPatch As String
    Private BuildLocation As String
    Private BuildZipLocation As String
    Private PackageLocation As String
    Private Application As String
    Private VersionFolder As String
    Private FilterDirectory As String
    Private ReadOnly FilterFiles As String
    Private ExcludedFiles() As String
    Private FilterVersion As String
    Private Tarifs As String
    Private TarifsFolder As String

    Private bMem_ButtonCreating As Boolean
    Private bMem_ButtonGetData As Boolean
    Private bMem_ButtonPigeAuto As Boolean
    Private bMem_ButtonGetTarifs As Boolean
    Private bMem_ButtonGetVersions As Boolean
    Private bMem_ComboBox1 As Boolean
    Private bMem_TextBox2 As Boolean

    Private Sub InitProjetInstallShield()

        ProjectFile = Select_GetIniString(FormBox.MenuSoft, "ProjectFile", FormBox.FacilityManagementIni).Trim
        MediaName = Select_GetIniString(FormBox.MenuSoft, "MediaName", FormBox.FacilityManagementIni).Trim
        FileTxtInfoPatch = Select_GetIniString(FormBox.MenuSoft, "InfoUpdate", FormBox.FacilityManagementIni).Trim
        BuildLocation = Select_GetIniString(FormBox.MenuSoft, "BuildLocation", FormBox.FacilityManagementIni).Trim
        BuildZipLocation = Select_GetIniString(FormBox.MenuSoft, "BuildZipLocation", FormBox.FacilityManagementIni).Trim
        PackageLocation = Select_GetIniString(FormBox.MenuSoft, "PackageLocation", FormBox.FacilityManagementIni).Trim
        Application = Select_GetIniString(FormBox.MenuSoft, "Application", FormBox.FacilityManagementIni).Trim
        VersionFolder = Select_GetIniString(FormBox.MenuSoft, "VersionFolder", FormBox.FacilityManagementIni).Trim
        TarifsFolder = Select_GetIniString(FormBox.MenuSoft, "TarifsFolder", FormBox.FacilityManagementIni).Trim
        FilterDirectory = Select_GetIniString(FormBox.MenuSoft, "FilterDirectory", FormBox.FacilityManagementIni).Trim
        FilterVersion = Select_GetIniString(FormBox.MenuSoft, "FilterVersion", FormBox.FacilityManagementIni).Trim
        Tarifs = Select_GetIniString(FormBox.MenuSoft, "Tarifs", FormBox.FacilityManagementIni).Trim
        ExcludedFiles = Split(Select_GetIniString(FormBox.MenuSoft, "ExcludedFiles", FormBox.FacilityManagementIni).Trim, ";")

        ButtonPigeAuto.Enabled = (Select_GetIniString(FormBox.MenuSoft, "Pigedata", FormBox.FacilityManagementIni).Trim <> "") And (FormBox.svAppBuild <> Nothing)
        ButtonGetTarifs.Enabled = (Tarifs <> "")
        ButtonGetData.Enabled = (Select_GetIniString(FormBox.MenuSoft, "Source", FormBox.FacilityManagementIni).Trim <> "")

    End Sub

    Private Sub CheckFolderEmpty()

        Dim i As Integer = 1
        Dim ListFolderNotEmpty As String = ""

        Dim NameFolderEmpty As String = Select_GetIniString(FormBox.MenuSoft, "CheckFolderEmpty" + Str(i).Trim, FormBox.FacilityManagementIni).Trim

        While NameFolderEmpty <> ""

            If IO.Directory.Exists(NameFolderEmpty) Then
                Dim nvCount As Integer = IO.Directory.GetFiles(NameFolderEmpty, "*.*", IO.SearchOption.AllDirectories).Length

                If nvCount > 0 Then
                    'MsgBox("List of folders that are not empty." + ListFolderNotEmpty, MsgBoxStyle.Exclamation, "Checking empty folders")

                    ListFolderNotEmpty = ListFolderNotEmpty + vbNewLine + " - " + ParsePath(NameFolderEmpty, FILENAME) + " ( " + Str(nvCount).Trim + " files )"
                End If
            Else
                IO.Directory.CreateDirectory(NameFolderEmpty)
            End If

            i += 1
            NameFolderEmpty = Select_GetIniString(FormBox.MenuSoft, "CheckFolderEmpty" + Str(i).Trim, FormBox.FacilityManagementIni).Trim
        End While

        If ListFolderNotEmpty <> "" Then

            If MsgBox("List of folders that are not empty." + ListFolderNotEmpty + vbNewLine + vbNewLine + "Would you erase all data?", CType(MsgBoxStyle.Exclamation + MsgBoxStyle.YesNo + MsgBoxStyle.DefaultButton1, MsgBoxStyle), "Clear Data") = MsgBoxResult.Yes Then

                i = 1
                NameFolderEmpty = Select_GetIniString(FormBox.MenuSoft, "CheckFolderEmpty" + Str(i).Trim, FormBox.FacilityManagementIni).Trim

                While NameFolderEmpty <> ""

                    Dim svFiles() As String = IO.Directory.GetFiles(NameFolderEmpty, "*.*", IO.SearchOption.AllDirectories)
                    For Each szfiles As String In svFiles
                        Try

                            If CBool(IO.File.GetAttributes(szfiles) = (IO.FileAttributes.ReadOnly Or IO.FileAttributes.Archive)) Then
                                IO.File.SetAttributes(szfiles, IO.FileAttributes.Archive)
                            End If

                            IO.File.Delete(szfiles)

                        Catch ex As Exception
                            MsgBox(ex.Message + vbNewLine + vbNewLine + szfiles, MsgBoxStyle.Critical)
                        End Try
                    Next

                    i += 1
                    NameFolderEmpty = Select_GetIniString(FormBox.MenuSoft, "CheckFolderEmpty" + Str(i).Trim, FormBox.FacilityManagementIni).Trim

                End While

            End If

            'MsgBox("List of folders that are not empty." + ListFolderNotEmpty, MsgBoxStyle.Exclamation, "Checking empty folders")
        End If


        'DeleteDir Empty
        i = 1
        NameFolderEmpty = Select_GetIniString(FormBox.MenuSoft, "CheckFolderEmpty" + Str(i).Trim, FormBox.FacilityManagementIni).Trim

        While NameFolderEmpty <> ""

            Try

                Dim svDir() As String = IO.Directory.GetDirectories(NameFolderEmpty, "*.*", IO.SearchOption.AllDirectories)

                For Each szDir As String In svDir
                    Dim nvCount As Integer = IO.Directory.GetFiles(szDir, "*.*", IO.SearchOption.AllDirectories).Length

                    If nvCount = 0 And StrComp(szDir, NameFolderEmpty, CompareMethod.Text) <> 0 Then
                        IO.Directory.Delete(szDir, True)
                    End If

                Next

                i += 1
                NameFolderEmpty = Select_GetIniString(FormBox.MenuSoft, "CheckFolderEmpty" + Str(i).Trim, FormBox.FacilityManagementIni).Trim

            Catch ex As Exception
                'Directory are deleted
            End Try

        End While


    End Sub

    Private Sub Form1_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load

        InitProjetInstallShield()

        Me.Opacity = FormBox.nvOpacity

        TextPatchInformation.Text = ""
        ToolStripStatusLabel1.Text = ""

        System.Windows.Forms.Application.DoEvents()

        ListPatch2Combo()

        If Application <> "" Then
            If Not IO.Directory.Exists(Application) Then
                IO.Directory.CreateDirectory(Application)
            End If
        Else
            ButtonGetVersion.Enabled = False
        End If

        CheckFolderEmpty()

        'TEST
        ButtonCreating.Enabled = (FormBox.svAppBuild <> Nothing)

        CheckBoxExe.Checked = True
        CheckBoxZip.Checked = True

        CheckBoxExe.Visible = FormBox.bAdvancedMenu
        CheckBoxZip.Visible = FormBox.bAdvancedMenu

    End Sub

    Public Sub ListPatch2Combo()

        ComboBoxPatchName.Items.Clear()

        For Each obj In FormBox.DicExes.Keys
            'For Each obj In ListExes
            ComboBoxPatchName.Items.Add(obj)
        Next

    End Sub

    Private Function ConvDate(ByVal svDate As String) As Double

        ConvDate = Val(Mid(svDate, 7, 4) + Mid(svDate, 4, 2) + Mid(svDate, 1, 2) + Mid(svDate, 12, 2) + Mid(svDate, 15, 2) + Mid(svDate, 18, 2))

    End Function

    Private Sub ButtonCreating_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ButtonCreating.Click

        CreateUpdate_Enabled(False)

        ComboBoxPatchName.Text.Trim()

        If TextPatchInformation.Text <> "" And ComboBoxPatchName.Text <> "" Then

            Dim Package As String = FormBox.mPathExes + ComboBoxPatchName.Text.Replace(FormBox.mExtFileExes, "")

            If Not IO.File.Exists(Package + FormBox.mExtFileExes) Then

                'Check Application
                If Application <> "" Then

                    Dim ListFiles() As String = IO.Directory.GetFiles(Application, "*.*", IO.SearchOption.AllDirectories)
                    ListFiles = (From w In ListFiles Select ParsePath(w, FILENAME_ONLY)).ToArray()

                    'If ListFiles.Length > 0 Then
                    '    If MsgBox("Information: Application Directory not empty." + vbNewLine + vbNewLine + "Do you really want to continue?", CType(MsgBoxStyle.Information + MsgBoxStyle.YesNo + MsgBoxStyle.DefaultButton2, MsgBoxStyle), "Application directory") = MsgBoxResult.No Then
                    '        CreateUpdate_Enabled()
                    '        Exit Sub
                    '    End If
                    'End If

                    If ExcludedFiles(0) <> "" Then

                        Dim MsgExport As String = vbNullString

                        For Each x As String In ExcludedFiles
                            If ListFiles.Contains(x) Then
                                MsgExport = MsgExport + vbNewLine + "- " + x
                            End If
                        Next

                        If MsgExport <> "" Then
                            If MsgBox("Warning: Exclude files presents" + MsgExport, CType(MsgBoxStyle.Critical + MsgBoxStyle.OkCancel + MsgBoxStyle.DefaultButton2, MsgBoxStyle), "Exclude files") = MsgBoxResult.Cancel Then
                                CreateUpdate_Enabled()
                                Exit Sub
                            End If
                        End If

                    End If

                End If

                Dim svPathFileTxtInfoPatch As String = ParsePath(FileTxtInfoPatch, PATH)

                If Not IO.Directory.Exists(svPathFileTxtInfoPatch) Then
                    IO.Directory.CreateDirectory(svPathFileTxtInfoPatch)
                End If

                If IO.File.Exists(FileTxtInfoPatch) Then
                    IO.File.Delete(FileTxtInfoPatch)
                End If

                Dim svInfoTxt As String = Replace(TextPatchInformation.Text, ",", ";")
                IO.File.WriteAllText(FileTxtInfoPatch, svInfoTxt, System.Text.Encoding.Default)

                Dim filetxt As String = Package + FormBox.mExtFileInfo
                IO.File.WriteAllText(filetxt, svInfoTxt, System.Text.Encoding.Default)

                If CheckBoxExe.Checked = True Then

                    CreatePatch(Package + FormBox.mExtFileExes)

                End If

                FormBox.CreateListExes()

                ListPatch2Combo()

            Else
                MsgBox("The patch already exist :" + vbNewLine + Chr(34) + Package + Chr(34), MsgBoxStyle.Exclamation)
            End If

        Else
            'MsgBox("Warning: Package Name is empty", MsgBoxStyle.Critical, "Create patch")
            MsgBox("the fields " + Chr(34) + "Package name" + Chr(34) + " or/and " + Chr(34) + "Infomation Package" + Chr(34) + " are empty", MsgBoxStyle.Critical, "Empty fields")
        End If

        CreateUpdate_Enabled()

    End Sub

    Private Sub ButtonExit_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ButtonExit.Click

        Me.Close()

    End Sub

    Private Sub ComboBox1_SelectedIndexChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ComboBoxPatchName.SelectedIndexChanged

        If TextPatchInformation.Text = Nothing Then

            Dim svFileInfo As String = FormBox.mPathExes + ParsePath(ComboBoxPatchName.Text, FILENAME_ONLY) + FormBox.mExtFileInfo

            If IO.File.Exists(svFileInfo) Then

                TextPatchInformation.Text = IO.File.ReadAllText(svFileInfo, System.Text.Encoding.Default)

            End If

        End If


    End Sub

    Private Sub ButtonGetData_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ButtonGetData.Click


        CreateUpdate_Enabled(False)

        If Select_GetIniString(FormBox.MenuSoft, "DataRecovery", FormBox.FacilityManagementIni).Trim = "P" Then
            DataRecoveryP.ShowDialog()
            TextPatchInformation.Text = DataRecoveryP.NameOfWave

        ElseIf Select_GetIniString(FormBox.MenuSoft, "DataRecovery", FormBox.FacilityManagementIni).Trim = "I" Then
            DataRecovery.ShowDialog()
            TextPatchInformation.Text = DataRecovery.NameOfWave

        ElseIf Select_GetIniString(FormBox.MenuSoft, "DataRecovery", FormBox.FacilityManagementIni).Trim = "C" Then

            Dim downloadFolder As String = Select_GetIniString(FormBox.MenuSoft, "Source", FormBox.FacilityManagementIni).Trim()

            If Not IO.Directory.Exists(downloadFolder) Then
                Dim userRoot As String = System.Environment.GetEnvironmentVariable("USERPROFILE")
                downloadFolder = IO.Path.Combine(userRoot, "Downloads")
            End If

            OpenFileDialog1.InitialDirectory = downloadFolder 'System.Environment.GetFolderPath(Environment.SpecialFolder.Desktop)
            OpenFileDialog1.Filter = "zip files (*.zip)|*.zip|All files (*.*)|*.*"
            OpenFileDialog1.FilterIndex = 1
            OpenFileDialog1.RestoreDirectory = True
            OpenFileDialog1.Multiselect = False
            OpenFileDialog1.FileName = ""
            OpenFileDialog1.ShowDialog()

            If IO.File.Exists(OpenFileDialog1.FileName) Then

                Dim TempFolder As String = IO.Path.GetTempPath() + FormBox.MenuSoft

                If IO.Directory.Exists(TempFolder) Then
                    IO.Directory.Delete(TempFolder, True)
                    If Not IO.Directory.Exists(TempFolder) Then
                        IO.Directory.CreateDirectory(TempFolder)
                    End If
                End If

                ToolStripStatusLabel1.Text = "Unzip in progress..."
                ToolStripProgressBar1.Style = ProgressBarStyle.Marquee
                ToolStripProgressBar1.MarqueeAnimationSpeed = 20
                System.Windows.Forms.Application.DoEvents()

                DataRecovery.ExtractZip(OpenFileDialog1.FileName, TempFolder)

                ToolStripStatusLabel1.Text = "Copy in progress..."
                System.Windows.Forms.Application.DoEvents()

                Dim DestinationFiles As String = Select_GetIniString(FormBox.MenuSoft, "Survey", FormBox.FacilityManagementIni).Trim

                If CopyLstFile2Dest("study.lst", TempFolder, DestinationFiles) = True Then

                    DestinationFiles = Select_GetIniString(FormBox.MenuSoft, "Vendor", FormBox.FacilityManagementIni).Trim

                    If CopyLstFile2Dest("vendorU.lst", TempFolder, DestinationFiles, True) = False Then

                        MsgBox("File 'vendorU.lst' not found.", MsgBoxStyle.Exclamation)

                    End If

                    Dim Sources_study() As String = IO.Directory.GetFiles(TempFolder, "study.inf", IO.SearchOption.AllDirectories)

                    If Sources_study.Length > 0 Then

                        Dim study() As String = IO.File.ReadAllLines(Sources_study(0))

                        TextPatchInformation.Text = study(1)
                        ComboBoxPatchName.Text = study(0)

                    Else

                        MsgBox("File 'study.inf' not found.", MsgBoxStyle.Exclamation)

                    End If

                Else

                    MsgBox("File 'study.lst' not found.", MsgBoxStyle.Critical)

                End If


                IO.Directory.Delete(TempFolder, True)

                ToolStripStatusLabel1.Text = "Completed."
                ToolStripProgressBar1.Style = ProgressBarStyle.Continuous
                System.Windows.Forms.Application.DoEvents()

                'cluster.lst
                'cluster.lst
                'direc.lst

            End If

        End If

        CreateUpdate_Enabled()



    End Sub

    Private Function CopyLstFile2Dest(ByVal szfile As String, ByVal SourceFolder As String, ByVal DestinationFolder As String, Optional ByVal bVendorFile As Boolean = False) As Boolean

        Try

            Dim Sources_lst() As String = IO.Directory.GetFiles(SourceFolder, szfile, IO.SearchOption.AllDirectories)

            If Sources_lst.Length = 0 Then Return False

            For Each svFilelst As String In Sources_lst

                'BUG
                Dim svPathFilelst As String = ParsePath(svFilelst, PATH)

                Dim svfile() As String = IO.File.ReadAllLines(svFilelst, System.Text.Encoding.Default)

                For Each File As String In svfile

                    File = File.Replace("/", "\")



                    ' ************** Modif Olivier le 04/01 ****************
                    Dim szOnlyFile As String = ParsePath(File, FILENAME)
                    'Dim szOnlyFile As String = RemoveFirstFolder(File)

                    Dim sourceFileName As String = svPathFilelst + szOnlyFile

                    If Not IO.File.Exists(sourceFileName) Then
                        Dim sourceFileNames() = IO.Directory.GetFiles(svPathFilelst, szOnlyFile, IO.SearchOption.AllDirectories)
                        sourceFileName = sourceFileNames(0)
                    End If
                    ' ************** Modif Olivier le 04/01 ****************



                    Dim CodeSurvey As String = ""

                    If bVendorFile Then
                        Dim szExt = ParsePath(szOnlyFile, EXTENSION_ONLY)

                        If szExt = ".DFX" Then
                            Dim code As String = ReadSurveyCode(sourceFileName)
                            If code <> "" Then
                                CodeSurvey = code + "\"
                            End If

                        ElseIf szExt = ".DEF" Then
                            Dim code As String = ReadFirstLine(sourceFileName)
                            If code <> "" Then
                                CodeSurvey = code + "\"
                            End If

                        ElseIf szExt = ".DFU" Then
                            Dim code As String = ReadFirstLine(sourceFileName)
                            If code <> "" Then
                                CodeSurvey = code + "\"
                            End If

                        End If
                    End If

                    Dim destFileName As String = DestinationFolder + CodeSurvey + File
                    Dim pathdestFileName As String = ParsePath(destFileName, PATH)

                    If Not IO.Directory.Exists(pathdestFileName) Then
                        IO.Directory.CreateDirectory(pathdestFileName)
                    End If

                    If IO.File.Exists(destFileName) Then
                        IO.File.Delete(destFileName)
                    End If

                    IO.File.Copy(sourceFileName, destFileName)

                Next

                For Each File As String In svfile

                    File = File.Replace("/", "\")

                    Dim szOnlyFile As String = ParsePath(File, FILENAME)

                    Dim sourceFileName As String = svPathFilelst + szOnlyFile

                    IO.File.Delete(sourceFileName)

                Next

            Next

            Return True

        Catch ex As Exception
            MsgBox(ex.Message, MsgBoxStyle.Critical)

            Return False
        End Try

    End Function

    Private Function ReadFirstLine(ByVal szFile As String) As String
        Dim lines() As String = IO.File.ReadAllLines(szFile)
        Return lines(0).Replace(Chr(0), "") 'Modif ".DFU"
    End Function

    Private Function ReadSurveyCode(ByVal szFile As String) As String

        'SurveyCode

        Dim SurveyCode As String = IO.File.ReadAllText(szFile)

        Return ExtractSurveyCode(SurveyCode)

    End Function

    Private Function ExtractSurveyCode(ByVal szText As String) As String

        Dim nPosServeyCode As Integer = InStr(szText, "SurveyCode", CompareMethod.Text)

        Dim nPosStart As Integer = InStr(nPosServeyCode, szText, Chr(34), CompareMethod.Text) + 1
        Dim nPosEnd As Integer = InStr(nPosStart, szText, Chr(34), CompareMethod.Text)

        Return Mid(szText, nPosStart, nPosEnd - nPosStart)

    End Function

    Private Sub PatchRadioKey(ByVal KeyPath As String, ByVal Pigedata As String)

        Dim PigeKey As String = Select_GetIniString(FormBox.MenuSoft, "Key1", FormBox.FacilityManagementIni).Trim

        Dim KeyFile As String = Select_GetIniString(FormBox.MenuSoft, "KeyFile", FormBox.FacilityManagementIni).Trim

        Dim i As Integer = 1

        Dim svPathFileTxtInfoPatch As String = ParsePath(FileTxtInfoPatch, PATH)

        If Not IO.Directory.Exists(svPathFileTxtInfoPatch) Then
            IO.Directory.CreateDirectory(svPathFileTxtInfoPatch)
        End If

        If IO.File.Exists(FileTxtInfoPatch) Then
            IO.File.Delete(FileTxtInfoPatch)
        End If

        Dim svInfoTxt As String = Replace(TextPatchInformation.Text, ",", ";")
        IO.File.WriteAllText(FileTxtInfoPatch, svInfoTxt, System.Text.Encoding.Default)

        While PigeKey <> ""

            Dim key() As String = Split(PigeKey, ",")

            Dim Package As String = FormBox.mPathExes + ComboBoxPatchName.Text + key(1)

            If IO.File.Exists(Package + FormBox.mExtFileExes) Then
                MsgBox("The patch already exist :" + vbNewLine + Chr(34) + Package + FormBox.mExtFileExes + Chr(34), MsgBoxStyle.Exclamation)
                Exit Sub
            End If

            Dim fic_source As String = KeyPath + key(0)

            If Not IO.Directory.Exists(Pigedata) Then
                IO.Directory.CreateDirectory(Pigedata)
            End If

            Try
                IO.File.Delete(Pigedata + KeyFile)
            Catch ex As Exception
                MsgBox("Unable to delete: " + Pigedata + KeyFile + vbNewLine + "Error: " + ex.ToString(), MsgBoxStyle.Critical)
                Exit Sub
            End Try


            If IO.File.Exists(fic_source) Then
                Try
                    IO.File.Copy(fic_source, Pigedata + KeyFile)
                Catch ex As Exception
                    MsgBox("Unable to copy: " + key(0) + vbNewLine + "Error: " + ex.ToString(), MsgBoxStyle.Critical)
                    Exit Sub
                End Try
            End If


            Dim filetxt As String = Package + FormBox.mExtFileInfo
            IO.File.WriteAllText(filetxt, svInfoTxt, System.Text.Encoding.Default)

            If CheckBoxExe.Checked = True Then

                CreatePatch(Package + FormBox.mExtFileExes)

            End If

            'MsgBox(FormBox.mPathExes + ComboBoxPatchName.Text + key(1), MsgBoxStyle.Information)

            i += 1
            PigeKey = Select_GetIniString(FormBox.MenuSoft, "Key" + Trim(CStr(i)), FormBox.FacilityManagementIni).Trim

        End While

        FormBox.CreateListExes()

        ComboBoxPatchName.Text = ""

        ListPatch2Combo()

    End Sub

    Private Sub PatchRadioGroupKey(ByVal KeyPath As String, ByVal Pigedata As String)

        Dim PigeGrp As String = Select_GetIniString(FormBox.MenuSoft, "KeyGroup0", FormBox.FacilityManagementIni).Trim
        If PigeGrp = Nothing Then Exit Sub

        Dim Package As String = FormBox.mPathExes + ComboBoxPatchName.Text + PigeGrp

        If IO.File.Exists(Package) Then
            MsgBox("The patch already exist :" + vbNewLine + Chr(34) + Package + Chr(34), MsgBoxStyle.Exclamation)
            Exit Sub
        End If

        Dim KeyFile As String = Select_GetIniString(FormBox.MenuSoft, "KeyFile", FormBox.FacilityManagementIni).Trim
        If KeyFile = Nothing Then Exit Sub


        Dim szFile As String = ParsePath(KeyFile, FILENAME_ONLY)
        Dim szExt As String = ParsePath(KeyFile, EXTENSION_ONLY)

        IO.File.Delete(Pigedata + szFile + "*." + szExt)

        PigeGrp = Select_GetIniString(FormBox.MenuSoft, "KeyGroup1", FormBox.FacilityManagementIni).Trim
        If PigeGrp = Nothing Then Exit Sub

        Dim i As Integer = 1

        While PigeGrp <> ""

            Dim key() As String = Split(PigeGrp, ",")

            Dim fic_source As String = KeyPath + key(0)

            If Not IO.Directory.Exists(Pigedata) Then
                IO.Directory.CreateDirectory(Pigedata)
            End If

            If IO.File.Exists(fic_source) Then
                Try
                    IO.File.Copy(fic_source, Pigedata + szFile + key(1) + szExt)
                Catch ex As Exception
                    MsgBox("Unable to copy: " + FileTxtInfoPatch + vbNewLine + "Error: " + ex.ToString(), MsgBoxStyle.Critical)
                    Exit Sub
                End Try
            End If

            i += 1
            PigeGrp = Select_GetIniString(FormBox.MenuSoft, "KeyGroup" + Trim(CStr(i)), FormBox.FacilityManagementIni).Trim

        End While

        CreatePatch(FormBox.mPathExes + ComboBoxPatchName.Text + PigeGrp)

        FormBox.CreateListExes()

        ComboBoxPatchName.Text = ""

        ListPatch2Combo()

    End Sub

    Private Sub CreatePatch(ByVal Package As String)


        'Dim svCmdBuild As String = "-s -p" + Chr(34) + ProjectFile + Chr(34) + " -m" + Chr(34) + MediaName + Chr(34) + " -b" + Chr(34) + BuildLocation + Chr(34)
        Dim svCmdBuild As String = "-s -p " + Chr(34) + ProjectFile + Chr(34) + " -r " + Chr(34) + MediaName + Chr(34) + " -b " + Chr(34) + BuildLocation + Chr(34)

        'IO_File_WriteAllText(FileTxtInfoPatch, svInfoTxt)

        ToolStripProgressBar1.Style = ProgressBarStyle.Marquee

        ToolStripStatusLabel1.Text = "Creating in Progress..."

        LaunchAppAndWait(Facility_Management.FormBox.svAppBuild, svCmdBuild, ProcessWindowStyle.Hidden)

        ToolStripProgressBar1.Style = ProgressBarStyle.Continuous

        If IO.File.Exists(PackageLocation) Then

            Try

                ToolStripProgressBar1.Maximum = 100
                ToolStripStatusLabel1.Text = "CopyFile Progress..."

                If CopyFileWithProgress(PackageLocation, Package, PROGRESS_FORM_CREATEUPDATE) Then
                    IO.File.Delete(PackageLocation)
                Else
                    MsgBox("Unable to move: " + PackageLocation + vbNewLine + "Error: CopyFileWithProgress", MsgBoxStyle.Critical)
                End If

                ToolStripProgressBar1.Value = 0
                ToolStripStatusLabel1.Text = ""

            Catch ex As Exception

                If MsgBox("Unable to move: " + PackageLocation + vbNewLine + "Error: " + ex.ToString() + vbNewLine + vbNewLine + "Would you like to retry?", CType(MsgBoxStyle.Exclamation + MsgBoxStyle.YesNo + MsgBoxStyle.DefaultButton1, MsgBoxStyle), "CopyFileWithProgress") = MsgBoxResult.Yes Then

                    If IO.File.Exists(PackageLocation) Then

                        If IO.File.Exists(Package) Then

                            Try
                                IO.File.Delete(Package)
                            Catch exDelete As Exception
                                MsgBox("Unable to delete: " + Package + vbNewLine + "Error: " + exDelete.ToString(), MsgBoxStyle.Critical, "Delete file")
                            End Try

                        End If

                        Try
                            IO.File.Move(PackageLocation, Package)
                        Catch exMove As Exception
                            MsgBox("Unable to move: " + PackageLocation + vbNewLine + "Error: " + exMove.ToString(), MsgBoxStyle.Critical, "Move file")
                        End Try

                    Else

                        MsgBox("Unable to move: " + PackageLocation + vbNewLine + "Error: Not found", MsgBoxStyle.Critical)

                    End If



                End If

            End Try

        End If

    End Sub

    Private Sub ButtonGetVersion_Click(sender As System.Object, e As System.EventArgs) Handles ButtonGetVersion.Click

        CreateUpdate_Enabled(False)
        ToolStripProgressBar1.Style = ProgressBarStyle.Marquee

        Try

            GetElements.ComboBoxVersion.Items.Clear()
            GetElements.ComboBoxVersion.Text = ""
            GetElements.Text = "Get Version"
            GetElements.ChooseLabel.Text = "Choose Version:"

            'Modif Pologne 25/04/2017
            Dim CodePays As String = FormBox.CountryDic(FormBox.MenuPays).Replace("Acces_", "").Replace("\", "")
            If Len(CodePays) = 2 Then
                If IO.Directory.Exists(VersionFolder.Replace("##", CodePays)) Then
                    VersionFolder = VersionFolder.Replace("##", CodePays)
                Else
                    VersionFolder = VersionFolder.Replace("##", "FR") 'Default
                End If
            Else
                VersionFolder = VersionFolder.Replace("##", "FR") 'Default
            End If



            If VersionFolder <> "" Then

                If FilterDirectory <> "" Then

                    Dim ListFolderVersion() As String = IO.Directory.GetDirectories(VersionFolder, FilterDirectory)

                    ListFolderVersion = (From w In ListFolderVersion Select w Order By IO.Directory.GetCreationTime(w) Descending).ToArray()

                    ListFolderVersion = (From w In ListFolderVersion Select Replace(w, VersionFolder, "")).ToArray()

                    GetElements.ComboBoxVersion.Items.AddRange(ListFolderVersion)

                ElseIf FilterVersion <> "" Then

                    Dim ListFileVersion() As String = IO.Directory.GetFiles(VersionFolder, FilterVersion)

                    ListFileVersion = (From w In ListFileVersion Select w Order By IO.File.GetCreationTime(w) Descending).ToArray()

                    ListFileVersion = (From w In ListFileVersion Select Replace(w, VersionFolder, "")).ToArray()

                    GetElements.ComboBoxVersion.Items.AddRange(ListFileVersion)

                End If

            End If

            If GetElements.ComboBoxVersion.Items.Count > 0 Then

                If GetElements.ShowDialog() = Windows.Forms.DialogResult.OK Then

                    If IO.Directory.GetFiles(Application, "*.*", IO.SearchOption.AllDirectories).Count > 0 Then
                        If MsgBox("Application Directory is not empty." + vbNewLine + vbNewLine + "Do you really want to erase?", CType(MsgBoxStyle.Exclamation + MsgBoxStyle.YesNo + MsgBoxStyle.DefaultButton1, MsgBoxStyle), "Application directory") = MsgBoxResult.Yes Then
                            IO.Directory.Delete(Application, True)
                            IO.Directory.CreateDirectory(Application)
                        End If
                    End If

                    Dim svSelectedVersion As String = VersionFolder + GetElements.ComboBoxVersion.Text.Trim

                    If IO.File.Exists(svSelectedVersion) Then

                        DataRecovery.ExtractZip(svSelectedVersion, Application)

                    ElseIf IO.Directory.Exists(svSelectedVersion) Then
                        svSelectedVersion += "\"


                        Dim ListFiles() As String = IO.Directory.GetFiles(svSelectedVersion, "*.*", System.IO.SearchOption.AllDirectories)

                        ListFiles = (From w In ListFiles Select Replace(w, svSelectedVersion, "")).ToArray()

                        For Each x As String In ListFiles

                            Dim Destination As String = ParsePath(Application + x, PATH)
                            If Not IO.Directory.Exists(Destination) Then
                                IO.Directory.CreateDirectory(Destination)
                            End If

                            'Dim nvIndexPatch As Integer = ListUpdates.FindIndex(Function(x) x.Contains("," + svPatch + ","))

                            Dim Excluded As String = ParsePath(Application + x, FILENAME_ONLY)

                            If Not ExcludedFiles.Contains(Excluded) Then

                                If IO.File.Exists(Application + x) Then
                                    IO.File.Delete(Application + x)
                                End If

                                IO.File.Copy(svSelectedVersion + x, Application + x)

                            End If

                        Next

                    Else

                        Exit Try

                    End If

                    If Not IO.Directory.Exists(Application + "\FRET") Then
                        IO.Directory.CreateDirectory(Application + "\FRET")
                    End If

                    If IO.File.Exists(Application + "\FRET\Version.txt") Then
                        IO.File.Delete(Application + "\FRET\Version.txt")
                    End If

                    'sqdqsdfsqdfsqd()

                    Dim nPosChar As Integer = InStr(FilterDirectory, "?", CompareMethod.Text)

                    Dim svVersion As String = Nothing

                    If nPosChar > 0 Then

                        svVersion = Mid(GetElements.ComboBoxVersion.Text, nPosChar, Len(GetElements.ComboBoxVersion.Text) - nPosChar + 1)

                        svVersion = Replace(svVersion, ".exe", "", 1, 1, CompareMethod.Text)

                        IO.File.WriteAllText(Application + "\FRET\Version.txt", svVersion)

                        TextPatchInformation.Text = "Version: " + svVersion

                    End If

                Else
                    TextPatchInformation.Text = ""
                    GetElements.ComboBoxVersion.Text = ""
                End If
            End If

        Catch ex As Exception
            MsgBox(ex.Message, MsgBoxStyle.Critical)

        End Try

        ToolStripProgressBar1.Style = ProgressBarStyle.Continuous

        CreateUpdate_Enabled()

    End Sub

    Private Sub CreateUpdate_Enabled(Optional ByVal Enable As Boolean = True)

        If Enable = False Then

            bMem_ButtonCreating = ButtonCreating.Enabled
            bMem_ButtonGetData = ButtonGetData.Enabled
            bMem_ButtonPigeAuto = ButtonPigeAuto.Enabled
            bMem_ButtonGetTarifs = ButtonGetTarifs.Enabled
            bMem_ButtonGetVersions = ButtonGetVersion.Enabled
            bMem_ComboBox1 = ComboBoxPatchName.Enabled
            bMem_TextBox2 = TextPatchInformation.Enabled

            ButtonCreating.Enabled = False
            ButtonGetData.Enabled = False
            ButtonPigeAuto.Enabled = False
            ButtonGetTarifs.Enabled = False
            ButtonGetVersion.Enabled = False
            ComboBoxPatchName.Enabled = False
            TextPatchInformation.Enabled = False

        Else

            ButtonCreating.Enabled = bMem_ButtonCreating
            ButtonGetData.Enabled = bMem_ButtonGetData
            ButtonPigeAuto.Enabled = bMem_ButtonPigeAuto
            ButtonGetTarifs.Enabled = bMem_ButtonGetTarifs
            ButtonGetVersion.Enabled = bMem_ButtonGetVersions
            ComboBoxPatchName.Enabled = bMem_ComboBox1
            TextPatchInformation.Enabled = bMem_TextBox2

        End If

    End Sub

    Private Sub ButtonPigeAuto_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ButtonPigeAuto.Click

        CreateUpdate_Enabled(False)

        Dim PigeName As String = Select_GetIniString(FormBox.MenuSoft, "PigeName", FormBox.FacilityManagementIni).Trim

        If ComboBoxPatchName.Text.Trim = "" Then
            MsgBox("Warning: Package Name is empty", MsgBoxStyle.Critical, "Create patch")
            CreateUpdate_Enabled()
            Exit Sub
        ElseIf TextPatchInformation.Text = "" Then
            MsgBox("Warning: Package Information is empty", MsgBoxStyle.Critical, "Create patch")
            CreateUpdate_Enabled()
            Exit Sub
        ElseIf MsgBox("Are you sure you want to automatically create patches (" + Chr(34) + PigeName + Chr(34) + ")?", CType(MsgBoxStyle.Exclamation + MsgBoxStyle.YesNo + MsgBoxStyle.DefaultButton2, MsgBoxStyle), "Create patch") = MsgBoxResult.No Then
            CreateUpdate_Enabled()
            Exit Sub
        End If

        GetElements.ComboBoxVersion.Items.Clear()
        GetElements.ComboBoxVersion.Text = ""
        GetElements.Text = "Get data file patch"
        GetElements.ChooseLabel.Text = "Please select the data file patch"
        GetElements.ButtonGet.Text = "Get"

        Dim bmp As System.Drawing.Bitmap = Facility_Management.My.Resources.ImportData
        Dim ico As System.Drawing.Icon = System.Drawing.Icon.FromHandle(bmp.GetHicon())
        GetElements.Icon = ico

        Dim KeyPath As String = Select_GetIniString(FormBox.MenuSoft, "KeyPath", FormBox.FacilityManagementIni).Trim
        Dim Pigedata As String = Select_GetIniString(FormBox.MenuSoft, "Pigedata", FormBox.FacilityManagementIni).Trim
        Dim DataPath As String = Select_GetIniString(FormBox.MenuSoft, "DataPath", FormBox.FacilityManagementIni).Trim
        Dim DataFind As String = Select_GetIniString(FormBox.MenuSoft, "DataFind", FormBox.FacilityManagementIni).Trim
        Dim NamePige As String = Select_GetIniString(FormBox.MenuSoft, "NamePige", FormBox.FacilityManagementIni).Trim

        If DataPath <> "" Then

            Dim ListPigeMajAccdb() As String = IO.Directory.GetFiles(DataPath, DataFind)

            ListPigeMajAccdb = (From w In ListPigeMajAccdb Select w Order By IO.File.GetCreationTime(w) Descending).ToArray()

            ListPigeMajAccdb = (From w In ListPigeMajAccdb Select Replace(w, DataPath, "")).ToArray()

            GetElements.ComboBoxVersion.Items.AddRange(ListPigeMajAccdb)

        End If

        If GetElements.ComboBoxVersion.Items.Count > 0 Then
            GetElements.ComboBoxVersion.Text = CStr(GetElements.ComboBoxVersion.Items.Item(0))
        End If

        Dim nvResult As Windows.Forms.DialogResult = GetElements.ShowDialog()

        If nvResult = Windows.Forms.DialogResult.OK Then

            Dim svSelectedElement As String = GetElements.ComboBoxVersion.Text

            If svSelectedElement <> "" Then
                If IO.File.Exists(DataPath + svSelectedElement) Then
                    IO.File.Copy(DataPath + svSelectedElement, Pigedata + svSelectedElement, True)
                End If
                If IO.File.Exists(DataPath + "InfosRad.txt") Then
                    IO.File.Copy(DataPath + "InfosRad.txt", Pigedata + "InfosRad.txt", True)
                End If
            End If

            PatchRadioKey(KeyPath, Pigedata)

            MsgBox("Completed.", CType(MsgBoxStyle.OkOnly + MsgBoxStyle.Information + MsgBoxStyle.SystemModal, MsgBoxStyle), "Create patch")

        ElseIf nvResult = Windows.Forms.DialogResult.Ignore Then

            If IO.File.Exists(DataPath + "InfosRad.txt") Then
                IO.File.Copy(DataPath + "InfosRad.txt", Pigedata + "InfosRad.txt", True)
            End If

            PatchRadioKey(KeyPath, Pigedata)

            MsgBox("Completed.", CType(MsgBoxStyle.OkOnly + MsgBoxStyle.Information + MsgBoxStyle.SystemModal, MsgBoxStyle), "Create patch")

        End If

        CreateUpdate_Enabled()

    End Sub

    Private Sub ButtonGetTarifs_Click(sender As System.Object, e As System.EventArgs) Handles ButtonGetTarifs.Click
        CreateUpdate_Enabled(False)

        Try

            OpenFileDialogVersion.Filter = "EXE Files (*.exe)|*.exe|ZIP files (*.zip)|*.zip"

            If TarifsFolder <> Nothing Then
                OpenFileDialogVersion.InitialDirectory = TarifsFolder
            Else
                OpenFileDialogVersion.InitialDirectory = VersionFolder
            End If

            OpenFileDialogVersion.FileName = ""

            If OpenFileDialogVersion.ShowDialog() = Windows.Forms.DialogResult.OK Then
                DataRecovery.ExtractZip(OpenFileDialogVersion.FileName, Tarifs, "ZONE")
            End If

        Catch ex As Exception
            MsgBox(ex.Message, MsgBoxStyle.Critical)

        End Try

        CreateUpdate_Enabled()

    End Sub

    Private Sub HandleKeyDown(ByVal sender As Object, ByVal e As KeyEventArgs) Handles MyBase.KeyDown
        'HandleKeyDown = False

        If e.KeyCode = Keys.Escape Then

            'close
            Me.Close()


        End If

        'HandleKeyDown = True

    End Sub

    Public Sub GenerateZip(ByVal inputPath As String, ByVal targetLocation As String)

        Dim files = IO.Directory.GetFiles(inputPath, "*.*", IO.SearchOption.AllDirectories)

        ToolStripProgressBar1.Value = 0
        ToolStripStatusLabel1.Text = ""
        System.Windows.Forms.Application.DoEvents()

        Dim fileMap As List(Of String()) = New List(Of String())

        inputPath = IO.Directory.GetParent(inputPath).FullName

        Dim len As Integer = inputPath.Length + 1

        Dim totalsize As Integer = 0

        For Each file In files

            fileMap.Add(New String() {file, file.Substring(len)})

            totalsize += CInt(FileLen(file) / 1024)

        Next

        ToolStripProgressBar1.Maximum = totalsize

        If (IO.File.Exists(targetLocation)) Then IO.File.Delete(targetLocation)

        Dim stream As IO.FileStream = New IO.FileStream(targetLocation, IO.FileMode.CreateNew)

        Dim zipStream As ZipOutputStream = New ZipOutputStream(stream) With {
            .Password = "JFC123" 'ZipPassword
            }
        'zipStream.SetLevel(9)

        For Each filePeer In fileMap

            Dim entryName As String = filePeer(1)
            Dim fileName As String = filePeer(0)

            'Supprime le sous-dossier ICON et les fichiers à la racine
            If entryName.Contains("\") And Not Strings.InStr(entryName, "ICON\", CompareMethod.Text) = 1 Then

                Dim nPosLastSlach As Integer = InStrRev(fileName, "\")
                ToolStripStatusLabel1.Text = "Zip: " + Mid(fileName, nPosLastSlach + 1, fileName.Length - nPosLastSlach)

                System.Windows.Forms.Application.DoEvents()

                Dim entry As ZipEntry = New ZipEntry(ZipEntry.CleanName(entryName))

                'Date Time File
                Dim svCreationTime As Date = IO.File.GetCreationTime(fileName)
                Dim svLastAccessTime As Date = IO.File.GetLastAccessTime(fileName)
                Dim svLastWriteTime As Date = IO.File.GetLastWriteTime(fileName)

                entry.DateTime = svLastWriteTime

                entry.IsUnicodeText = True

                entry.AESKeySize = 256

                zipStream.PutNextEntry(entry)

                ' Read from response and write to file
                Dim fileStream As IO.FileStream = IO.File.OpenRead(fileName)

                Dim bufferSize As Integer = 64 * 1024
                Dim buffer(bufferSize - 1) As Byte

                Dim bytesRead As Integer = fileStream.Read(buffer, 0, bufferSize)

                While (bytesRead <> 0)

                    zipStream.Write(buffer, 0, bytesRead)

                    bytesRead = fileStream.Read(buffer, 0, bufferSize)

                    ToolStripProgressBar1.Value = ToolStripProgressBar1.Value + CInt(bytesRead / 1024)

                    System.Windows.Forms.Application.DoEvents()

                End While

                zipStream.CloseEntry()

                fileStream.Close()

            End If

            System.Windows.Forms.Application.DoEvents()

        Next

        ToolStripProgressBar1.Value = 0
        ToolStripStatusLabel1.Text = ""

        zipStream.Finish()
        zipStream.Close()

        stream.Close()

        System.Windows.Forms.Application.DoEvents()

    End Sub

End Class
