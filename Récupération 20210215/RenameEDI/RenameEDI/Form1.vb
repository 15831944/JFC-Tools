Option Strict On
Option Explicit On

Imports VB = Microsoft.VisualBasic

'Ajout DataGridView

Public Class FormBox

    Dim PathEDI As String '= "I:\JFC\ARTRadio\EXTERNE\reception\"
    'Dim PathEDI As String = "C:\ARTRADIO\EXTERNE\reception\"

    Dim FileIO As System.IO.FileStream

    Dim ExtFilesSrc As String
    Dim ExtFilesDst As String
    Dim szMessage As String
    Dim GestionEDIIni As String

    Public cMSDic As New Dictionary(Of String, Integer)
    Public UserDic As New Dictionary(Of String, String)
    Public EmailDic As New Dictionary(Of String, String)
    Public EdiDic As New Dictionary(Of String, String)


    Friend WithEvents ExitMenu As System.Windows.Forms.ToolStripMenuItem
    Friend WithEvents RefreshMenu As System.Windows.Forms.ToolStripMenuItem
    Friend WithEvents ModeMenu As System.Windows.Forms.ToolStripMenuItem
    Friend WithEvents ModeRestaure As System.Windows.Forms.ToolStripMenuItem

    Public Const DIRECTORY As Short = 1 'Indicates that the path minus the disk drive letter and filename should be returned in svReturnString. When this option is used with a UNC path, ParsePath returns the path without the server and shared device name, and without the filename if one was specified. For example, the UNC path \\TheServer\TheSharedDevice\TheApp\TheFile.exe is returned in svReturnString as \TheApp\. 
    Public Const DISK As Short = 2 'Indicates that the disk drive designation (drive letter followed by a colon) should be returned in svReturnString. When this option is used with a UNC path, ParsePath returns the server and shared device name. For example, the UNC path \\TheServer\TheSharedDevice\TheApp\TheFile.exe is returned in svReturnString as \\TheServer\TheSharedDevice. 
    Public Const EXTENSION_ONLY As Short = 3 'Indicates that the file extension should be returned in svReturnString. It does not include the period.
    Public Const FILENAME As Short = 4 'Indicates that the complete filename (that is, with its file extension) should be returned in svReturnString.
    Public Const FILENAME_ONLY As Short = 5 'Indicates that the filename only (that is, without its file extension) should be returned in svReturnString.
    Public Const PATH As Short = 6 'Indicates that the path minus the filename should be returned in svReturnString. This option differs from DIRECTORY in that the drive designation (if specified in szPath) is included in the returned path. When szPath specifies a UNC path the server and shared device name are included in the returned path. For example, the UNC path \\TheServer\TheSharedDevice\TheApp\TheFile.exe is returned in svReturnString as \\TheServer\TheSharedDevice\TheApp\.
    Public Const PATH_FILENAME_ONLY As Short = 7

    Structure FORMAT_MSG_EDI
        Dim Email As String
        Dim Regie As String
        Dim Command As String
        Dim Campagne As String
        Dim Name As String
        Dim DateFile As String
        'Public Function All() As String()
        '    Dim line() As String = {Campagne, Command, Regie, Name, Email}
        '    Return line
        'End Function
    End Structure

    Public EDI As FORMAT_MSG_EDI

    Public Function ParsePath(ByVal szPath As String, ByVal nOperation As Short) As String

        Dim nPosSlash, nPosPoint, nLenPath As Integer

        nPosPoint = InStrRev(szPath, ".") - 1
        nPosSlash = InStrRev(szPath, "\")
        nLenPath = Len(szPath)

        If nOperation = EXTENSION_ONLY Then
            Return VB.Right(szPath, nLenPath - nPosPoint)
        ElseIf nOperation = FILENAME_ONLY Then
            Return Mid(szPath, nPosSlash + 1, nPosPoint - nPosSlash)
        ElseIf nOperation = FILENAME Then
            Return VB.Right(szPath, nLenPath - nPosSlash)
        ElseIf nOperation = PATH Then
            Return VB.Left(szPath, nPosSlash)
        ElseIf nOperation = PATH_FILENAME_ONLY Then
            Return VB.Left(szPath, nPosPoint)
        ElseIf nOperation = DIRECTORY Then
            Return VB.Left(szPath, nPosSlash - 1)
        Else
            Return szPath
        End If


    End Function

    Private Function CampagneEDI(ByVal szString As String) As String

        Dim nPosStart As Integer = InStr(szString, Chr(34) + "campagne" + Chr(34), CompareMethod.Text)
        If nPosStart > 0 Then
            Dim nPosEnd As Integer = InStr(nPosStart, szString, Chr(10), CompareMethod.Text)
            Dim Line() As String = Split(Replace(Mid(szString, nPosStart, nPosEnd - nPosStart), Chr(34), ""), ";")
            Return Line(1)
        Else
            Return vbNullString
        End If

    End Function

    Private Function CommandEDI(ByVal szString As String) As String

        Dim nPosStart As Integer = InStr(szString, Chr(34) + "commande" + Chr(34), CompareMethod.Text)
        If nPosStart > 0 Then
            Dim nPosEnd As Integer = InStr(nPosStart, szString, Chr(10), CompareMethod.Text)
            Dim Line() As String = Split(Replace(Mid(szString, nPosStart, nPosEnd - nPosStart), Chr(34), ""), ";")
            Return Line(7) 'VB.Left(Line(4), InStrRev(Line(4), "/") - 1)
        Else
            Return vbNullString
        End If

    End Function

    Private Function EmailEDI(ByVal szString As String) As String

        Dim nPosStart As Integer = InStr(szString, Chr(34) + "tiers" + Chr(34) + ";" + Chr(34) + "PO" + Chr(34), CompareMethod.Text)
        If nPosStart > 0 Then
            Dim nPosEnd As Integer = InStr(nPosStart, szString, Chr(10), CompareMethod.Text)
            If nPosEnd > nPosStart Then
                Dim Line() As String = Split(Replace(Mid(szString, nPosStart, nPosEnd - nPosStart), Chr(34), ""), ";")
                Return Line(10)
            Else
                Return vbNullString
            End If
        Else
            Return vbNullString
        End If

    End Function

    Private Function RegieEDI(ByVal szString As String) As String

        Dim nPosStart As Integer = InStr(szString, Chr(34) + "tiers" + Chr(34) + ";" + Chr(34) + "DS" + Chr(34), CompareMethod.Text)
        If nPosStart > 0 Then
            Dim nPosEnd As Integer = InStr(nPosStart, szString, Chr(10), CompareMethod.Text)
            Dim Line() As String = Split(Replace(Mid(szString, nPosStart, nPosEnd - nPosStart), Chr(34), ""), ";")
            Return Line(4)
        Else
            Return vbNullString
        End If

    End Function

    Private Sub Form1_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load

        TreeViewEDI.Sort()

        szMessage = "renommer"
        ExtFilesSrc = ".txt"
        ExtFilesDst = ".bak"
        Dim GetPathEDI As String = ""
        Dim GetEmail As String = ""

        ' Fichier INI de paramétrages
        GestionEDIIni = ParsePath(Application.ExecutablePath, PATH_FILENAME_ONLY) & ".ini"
        If IO.File.Exists(GestionEDIIni) Then

            Dim nvCount As Integer = 0
            Do
                Dim User As String = Select_GetIniString("Parametres", "User" + Str(nvCount).Trim, GestionEDIIni)
                If User = "" Then Exit Do
                Dim InfoUser() As String = Split(User, ";")
                UserDic.Add(InfoUser(0), InfoUser(2))
                EmailDic.Add(InfoUser(0), InfoUser(1))
                'EdiDic.Add(InfoUser(2), InfoUser(1))
                EdiDic.Add(InfoUser(1), InfoUser(2))
                nvCount = nvCount + 1
            Loop

            Bt_Users.Enabled = (UserDic.Count > 0)

            'GetPathEDI = EdiDic.First.Key
            GetPathEDI = EdiDic.First.Value
            If GetPathEDI <> "" Then
                If VB.Right(GetPathEDI, 1) <> "\" Then GetPathEDI = GetPathEDI + "\"
            End If

            PathEDI = GetPathEDI

            For Each u In EdiDic
                If u.Value = GetPathEDI Then
                    GetEmail = u.Key
                    Exit For
                End If
            Next

            'GetEmail = EdiDic(GetPathEDI)
            If GetEmail = "" Then GetEmail = "Tous"

        Else

            GetPathEDI = Select_GetIniString("SAS", "Repertoire SE", My.Application.Info.DirectoryPath + "\UFR02.ini")
            If GetPathEDI <> "" Then
                If VB.Right(GetPathEDI, 1) <> "\" Then GetPathEDI = GetPathEDI + "\"
                PathEDI = GetPathEDI + "reception\"
            End If

            GetEmail = Select_GetIniString("Definition_du_site", "EInterloc", My.Application.Info.DirectoryPath + "\ATAAETUD.ini")
            If GetEmail = "" Then GetEmail = "Tous"

        End If


        TextPath.Text = PathEDI
        FolderBrowserDialog1.SelectedPath = PathEDI

        cMSDic.Add("Rafraîchir", 0)
        cMSDic.Add("Développer", 1)
        cMSDic.Add("Réduire", 2)
        cMSDic.Add("Développer Tout", 3)
        cMSDic.Add("Réduire tout", 4)
        cMSDic.Add("Restaurer", 5)
        cMSDic.Add("Archiver", 6)
        cMSDic.Add("Effacer", 7)
        cMSDic.Add("Déplacer", 8)

        'cMSDic.Add("Refresh", 0)
        'cMSDic.Add("Expand", 1)
        'cMSDic.Add("Collapse", 2)
        'cMSDic.Add("Expand all", 3)
        'cMSDic.Add("Collapse all", 4)
        'cMSDic.Add("Restore", 5)
        'cMSDic.Add("Backup", 6)
        'cMSDic.Add("Delete", 7)

        CMS_TreeViewEDI.Items.Add("Refresh")
        CMS_TreeViewEDI.Font = New System.Drawing.Font("Tahoma", 11.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))

        Me.ExitMenu = New System.Windows.Forms.ToolStripMenuItem
        ExitMenu.Image = GestionEDI.My.Resources.sortie
        ExitMenu.Text = "Quitter"
        ExitMenu.Font = New System.Drawing.Font("Tahoma", 12.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        ExitMenu.ImageScaling = ToolStripItemImageScaling.SizeToFit
        'ExitMenu.AutoSize = True
        'ExitMenu.Size = New System.Drawing.Size(140, 40)

        Me.ModeMenu = New System.Windows.Forms.ToolStripMenuItem
        ModeMenu.Image = GestionEDI.My.Resources.Outils
        ModeMenu.Text = "Option"
        ModeMenu.Font = New System.Drawing.Font("Tahoma", 12.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        ModeMenu.ImageScaling = ToolStripItemImageScaling.SizeToFit
        'RefreshMenu.AutoSize = True
        'RefreshMenu.Size = New System.Drawing.Size(140, 40)

        Me.RefreshMenu = New System.Windows.Forms.ToolStripMenuItem
        RefreshMenu.Image = GestionEDI.My.Resources.Refresh
        RefreshMenu.Text = "Rafraîchir"
        RefreshMenu.Font = New System.Drawing.Font("Tahoma", 10.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        RefreshMenu.ImageScaling = ToolStripItemImageScaling.SizeToFit
        'RefreshMenu.AutoSize = True
        'RefreshMenu.Size = New System.Drawing.Size(140, 40)
        RefreshMenu.ShortcutKeys = Keys.F5

        Me.ModeRestaure = New System.Windows.Forms.ToolStripMenuItem
        'ModeRestaure.Image = GestionEDI.My.Resources.Restore
        ModeRestaure.CheckState = System.Windows.Forms.CheckState.Checked
        ModeRestaure.Checked = False
        ModeRestaure.Text = "Mode Restauration"
        ModeRestaure.Font = New System.Drawing.Font("Tahoma", 10.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        'ModeRestaure.ImageScaling = ToolStripItemImageScaling.SizeToFit
        'ModeRestaure.AutoSize = True
        'ModeRestaure.Size = New System.Drawing.Size(183, 22)
        ModeRestaure.ShortcutKeys = CType(CType(Keys.Control, Integer) + (CType(Keys.R, Integer)), Keys) ' CType(131154, Keys)
        ModeRestaure.ShowShortcutKeys = True

        Me.ModeMenu.DropDownItems.AddRange(New System.Windows.Forms.ToolStripItem() {Me.ModeRestaure, Me.RefreshMenu})

        Me.MenuStripFM.Items.AddRange(New System.Windows.Forms.ToolStripItem() {Me.ModeMenu, Me.ExitMenu})

        Show()

        System.Windows.Forms.Application.DoEvents()

        If PathEDI <> "" Then
            TVRefresh(GetEmail)
        End If


    End Sub

    Private Sub Form_Enable(Optional ByVal bBOOL As Boolean = True)

        If Not bBOOL Then
            Me.RefreshMenu.HideDropDown()
            Me.ModeMenu.HideDropDown()
            Me.ExitMenu.HideDropDown()
            CMS_TreeViewEDI.Hide()
        End If

        Me.MenuStripFM.Enabled = bBOOL
        TreeViewEDI.Enabled = bBOOL
        CMS_TreeViewEDI.Enabled = bBOOL
        ComboEmail.Enabled = bBOOL
        Bt_Browse.Enabled = bBOOL
        Bt_Users.Enabled = bBOOL And (UserDic.Count > 0)


        System.Windows.Forms.Application.DoEvents()

    End Sub

    Private Sub TreeViewEDI_MouseClick(ByVal sender As Object, ByVal e As System.Windows.Forms.MouseEventArgs) Handles TreeViewEDI.MouseClick

        Dim TVSelectedNode As TreeNode = TreeViewEDI.GetNodeAt(e.X, e.Y)

        TreeViewEDI.SelectedNode = TVSelectedNode

        If TreeViewEDI.SelectedNode Is Nothing Then Exit Sub

        DataGridView1.Visible = Not (TVSelectedNode.Level = 3)
        TextEDI.Visible = (TVSelectedNode.Level = 3)

    End Sub

    Private Sub TreeViewEDI_MouseMove(ByVal sender As Object, ByVal e As System.Windows.Forms.MouseEventArgs) Handles TreeViewEDI.MouseMove

        Dim TVSelectedNode As TreeNode = TreeViewEDI.GetNodeAt(e.X, e.Y)

        If TVSelectedNode Is Nothing Then
            TreeViewEDI.SelectedNode = Nothing
        End If

    End Sub

    Private Sub CMS_TreeViewEDI_Opening(ByVal sender As Object, ByVal e As System.ComponentModel.CancelEventArgs) Handles CMS_TreeViewEDI.Opening

        CMS_TreeViewEDI.Items.Clear()

        'CMS_TreeViewEDI.Items.Add(cMSDic.Keys(0), GestionFichierEDI.My.Resources.Refresh)
        'CMS_TreeViewEDI.Items.Item(CMS_TreeViewEDI.Items.Count - 1).ImageScaling = ToolStripItemImageScaling.None

        'Dim TVSelectedNode As TreeNode = TreeViewEDI.SelectedNode

        If TreeViewEDI.SelectedNode Is Nothing Then Exit Sub

        If ModeRestaure.Checked Then
            CMS_TreeViewEDI.Items.Add(cMSDic.Keys(5), GestionEDI.My.Resources.Restore)
            CMS_TreeViewEDI.Items.Item(CMS_TreeViewEDI.Items.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        Else
            CMS_TreeViewEDI.Items.Add(cMSDic.Keys(6), GestionEDI.My.Resources.Backup)
            CMS_TreeViewEDI.Items.Item(CMS_TreeViewEDI.Items.Count - 1).ImageScaling = ToolStripItemImageScaling.None
        End If

        CMS_TreeViewEDI.Items.Add(cMSDic.Keys(8), GestionEDI.My.Resources.deplacer)
        CMS_TreeViewEDI.Items.Item(CMS_TreeViewEDI.Items.Count - 1).ImageScaling = ToolStripItemImageScaling.None

        CMS_TreeViewEDI.Items.Add(cMSDic.Keys(1), GestionEDI.My.Resources.Expand)
        CMS_TreeViewEDI.Items.Item(CMS_TreeViewEDI.Items.Count - 1).ImageScaling = ToolStripItemImageScaling.None

        CMS_TreeViewEDI.Items.Add(cMSDic.Keys(2), GestionEDI.My.Resources.Collapse)
        CMS_TreeViewEDI.Items.Item(CMS_TreeViewEDI.Items.Count - 1).ImageScaling = ToolStripItemImageScaling.None

        CMS_TreeViewEDI.Items.Add(cMSDic.Keys(3), GestionEDI.My.Resources.ExpandAll)
        CMS_TreeViewEDI.Items.Item(CMS_TreeViewEDI.Items.Count - 1).ImageScaling = ToolStripItemImageScaling.None

        CMS_TreeViewEDI.Items.Add(cMSDic.Keys(4), GestionEDI.My.Resources.CollapseAll)
        CMS_TreeViewEDI.Items.Item(CMS_TreeViewEDI.Items.Count - 1).ImageScaling = ToolStripItemImageScaling.None

        CMS_TreeViewEDI.Items.Add(cMSDic.Keys(7), GestionEDI.My.Resources.Delete)
        CMS_TreeViewEDI.Items.Item(CMS_TreeViewEDI.Items.Count - 1).ImageScaling = ToolStripItemImageScaling.None

        System.Windows.Forms.Application.DoEvents()

    End Sub

    Private Sub CMS_TreeViewEDI_ItemClicked(ByVal sender As Object, ByVal e As System.Windows.Forms.ToolStripItemClickedEventArgs) Handles CMS_TreeViewEDI.ItemClicked

        Form_Enable(False)

        Dim str As String = e.ClickedItem.Text

        Dim int As Integer = cMSDic(str)

        If TreeViewEDI.SelectedNode Is Nothing Then Exit Sub

        Select Case int

            Case 0 'Refresh
                TVRefresh(ComboEmail.Text)
            Case 1 'Expand
                TreeViewEDI_Expand()
            Case 2 'Collapse
                TreeViewEDI_Collapse()
            Case 3 'Expand all
                TreeViewEDI.ExpandAll()
            Case 4 'Collapse all
                TreeViewEDI.CollapseAll()
            Case 5 'Restore
                RenameFileEDI()
            Case 6 'Backup
                RenameFileEDI()
            Case 7 'Delete
                DeleteFileEDI()
            Case 8 'Deplacer
                MoveFileEDI()
        End Select

        Form_Enable()


    End Sub

    Private Sub RefreshMenu_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles RefreshMenu.Click
        TVRefresh(ComboEmail.Text)
    End Sub

    Private Sub ExitMenu_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ExitMenu.Click
        Me.Close()

    End Sub

    Private Sub TVRefresh(Optional ByVal SetEmailDefault As String = "Tous", Optional ByVal ResetListEmail As Boolean = False)


        SyncLock Me

            Form_Enable(False)

            Dim CreateListEmail As Boolean = (ComboEmail.Items.Count = 0)

            If ResetListEmail Then
                ComboEmail.Items.Clear()
            End If

            If CreateListEmail Or ResetListEmail Then
                ComboEmail.Items.Add("Tous")
            End If

            TreeViewEDI.Nodes.Clear()
            TextEDI.Text = ""

            DataGridView1.Columns.Clear()
            DataGridView1.ColumnCount = 5
            'DataGridView1.AutoResizeColumns(DataGridViewAutoSizeColumnsMode.Fill)
            'DataGridView1.AutoResizeColumns(DataGridViewAutoSizeColumnsMode.DisplayedCells)
            'DataGridView1.AutoResizeColumns(DataGridViewAutoSizeColumnsMode.DisplayedCellsExceptHeader)
            'DataGridView1.AutoResizeColumns(DataGridViewAutoSizeColumnsMode.AllCellsExceptHeader)
            'DataGridView1.AutoResizeColumns(DataGridViewAutoSizeColumnsMode.AllCells)
            'DataGridView1.AutoResizeColumns(DataGridViewAutoSizeColumnsMode.ColumnHeader)
            'DataGridView1.AutoResizeColumns(DataGridViewAutoSizeColumnsMode.None)
            'DataGridView1.AutoSize = True

            'DataGridView1.Columns.Add("Campagne", "Campagne")
            'DataGridView1.Columns.Add("Command", "Command")
            'DataGridView1.Columns.Add("Regie", "Regie")
            'DataGridView1.Columns.Add("Name", "Name")
            'DataGridView1.Columns.Add("Email", "Email")

            'DataGridView1.Columns.Add("Campagne", "Command", "Regie", "Name", "Email")

            Dim ListAcces() As String = System.IO.Directory.GetFiles(PathEDI, "*" + ExtFilesSrc, System.IO.SearchOption.TopDirectoryOnly)

            For Each obj In ListAcces

                If IO.File.Exists(obj) Then

                    Dim lPatch As String = ""

                    Try
                        lPatch = IO.File.ReadAllText(obj, System.Text.Encoding.UTF7)
                    Catch ex As Exception
                        Dim nvResult As Integer = MsgBox("Impossible de lire le fichier suivant: " + obj + vbNewLine + ex.Message, CType(MsgBoxStyle.OkOnly + MsgBoxStyle.Critical, MsgBoxStyle))
                    End Try

                    If lPatch <> "" Then

                        Dim EmailFound As String = LCase(EmailEDI(lPatch))

                        If Not (EmailFound Is Nothing) Then

                            If CreateListEmail Or ResetListEmail Then
                                If Not ComboEmail.Items.Contains(EmailFound) Then
                                    ComboEmail.Items.Add(EmailFound)
                                End If
                            End If

                            If (SetEmailDefault = "Tous" Or SetEmailDefault = EmailFound) Then

                                'Dim CreationTime As Date = IO.File.GetLastWriteTime(obj)
                                'EDI.DateFile = CStr(CreationTime.Year) + "/" + CStr(CreationTime.Month) + "/" + CStr(CreationTime.Day) + " - " + CStr(CreationTime.Hour) + ":" + CStr(CreationTime.Minute) + ":" + CStr(CreationTime.Second)

                                System.Windows.Forms.Application.DoEvents()

                                EDI.Command = CommandEDI(lPatch)
                                EDI.Email = EmailFound
                                EDI.Campagne = CampagneEDI(lPatch)
                                EDI.Regie = RegieEDI(lPatch)
                                EDI.Name = ParsePath(obj, FILENAME)

                                System.Windows.Forms.Application.DoEvents()

                                If EDI.Command <> vbNullString And EDI.Campagne <> vbNullString And EDI.Regie <> vbNullString And EDI.Name <> vbNullString Then

                                    EDI.Email = EmailEDI(lPatch)

                                    'Dim champs() As String = EDI.All
                                    'DataGridView1.Rows.Add(champs)
                                    DataGridView1.Rows.Add(EDI.Campagne, EDI.Command, EDI.Regie, EDI.Name, EDI.Email)

                                    If Not TreeViewEDI.Nodes.ContainsKey(EDI.Campagne) Then
                                        TreeViewEDI.Nodes.Add(EDI.Campagne, EDI.Campagne)
                                    End If

                                    Dim IndexNoeud As Integer = TreeViewEDI.Nodes.IndexOfKey(EDI.Campagne)
                                    If Not TreeViewEDI.Nodes(IndexNoeud).Nodes.ContainsKey(EDI.Regie) Then
                                        TreeViewEDI.Nodes(IndexNoeud).Nodes.Add(EDI.Regie, EDI.Regie)
                                    End If

                                    Dim IndexNoeudCommand As Integer = TreeViewEDI.Nodes(IndexNoeud).Nodes.IndexOfKey(EDI.Regie)
                                    If Not TreeViewEDI.Nodes(IndexNoeud).Nodes(IndexNoeudCommand).Nodes.ContainsKey(EDI.Command) Then
                                        TreeViewEDI.Nodes(IndexNoeud).Nodes(IndexNoeudCommand).Nodes.Add(EDI.Command, EDI.Command)
                                    End If

                                    Dim IndexNoeudRegie As Integer = TreeViewEDI.Nodes(IndexNoeud).Nodes(IndexNoeudCommand).Nodes.IndexOfKey(EDI.Command)
                                    If Not TreeViewEDI.Nodes(IndexNoeud).Nodes(IndexNoeudCommand).Nodes(IndexNoeudRegie).Nodes.ContainsKey(EDI.Name) Then
                                        TreeViewEDI.Nodes(IndexNoeud).Nodes(IndexNoeudCommand).Nodes(IndexNoeudRegie).Nodes.Add(EDI.Name, EDI.Name)
                                    End If

                                End If

                                System.Windows.Forms.Application.DoEvents()

                            End If

                        End If
                    End If
                End If
            Next


            If SetEmailDefault <> "Tous" Then
                If ComboEmail.Items.Contains(SetEmailDefault) Then
                    ComboEmail.Text = SetEmailDefault
                Else
                    ComboEmail.Text = SetEmailDefault
                End If
            Else
                ComboEmail.Text = SetEmailDefault
            End If

            Form_Enable()

            ComboEmail.Focus()

        End SyncLock

    End Sub

    Private Function RenameEDI(ByVal szPathFile As String, Optional ByVal YesToAll As Boolean = False) As Integer

        Dim nvResult As Integer = vbNo

        If IO.File.Exists(szPathFile) Then

            If Not YesToAll Then
                nvResult = MsgBox("Êtes-vous sûr de vouloir " + szMessage + " le message EDI suivant: " + vbNewLine + szPathFile, CType(MsgBoxStyle.YesNoCancel + MsgBoxStyle.Exclamation, MsgBoxStyle))
            Else
                nvResult = vbYes
            End If

            If nvResult = vbYes Then

                Dim szPathFileBak As String = ParsePath(szPathFile, PATH_FILENAME_ONLY) + ExtFilesDst

                On Error Resume Next

                Err.Clear()

                If IO.File.Exists(szPathFileBak) Then
                    IO.File.Delete(szPathFileBak)
                    While Err.Number <> 0
                        If MsgBox("Impossible de supprimer le fichier suivant:" + vbNewLine + szPathFileBak + vbNewLine + vbNewLine + Err.Description, CType(MsgBoxStyle.RetryCancel + MsgBoxStyle.Critical, MsgBoxStyle)) = MsgBoxResult.Cancel Then
                            nvResult = vbNo
                            Exit While
                        End If
                        Err.Clear()
                        IO.File.Delete(szPathFileBak)
                    End While
                End If

                If nvResult = vbYes Then
                    IO.File.Move(szPathFile, szPathFileBak)
                    While Err.Number <> 0
                        If MsgBox("Impossible de renommer le fichier suivant:" + vbNewLine + szPathFile + vbNewLine + vbNewLine + Err.Description, CType(MsgBoxStyle.RetryCancel + MsgBoxStyle.Critical, MsgBoxStyle)) = MsgBoxResult.Cancel Then
                            nvResult = vbNo
                            Exit While
                        End If
                        Err.Clear()
                        IO.File.Move(szPathFile, szPathFileBak)
                    End While
                End If

                On Error GoTo 0

            End If
        End If

        Return nvResult

    End Function

    Private Sub RenameFileEDI()

        If TreeViewEDI.SelectedNode Is Nothing Then Exit Sub

        Dim TVSelectedNode As TreeNode = TreeViewEDI.SelectedNode

        Dim ListEDI As ArrayList = FindTreeNode(TVSelectedNode)

        Dim nvResult As Integer = vbNo
        If ListEDI.Count > 1 Then

            nvResult = MsgBox("Êtes-vous sûr de vouloir " + szMessage + " les messages EDI selectionnés ?", CType(MsgBoxStyle.YesNoCancel + MsgBoxStyle.Exclamation, MsgBoxStyle))
            If nvResult = vbCancel Then Exit Sub

        End If

        Dim YesToAll As Boolean = (nvResult = vbYes)

        For FileEDI = 0 To ListEDI.Count - 1
            Dim EDITreeNode As TreeNode = CType(ListEDI(FileEDI), TreeNode)
            nvResult = RenameEDI(PathEDI + EDITreeNode.Name, YesToAll)
            If nvResult = vbYes Then
                DeleteTreeNode(EDITreeNode)
                DeleteDataGrid(EDITreeNode.Name)
            ElseIf nvResult = vbCancel Then
                Exit For
            End If
        Next

    End Sub


    Private Function MoveEDI(ByVal szPathFile As String, ByVal szNewPathFile As String, Optional ByVal YesToAll As Boolean = False) As Integer

        Dim nvResult As Integer = vbNo

        If IO.File.Exists(szPathFile) Then

            If Not YesToAll Then
                nvResult = MsgBox("Êtes-vous sûr de vouloir deplacer le message EDI suivant: " + vbNewLine + szPathFile, CType(MsgBoxStyle.YesNoCancel + MsgBoxStyle.Exclamation, MsgBoxStyle))
            Else
                nvResult = vbYes
            End If

            If nvResult = vbYes Then

                On Error Resume Next

                Err.Clear()

                If IO.File.Exists(szNewPathFile) Then
                    IO.File.Delete(szNewPathFile)
                    While Err.Number <> 0
                        If MsgBox("Impossible de supprimer le fichier suivant:" + vbNewLine + szNewPathFile + vbNewLine + vbNewLine + Err.Description, CType(MsgBoxStyle.RetryCancel + MsgBoxStyle.Critical, MsgBoxStyle)) = MsgBoxResult.Cancel Then
                            nvResult = vbNo
                            Exit While
                        End If
                        Err.Clear()
                        IO.File.Delete(szNewPathFile)
                    End While
                End If

                If nvResult = vbYes Then
                    IO.File.Move(szPathFile, szNewPathFile)
                    While Err.Number <> 0
                        If MsgBox("Impossible de déplacer le fichier suivant:" + vbNewLine + szPathFile + vbNewLine + vbNewLine + Err.Description, CType(MsgBoxStyle.RetryCancel + MsgBoxStyle.Critical, MsgBoxStyle)) = MsgBoxResult.Cancel Then
                            nvResult = vbNo
                            Exit While
                        End If
                        Err.Clear()
                        IO.File.Move(szPathFile, szNewPathFile)
                    End While
                End If

                On Error GoTo 0

            End If
        End If

        Return nvResult

    End Function

    Private Sub MoveFileEDI()

        If TreeViewEDI.SelectedNode Is Nothing Then Exit Sub

        Dim TVSelectedNode As TreeNode = TreeViewEDI.SelectedNode

        Dim ListEDI As ArrayList = FindTreeNode(TVSelectedNode)

        Dim nvResult As Integer = vbNo
        If ListEDI.Count > 1 Then

            nvResult = MsgBox("Êtes-vous sûr de vouloir déplacer les messages EDI selectionnés ?", CType(MsgBoxStyle.YesNoCancel + MsgBoxStyle.Exclamation, MsgBoxStyle))
            If nvResult = vbCancel Then Exit Sub

        End If


        DialogUsers.ListUsers.Items.Clear()
        For Each u In UserDic
            If StrComp(PathEDI, u.Value, CompareMethod.Text) <> 0 Then
                DialogUsers.ListUsers.Items.Add(u.Key)
            End If
        Next

        Dim NewPathEDI As String = ""

        If DialogUsers.ListUsers.Items.Count > 0 Then
            If DialogUsers.ShowDialog = DialogResult.OK Then
                NewPathEDI = UserDic(DialogUsers.ListUsers.Text)
            End If
        Else
            If FolderBrowserDialog1.ShowDialog = DialogResult.OK Then
                NewPathEDI = FolderBrowserDialog1.SelectedPath
            End If
        End If


        If VB.Right(NewPathEDI, 1) <> "\" Then NewPathEDI = NewPathEDI + "\"

        Dim YesToAll As Boolean = (nvResult = vbYes)

        For FileEDI = 0 To ListEDI.Count - 1
            Dim EDITreeNode As TreeNode = CType(ListEDI(FileEDI), TreeNode)
            nvResult = MoveEDI(PathEDI + EDITreeNode.Name, NewPathEDI + EDITreeNode.Name, YesToAll)
            If nvResult = vbYes Then
                DeleteTreeNode(EDITreeNode)
                DeleteDataGrid(EDITreeNode.Name)
            ElseIf nvResult = vbCancel Then
                Exit For
            End If
        Next

        'TVRefresh(ComboEmail.Text, True)

    End Sub

    Private Sub DeleteTreeNode(ByVal node As TreeNode)

        Dim Level As TreeNode = node

        Do While Level.Level > 0

            If Level.Parent.Nodes.Count = 1 Then
                Level = Level.Parent
            Else
                Exit Do
            End If

        Loop

        Level.Nodes.Remove(Level)

    End Sub

    Private Sub DeleteDataGrid(ByVal szfile As String)

        'Dim intcount As Integer = 0
        'If DataGridView1.Rows(intcount).Cells(3).Value.ToString = szfile Then
        'DataGridView1.Rows.Remove(DataGridView1.Rows.Item(intcount))
        'intcount += 1

        For Each Row As DataGridViewRow In DataGridView1.Rows
            If Row.Cells(3).Value.ToString = szfile Then
                DataGridView1.Rows.Remove(Row)
            End If
        Next Row
        '
    End Sub

    'Private Function boucle(ByVal node As TreeNode) As ArrayList

    '    Dim ListEDI As ArrayList = New ArrayList

    '    Dim Count As Integer = node.Nodes.Count - 1
    '    For Nodes = 0 To Count
    '        ListEDI.Add(node.Nodes(Nodes).Name)
    '    Next

    '    Return ListEDI

    'End Function

    Private Function FindTreeNode(ByVal node As TreeNode) As ArrayList

        Dim ListEDI As ArrayList = New ArrayList()

        ListEDI.Clear()

        If node.Level = 0 Then
            Dim LevelCount0 As Integer = node.Nodes.Count - 1
            For boucleLevel0 = 0 To LevelCount0
                Dim LevelCount1 As Integer = node.Nodes(boucleLevel0).Nodes.Count - 1
                For boucleLevel1 = 0 To LevelCount1
                    Dim LevelCount2 As Integer = node.Nodes(boucleLevel0).Nodes(boucleLevel1).Nodes.Count - 1
                    For boucleLevel2 = 0 To LevelCount2
                        Dim CurrentNode As TreeNode = node.Nodes(boucleLevel0).Nodes(boucleLevel1).Nodes(boucleLevel2)
                        ListEDI.Add(CurrentNode) '.FullPath) ', node.Nodes(boucleLevel0).Name)
                    Next
                Next
            Next
        End If

        If node.Level = 1 Then
            Dim LevelCount1 As Integer = node.Nodes.Count - 1
            For boucleLevel1 = 0 To LevelCount1
                Dim LevelCount2 As Integer = node.Nodes(boucleLevel1).Nodes.Count - 1
                For boucleLevel2 = 0 To LevelCount2
                    Dim CurrentNode As TreeNode = node.Nodes(boucleLevel1).Nodes(boucleLevel2)
                    ListEDI.Add(CurrentNode) '.FullPath)
                Next
            Next
        End If

        If node.Level = 2 Then
            Dim LevelCount1 As Integer = node.Nodes.Count - 1
            Dim v As Integer = node.Nodes.Count - 1
            For a = 0 To LevelCount1
                Dim CurrentNode As TreeNode = node.Nodes(a)
                ListEDI.Add(CurrentNode) '.FullPath)
            Next
        End If

        If node.Level = 3 Then
            Dim CurrentNode As TreeNode = node
            ListEDI.Add(CurrentNode) '.FullPath)
        End If

        Return ListEDI

    End Function

    Private Function DeleteEDI(ByVal szPathFile As String, Optional ByVal YesToAll As Boolean = False, Optional ByVal Title As String = "") As Integer

        Dim nvResult As Integer = vbNo

        If IO.File.Exists(szPathFile) Then

            If Not YesToAll Then
                Dim TabInfoEDI() As String = Split(Title, "\")
                Dim InfoEDI As String = "Campagne: " + TabInfoEDI(0) + vbNewLine + "Commande: " + TabInfoEDI(1) + vbNewLine + "Régie: " + TabInfoEDI(2) + vbNewLine + "Fichier: " + TabInfoEDI(3)
                nvResult = MsgBox("Êtes-vous sûr de vouloir supprimer définitivement l'EDI suivant: " + vbNewLine + InfoEDI, CType(MsgBoxStyle.YesNoCancel + MsgBoxStyle.Critical, MsgBoxStyle))
                'nvResult = MsgBox("Êtes-vous sûr de vouloir supprimer définitivement le message EDI suivant: " + vbNewLine + szPathFile, CType(MsgBoxStyle.YesNoCancel + MsgBoxStyle.Critical, MsgBoxStyle))
            Else
                nvResult = vbYes
            End If

            If nvResult = vbYes Then

                On Error Resume Next

                Err.Clear()

                IO.File.Delete(szPathFile)
                While Err.Number <> 0
                    If MsgBox("Impossible de supprimer le fichier suivant:" + vbNewLine + szPathFile + vbNewLine + vbNewLine + Err.Description, CType(MsgBoxStyle.RetryCancel + MsgBoxStyle.Critical, MsgBoxStyle)) = MsgBoxResult.Cancel Then
                        nvResult = vbNo
                        Exit While
                    End If
                    Err.Clear()
                    IO.File.Delete(szPathFile)
                End While

            End If

            On Error GoTo 0

        End If

        Return nvResult

    End Function

    Private Sub DeleteFileEDI()

        If TreeViewEDI.SelectedNode Is Nothing Then Exit Sub

        Dim TVSelectedNode As TreeNode = TreeViewEDI.SelectedNode

        Dim ListEDI As ArrayList = FindTreeNode(TVSelectedNode)

        Dim nvResult As Integer = vbNo
        If ListEDI.Count > 1 Then

            nvResult = MsgBox("Êtes-vous sûr de vouloir supprimer les messages EDI selectionnés ?", CType(MsgBoxStyle.YesNoCancel + MsgBoxStyle.Critical, MsgBoxStyle))
            If nvResult = vbCancel Then Exit Sub

        End If

        Dim YesToAll As Boolean = (nvResult = vbYes)

        For FileEDI = 0 To ListEDI.Count - 1
            Dim EDITreeNode As TreeNode = CType(ListEDI(FileEDI), TreeNode)
            nvResult = DeleteEDI(PathEDI + EDITreeNode.Name, YesToAll, EDITreeNode.FullPath)
            If nvResult = vbYes Then
                DeleteTreeNode(EDITreeNode)
            ElseIf nvResult = vbCancel Then
                Exit For
            End If
        Next

    End Sub

    Private Sub TreeViewEDI_AfterSelect(ByVal sender As Object, ByVal e As System.Windows.Forms.TreeViewEventArgs) Handles TreeViewEDI.AfterSelect

        Dim TVSelectedNode As TreeNode = TreeViewEDI.SelectedNode

        If TVSelectedNode.Level = 3 Then
            If IO.File.Exists(PathEDI + TVSelectedNode.Name) Then
                TextEDI.Text = IO.File.ReadAllText(PathEDI + TVSelectedNode.Name, System.Text.Encoding.UTF7)
            Else
                TextEDI.Text = "Fichier introuvable !!!"
            End If
        ElseIf ComboEmail.Text <> "" Then
            TextEDI.Text = ""
        End If

        'Dim szFullPath As String = TreeView1.SelectedNode.FullPath

    End Sub

    Private Sub Bt_Browse_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Bt_Browse.Click

        FolderBrowserDialog1.SelectedPath = PathEDI

        If FolderBrowserDialog1.ShowDialog = DialogResult.OK Then

            PathEDI = FolderBrowserDialog1.SelectedPath

            If VB.Right(PathEDI, 1) <> "\" Then PathEDI = PathEDI + "\"

            TextPath.Text = PathEDI

            TVRefresh(ComboEmail.Text, True)

        End If
    End Sub

    Private Sub ComboEmail_KeyPress(ByVal sender As Object, ByVal e As System.Windows.Forms.KeyPressEventArgs) Handles ComboEmail.KeyPress
        e.KeyChar = ChrW(0)
    End Sub

    Private Sub ComboEmail_SelectedIndexChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ComboEmail.SelectedIndexChanged

        If ComboEmail.Enabled = True Then

            TVRefresh(ComboEmail.Text)

        End If

    End Sub

    Private Sub TreeViewEDI_Expand()
        Dim TVSelectedNode As TreeNode = TreeViewEDI.SelectedNode

        If Not (TVSelectedNode Is Nothing) Then
            'If TreeView1.SelectedNode.IsExpanded Then
            'TreeView1.SelectedNode.Collapse()
            TreeViewEDI.SelectedNode.ExpandAll()
        End If
    End Sub

    Private Sub TreeViewEDI_Collapse()
        Dim TVSelectedNode As TreeNode = TreeViewEDI.SelectedNode

        If Not (TVSelectedNode Is Nothing) Then
            'If TreeView1.SelectedNode.IsExpanded Then
            'TreeView1.SelectedNode.Collapse()
            TreeViewEDI.SelectedNode.Collapse()
        End If
    End Sub

    Private Sub ModeRestaure_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles ModeRestaure.Click

        If ModeRestaure.Checked Then
            szMessage = "renommer"
            ExtFilesSrc = ".txt"
            ExtFilesDst = ".bak"
            ModeRestaure.Checked = False
            Me.Text = "Gestion des EDI"
        Else
            szMessage = "restaurer"
            ExtFilesSrc = ".bak"
            ExtFilesDst = ".txt"
            ModeRestaure.Checked = True
            Me.Text = "Gestion des EDI - Mode Restauration"
        End If

        System.Windows.Forms.Application.DoEvents()

        TVRefresh(ComboEmail.Text, True)
    End Sub

    Private Sub Bt_Users_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Bt_Users.Click

        DialogUsers.ListUsers.Items.Clear()
        For Each u In UserDic.Keys
            DialogUsers.ListUsers.Items.Add(u)
        Next

        If DialogUsers.ShowDialog = DialogResult.OK Then

            Dim User As String = DialogUsers.ListUsers.Text

            PathEDI = UserDic(User)

            If VB.Right(PathEDI, 1) <> "\" Then PathEDI = PathEDI + "\"

            TextPath.Text = PathEDI

            TVRefresh(EmailDic(User), True)

        End If


    End Sub
End Class
